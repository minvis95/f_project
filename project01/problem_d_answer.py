import json


def max_revenue(movies):
    pass
    # 여기에 코드를 작성합니다.

    # max_revenue 초기화
    max_revenue = 0
    # revenue가 가장 높은 영화 번호를 기억하기 위해 변수를 초기화
    result_number_movie = 0
    # movies.json에 있는 movie를 하나씩 뽑는다. movie의 type은 dict이다.
    for movie in movies:
        # Ex) number_json = 13_json, 122.json, ...을 open한것을 저장.
        number_json = open(f'data/movies/{movie.get("id")}.json', encoding='UTF8')
        number_dict = json.load(number_json)
        
        # 현재 number의 영화와 이전 영화의 max값과 비교한다.
        if max_revenue < number_dict.get('revenue'):
            max_revenue = number_dict.get('revenue')
            # 그 영화의 번호를 기억한다. 즉, 번호.json 를 기억하는 것
            result_number_movie = movie['id']

    # 수익이 최대값인 번호.json을 open한다.
    number_json = open(f'data/movies/{result_number_movie}.json', encoding='UTF8')
    number_dict = json.load(number_json)
    # 그 영화의 title를 return해준다.
    return number_dict.get('title')
        

if __name__ == '__main__':
    movies_json = open('data/movies.json', encoding='UTF8')
    movies_list = json.load(movies_json)

    print(max_revenue(movies_list))