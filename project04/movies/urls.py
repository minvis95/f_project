from django.urls import path
from . import views


app_name = 'movies'
urlpatterns = [
    path('', views.index, name="index"),
    path('new/', views.new, name="new"),
    path('detail/<int:pk>/', views.detail, name="detail"),
    path('create/', views.create, name="create"),
]