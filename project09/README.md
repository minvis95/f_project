# PJT09

> AJAX통신과 JSON 구조에 대한 이해



## 리뷰 좋아요 기능

- 다음은 community/views.py 의 like 함수이다.

```python
@require_POST
def like(request, review_pk):
    if request.user.is_authenticated:
        review = get_object_or_404(Review, pk=review_pk)
        user = request.user
		# 해당 게시글을 좋아하는 유저들중 특정 user가 존재하는지 확인
        if review.like_users.filter(pk=user.pk).exists():
            # 있으면 제거하고 check값 False
            review.like_users.remove(user)
            liked = False
        else:
            # 없으면 추가하고 check값 True
            review.like_users.add(user)
            liked = True
        # 값들을 할당하고 Json으로 응답한다.
        like_status = {
            'liked': liked,
            'count': review.like_users.count(),
        }
        return JsonResponse(like_status)
    return redirect('accounts:login')
```

- 다음은 community/templates/community/index.html 이다.

```django
{% extends 'base.html' %}

{% block content %}
  <h1>Community</h1>
  {% if user.is_authenticated %}
    <a href="{% url 'community:create' %}">NEW</a>
  {% else %}
    <a href="{% url 'accounts:login' %}">[새 글을 작성하려면 로그인하세요]</a>
  {% endif %}
  <hr>
  {% for review in reviews %}
    <p><b>작성자 : <a href="{% url 'accounts:profile' review.user.username %}">{{ review.user }}</a></b></p>
    <p>글 번호: {{ review.pk }}</p>
    <p>글 제목: {{ review.title }}</p>
    <p>글 내용: {{ review.content }}</p>
    {% if request.user.is_authenticated %}
    <form class="d-inline like-form" data-id="{{ review.pk }}">
      {% csrf_token %}
      {% if user in review.like_users.all %}
        <button class="btn btn-link">
          <i id="like-{{ review.pk }}" class="fas fa-heart fa-lg" style="color:crimson;"></i>
        </button>
      {% else %}
        <button class="btn btn-link">
          <i id="like-{{ review.pk }}" class="fas fa-heart fa-lg" style="color:black;"></i>
        </button>
      {% endif %}
    {% endif %}
    </form>
    <span id="like-count-{{ review.pk }}">{{ review.like_users.all|length }}</span>명이 이 글을 좋아합니다.<br>
    <a href="{% url 'community:detail' review.pk %}">[detail]</a>
    <hr>
  {% endfor %}
  <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
  <script>
    const forms = document.querySelectorAll('.like-form')
    const csrftoken = document.querySelector('[name=csrfmiddlewaretoken]').value

    forms.forEach(function (form) {
      form.addEventListener('submit', function (event) {
        event.preventDefault()
        const reviewId = event.target.dataset.id
        
        axios.post(`http://127.0.0.1:8000/community/${reviewId}/like/`, {},
          {headers: {'X-CSRFToken': csrftoken}
        }).then(function (response) {
          const liked = response.data.liked
          const count = response.data.count

          const likeButton = document.querySelector(`#like-${reviewId}`)
          likeButton.style.color = liked ? 'crimson' : 'black' 
          const likeCount = document.querySelector(`#like-count-${reviewId}`)
          likeCount.innerText = count
        })
      })
    })
  </script>
{% endblock %}
```



## 유저 팔로우 기능

- 다음은 accounts/views.py 의 follow 함수이다.

```python
@require_POST
def follow(request, user_pk):
    if request.user.is_authenticated:
        # 팔로우할 대상 인스턴스 생성
        you = get_object_or_404(get_user_model(), pk=user_pk)
        me = request.user
		# 자기자신은 팔로우 하면 안되므로 다음과 같이 조건문을 작성
        if you != me:
            if you.followers.filter(pk=me.pk).exists():
                you.followers.remove(me)
                # 팔로우 끊었다는 것을 check하기 위해 True, False 작성
                followed = False
            else:
                you.followers.add(me)
                followed = True
			# 팔로워와 팔로잉의 숫자를 상태dic에 할당
            follow_status = {
                'followers_count': you.followers.count(),
                'followings_count': you.followings.count(),
                'followed': followed,
            }
        # follow_status를 Json으로 response해준다.
        return JsonResponse(follow_status)
    return redirect('accounts:login')
