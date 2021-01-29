import requests
from tmdb import URLMaker
from pprint import pprint


def ranking():
    # return이 될 변수 초기화
    result = []
    # 평점과 제목이 한쌍이 될 리스트임
    rank_list = []
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
        # 평점과, 제목을 튜플로 묵어 list에 추가
        rank_list.append((movie.get('vote_average'), movie.get('title')))
    # 정렬을 시킴.
    # 튜플로 이루어진 리스트를 정렬할 때 튜플의 첫번째 원소를 기준으로 정렬
    # 만약에 0번재와 1번째의 튜플의 첫번째 원소가 같다면 두번째 원소를 비교하며 정렬
    rank_list.sort(reverse = -1)
    
    # 상위 5개만 뽑아야 하므로 내림차순으로 된 list에서 0~4번째의 제목을
    # result에 추가함
    for idx, movie in enumerate(rank_list):
        if idx < 5:
            result.append(movie[1])
            continue
        break
    
    return result
        
if __name__ == '__main__':
    # popular 영화 평점순 5개 출력
    pprint(ranking())