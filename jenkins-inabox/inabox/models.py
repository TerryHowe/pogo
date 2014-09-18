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
    name = models.CharField(max_length=100, blank=False, primary_key=True)
    description = models.CharField(max_length=100, blank=True, default='')
    command = models.TextField()

    @classmethod
    def find_job(cls, name):
        try:
            return cls.objects.get(name=name)
        except:
            pass
        return None


class Build(models.Model):
    job = models.ForeignKey(Job)
    number = models.CharField(max_length=20, blank=False, primary_key=True)
    building = models.BooleanField()
    result = models.CharField(max_length=20, blank=False)
    duration = models.CharField(max_length=20, blank=False)
    timestamp = models.CharField(max_length=20, blank=False)
    console = models.TextField()
