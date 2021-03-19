# 프레임워크 기반 웹페이지 구현

> `Python 3.8.7`, `Django 3.1.7`, `asgiref 3.3.1`, `pytz 2021.1`, `sqlparse 0.4.1`



## 기본 설정

1. bash 창에 `venv` 모듈을 실행시킨다. 파일이름은 venv로 동일하게 한다.

```bash
$ python -m venv venv
```

2. `가상환경`을 실행

```bash
$ source venv/Scripts/activate
```

3. `django`를 설치한다.

```bash
$ pip install django
```

4. `requirements`를 작성

```bash
$ pip freeze > requirements
```

5. 기타
   - .gitignore 생성과 movies.json을 fixtures 파일에 넣기 와 pjt05/settings 와 pjt05/urls 에서의 app등록, 경로 등등을 작성한다.



## Model

> 데이터베이스에 movie.json의 내용을 저장하기 위해서는 models.py에서 클래스를 먼저 작성해야 한다.

1. `class` 작성

```python
from django.db import models

class Movie(models.Model) :
    title = models.CharField(max_length=100) 
    overview = models.TextField()
    poster_path = models.CharField(max_length=500)
```

2. `makemigrations` 와 `migrate`
   - 설계도를 만들고 데이터표를 만들어보자

```bash
$ python manage.py makemigrations
$ python manage.py migrate
```

3. `movies.json`의 데이터를 데이터베이스로 `load` 시킨다.

```bash
$ python manage.py loaddata movies/movies.json
```



## Forms

- 다음은 `forms.py`의 `MovieForm` 클레스이다. Movie 모델의 모든 field들을 보여준다.

```python
from django import forms
from .models import Movie

class MovieForm(forms.ModelForm):
    class Meta():
        model = Movie
        fields = '__all__'
```



## base.html

- `bootstrap`에서 `Nav` 코드를 이용하였다. 그리고 Navbar에서 index.html로 올 수 있는 `전체목록조회` 와 새로운 게시글을 create 할 수 있는 `NEW`를 만들었다.



## index

1. 다음은 views.py에서의 index 함수 이다. 데이터베이스의 해당되는 데이터를 모두 가지고 온다.

```python
@require_safe
def index(request):
    movies = Movie.objects.all()[::-1]
    context = {
        'movies': movies,
    }
    return render(request, 'movies/index.html', context)
```

2. 다음은 index.html 이다. `bootstrap`에서 components/card/ 부분에 grid 두번째 코드를 사용하였다. for 반복문을 이용하여 image와 title을 보여준다. 만약 클릭을 한다면 detail.html로 이동할 수 있게 pk를 넘겨주는 코드를 작성하였다.

```django
{% extends 'base.html' %}

{% block content %}
<div class="row row-cols-1 row-cols-md-3 g-4">
  {% for movie in movies %}
    <div class="col">
      <div class="card">
        <a href="{% url 'movies:detail' movie.pk %}">
          <img src="{{ movie.poster_path }}" class="card-img-top" alt="사진">
        </a>
        <div class="card-body">
          <a href="{% url 'movies:detail' movie.pk %}">
            <h5 class="card-title">{{ movie.title }}</h5>
          </a>
          <p class="card-text">{{ movie.content }}</p>
        </div>
      </div>
    </div>
  {% endfor %}
</div>  

{% endblock  %}

```



## Detail

1. 다음은 views.py에서 detail 함수이다. `pk`값이 input으로 들어고 그에 해당되는 데이터를 뽑아온다.

```python
@require_safe
def detail(request, pk):
    movie = get_object_or_404(Movie, pk=pk)
    context = {
        'movie': movie,
    }
    return render(request, 'movies/detail.html', context)
```

2. 다음은 detail.html이다. movie 인스턴스의 이미지, 제목, 줄거리를 출력할 수 있게 코드를 작성하였다. 또한 수정과 삭제 영화목록조회를 위해 back도 추가해 주었다.

```django
{% extends 'base.html' %}
{% block content %}
<img src="{{ movie.poster_path }}" alt="사진이용">
<p>{{ movie.title }}</p>
<p>{{ movie.overview }}</p>

<a href="{% url 'movies:update' movie.pk %}">수정</a>

<form action="{% url 'movies:delete' movie.pk %}" method="POST">
  {% csrf_token %}
  <input type="submit" value="삭제">
</form>
<a href="{% url 'movies:index' %}">BACK</a>
{% endblock  %}
```



## Create

1. view.py에서의 new함수는 create.html을 render해주기만 한다.
2. 다음은 create.html 이다. `ModelForm`을 사용하여 필드들을 보여주었다.

```django
{% extends 'base.html' %}
{% load bootstrap5 %}

{% block content %}
<form action="" method="POST" enctype="multipart/form-data">
  {% csrf_token %}
  {% bootstrap_form form %}
  <input type="submit">
</form>
{% endblock  %}
```

3. 다음은 views.py에서의 create 함수이다. 메서드가 `GET`이면 ModelForm을 포함해  create.html로 render해주었다. `POST`이면 유효성검사와 함께 저장해주고 해당 pk의 detail을 보여주기 위해 namespace가 detail인 곳으로 이동한다.

```python
@require_http_methods(['GET', 'POST'])
def create(request):
    if request.method == 'POST':
        form = MovieForm(request.POST)
        if form.is_valid():
            movie = form.save()
            return redirect('movies:detail', movie.pk)
    else:
        form = MovieForm()
    context = {
        'form': form,
    }

    return render(request, 'movies/create.html', context)
```



## Update

1. 다음은 `views.py`의 update 함수이다. create함수와 비슷하나, 수정되기를 원하는 field의 pk값을 이용해 해당 row를 수정하고 유효성검사와 저장을 한다. 초기상태는 이전에 저장된 값에서 수정되기를 원하기에 `form = MovieForm(instance=movie)`를 추가한다는 것이다.

```python
@require_http_methods(['GET', 'POST'])
def update(request, pk):
    movie = get_object_or_404(Movie, pk=pk)
    if request.method == 'POST':
        form = MovieForm(request.POST, instance=movie)
        if form.is_valid():
            form.save()
            return redirect('movies:detail', movie.pk)
    else:
        form = MovieForm(instance=movie)

    context = {
        'form': form,
        'movie': movie,
    }
    return render(request, 'movies/update.html', context)
```

2. 다음은 update.html 이다. create.html과 동일하다.

```django
{% extends 'base.html' %}
{% load bootstrap5 %}
{% block content %}

<form action="" method="POST" enctype="multipart/form-data">
  {% csrf_token %}
  {% bootstrap_form form %}
  <input type="submit">
</form>
{% endblock  %}
```



## 마무리

project를 수행하면서 이번주에 배웠던 django를 복습하는 것도 중요한 의미이긴 하나, 협업 또한 중요한 의미가 있었다. 협업을 통해서 코드작성의 흐름, 코드작성할 때의 실수하는 부분, 잊어버렸던 내용을 더 확실하고 빠르게 서로 극복해 나아가면서 완성도가 높은 프로젝트를 수행할 수 있다는 것이다. 