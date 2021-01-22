# Python을 활용한 데이터 수집

> (중요) 요구사항을 준수하여 구현.
>
> A~E 까지 구현을 하였을 때, 쓰이는 문법이나 어려웠던 점을 정리하였다.
>
> README에는 코드 전체를 포함하고 있지않습니다. 즉, 코드의 일부분만 포함하고 있습니다.



## A. 제공되는 영화 데이터의 주요 내용 수집

> input : movie	# type(movie_dict)는 dict
>
> output : info     # type(info)는 dict

- dict의 변수(test)에서 value 값을 뽑아낼 때 다음과 같이 할 수 있다.
  - test['key']	# key가 존재하면 오류를 뿜고 정지한다.
  - test.get('key')     # 정지되지 않는다.
- dict의 변수(test)에 키값과 value를 넣고 싶다?
  - test['key'] = value
  - 여기서 value값을 가지고 있는 변수를 넣어도 좋다.
- problem_a 는 위와 같은 문법(?)을 가지고 충분히 코드를 작성할 수 있다. 

```python
def movie_info(movie): 
    # dict 타입인 info를 생성
    info = {}
    
    # genre_ids, id, overview, poster_path, title, vote_average
    # 의 value값을 저장한다.
    genre_ids = movie.get('genre_ids')
    id = movie.get('id')
    overview = movie.get('overview')
    poster_path = movie.get('poster_path')
    title = movie.get('title')
    vote_average = movie.get('vote_average')
    
    # 저장된 value를 dict 타입인 info에 저장한다.
    info['genre_ids'] = genre_ids
    info['id'] = id 
    info['overview'] = overview
    info['poster_path'] = poster_path
    info['title'] = title
    info['vote_average'] = vote_average 
    
    # 출력 예시처럼 똑같이 출력할 수 있도록 info를 return해준다.
    return info

if __name__ == '__main__':
    movie_json = open('data/movie.json', encoding='UTF8')
    movie_dict = json.load(movie_json)
    
    pprint(movie_info(movie_dict))
```



## B. 제공되는 영화 데이터의 주요내용 수정

> input : movie,  genres	# 타입은 순서대로 dict과 list이다.
>
> output : info                     # 타입은 dict이다.

- `promblem_a`에서 쓰였던 `문법(?)`과 함께 추가적으로 `이중반복문`(for)을 사용하였다.
  - 해당 movie의 장르의 id값과 genres의 여러쌍을 불러와 그에 맞는 id값을 비교하기 위해서 이중 반복문을 사용하였다.
  - movie의 genre_ids의 value가 list이므로 range(0, len(genre_ids))을 사용하여 접근을 시도하였다.
- 해당되는 반복문은 다음과 같다. `주석`을 달아 놓았다.

```python
# genres의 list에서 dict타입을 하나씩 뽑아 
for genre in genres:
    # (list의 길이 - 1) 까지 반복문을 돌림
    for i in range(0, len(genre_ids)):
        # id값을 비교하여 일치하면 if문을 실행
        if genre_ids[i] == genre['id']:
            # id가 일치 하므로 해당되는 name을 리스트에 추가
            genre_list += [genre['name']]
```



## C. 다중 데이터 분석 및 수정

> input : movies, genres	# 둘다 list이다.
>
> output : info_list	# list이다.

- `promblem_a and b ` 에서 쓰였던 `문법` 을 사용하였다.
- `a 와 b`와 로직은 비슷하나 다른점은 함수의 4/5정도 내용의 code를`반복문`(for) 으로 한번에 덮는 다는 것이다.
- 다음과 같이 for로 덮고 있다. 

```python
for movie in movies:
        # info 딕셔너리를 생성
        info = {}
        # name으로 이루어질 장르 리스트를 생성
        genre_list = []
        # movie 딕셔너리에서 genre_ids의 value를 뽑아와.
        # 이때 value는 list로 되어있음.
        genre_ids = movie.get('genre_ids')
        # genres의 list에서 하나씩 뽑아 id의 해당 되는 값을 genre_ids의 원소와 		비교.
        for genre in genres:
            for i in range(0, len(genre_ids)):
                if genre_ids[i] == genre['id']:
                    genre_list += [genre['name']]
        
        id = movie.get('id')
        # overview 출력이 이상하게 나옴, 수정이 필요
        overview = movie.get('overview')
        poster_path = movie.get('poster_path')
        title = movie.get('title')
        vote_average = movie.get('vote_average')
        
        info['genre_names'] = genre_list
        info['id'] = id 
        info['overview'] = overview
        info['poster_path'] = poster_path
        info['title'] = title
        info['vote_average'] = vote_average 
   
        info_list += [info]
```

