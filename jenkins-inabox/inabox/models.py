from django.db import models


class Job(models.Model):
    name = models.CharField(max_length=100, blank=False, primary_key=True)
    description = models.CharField(max_length=100, blank=True, default='')
    gitrepo = models.CharField(max_length=100, blank=True, default='')
    command = models.TextField()
    build_number = models.CharField(max_length=20, blank=True)

    @classmethod
    def find_job(cls, name):
        try:
            return cls.objects.get(name=name)
        except:
            pass
        return None


class Build(models.Model):
    name = models.CharField(max_length=100, blank=False, primary_key=True)
    command = models.TextField()
    build_number = models.CharField(max_length=20, blank=False)
    building = models.BooleanField()
    result = models.CharField(max_length=20, blank=False)
    duration = models.CharField(max_length=20, blank=False)
    timestamp = models.CharField(max_length=20, blank=False)
    console = models.TextField()
