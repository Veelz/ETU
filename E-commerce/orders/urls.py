from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^update_cart/$', views.update_cart, name='update_cart'),
    url(r'^checkout/$', views.checkout, name='checkout'),
]

"""
    url(r'^$', views.post_list, name='post_list'),
    url(r'^post/(?P<pk>[0-9]+)/$', views.post_detail, name='post_detail'),
    url(r'^post/new/$', views.post_new, name='post_new'),
    url(r'^post/(?P<pk>[0-9]+)/edit/$', views.post_edit, name='post_edit'),
    """
