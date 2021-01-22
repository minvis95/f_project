import json
from pprint import pprint


def movie_info(movie):
    pass
    # 여기에 코드를 작성합니다.    
    
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

# 아래의 코드는 수정하지 않습니다.
if __name__ == '__main__':
    movie_json = open('data/movie.json', encoding='UTF8')
    movie_dict = json.load(movie_json)
    
    pprint(movie_info(movie_dict))