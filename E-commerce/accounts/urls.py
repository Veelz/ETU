from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^register/$', views.register, name='register'),
    url(r'^account/$', views.account, name='account'),
    url(r'^login/$', views.login, name='login'),
    url(r'^logout/$', views.logout, name='logout'),
    # url(r'^order_details/(?P<order_id>[\d]+)/$', views.order_details, name='order_details'),
]
