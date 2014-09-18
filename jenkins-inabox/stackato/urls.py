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
job_start = views.JenkinsViewSet.as_view({'get': 'start'})
job_stop = views.JenkinsViewSet.as_view({'get': 'stop'})
job_finish = views.JenkinsViewSet.as_view({'get': 'finish'})
job_status = views.JenkinsViewSet.as_view({'get': 'status'})
job_builds = views.JenkinsViewSet.as_view({'get': 'builds'})
job_build_info = views.JenkinsViewSet.as_view({'get': 'build_info'})

# Wire up our API using automatic URL routing.
# Additionally, we include login URLs for the browseable API.
urlpatterns = [
    url(r'^', include(router.urls)),
    url(r'^jobs/$', job_list),
    url(r'^jobs/(?P<pk>[^/.]+)/$', job_detail),
    url(r'^jobs/(?P<pk>[^/.]+)/start/$', job_start),
    url(r'^jobs/(?P<pk>[^/.]+)/stop/$', job_stop),
    url(r'^jobs/(?P<pk>[^/.]+)/finish/$', job_finish),
    url(r'^jobs/(?P<pk>[^/.]+)/status/$', job_status),
    url(r'^jobs/(?P<pk>[^/.]+)/builds/$', job_builds),
    url(r'^jobs/(?P<pk>[^/.]+)/builds/(?P<number>[^/.]+)/$', job_build_info),
    url(r'^api-auth/', include('rest_framework.urls', namespace='rest_framework')),
    url(r'^static\/(?P<path>.*)$>', serve),
]

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()
