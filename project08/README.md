# 데이터베이스 설계를 통한 REST API 설계



## Serializer

> 다음은 serializers.py이다.

- `MovieSerializer` 에서 해당 movie의 review내용과 개수와 해당 review의 comment내용과 개수를 보여준다.

```python
from rest_framework import serializers
from .models import Movie, Review, Comment

class MovieListSerializer(serializers.ModelSerializer):
    class Meta:
        model = Movie
        fields = ('title', 'overview', 'poster_path')

class ReviewSerializer(serializers.ModelSerializer):
    class Meta:
        model = Review
        fields = '__all__'
        read_only_fields = ('movie',)

class CommentSerializer(serializers.ModelSerializer):
    class Meta:
        model = Comment
        fields = '__all__'
        read_only_fields = ('review',)

class MovieSerializer(serializers.ModelSerializer):
    review_set = ReviewSerializer(many=True, read_only=True)
    review_count = serializers.IntegerField(source='review_set.count', read_only=True)
	
    comment_set = CommentSerializer(many=True, read_only=True)
    comment_count = serializers.IntegerField(source='comment_set.count', read_only=True)

    class Meta:
        model = Movie
        fields = '__all__'
```



## URL

- RESTful하게 작성하기 위해 노력하였다. 다음은 urls.py이다.

```python
from django.urls import path
from . import views

urlpatterns = [
    # 영화 전체 조회
    path('', views.movie_list),
    # 영화 개별 조회
    path('<int:movie_pk>/', views.movie_detail),
    # 리뷰 전체 조회
    path('reviews/', views.review_list),
    # 리뷰 각각 관리
    path('reviews/<int:review_pk>/', views.review_detail),
    # 리뷰 생성
    path('<int:movie_pk>/reviews/', views.review_create),
    # 댓글 생성
    path('<int:review_pk>/comments/', views.comment_create),
    # 댓글 전체 조회
    path('comments/', views.comment_list),
    # 댓글 각각 조회
    path('comments/<int:comment_pk>/', views.comment_detail),
]

```



## Movie, Review, Comment

- 딱히 어려운부분은 없었다. 수업때 배운 로직 그대로 작성하려고 노력하였다. 다음은 views.py이다.

```python
from rest_framework.response import Response
from rest_framework.decorators import api_view
from rest_framework import status
from drf_yasg.utils import swagger_auto_schema

from django.shortcuts import render, get_list_or_404, get_object_or_404
from .models import Movie, Review, Comment
from .serializers import MovieListSerializer, ReviewSerializer, CommentSerializer, MovieSerializer

# Create your views here.
@api_view(['GET', 'POST'])
def movie_list(request):
    if request.method == 'GET':
        movies = get_list_or_404(Movie)
        serializer = MovieListSerializer(movies, many=True)
        return Response(serializer.data)
    elif request.method == 'POST':
        # 생성
        serializer = MovieSerializer(data=request.data)
        if serializer.is_valid(raise_exception=True):
            serializer.save()
            return Response(serializer.data, status=status.HTTP_201_CREATED)

@api_view(['GET', 'DELETE', 'PUT'])
def movie_detail(request, movie_pk):
    movie = get_object_or_404(Movie, pk=movie_pk)
    if request.method == "GET":
        serializer = MovieSerializer(movie)
        return Response(serializer.data)
    elif request.method == "DELETE":
        movie.delete()
        data = {
            'delete': f'{movie_pk}번 영화가 삭제되었습니다.'
        }
        return Response(data, status=status.HTTP_204_NO_CONTENT)
    elif request.method == "PUT":
        serializer = MovieSerializer(instance=movie, data=request.data)
        if serializer.is_valid(raise_exception=True):
            serializer.save()
            return Response(serializer.data)

@api_view(['GET'])
def review_list(request):
    reviews = get_list_or_404(Review)
    serializer = ReviewSerializer(reviews, many=True)
    return Response(serializer.data)

@api_view(['GET', 'DELETE', 'PUT'])
def review_detail(request, review_pk):
    review = get_object_or_404(Review, pk=review_pk)
    if request.method == 'GET':
        serializer = ReviewSerializer(review)
        return Response(serializer.data)
    elif request.method == 'DELETE':
        review.delete()
        data = {
            'delete': f'리뷰 {review_pk}번이 삭제 되었습니다.'
        }
        return Response(data, status=status.HTTP_204_NO_CONTENT)

    elif request.method == 'PUT':
        serializer = ReviewSerializer(instance=review, data=request.data)
        if serializer.is_valid(raise_exception=True):
            serializer.save()
            return Response(serializer.data)

@api_view(['POST'])
def review_create(request, movie_pk):
    movie = get_object_or_404(Movie, pk=movie_pk)
    serializer = ReviewSerializer(data=request.data)
    if serializer.is_valid(raise_exception=True):
        serializer.save(movie=movie)
        return Response(serializer.data, status=status.HTTP_201_CREATED)

@api_view(['GET'])
def comment_list(request):
    comments = get_list_or_404(Comment)
    serializer = CommentSerializer(comments, many=True)
    return Response(serializer.data)

@api_view(['GET', 'DELETE', 'PUT'])
def comment_detail(request, comment_pk):
    comment = get_object_or_404(Comment, pk=comment_pk)
    if request.method == 'GET':
        serializer = CommentSerializer(comment)
        return Response(serializer.data)
    elif request.method == 'DELETE':
        comment.delete()
        data = {
            'delete': f'댓글 {comment_pk}번이 삭제 되었습니다.'
        }
        return Response(data, status=status.HTTP_204_NO_CONTENT)

    elif request.method == 'PUT':
        serializer = CommentSerializer(instance=comment, data=request.data)
        if serializer.is_valid(raise_exception=True):
            serializer.save()
            return Response(serializer.data)

@api_view(['POST'])
def comment_create(request, review_pk):
    review = get_object_or_404(Review, pk=review_pk)
    serializer = CommentSerializer(data=request.data)
    if serializer.is_valid(raise_exception=True):
        serializer.save(review=review)
        return Response(serializer.data, status=status.HTTP_201_CREATED)
```



## 결과

- 단일영화 정보를 제공

![1_1_A](README.assets/1_1_A.PNG)

- 전체영화 정보를 제공

![1_1_B](README.assets/1_1_B.PNG)

- 리뷰를 생성

![2_1_A](README.assets/2_1_A.PNG)

- 리뷰정보를 반환 (전체, 각각)

![2_1_B](README.assets/2_1_B.PNG)

![2_1_B2](README.assets/2_1_B2.PNG)

- 리뷰를 수정

![2_1_C](README.assets/2_1_C.PNG)

- 리뷰를 삭제

![2_1_D](README.assets/2_1_D.PNG)

- 댓글을 생성

![3_1_A](README.assets/3_1_A.PNG)

- 댓글정보를 반환

![3_1_B](README.assets/3_1_B.PNG)



## 마무리

project를 수행하면서 이번주에 배웠던 REST framework를 복습하는 것도 중요한 의미이긴 하나, 협업 또한 중요한 의미가 있었다. 협업을 통해서 코드작성의 흐름, 코드작성할 때의 실수하는 부분, 잊어버렸던 내용을 더 확실하고 빠르게 서로 극복해 나아가면서 완성도가 높은 프로젝트를 수행할 수 있다는 것이다.  특히 협업을 하기 위해 branch와 merge를 사용하였는데, 처음에는 어려웠으나 익숙해지니 엄청 편했던 것 같다. RESTful한 url를 작성할 수 있었고, serializer를 익숙하게 다룰 수 있었다.