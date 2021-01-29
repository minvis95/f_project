import requests
from tmdb import URLMaker
from pprint import pprint


def credits(title):
    # 변수 초기화
    movie_dict = {}
    movie_list = []
    actor_list = []
    director_list = []

    # 결과값 리스트
    result = {}
    # key값이 있는 URL 생성
    maker = URLMaker('a9b6f3bf14632ae1e588187372385ed4')
    # 그 영화의 id값을 뽑아와서 ch에 저장
    ch = maker.movie_id(title)
    # credits에 상세주소를 적으므로써 url을 얻어냄
    url = maker.get_url('movie', f'{ch}/credits', language='ko')
    # 요청 보내서 받은 값을 res에 저장
    res = requests.get(url)
    # json화 시킴
    movie_dict = res.json()
    # 이상한 title로 검색하였을 때
    # None을 반환
    if movie_dict.get('success') == False:
        return None
    
    # cast와 crew에 해당되는 각각의 value를 뽑음. 이때 value는 list로 되어있음
    movie_list = movie_dict.get('cast')
    movie_list2 = movie_dict.get('crew')
    # movie_list에서 하나씩 뽑음. 이때 person은 dict타입임
    for person in movie_list:
        # dict 타입 person에서 cast_id에 해당되는 value가 10 이하면
        if person.get('cast_id') < 10:
            # list에 그 배우이름 추가
            actor_list.append(person.get('name'))
    # 위와 비슷한방법으롤
    for person2 in movie_list2:
        # dict 타입 person에서 감독인 사람만 뽑음
        if person2.get('department') == 'Directing':
            # 그사람 list에 추가
            director_list.append(person2.get('name'))
    # cast, crew 두개의 key를 가지고 쌍의 맞는 list를 value에 넣음
    result.update({'cast': actor_list, 'crew': director_list})
    
    return result
    
if __name__ == '__main__':
    # id 기준 주연배우 감독 출력
    pprint(credits('기생충'))
    # => 
    # {
    #     'cast': [
    #         'Song Kang-ho',
    #         'Lee Sun-kyun',
    #         'Cho Yeo-jeong',
    #         'Choi Woo-shik',
    #         'Park So-dam',
    #         'Lee Jung-eun',
    #         'Chang Hyae-jin'
    #     ],
    #      'crew': [
    #         'Bong Joon-ho',
    #         'Han Jin-won',
    #         'Kim Seong-sik',
    #         'Lee Jung-hoon',
    #         'Park Hyun-cheol',
    #         'Yoon Young-woo'
    #     ]
    # } 
    pprint(credits('id없는 영화'))
    # => None