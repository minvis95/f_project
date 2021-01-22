import json


def dec_movies(movies):
    pass
    # 여기에 코드를 작성합니다.  
    
    # 개봉일 12월인 영화 제목을 저장시키는 리스트
    december_movie_list = []
    # movies.json에 있는 movie를 하나씩 뽑는다. movie의 type은 dict이다.
    for movie in movies:
        # Ex) number_json = 13_json, 122.json, ...을 open한것을 저장.
        number_json = open(f'data/movies/{movie.get("id")}.json', encoding='UTF8')
        number_dict = json.load(number_json)
        # str 타입인 "1994-09-10"은 0번째 시작을 기준으로 해서 5번째 6번째가 month를 나타낸다.
        # 따라서 그 month가 '10' 이면 if문 아래가 실행된다.
        if number_dict.get("release_date")[5:7] == '10':
            # list에 10월에 개봉한 title을 넣은다.
            december_movie_list += [number_dict.get('title')]
    
    # 그 영화의 title를 return해준다.
    return december_movie_list

# 아래의 코드는 수정하지 않습니다.
if __name__ == '__main__':
    movies_json = open('data/movies.json', encoding='UTF8')
    movies_list = json.load(movies_json)
    
    print(dec_movies(movies_list))