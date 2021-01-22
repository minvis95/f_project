import json
from pprint import pprint


def movie_info(movie, genres):
    pass
    # 여기에 코드를 작성합니다.  
    
    # dict 타입인 info를 생성
    info = {}
    
    # 장르 name으로 이루어질 리스트를 생성
    genre_list = []
    
    # dict 타입인 movie에서 genre_ids의 value를 뽑아와.
    # 이때 value는 list로 되어있음.
    genre_ids = movie.get('genre_ids')
    
    # genres의 list에서 dict타입을 하나씩 뽑아 
    for genre in genres:
        # (list의 길이 - 1) 까지 반복문을 돌림
        for i in range(0, len(genre_ids)):
            # id값을 비교하여 일치하면 if문을 실행
            if genre_ids[i] == genre['id']:
                # id가 일치 하므로 해당되는 name을 리스트에 추가
                genre_list += [genre['name']]

    # genre_ids, id, overview, poster_path, title, vote_average
    # 의 value값을 저장한다.
    id = movie.get('id')
    overview = movie.get('overview')
    poster_path = movie.get('poster_path')
    title = movie.get('title')
    vote_average = movie.get('vote_average')
    
    # 저장된 value를 dict 타입인 info에 저장한다.
    info['genre_names'] = genre_list
    info['id'] = id 
    info['overview'] = overview
    info['poster_path'] = poster_path
    info['title'] = title
    info['vote_average'] = vote_average 
   
    return info

# 아래의 코드는 수정하지 않습니다.
if __name__ == '__main__':
    movie_json = open('data/movie.json', encoding='UTF8')
    movie = json.load(movie_json)

    genres_json = open('data/genres.json', encoding='UTF8')
    genres_list = json.load(genres_json)

    pprint(movie_info(movie, genres_list))