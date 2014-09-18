import jenkins
import os
from xml.etree import ElementTree
from rest_framework.views import APIView
from rest_framework.response import Response
from django.contrib.auth.models import User, Group
from models import Job
from rest_framework import viewsets
from inabox.serializers import UserSerializer, GroupSerializer
from inabox.serializers import JobSerializer
from rest_framework import status
from rest_framework.generics import ListCreateAPIView
from django.forms.models import model_to_dict
from six.moves.urllib.request import Request


class UserViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows users to be viewed or edited.
    """
    queryset = User.objects.all()
    serializer_class = UserSerializer


class GroupViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows groups to be viewed or edited.
    """
    queryset = Group.objects.all()
    serializer_class = GroupSerializer


class JobViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows groups to be viewed or edited.
    """
    queryset = Job.objects.all()
    serializer_class = JobSerializer


class ResponseNotFound(Response):
    def __init__(self, message):
        self.status_code = status.HTTP_404_NOT_FOUND
        self.content = ("<title>%s</title>%s" % (message, message))


class ResponseServerError(Response):
    def __init__(self, message):
        self.status_code = status.HTTP_500_INTERNAL_SERVER_ERROR
        self.content = ("<title>%s</title>%s" % (message, message))


class ResponseCreated(Response):
    def __init__(self, message):
        self.status_code = status.HTTP_201_CREATED
        self.content = ("<title>%s</title><code>%s</code>" % (message, message))


class ResponseOk(Response):
    def __init__(self, message):
        self.status_code = status.HTTP_200_OK
        self.content = ("<title>%s</title><code>%s</code>" % (message, message))


class JenkinsController(object):
    def __init__(self):
        try:
            self.url=os.environ['JENKINS_URL']
        except KeyError:
            self.url = 'http://15.125.78.44:8080/'
        try:
            self.username=os.environ['JENKINS_USER']
        except KeyError:
            self.username = 'admin'
        try:
            self.password=os.environ['JENKINS_PASSWORD']
        except KeyError:
            self.password = 'notset'
        self.jay = jenkins.Jenkins(self.url, self.username, self.password)

    def create_job_xml(self, job):
        xml = """
<?xml version='1.0' encoding='UTF-8'?>
<project>
  <actions/>
  <description>%(description)s</description>
  <keepDependencies>false</keepDependencies>
  <scm class="hudson.plugins.git.GitSCM" plugin="git@2.2.5">
    <configVersion>2</configVersion>
    <userRemoteConfigs>
      <hudson.plugins.git.UserRemoteConfig>
        <url>%(gitrepo)s</url>
        <credentialsId>2ff426bc-2641-42bb-a243-da2c4cd8a179</credentialsId>
      </hudson.plugins.git.UserRemoteConfig>
    </userRemoteConfigs>
    <branches>
      <hudson.plugins.git.BranchSpec>
        <name>*/master</name>
      </hudson.plugins.git.BranchSpec>
    </branches>
    <doGenerateSubmoduleConfigurations>false</doGenerateSubmoduleConfigurations>
    <submoduleCfg class="list"/>
    <extensions/>
  </scm>
  <canRoam>true</canRoam>
  <disabled>false</disabled>
  <blockBuildWhenDownstreamBuilding>false</blockBuildWhenDownstreamBuilding>
  <blockBuildWhenUpstreamBuilding>false</blockBuildWhenUpstreamBuilding>
  <triggers/>
  <concurrentBuild>false</concurrentBuild>
  <builders>
    <hudson.tasks.Shell>
      <command>%(command)s</command>
    </hudson.tasks.Shell>
  </builders>
  <publishers/>
  <buildWrappers/>
</project>
"""
        args = {
            'description': job.description,
            'gitrepo': job.gitrepo,
            'command': job.command,
        }
        xml = (xml % (args))
        xml = xml.strip()
        return xml

    def create_job(self, job):
        xml = self.create_job_xml(job)
        self.jay.create_job(job.name, xml)

    def update_job(self, job):
        xml = self.create_job_xml(job)
        self.jay.reconfig_job(job.name, xml)

    def get_jobs(self):
        return self.jay.get_jobs()

    def get_job(self, name):
        xmlstr = self.jay.get_job_config(name)
        print '=============================================='
        print str(xmlstr)
        print '=============================================='
        if not xmlstr:
            return None
        command = ''
        description = ''
        try:
            xmlstr = xmlstr.strip()
            xml = ElementTree.fromstring(xmlstr)
            try:
                shell = xml.find('builders').find('hudson.tasks.Shell')
                command = shell.find('command').text
            except:
                command = ''
            try:
                description = xml.find('description').text
                if not description:
                    description = ''
            except:
                description = ''
        except ElementTree.ParseError as e:
            pass
        return {
            'name': name,
            'description': description,
            'command': command,
            'xmlstr': xmlstr,
        }

    def build_job(self, job, parameters):
        data = self.jay.get_job_info(job.name)
        # Launchpad bug #1177831
        req = Request(self.jay.build_job_url(job.name), "")
        self.jay.jenkins_open(req)
        return data['nextBuildNumber']

    def stop_build(self, job):
        if not job.build_number:
            return
        try:
            self.jay.cancel_queue(job.build_number)
        except jenkins.JenkinsException as e:
            print(str(e))
        try:
            self.jay.stop_build(job.name, job.build_number)
        except jenkins.JenkinsException as e:
            print(str(e))
        job.build_number = None

    def delete_job(self, job):
        self.jay.delete_job(job.name)


class JenkinsViewSet(viewsets.ViewSet):
    queryset = Job.objects.all()
    serializer_class = JobSerializer

    def start(self, request, pk, format=None):
        job = Job.find_job(pk)
        if not job:
            return ResponseNotFound("Job '%s' not found" % pk)
        try:
            jay = JenkinsController()
            jjob = jay.get_job(pk)
            if not jjob:
                jay.create_job(job)
            else:
                jay.update_job(job)
            job.build_number = jay.build_job(job, parameters={'foo': 'bar'})
            if job.build_number:
                job.save()
        except jenkins.JenkinsException as e:
            return ResponseServerError(str(e))
        return Response(model_to_dict(job))

    def stop(self, request, pk, format=None):
        job = Job.find_job(pk)
        if not job:
            return ResponseNotFound("Job '%s' not found" % pk)
        jay = JenkinsController()
        jjob = jay.get_job(pk)
        if not jjob:
            return ResponseNotFound("Job '%s' not found on Jenkins" % pk)
        try:
            jay.stop_build(job)
            jay.delete_job(job)
        except jenkins.JenkinsException as e:
            return ResponseServerError(str(e))
        job.save()
        return Response(model_to_dict(job))

    def finish(self, request, pk, format=None):
        job = Job.find_job(pk)
        if not job:
            return ResponseNotFound("Job '%s' not found" % pk)
        jjob = JenkinsController().get_job(pk)
        if not jjob:
            return ResponseNotFound("Job '%s' not found on Jenkins" % pk)
        try:
            jay.stop_build(job)
            jay.delete_job(job)
        except jenkins.JenkinsException as e:
            return ResponseServerError(str(e))
        job.save()
        return Response(model_to_dict(job))

    def status(self, request, pk, format=None):
        job = Job.find_job(pk)
        if not job:
            return ResponseNotFound("Job '%s' not found" % pk)
        jjob = JenkinsController().get_job(pk)
        if not jjob:
            return ResponseNotFound("Job '%s' not found on Jenkins" % pk)
        return Response(model_to_dict(job))
