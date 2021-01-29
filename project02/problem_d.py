import requests
from tmdb import URLMaker
from pprint import pprint

# def get_url(self, category='movie', feature='popular', **kwargs):
def recommendation(title):
    # 변수 초기화
    movie_dict = {}
    # movie_dict에서 list인 value를 뽑아내기 위해 변수 설정함
    movie_list = []
    # 결과값 리스트
    result = []
    # key값이 있는 URL 생성
    maker = URLMaker('a9b6f3bf14632ae1e588187372385ed4')
    # 그 영화의 id값을 뽑아와서 ch에 저장
    ch = maker.movie_id(title)
    # recommendation에 상세주소를 적으므로써 url을 얻어냄
    url = maker.get_url('movie', f'{ch}/recommendations', language='ko')
    # 요청 보내서 받은 값을 res에 저장
    res = requests.get(url)
    # json화 시킴
    movie_dict = res.json()
    
    # 이상한 title로 검색하였을 때
    # None을 반환
    if movie_dict.get('success') == False:
        return None
    # results에 해당되는 value를 뽑아냄
    movie_list = movie_dict.get('results')
    # list를 돌림. 이때 movie는 dict타입임.
    for movie in movie_list:
        result.append(movie.get('title'))

    return result
    
if __name__ == '__main__':
    # 제목 기반 영화 추천
    pprint(recommendation('기생충'))
    # =>   
    # ['원스 어폰 어 타임 인… 할리우드', '조조 래빗', '결혼 이야기', '나이브스 아웃', '1917', 
    # '조커', '아이리시맨', '미드소마', '라이트하우스', '그린 북', 
    # '언컷 젬스', '어스', '더 플랫폼', '블랙클랜스맨', '포드 V 페라리', 
    # '더 페이버릿: 여왕의 여자', '두 교황', '작은 아씨들', '테넷', '브레이킹 배드 무비: 엘 카미노']
    pprint(recommendation('그래비티'))    
    # => []
    pprint(recommendation('id없는 영화'))
    # => None

'''
class URLMaker:    
    url = 'https://api.themoviedb.org/3'

    def __init__(self, key):
        self.key = key

    def get_url(self, category='movie', feature='popular', **kwargs):
        url = f'{self.url}/{category}/{feature}'
        url += f'?api_key={self.key}'

        for k, v in kwargs.items():
            url += f'&{k}={v}'

        return url
        

    def movie_id(self, title):
        url = self.get_url('search', 'movie', region='KR', language='ko', query=title)
        res = requests.get(url)
        movie = res.json()

        if len(movie.get('results')):
            return movie.get('results')[0].get('id')
        else:
            return None
    
'''