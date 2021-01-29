import requests
from tmdb import URLMaker
from pprint import pprint

def popular_count():
    # key값이 있는 URL 생성
    maker = URLMaker('a9b6f3bf14632ae1e588187372385ed4')
    # URL에 required와 optional 추가
    url = maker.get_url('movie', 'popular', region='KR', language='ko')
    # 요청 보내서 받은 값을 res에 저장
    res = requests.get(url)
    # json화 시켜
    movie_dict = res.json()
    # popular인 영화의 갯수를 return함
    return len(movie_dict.get('results'))

if __name__ == '__main__':
    print(popular_count())
