from django.contrib import admin
from .models import Movie

# Register your models here.
# user 만들기 
# python managy.py createuser # 아이디 : admin 비번: admin
class MovieAdmin(admin.ModelAdmin) :
    list_display = ('title', 'overview', 'poster_path' )
admin.site.register(Movie, MovieAdmin)
