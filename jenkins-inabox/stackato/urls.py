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
build_start = views.JenkinsViewSet.as_view({'get': 'start'})
build_stop = views.JenkinsViewSet.as_view({'get': 'stop'})
build_finish = views.JenkinsViewSet.as_view({'get': 'finish'})
build_status = views.JenkinsViewSet.as_view({'get': 'status'})

# Wire up our API using automatic URL routing.
# Additionally, we include login URLs for the browseable API.
urlpatterns = [
    url(r'^', include(router.urls)),
    url(r'^jobs/$', job_list),
    url(r'^jobs/(?P<pk>[^/.]+)/$', job_detail),
    url(r'^jobs/(?P<pk>[^/.]+)/start/$', build_start),
    url(r'^jobs/(?P<pk>[^/.]+)/stop/$', build_stop),
    url(r'^jobs/(?P<pk>[^/.]+)/finish/$', build_finish),
    url(r'^jobs/(?P<pk>[^/.]+)/status/$', build_status),
    url(r'^api-auth/', include('rest_framework.urls', namespace='rest_framework')),
    url(r'^static\/(?P<path>.*)$>', serve),
]

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()
