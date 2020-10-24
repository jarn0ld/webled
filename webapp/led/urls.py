from django.conf.urls import url

from . import views

urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'^api/set_color/([0-9]+)/([0-9]+)/([0-9]+)/$', views.api_set_color, name='api_set_color'),
    url(r'^api/off/$', views.api_off, name='api_off'),
]
