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
