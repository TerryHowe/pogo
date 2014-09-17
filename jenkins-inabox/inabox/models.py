from django.db import models

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


class Job(models.Model):
    name = models.CharField(max_length=100, blank=False, default='', unique=True)
    description = models.CharField(max_length=100, blank=True, default='')
    command = models.TextField()

class Jenkins(models.Model):
    name = models.CharField(max_length=100, blank=False, default='', unique=True)
    description = models.CharField(max_length=100, blank=True, default='')
    disabled = models.CharField(max_length=100, blank=True, default='')
    command = models.TextField()
