from django.conf.urls import url, include
from rest_framework import routers
from inabox import views

router = routers.DefaultRouter()
router.register(r'users', views.UserViewSet)
router.register(r'groups', views.GroupViewSet)
router.register(r'jobs', views.JobViewSet)

# Wire up our API using automatic URL routing.
# Additionally, we include login URLs for the browseable API.
urlpatterns = [
    url(r'^', include(router.urls)),
    url(r'^jenkins/$', views.JenkinsViewSet.as_view()),
    url(r'^jenkins/(?P<name>[0-9]+)/$', views.JenkinsDetailViewSet.as_view()),
    url(r'^api-auth/', include('rest_framework.urls', namespace='rest_framework'))
]

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()
