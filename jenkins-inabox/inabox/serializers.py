from django.contrib.auth.models import User, Group
from models import Job, Jenkins
from rest_framework import serializers


class UserSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = User
        fields = ('url', 'username', 'email', 'groups')


class GroupSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Group
        fields = ('url', 'name')


class JobSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Job
        fields = ('name', 'description', 'command')


class JenkinsSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Jenkins
        fields = ('name', 'description', 'command')
