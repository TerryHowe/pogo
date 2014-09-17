import jenkins
import os
from xml.etree import ElementTree
from rest_framework.views import APIView
from rest_framework.response import Response
from django.contrib.auth.models import User, Group
from models import Job, Jenkins
from rest_framework import viewsets
from inabox.serializers import UserSerializer, GroupSerializer, JobSerializer
from inabox.serializers import JenkinsSerializer
from rest_framework import status


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

    def create_job_xml(self, description, command):
        xml = """
<?xml version='1.0' encoding='UTF-8'?>
<project>
  <actions/>
  <description>%(description)s</description>
  <keepDependencies>false</keepDependencies>
  <properties/>
  <scm class=\"hudson.scm.NullSCM\"/>
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
        xml = (xml % ({'description': description, 'command': command}))
        xml = xml.strip()
        return xml

    def create_job(self, job):
        xml = self.create_job_xml(job.description, job.command)
        self.jay.create_job(job.name, xml)

    def update_job(self, job):
        xml = self.create_job_xml(job.description, job.command)
        self.jay.reconfig_job(job.name, xml)

    def get_jobs(self):
        return self.jay.get_jobs()

    def get_job(self, name):
        return self.jay.get_job_config(name)

    def build_job(self, job):
        self.jay.build_job(job.name)

    def delete_job(self, job):
        self.jay.delete_job(job.name)


class JenkinsViewSet(APIView):
    queryset = Jenkins.objects.all()
    serializer_class = JenkinsSerializer

    def find_job(self, name):
        for job in Job.objects.all():
            if job.name == name:
                return job
        return None

    def get(self, request, format=None):
        r = []
        for result in JenkinsController().get_jobs():
            if self.find_job(result['name']):
                r.append(result)
        return Response(r)

    def post(self, request, format=None):
        name = request.DATA.get('name', '')
        if not name:
            resp = Response()
            resp.status_code = status.HTTP_400_BAD_REQUEST
            content = 'Body must contain {"name": "job_name"}'
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        job = self.find_job(name)
        if not job:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found" % name
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        try:
            JenkinsController().create_job(job)
        except jenkins.JenkinsException as e:
            resp = Response()
            resp.status_code = status.HTTP_500_INTERNAL_SERVER_ERROR
            content = str(e)
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        resp = Response()
        resp.status_code = status.HTTP_201_CREATED
        content = "Job '%s' created" % name
        resp.content = ("<title>%s</title><code>%s" % (content, content))
        return resp


class JenkinsDetailViewSet(APIView):
    queryset = Jenkins.objects.all()
    serializer_class = JenkinsSerializer

    def find_job(self, name):
        for job in Job.objects.all():
            if job.name == name:
                return job
        return None

    def get(self, request, pk, format=None):
        job = self.find_job(pk)
        if not job:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found" % pk
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        s = JenkinsController().get_job(pk)
        if not s:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found on Jenkins" % pk
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        try:
            s = s.strip()
            xml = ElementTree.fromstring(s)
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
            command = ''
            description = s
            
        r = {}
        r['name'] = pk
        r['description'] = description
        r['command'] = command
        return Response(r)

    def put(self, request, pk, format=None):
        job = self.find_job(pk)
        if not job:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found" % pk
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        try:
            JenkinsController().update_job(job)
        except jenkins.JenkinsException as e:
            resp = Response()
            resp.status_code = status.HTTP_500_INTERNAL_SERVER_ERROR
            content = str(e)
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        resp = Response()
        resp.status_code = status.HTTP_201_CREATED
        content = "Job '%s' updated" % pk
        resp.content = ("<title>%s</title><code>%s" % (content, content))
        return resp

    def delete(self, request, pk, format=None):
        job = self.find_job(pk)
        if not job:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found" % pk
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        jay = JenkinsController()
        s = jay.get_job(pk)
        if not s:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found on Jenkins" % pk
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        try:
            jay.delete_job(job)
        except jenkins.JenkinsException as e:
            resp = Response()
            resp.status_code = status.HTTP_500_INTERNAL_SERVER_ERROR
            content = str(e)
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        resp = Response()
        resp.status_code = status.HTTP_201_CREATED
        content = "Job '%s' deleted" % pk
        resp.content = ("<title>%s</title><code>%s" % (content, content))
        return resp


class JenkinsBuildViewSet(APIView):
    queryset = Jenkins.objects.all()
    serializer_class = JenkinsSerializer

    def find_job(self, name):
        for job in Job.objects.all():
            if job.name == name:
                return job
        return None

    def get(self, request, pk, format=None):
        job = self.find_job(pk)
        if not job:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found" % pk
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        s = JenkinsController().get_job(pk)
        if not s:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found on Jenkins" % pk
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        try:
            s = s.strip()
            xml = ElementTree.fromstring(s)
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
            print str(e)
            command = ''
            description = s
            
        r = {}
        r['name'] = pk
        r['description'] = description
        r['command'] = command
        return Response(r)

    def put(self, request, pk, format=None):
        job = self.find_job(pk)
        if not job:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found" % pk
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        try:
            JenkinsController().update_job(job)
        except jenkins.JenkinsException as e:
            resp = Response()
            resp.status_code = status.HTTP_500_INTERNAL_SERVER_ERROR
            content = str(e)
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        resp = Response()
        resp.status_code = status.HTTP_201_CREATED
        content = "Job '%s' updated" % pk
        resp.content = ("<title>%s</title><code>%s" % (content, content))
        return resp

    def delete(self, request, pk, format=None):
        job = self.find_job(pk)
        if not job:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found" % pk
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        jay = JenkinsController()
        s = jay.get_job(pk)
        if not s:
            resp = Response()
            resp.status_code = status.HTTP_404_NOT_FOUND
            content = "Job '%s' not found on Jenkins" % pk
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        try:
            jay.delete_job(job)
        except jenkins.JenkinsException as e:
            resp = Response()
            resp.status_code = status.HTTP_500_INTERNAL_SERVER_ERROR
            content = str(e)
            resp.content = ("<title>%s</title>%s" % (content, content))
            return resp
        resp = Response()
        resp.status_code = status.HTTP_201_CREATED
        content = "Job '%s' deleted" % pk
        resp.content = ("<title>%s</title><code>%s" % (content, content))
        return resp
