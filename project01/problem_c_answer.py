import json
from pprint import pprint


def movie_info(movies, genres):
    pass
    # 여기에 코드를 작성합니다.  
    
    # Return값이 list인 info를 생성
    info_list = []

    # movies라는 list에서 dict을 하나씩 추출함
    for movie in movies:
        # info 딕셔너리를 생성
        info = {}
        # name으로 이루어질 장르 리스트를 생성
        genre_list = []
        # movie 딕셔너리에서 genre_ids의 value를 뽑아와.
        # 이때 value는 list로 되어있음.
        genre_ids = movie.get('genre_ids')
        # genres의 list에서 하나씩 뽑아 id의 해당 되는 값을 genre_ids의 원소와 비교.
        for genre in genres:
            for i in range(0, len(genre_ids)):
                if genre_ids[i] == genre['id']:
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
   
        info_list += [info]
    
    return info_list

        
# 아래의 코드는 수정하지 않습니다.
if __name__ == '__main__':
    movies_json = open('data/movies.json', encoding='UTF8')
    movies_list = json.load(movies_json)

    genres_json = open('data/genres.json', encoding='UTF8')
    genres_list = json.load(genres_json)

    pprint(movie_info(movies_list, genres_list))