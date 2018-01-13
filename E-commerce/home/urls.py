from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^$', views.homepage, name='homepage'),
    url(r'^category/(?P<cat_id>\d+)/$', views.category, name='category'),
    url(r'^category/(?P<cat_id>\d+)/page/(?P<page_number>\d+)/$', views.category, name='cat_page'),
    url(r'^search/$', views.search, name='search'),
]
