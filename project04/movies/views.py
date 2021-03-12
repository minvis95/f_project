from django.shortcuts import render, redirect
from .models import Movie

# Create your views here.
def index(request) :
    movies = Movie.objects.all()

    context = {
        'movies' : movies,
    }

    return render(request, 'movies/index.html', context)

def new(request) :
    return render(request,'movies/new.html')

def create(request) :
    title = request.POST.get('title')
    overview = request.POST.get('overview')
    poster_path = request.POST.get('poster_path')

    movie = Movies(title=title, overview=overview, poster_path=poster_path)
    movie.save()

    return redirect('movies:detail', movie.pk) # 영화세부정보로 이동

def detail(request, pk) :
    movie = Movie.objects.get(pk=pk)
    context ={
        "movie" : movie,
    }
    return render(request, 'movies/detail.html', context)