- Why? list타입인 movies는 dict 타입인 원소를 가지고 있기도 하고, 그 dict타입인 원소 하나 하나에 접근해서 key에 해당되는 value를 추출하여 비교대상과 비교해야 하기 때문이다. 



## D. 알고리즘을 통한 데이터 출력

> input : movies	# list 이다.
>
> output : title_max_revenue	# str이다.(수익이 최대값인 영화의 title)

- 고민했던 것 : movies 폴더에 많은 number.json을 어떻게 불러올까?

  - 두가지 해결 방법.

    - 노가다로 movie_number_list = [13, 122, 129, ...]를 저장해서 반복문으로 하나씩 불러오기. `바보 같았다.` -> movies 폴더의 파일의 이름은 id값이라는 것을.... 다음 부터는 `README.pdf` 에 나와있는 글을 `꼼꼼히` 읽어야 겠다.
    - list인 movies에서 하나씩 movie를 뽑아 그 movie의 id값으로 number.json을 open할 수 있다. 다음과 같다. 

    ```python
    for movie in movies:
        number_json = open(f'data/movies/{movie.get("id")}.json', encoding='UTF8')
        number_dict = json.load(number_json)
    ```

  - 당연히 `후자`로 선택!!! 하고 반복문을 돌려주면서 movies의 들어있는 movie를 하나씩 뽑아 그 movie의 id에 해당되는 value를 뽑아 open하였다.

- max값을 비교하기 위해서는 예전에 풀었던 문제중 `리스트 숫자중 최대값이 뭐에요?`에 썻던 로직을 사용하였다.  다른점은 값을 꺼내야 한다는 것이다. 그것은 위에 썻던 `문법`을 사용하면 된다. 코드는 다음과 같다.

  ```python
  # 현재 number의 영화와 이전 영화의 max값과 비교한다.
  if max_revenue < number_dict.get('revenue'):
      max_revenue = number_dict.get('revenue')
      # 그 영화의 번호를 기억한다. 즉, 번호.json 를 기억하는 것
          result_number_movie = movie['id']
  ```

- 영화의 번호를 기억한 다음 그에 해당되는 title을 return해 주면 된다.



## E. 알고리즘을 통한 데이터 출력

> input : movies	# list 이다.
>
> output : december_movie_list	# list 이다.

- 많은 number.json 파일을 불러오는 방법은 `problem_d` 것과 동일하다.

- number.json 파일 안에는 타입이 dict이다. 그래서 그 dict의 release_date의 value만 뽑아 내어 `10`인지만을 판단하면 된다.

- release_date의 value를 보았을 때, str타입이다. month를 표현하는 부분은 `[5:7]`이다. 따라서 다음과 같이 조건문을 만들 수 있다.

  ```python
  # 개봉일 12월인 영화 제목을 저장시키는 리스트
  december_movie_list = []
  
  if number_dict.get("release_date")[5:7] == '10':
      # list에 10월에 개봉한 title을 넣은다.
      december_movie_list += [number_dict.get('title')]
  ```

- 위와 같이 개봉일이 10월인 영화를 list에 추가시킨다.
- 마지막으로 그것을 return 해주면 된다.



## 스스로 피드백

오늘 페어 프로젝트를 처음 진행하였다. 처음에는 '나보다 못하거나 잘하거나 하면 어쩌지...' 라는 걱정이 앞섰다. 허나 나와 같은 실력을 가진 학우와 같은 조가 되어 다행이었다. 코드를 각자 짜보고 각자의 어려운 부분이 있으면 서로 설명해 주고 , 어려움이 찾아오면 같이 머리를 굴려 해결해 나아갈 수 있었다. 마지막으로 각자 다른 코드 스타일을 보고 영감을 얻을 수 있었고 이러한 것들로 인해 역량을 기르는 것에 있어서 도움이 많이 되었다.





 