"""park URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.11/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url
from django.contrib import admin
from parkxes import views
urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^login/',views.loginIndex),
    url(r'^register/', views.register),
    url(r'^add_user/', views.add_user_new),
    url(r'^check_user/', views.check_user),
    url(r'^logout/', views.logoutIndex),
    url(r'^dashboard/', views.dashboard),
    url(r'^add_parking_location/', views.add_parking_location),
    url(r'^edit', views.editor),
    url(r'^add_lots/', views.add_lots),
    url(r'^add_camera/', views.add_camera),
    url(r'^user/', views.userArea),
    url(r'^verify/', views.book),
    url(r'^new_user/', views.new_user),
    url(r'^add_caruser/', views.add_caruser),
    url(r'^intro', views.intro),
    url('', views.index),

]
