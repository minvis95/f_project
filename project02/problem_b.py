import requests
from tmdb import URLMaker
from pprint import pprint


def vote_average_movies():
    # return이 될 변수 초기화
    result_list = []
    # key값이 있는 URL 생성
    maker = URLMaker('a9b6f3bf14632ae1e588187372385ed4')
    # URL에 required와 optional 추가
    url = maker.get_url('movie', 'popular', region='KR', language='ko')
    # 요청 보내서 받은 값을 res에 저장
    res = requests.get(url)
    # json화 시켜
    movie_dict = res.json()

    # dict of dict이므로 dict을 뽑음
    for movie in movie_dict.get('results'):
        # 그 dict타입인 movie의 평점이 8이상이야??
        if movie.get('vote_average') >= 8:
            # Then 그 영화 제목을 result에 추가
            result_list.append(movie.get('title'))

    return result_list

if __name__ == '__main__':
    pprint(vote_average_movies())    