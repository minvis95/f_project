# Python을 활용한 데이터 수집2

> (중요) 요구사항을 준수하여 구현.
> A~E 까지 구현을 하였을 때 어려웠던 점을 정리.
> README에는 코드 전체를 포함하고 있지않습니다. 즉, 코드의 일부분만 포함



## A. 영화 개수 카운트 기능 구현

1. key값이 있는 URL 생성
2. URL에 required와 optional 추가
3. 요청 보내서 받은 값을 res에 저장
4. json화 시켜
5. popular인 영화의 갯수를 return함(아래 코드와 같다.)

```python
return len(movie_dict.get('results'))
```



## B. 특정 조건에 맞는 영화 출력

1. A에 1~4번 과정을 수행한다.
2. 반복문을 돌려 평점이 8이상이면 결과 리스트에 `append`하는 방식으로 수행하였다.

```python
movie_dict = res.json()

# dict of dict이므로 dict을 뽑음
for movie in movie_dict.get('results'):
    # 그 dict타입인 movie의 평점이 8이상이야??
    if movie.get('vote_average') >= 8:
        # Then 그 영화 제목을 result에 추가
        result_list.append(movie.get('title'))
```

## C. 평점 순 정렬

1. A에 1~4번 과정을 수행한다.
2. 평점과, 제목을 튜플로 묵어 `rank_list`에 추가
3. 정렬시킨다.(내림차순으로)
4. 상위 5개만 뽑아야 하므로 내림차순으로 된 list에서 0~4번째의 제목을 result에 추가함

- 어려웠던 점
  - 튜플로 이루어진 리스트를 정렬할 때 튜플의 첫번째 원소를 기준으로 정렬한다. 만약에 0번재와 1번째의 튜플의 첫번째 원소가 같다면 두번째 원소를 비교하며 정렬.(밑에 코드와 같다.)

```python
# dict of dict이므로 dict을 뽑음
for movie in movie_dict.get('results'):
    # 평점과, 제목을 튜플로 묵어 list에 추가
    rank_list.append((movie.get('vote_average'), movie.get('title')))
# 정렬을 시킴.
# 튜플로 이루어진 리스트를 정렬할 때 튜플의 첫번째 원소를 기준으로 정렬
# 만약에 0번재와 1번째의 튜플의 첫번째 원소가 같다면 두번째 원소를 비교하며 정렬
rank_list.sort(reverse = -1)
```

```python
# 상위 5개만 뽑아야 하므로 내림차순으로 된 list에서 0~4번째의 제목을
# result에 추가함
for idx, movie in enumerate(rank_list):
    if idx < 5:
        result.append(movie[1])
        continue
    break
```



## D. 제목 검색, 영화 추천

1. A에 1~4번 과정을 수행과 비슷하나 다른 점은 인스턴스 메서드인 get_url()을 쓴다는 점이다. 다음과 같이 id값을 받아오고 url 값을 가지고 왔다.

```python
# 그 영화의 id값을 뽑아와서 ch에 저장
ch = maker.movie_id(title)
# recommendation에 상세주소를 적으므로써 url을 얻어냄
url = maker.get_url('movie', f'{ch}/recommendations', language='ko')
```

2.  이상한 title로 검색하였을 때 `None`을 반환.

```python
if movie_dict.get('success') == False:
    return None
```

3. title을 뽑아 낸다.

```python
# results에 해당되는 value를 뽑아냄
movie_list = movie_dict.get('results')
# list를 돌림. 이때 movie는 dict타입임.
for movie in movie_list:
    result.append(movie.get('title'))
```

- 어려웠던 점
  - 인스턴스 함수 get_url()이 tmdb.py에 있었던 것을 까먹어버리고 또 다시 구현해 버리는 실수를 했다. -> 명세서 꼼꼼히 읽자...



## E. 배우, 감독 리스트 출력

1. D의 과정처럼 id값과 그에 대한 주소값을 이용한다.
2. D의 2번 과정을 수행한다.
3. cast와 crew에 해당되는 각각의 value를 뽑음

```python
# (참고)movie_dict = res.json()
# cast와 crew에 해당되는 각각의 value를 뽑음. 이때 value는 list로 되어있음
movie_list = movie_dict.get('cast')
movie_list2 = movie_dict.get('crew')
```

4. cast_id에 해당되는 value가 10 이하면 list에 그 배우이름 추가

```python
# movie_list에서 하나씩 뽑음. 이때 person은 dict타입임
for person in movie_list:
    # dict 타입 person에서 cast_id에 해당되는 value가 10 이하면
    if person.get('cast_id') < 10:
        # list에 그 배우이름 추가
        actor_list.append(person.get('name'))
```

5. 위와 비슷한 방법으로 감독인 사람이름만 뽑음

```python
for person2 in movie_list2:
    # dict 타입 person에서 감독인 사람만 뽑음
    if person2.get('department') == 'Directing':
        # 그사람 list에 추가
        director_list.append(person2.get('name'))
```

6. cast, crew 두개의 key를 가지고 쌍의 맞는 list를 value에 넣음

```python
result.update({'cast': actor_list, 'crew': director_list})
```



## 후기

- 명세서 꼼꼼히 읽기.
- 튜플로 이루어진 list를 sort할 때 어떻게 되는지.
- 다시 강조하지만 명세서 꼼꼼히 읽기