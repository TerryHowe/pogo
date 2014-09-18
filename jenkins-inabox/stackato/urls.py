from django.contrib.staticfiles.views import serve
from django.conf.urls import url, include
from rest_framework import routers
from inabox import views

router = routers.DefaultRouter()
router.register(r'users', views.UserViewSet)
router.register(r'groups', views.GroupViewSet)

job_list = views.JobViewSet.as_view({
    'get': 'list',
    'post': 'create',
})
job_detail = views.JobViewSet.as_view({
    'get': 'retrieve',
    'put': 'update',
    'patch': 'partial_update',
    'delete': 'destroy',
})
jenkins_list = views.JenkinsViewSet.as_view({
    'get': 'list',
    'post': 'create',
})
jenkins_detail = views.JenkinsViewSet.as_view({
    'get': 'retrieve',
    'put': 'update',
    'delete': 'destroy',
})
jenkins_list = views.BuildViewSet.as_view({
    'get': 'list',
    'post': 'create',
})

# Wire up our API using automatic URL routing.
# Additionally, we include login URLs for the browseable API.
urlpatterns = [
    url(r'^', include(router.urls)),
    url(r'^jenkins/$', jenkins_list),
    url(r'^jenkins/(?P<pk>[^/.]+)/$', jenkins_detail),
    url(r'^jobs/$', job_list),
    url(r'^jobs/(?P<pk>[^/.]+)/$', job_detail),
    url(r'^jobs/(?P<pk>[^/.]+)/build/$', views.BuildViewSet.as_view()),
    url(r'^api-auth/', include('rest_framework.urls', namespace='rest_framework')),
    url(r'^static\/(?P<path>.*)$>', serve),
]

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()