```

- 다음은 accounts/templates/accounts/_follow.html 이다.

```django
<div class="jumbotron text-center text-white bg-dark">
  <p class="lead mb-1">작성자 정보</p>
  <h1 class="display-4">{{ person.username }}</h1>
  <hr>
  {% with followings=person.followings.all followers=person.followers.all %}
    <!-- querySelector을 하기 위해 id값을 세팅 -->
    <p class="lead" id="follow-count">
      팔로잉 : {{ followings|length }} / 팔로워 : {{ followers|length }}
    </p>
    <!-- 팔로우 버튼 -->
    {% if request.user != person %}
   	  <!-- querySelector을 하기 위해 id값을 세팅 -->
      <form id="follow-form" data-user-id="{{ person.pk }}">
        {% csrf_token %}
        {% if request.user in followers %}
          <button class="btn-secondary btn-lg" role="button">Unfollow</button>
        {% else %}
          <button class="btn-primary btn-lg" role="button">Follow</button>
        {% endif %}
      </form>
    {% endif %}
  {% endwith %}
</div>

<script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
<script>
  const form = document.querySelector('#follow-form')
  const csrftoken = document.querySelector('[name=csrfmiddlewaretoken]').value
  // 해당 form 태그가 submit이 될 때
  form.addEventListener('submit', function (event) {
    // 해당 이밴트를 막는다.
    event.preventDefault()
    const userId = event.target.dataset.userId
    // userId를 post로 follow함수로 보내서 팔로우할 대상의 정보를 Json으로 가지고 온다.
    axios({
      method: 'post',
      url: `/accounts/follow/${userId}/`,
      headers: { 'X-CSRFToken': csrftoken },
    })
    .then(response => {
      // 정보를 가지고 온다.
      const followersCount = response.data.followers_count
      const followingsCount = response.data.followings_count
      const followed = response.data.followed

      const followCount = document.querySelector('#follow-count')
      followCount.innerText = `팔로잉 : ${followingsCount} / 팔로워 : ${followersCount}`

      const followBtn = document.querySelector('#follow-form > button')
	  // followed는 True or False로 설정되어 있다.
      if (followed) {
        followBtn.innerText = 'Unfollow'
      } else {
        followBtn.innerText = 'Follow'
      }
    })
  })
</script>
```



## 추천 알고리즘 작성

- Random으로 10개의 데이터를 `movies/recommended.html`로 render 하려고 한다. 다음은 movies/views.py 의 recommended이다.

```python
import random
@require_GET
def recommended(request):
    # Movie모델의 모든 데이터값을 가지고 온다.
    movies = Movie.objects.all()
    # 현재 movies는 쿼리셋이므로 list로 바꾸고 10개를 샘플링하고 변수에 할당한다.
    randmovies = random.sample(list(movies), 10)
    context = {
        'randmovies': randmovies,
    }
    return render(request, 'movies/recommended.html', context)
```

- 다음은 recommended.html이다. bootstrap cards를 이용하였다.

```django
{% extends 'base.html' %}

{% block content %}
  <h1>Recommended</h1>

<div class="row row-cols-1 row-cols-md-3 g-4">
  {% for movie in randmovies %}
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
{% endblock %}
```



## 마무리

좋아요, 팔로우 기능을 구현하므로써 데이터 관계에 대해서 다시 한번 복습할 수 있었고, JS를 사용하여 AJAX통신과 JSON 구조에 대해 확실히 더 잘 이해할 수 있었다. 추천 알고리즘을 구현하기 위해서 모든 데이터값을 불러와서 샘플링하는 것에서 시간을 많이 소비하였다. 그 이유는 random.sampe()은 쿼리셋을 샘플링할 수 없었던 것이다. 따라서 list로 변환시켜주는 작업이 필요하였다. `movies = Movie.objects.all()` 를 통해서 쿼리셋을 가지고 온다는 것을 놓치고 있었던 것이다. 좋은 팀원과 만나 소통을 통해 어려움을 해결해 나가는 협업을 통해 수월하게 프로젝트를 완성할 수 있었다.