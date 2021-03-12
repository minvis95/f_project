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
   - .gitignore 생성과 movies.json을 fixtures 파일에 넣기 와 pjt04/settings 와 pjt04/urls 에서의 app등록, 경로 등등을 작성한다.



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



## base.html

- `bootstrap`에서 `Nav` 코드를 이용하였다. 그리고 Navbar에서 index.html로 올 수 있는 `전체목록조회` 와 새로운 게시글을 create 할 수 있는 `NEW`를 만들었다.



## index

1. 다음은 views.py에서의 index 함수 이다. 데이터베이스의 해당되는 데이터를 모두 가지고 온다.

```python
def index(request) :
    movies = Movie.objects.all()
    context = {
        'movies' : movies,
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
{% endblock content %}
```



## Detail

1. 다음은 views.py에서 detail 함수이다. `pk`값이 input으로 들어고 그에 해당되는 데이터를 뽑아온다.

```python
def detail(request, pk) :
    movie = Movie.objects.get(pk=pk)
    context ={
        "movie" : movie,
    }
    return render(request, 'movies/detail.html', context)
```

2. 다음은 detail.html이다. movie 인스턴스의 이미지, 제목, 줄거리를 출력할 수 있게 코드를 작성하였다.

```django
{% extends 'base.html' %}
{% block content %}
<img src="{{ movie.poster_path }}" alt="사진">
<h1> 제목 : {{ movie.title }} </h1>
<p> 줄거리 : {{ movie.overview }} </p>
{% endblock content %}
```



## NEW

1. view.py에서의 new함수는 new.html을 render해주기만 한다.
2. 다음은 new.html 이다. `form` 태그를 사용하여 각각의 name으로 값을 입력 받고 `POST` 매소드를 사용하여 namespace가 `create`인 url로 보내준다.

```django
{% extends 'base.html' %}
{% block content %}
<form action="{% url 'movies:create' %}" method="POST">
  {% csrf_token %}
  <p>
    <label for="title">제목: </label>
    <input type="text" id="title" name="title">
  </p>
  <p>
    <label for="overview"></label>
    <textarea name="overview" id="overview" cols="30" rows="10"></textarea>
  </p>
  <p>
    <label for="poster_path"></label>
    <input type="text" id="poster_path" name="poster_path">
  </p>
  <input type="submit" value="제출">
</form>
{% endblock content %}
```

3. 다음은 views.py에서의 create 함수이다. `request`에서 `POST`공간에 우리가 원하는 영화 제목, 줄거리, 이미지경로가 저장되어 있다. 따라서 `get`해준다. 마지막으로 데이터베이스의 그 값들을 저장해주고 namespace가 `detail`인 url로 `pk`값과 함께 `redirect` 해주면서 새롭게 추가된 게시글의 detail을 보여준다.

```python
from django.shortcuts import 
def create(request) :
    title = request.POST.get('title')
    overview = request.POST.get('overview')
    poster_path = request.POST.get('poster_path')

    movie = Movies(title=title, overview=overview, poster_path=poster_path)
    movie.save()

    return redirect('movies:detail', movie.pk)render, redirect
```



## 마무리

1시간 정도의 project시간이 주어졌다. 다행히도 Read와 Create만 하는 것이라서 다행이였다. project를 수행하면서 이번주에 배웠던 django를 복습하는 것도 중요한 의미이긴 하나, 협업 또한 중요한 의미가 있었다. 15분간격으로 서로 역할을 바꾸어 진행하였는데, 예전에 했던 프로젝트보다 더 효율적이었다고 느꼈다. 그 이유는 코드작성의 흐름, 코드작성할 때의 실수하는 부분, 잊어버렸던 내용을 더 확실하고 빠르게 극복하면서 완성도가 높은 프로젝트를 수행할 수 있다는 것이다. 이번 계기로 페어 프로그래밍에서의 드라이버와 네비게이터의 중요성을 알 수 있었던 시간이었다.