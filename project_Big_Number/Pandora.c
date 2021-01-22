#include "Pandora.h"

int64_t cpucycles(void)
{
	return __rdtsc();
}

void delete_bigint(bigint_ptr* bi_X) //구조체에 할당된 메모리를 삭제와 초기화
{
	if (*bi_X == NULL)
		return;
	for (int i = 0; i < (*bi_X)->wordlen; i++)
	{
		(*bi_X)->a[i] = 0x00; //메모리 값 초기화
	}
	free((*bi_X)->a);
	free(*bi_X);
	*bi_X = NULL;
}

void new_bigint(bigint_ptr* bi_X, int wordlen) //새로운 bigint를 만듬
{
	if (*bi_X != NULL) //포인터값이 할당되어있다면 해제
		delete_bigint(bi_X);

	*bi_X = (bigint*)malloc(sizeof(bigint));
	
	(*bi_X)->sign = NON_NEGATIVE;
	(*bi_X)->wordlen = wordlen;
	(*bi_X)->a = (word*)calloc(wordlen, sizeof(word));
}

void set_bigint(bigint_ptr* bi_X, word* input, int sign, int wordlen) //입력된 input값을 통해 bigint를 만듬
{
	if (*bi_X != NULL) //포인터값이 할당되어있다면 해제
		delete_bigint(bi_X);
	
	*bi_X = (bigint*)malloc(sizeof(bigint));
	(*bi_X)->a = (word*)calloc(wordlen, sizeof(word));

	for (int i = 0; i < wordlen; i++)
	{
		(*bi_X)->a[i] = input[i];
	}

	(*bi_X)->sign = sign;
	(*bi_X)->wordlen = wordlen;	
}

void show_bigint(bigint* X, int base) //base = hexa, deci, binary
{
	int i = 0;
	
	if (X->sign == NEGATIVE)
		printf("-");
	
	if (base == 16)
	{
		printf("0x");
		for (i = X->wordlen - 1; i >= 0; i--)
		{
#if WORD_BITLEN == 8
			printf("%02x", X->a[i]);
#elif WORD_BITLEN == 32
			printf("%08x", X->a[i]);
#else //WORD_BITLEN == 64
			printf("%016llx", X->a[i]);
#endif
		}
		printf("\n");
	}
	else if (base == 10)
	{
		for (i = X->wordlen - 1; i >= 0; i--)
		{
#if WORD_BITLEN == 8
			printf("%d", X->a[i]);
#elif WORD_BITLEN == 32
			printf("%d", X->a[i]);
#else //WORD_BITLEN == 64
			printf("%d", X->a[i]);
#endif
			printf("/n");
		}
	}
	else //base == 2
	{
		int array_binary_size = 1;
		int* array_binary = (int*)calloc(array_binary_size, sizeof(int));
		bigint_ptr* Y = NULL;

		assign_bigint(Y, X); // Y <- X (copy)
		
		for (int j = 0; j < (*Y)->wordlen; j++)
		{
			for (i = 0; (*Y)->a[j] != 0; i++)
			{
				array_binary[i] = (*Y)->a[j] % 2;
				(*Y)->a[j] /= 2;
				if ((*Y)->a[j] != 0)
				{
					array_binary_size++;
					realloc(array_binary, sizeof(int) * array_binary_size);
				}
			}
		}
		for (i = array_binary_size - 1; i >= 0; i--)
		{
			printf("%d", array_binary[i]);
		}
		free(array_binary);
	}
}

void show_bigint2(bigint_ptr* X, int base) //base = hexa, 코드 분석용
{
	int i = 0;

	if ((*X)->sign == NEGATIVE)
		printf("-");

	if (base == 16)
	{
		printf("0x");
		for (i = (*X)->wordlen - 1; i >= 0; i--)
		{
#if WORD_BITLEN == 8
			printf("%02x", (*X)->a[i]);
#elif WORD_BITLEN == 32
			printf("%08x", (*X)->a[i]);
#else //WORD_BITLEN == 64
			printf("%016llx", (*X)->a[i]);
#endif
		}
		printf("\n");
	}

}

void refine_bigint(bigint_ptr* bi_X) // EX) A(n개) + B(n개) = 최대 n+1개 워드, 그런데 n+1번째 워드 0...0이여서 n개 워드로 정정하기 위한 함수
{
	int new_wordlen;
	if ((*bi_X) == NULL)
	{
		return;
	}

	new_wordlen = (*bi_X)->wordlen;
	while (new_wordlen > 1)
	{
		if ((*bi_X)->a[new_wordlen - 1] != 0)
			break;
		new_wordlen--;
	}
	if ((*bi_X)->wordlen != new_wordlen)
	{
		(*bi_X)->wordlen = new_wordlen;
		(*bi_X)->a = (word*)realloc((*bi_X)->a, sizeof(word) * new_wordlen);
	}

	if (((*bi_X)->wordlen == 1) && ((*bi_X)->a[0] == 0x0))
		(*bi_X)->sign = NON_NEGATIVE;
}

void assign_bigint(bigint_ptr* bi_Y, bigint_ptr* bi_X) //X를 Y로 복사
{
	set_bigint(bi_Y, (*bi_X)->a, (*bi_X)->sign, (*bi_X)->wordlen);
}

void generate_random_bigint(bigint_ptr* bi_X, int sign, int wordlen)
{
	if (*bi_X != NULL)
		delete_bigint(bi_X);
	
	word* Input;
	Input = (word*)calloc(wordlen, sizeof(word)); //random값을 저장하기위해 메모리 할당
	
	srand((unsigned int)time(NULL)); //실행할때 마다 다른 random값을 생성하기 위해
	array_rand(Input, wordlen);
	
	set_bigint(bi_X, Input, sign, wordlen);
	refine_bigint(bi_X);
}

void array_rand(word* dst, int wordlen)//랜덤값 채워넣기
{
	for (int i = 0; i < wordlen; i++)
	{
		dst[i] = one_array_rand();//한 워드씩 랜던값 채워넣기
	}
}

#if WORD_BITLEN == 8
word one_array_rand()
{
	return (word)one_rand_8bits(); //8bit짜리 random값 생성
}
#elif WORD_BITLEN == 32
word one_array_rand()
{
	return (word)one_rand_32bits(); //32bit짜리 random값 생성
}
#else//64
word one_array_rand()
{
	return (word)one_rand_64bits(); //64bit짜리 random값 생성
}
#endif

unsigned char one_rand_8bits()
{
	return rand() & 0xff;
	//rand()는 15bit를 return해준다.
	//따라서 8bit를 random한 값을 return해주기 위해서는 0xff를 &연산 취해준다.
}

unsigned int one_rand_32bits()
{
	int i = 3;
	unsigned int tmp = one_rand_8bits();
	while (i > 0) //채워넣는거 3번 8bit + 3번 * 8bit = 32bit
	{
		tmp = tmp << 8; //8bit 옆으로 밀고
		tmp = tmp ^ one_rand_8bits(); //8bit의 random값 채워넣고
		i--;
	}
	return tmp;
}

unsigned long long one_rand_64bits()
{
	unsigned long long tmp = one_rand_32bits();
	tmp = tmp << 32;
	tmp = tmp ^ one_rand_32bits();
	return tmp;
}


int get_word_length(bigint_ptr* bi_X) //wordlen을 return해줌
{
	return (*bi_X)->wordlen;
}

int get_bit_length(bigint_ptr* bi_X)
{
	int total_word_bit_length = ((*bi_X)->wordlen) * WORD_BITLEN; //이 bigint가 가지고 있는 word의 전체bit를 구하는것
	int i, cnt = 0;
	
	for (i = WORD_BITLEN; (i >= 0) && (((*bi_X)->a[(*bi_X)->wordlen - 1] >> (i - 1)) == 0); i--)
	{
		cnt++; //최상위 word에서의 최상위 bit부터 -1하면서 0의 개수를 구함
	}
	return total_word_bit_length - cnt; 
}

int get_j_th_bit(bigint_ptr* bi_X, int j_th) //j = 0부터 word의배수 - 1 까지
{
	if (j_th >= WORD_BITLEN)
		return (((*bi_X)->a[j_th / WORD_BITLEN] >> (j_th % WORD_BITLEN)) & 1);
	
	return (((*bi_X)->a[0] >> j_th) & 1);
}

int get_sign(bigint_ptr* bi_X)
{
	return (*bi_X)->sign;
}

void flip_sign(bigint_ptr* bi_X) //음수->양수, 양수->음수 ,즉 바꿔치기
{
	(*bi_X)->sign = (*bi_X)->sign ^ 1;
}

void swap_bigint(bigint_ptr* bi_X, bigint_ptr* bi_Y)
{
	bigint_ptr temp = NULL;
	temp = (bigint_ptr)calloc(1, sizeof(bigint)); //new_bigint()함수는 wordlen인자를 받아야하므로 쓰지않음

	temp = *bi_X;
	*bi_X = *bi_Y;
	*bi_Y = temp;

}

void set_one(bigint_ptr* bi_X)//지수승을 위해 필요
{
	new_bigint(bi_X, 1);
	//(*bi_X)->sign = NON_NEGATIVE;  new_bigint()에서 선언하고 오기때문에 생략 
	(*bi_X)->a[0] = 0x1;
}

void set_zero(bigint_ptr* bi_X)//지수승을 위해 필요
{
	new_bigint(bi_X, 1);
	//(*bi_X)->sign = NON_NEGATIVE;	 new_bigint()에서 선언하고 오기때문에 생략
	(*bi_X)->a[0] = 0x0;
}

int is_zero(bigint_ptr* bi_X) //EX)A=0일때, A*B=0 할때 필요
{
	if ((*bi_X)->sign == NEGATIVE || ((*bi_X)->a[0]) != 0x0)
		return FALSE;
	
	for (int i = (*bi_X)->wordlen - 1; i >= 1; i--)
	{
		if ((*bi_X)->a[i] != 0)
			return FALSE;
	}
	return TRUE;
}

int is_one(bigint_ptr* bi_X) //EX)A=1일때, A*B=B 할때 필요
{
	if ((*bi_X)->sign == NEGATIVE || (*bi_X)->a[0] != 0x1)
		return FALSE;
	for (int i = (*bi_X)->wordlen - 1; i >= 1; i--)
	{
		if ((*bi_X)->a[i] != 0)
			return FALSE;
	}
	return TRUE;
}

int compare_ABS(bigint_ptr* bi_X, bigint_ptr* bi_Y) //compare()함수를 위한 함수(X와Y가 둘다 양수)
{
	if ((*bi_X)->wordlen > (*bi_Y)->wordlen)
		return 1; //X > Y
	else if ((*bi_X)->wordlen < (*bi_Y)->wordlen)
		return -1; //X < Y

	for (int i = (*bi_X)->wordlen - 1; i >= 0; i--)
	{
		if ((*bi_X)->a[i] > (*bi_Y)->a[i])
			return 1; //X > Y
		else if ((*bi_X)->a[i] < (*bi_Y)->a[i])
			return -1; //X < Y
	}
	
	return 0; // X = Y
}

int compare(bigint_ptr* bi_X, bigint_ptr* bi_Y)
{
	int ret = 0;
	if ((*bi_X)->sign == NON_NEGATIVE && (*bi_Y)->sign == NEGATIVE)
		return 1; //X > Y
	if ((*bi_X)->sign == NEGATIVE && (*bi_Y)->sign == NON_NEGATIVE)
		return -1; //X < Y
	
	ret = compare_ABS(bi_X, bi_Y); //XY>=0
	if ((*bi_X)->sign == NON_NEGATIVE) //X, Y >=0
		return ret;
	else
		return -ret;
}

void left_shift(bigint_ptr* bi_X, int shift)
{
	int i;
	if (shift % WORD_BITLEN == 0)
	{
#if WORD_BITLEN == 8
		(*bi_X)->a = (word*)realloc((*bi_X)->a, (((*bi_X)->wordlen) + (shift / WORD_BITLEN)));
#elif WORD_BITLEN == 32
		(*bi_X)->a = (word*)realloc((*bi_X)->a, 4*(((*bi_X)->wordlen) + (shift / WORD_BITLEN)));
#else// WORD_BITLEN == 64
		(*bi_X)->a = (word*)realloc((*bi_X)->a, 8 * (((*bi_X)->wordlen) + (shift / WORD_BITLEN)));
#endif
		
		(*bi_X)->wordlen += shift / WORD_BITLEN;

		for (i = ((*bi_X)->wordlen) - (shift / WORD_BITLEN) - 1; i >= 0; i--)
		{
			(*bi_X)->a[i + (shift / WORD_BITLEN)] = (*bi_X)->a[i]; //추가된 배열로 이동(shift)
		}

		for (i = 0; i < (shift / WORD_BITLEN); i++)
		{
			(*bi_X)->a[i] = 0x0;
		}
	}
	else //shift % WORD_BITLEN != 0 ,즉 몫과 나머지가 존재
	{
#if WORD_BITLEN == 8
		(*bi_X)->a = (word*)realloc((*bi_X)->a, (((*bi_X)->wordlen) + (shift / WORD_BITLEN) + 1)); //나머지가 존재하므로 +1을 해주어야함
#elif WORD_BITLEN == 32
		(*bi_X)->a = (word*)realloc((*bi_X)->a, 4 * (((*bi_X)->wordlen) + (shift / WORD_BITLEN) + 1)); 
#else //WORD_BITLEN ==64
		(*bi_X)->a = (word*)realloc((*bi_X)->a, 8 * (((*bi_X)->wordlen) + (shift / WORD_BITLEN) + 1)); 
#endif
		
		(*bi_X)->wordlen += shift / WORD_BITLEN + 1; //새롭게 갱신된 wordlen

		for (i = (*bi_X)->wordlen - 2; i > (shift / WORD_BITLEN) - 1; i--) //몫만큼 shift이동
		{
			(*bi_X)->a[i] = (*bi_X)->a[i - (shift / WORD_BITLEN)]; 
		}
		for (i = 0; i < (shift / WORD_BITLEN); i++) //이동완료후 0x0으로 채워줌
		{
			(*bi_X)->a[i] = 0x0;
		}
		//이제 나머지만큼에 shift 이동이 필요함
		(*bi_X)->a[(*bi_X)->wordlen - 1] = (*bi_X)->a[(*bi_X)->wordlen - 2] >> (WORD_BITLEN - shift % WORD_BITLEN); //최상위 word 부분
		
		for (i = (*bi_X)->wordlen - 2; i > shift / WORD_BITLEN; i--)//중간word부분
		{
			(*bi_X)->a[i] = ((*bi_X)->a[i] << (shift % WORD_BITLEN)) | ((*bi_X)->a[i - 1] >> (WORD_BITLEN - shift % WORD_BITLEN));
		}
		//최하위word부분
		(*bi_X)->a[shift / WORD_BITLEN] = (*bi_X)->a[shift / WORD_BITLEN] << (shift % WORD_BITLEN);


		if ((*bi_X)->a[(*bi_X)->wordlen - 1] == 0x0) //마지막word부분이 0이면 refine bigint해준다.
		{
			refine_bigint(bi_X);
		}
	}
}

void right_shift(bigint_ptr* bi_X, int shift)
{
	int i;
	if (shift % WORD_BITLEN == 0) //나머지 0일때
	{
		for (i = 0; i < (*bi_X)->wordlen - (shift / WORD_BITLEN); i++)
		{
			(*bi_X)->a[i] = (*bi_X)->a[i + (shift / WORD_BITLEN)]; //word 이동
		}
		for (i = (*bi_X)->wordlen - (shift / WORD_BITLEN); i < (*bi_X)->wordlen; i++) //이동시킨후 기존에 있던 값을 0x0으로 채움(refine을 하기위해)
		{
			(*bi_X)->a[i] = 0x0;
		}
		refine_bigint(bi_X); //remove zero(메모리 재할당)
	}
	else // (shift % WORD_BITLEN(=8) != 0)
	{
		for (i = 0; i < (*bi_X)->wordlen - (shift / WORD_BITLEN); i++)//일단 몫만큼 이동
		{
			(*bi_X)->a[i] = (*bi_X)->a[i + (shift / WORD_BITLEN)];
		}
		for (i = (*bi_X)->wordlen - (shift / WORD_BITLEN); i < (*bi_X)->wordlen; i++)
		{
			(*bi_X)->a[i] = 0x0; // 나머지 부분 0x0대입
		}
		for (i = 0; i < (*bi_X)->wordlen - shift / WORD_BITLEN - 1; i++) //값이 있는word부분부터 나머지를 shifting 시켜줌
		{
			(*bi_X)->a[i] = (((*bi_X)->a[i] >> (shift % WORD_BITLEN)) | ((*bi_X)->a[i + 1] << (WORD_BITLEN - (shift % WORD_BITLEN))));
		}
		//값이 있는 최상위 word만 따로 해줌 
		(*bi_X)->a[(*bi_X)->wordlen - (shift / WORD_BITLEN) - 1] = (*bi_X)->a[(*bi_X)->wordlen - (shift / WORD_BITLEN) - 1] >> ((shift % WORD_BITLEN));

		refine_bigint(bi_X); //remove zero(메모리 재할당)
	}
}

void reduction(bigint_ptr* bi_X, int power_of_2) //power_of_2 는 r을 의미함
{
	if (power_of_2 >= get_bit_length(bi_X)) //case1 r >= wn
		return;

	if (power_of_2 % WORD_BITLEN == 0 && power_of_2 < get_bit_length(bi_X)) //case2 r = wk where k < n
	{
#if WORD_BITLEN == 8
		(*bi_X)->a = (word*)realloc((*bi_X)->a, power_of_2 / WORD_BITLEN);
#elif WORD_BITLEN == 32
		(*bi_X)->a = (word*)realloc((*bi_X)->a, 4 * (power_of_2 / WORD_BITLEN));
#else //WORD_BITLEN == 64
		(*bi_X)->a = (word*)realloc((*bi_X)->a, 8 * (power_of_2 / WORD_BITLEN));
#endif

		(*bi_X)->wordlen = power_of_2 / WORD_BITLEN; //wordlen 갱신
		return;
	} 

	//case3 : 배수가 아닌경우
	//power_of_2 안쪽으로 들어오는 word중 최상위 word만 수정해시켜주면됌
	(*bi_X)->a[(power_of_2 / WORD_BITLEN)] &= (0xff >> (WORD_BITLEN - (power_of_2 % WORD_BITLEN))); //   A & (2^r' - 1) ,k번재 word에서

#if WORD_BITLEN == 8
	(*bi_X)->a = (word*)realloc((*bi_X)->a, (power_of_2 / WORD_BITLEN) + 1);
#elif WORD_BITLEN == 32
	(*bi_X)->a = (word*)realloc((*bi_X)->a, 4 * ((power_of_2 / WORD_BITLEN) + 1));
#else //WORD_BITLEN == 64
	(*bi_X)->a = (word*)realloc((*bi_X)->a, 8 * ((power_of_2 / WORD_BITLEN) + 1));
#endif
	
	(*bi_X)->wordlen = (power_of_2 / WORD_BITLEN) + 1;
	return;
}

void addition(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)//X+Y=Z
{
	if (is_zero(bi_X) == TRUE)
	{
		assign_bigint(bi_Z, bi_Y);
		return;
	}
	if (is_zero(bi_Y) == TRUE)
	{
		assign_bigint(bi_Z, bi_X);
		return;
	}
	if (get_sign(bi_X) == NON_NEGATIVE && get_sign(bi_Y) == NEGATIVE)
	{
		flip_sign(bi_Y);
		subtraction(bi_X, bi_Y, bi_Z);
		flip_sign(bi_Y);
		return;
	}
	if (get_sign(bi_X) == NEGATIVE && get_sign(bi_Y) == NON_NEGATIVE)
	{
		flip_sign(bi_X);
		subtraction(bi_Y, bi_X, bi_Z);
		flip_sign(bi_X);
		return;
	}
	if (get_word_length(bi_X) >= get_word_length(bi_Y))
	{
		ADDC(bi_X, bi_Y, bi_Z); //여기서 길이로 인한 조정 시켜줌
		return;
	}
	else
	{
		ADDC(bi_Y, bi_X, bi_Z);
		return;
	}
}
void ADDC(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)
{
	int i;

	if (get_word_length(bi_X) > get_word_length(bi_Y)) //wordlen을 똑같이 맞추기위해 bi_X의 wordlen만큼 재할당 받음
	{
#if WORD_BITLEN == 8
		(*bi_Y)->a = (word*)realloc((*bi_Y)->a, ((*bi_X)->wordlen));
#elif WORD_BITLEN == 32
		(*bi_Y)->a = (word*)realloc((*bi_Y)->a, 4 * ((*bi_X)->wordlen));
#else //WORD_BITLEN ==64
		(*bi_Y)->a = (word*)realloc((*bi_Y)->a, 8 * ((*bi_X)->wordlen));
#endif
		for (i = get_word_length(bi_X) - 1; i > get_word_length(bi_Y) - 1; i--)
		{
			(*bi_Y)->a[i] = 0x0; //재할당된 부분 0x0으로 초기화
		}

		(*bi_Y)->wordlen = get_word_length(bi_X);//bi_X와 wordlen이 같아졌으므로 수정
	}
	//if (*bi_Z != NULL) //bi_Z가 할당되어있으면 초기화와 free시키고 다시 메모리할당 받음
	//{
		delete_bigint(bi_Z);
		*bi_Z = (bigint_ptr)calloc(1, sizeof(bigint));
		(*bi_Z)->wordlen = (*bi_X)->wordlen + 1;
		(*bi_Z)->a = (word*)calloc((*bi_Z)->wordlen, sizeof(word));
		(*bi_Z)->sign = (*bi_X)->sign; // ADDC함수로 들어오는 bi_X와 bi_Y는 부호가 똑같기 때문에 bi_Z도 x와y랑 똑같음
	//}
	
	int carry = 0;

	for (i = 0; i < get_word_length(bi_X); i++)
	{
		(*bi_Z)->a[i] = (*bi_X)->a[i] + (*bi_Y)->a[i] + carry; //자동으로 모듈러해줌
		ADD_carry((*bi_Z)->a[i], (*bi_X)->a[i], (*bi_Y)->a[i], &carry);
		if (i == get_word_length(bi_X) - 1)
		{
			(*bi_Z)->a[i+1] = carry; //결과값의 최상위 word는 1또는 0이다
		}
	}
	refine_bigint(bi_Y);//wordlen 되돌려줌
	refine_bigint(bi_Z); //최상위 word부분이 carry를 안받으면 0x0일것이므로 refine시킨다
}

void ADD_carry(word z, word x, word y, int* carry)
{
	if (z < x || z < y) //x=0x01, y=0xff, carry=0x01이면, z = x+y+carry = 0x01 = x 이므로 y까지 비교하도록 포함시켰음 
		*carry = 1;
	if (z > x)
		*carry = 0;

}

void subtraction(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)
{
	if (is_zero(bi_X) == TRUE)
	{
		assign_bigint(bi_Z, bi_Y);
		flip_sign(bi_Z);
		return;
	}

	if (is_zero(bi_Y) == TRUE)
	{
		assign_bigint(bi_Z, bi_X);
		return;
	}

	if (get_sign(bi_X) == NON_NEGATIVE && get_sign(bi_Y) == NON_NEGATIVE && compare(bi_X, bi_Y) == 1)
	{
		SUBC(bi_X, bi_Y, bi_Z);
		return;
	}
	else if(get_sign(bi_X) == NON_NEGATIVE && get_sign(bi_Y) == NON_NEGATIVE && compare(bi_X, bi_Y) == -1)
	{
		SUBC(bi_Y, bi_X, bi_Z);
		flip_sign(bi_Z);
		return;
	}

	if (get_sign(bi_X) == NEGATIVE && get_sign(bi_Y) == NEGATIVE && compare(bi_X, bi_Y) == 1)
	{
		flip_sign(bi_X);
		flip_sign(bi_Y);
		SUBC(bi_Y, bi_X, bi_Z);
		flip_sign(bi_X);
		flip_sign(bi_Y);
		return;
	}
	else if (get_sign(bi_X) == NEGATIVE && get_sign(bi_Y) == NEGATIVE && compare(bi_X, bi_Y) == -1)
	{
		flip_sign(bi_X);
		flip_sign(bi_Y);
		SUBC(bi_X, bi_Y, bi_Z);
		flip_sign(bi_X);
		flip_sign(bi_Y);
		flip_sign(bi_Z);
		return;
	}

	if (get_sign(bi_X) == NON_NEGATIVE && get_sign(bi_Y) == NEGATIVE)
	{
		flip_sign(bi_Y);
		addition(bi_X, bi_Y, bi_Z);
		flip_sign(bi_Y);
		return;
	}
	else if (get_sign(bi_X) == NEGATIVE && get_sign(bi_Y) == NON_NEGATIVE)
	{
		flip_sign(bi_X);
		addition(bi_X, bi_Y, bi_Z);
		flip_sign(bi_X);
		flip_sign(bi_Z);
		return;
	}

	//위에 모든 조건을 만족하지 않는다면 bi_X=bi_Y는 소리이다.
	//따라서 결과값은 0이다.
	set_zero(bi_Z);
	return;
}

void SUBC(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)
{
	int i;

	if (get_word_length(bi_X) > get_word_length(bi_Y)) //wordlen을 똑같이 맞추기위해 bi_X의 wordlen만큼 재할당 받음
	{
#if WORD_BITLEN == 8
		(*bi_Y)->a = (word*)realloc((*bi_Y)->a, ((*bi_X)->wordlen));
#elif WORD_BITLEN == 32
		(*bi_Y)->a = (word*)realloc((*bi_Y)->a, 4 * ((*bi_X)->wordlen));
#else //WORD_BITLEN ==64
		(*bi_Y)->a = (word*)realloc((*bi_Y)->a, 8 * ((*bi_X)->wordlen));
#endif
		for (i = get_word_length(bi_X) - 1; i > get_word_length(bi_Y) - 1; i--)
		{
			(*bi_Y)->a[i] = 0x0; //재할당된 부분 0x0으로 초기화
		}
		(*bi_Y)->wordlen = get_word_length(bi_X);//bi_X와 wordlen이 같아졌으므로 수정
	}
	/*
	if (get_word_length(bi_X) < get_word_length(bi_Y)) //wordlen을 똑같이 맞추기위해 bi_X의 wordlen만큼 재할당 받음
	{
#if WORD_BITLEN == 8
		(*bi_X)->a = (word*)realloc((*bi_X)->a, ((*bi_Y)->wordlen));
#elif WORD_BITLEN == 32
		(*bi_X)->a = (word*)realloc((*bi_X)->a, 4 * ((*bi_Y)->wordlen));
#else //WORD_BITLEN ==64
		(*bi_X)->a = (word*)realloc((*bi_X)->a, 8 * ((*bi_Y)->wordlen));
#endif
		for (i = get_word_length(bi_Y) - 1; i > get_word_length(bi_X) - 1; i--)
		{
			(*bi_X)->a[i] = 0x0; //재할당된 부분 0x0으로 초기화
		}
		(*bi_X)->wordlen = get_word_length(bi_Y);//bi_X와 wordlen이 같아졌으므로 수정
	}
	*/
	//if (*bi_Z != NULL) //bi_Z가 할당되어있으면 초기화와 free시키고 다시 메모리할당 받음
	//{
		delete_bigint(bi_Z);
		*bi_Z = (bigint_ptr)calloc(1, sizeof(bigint));
		(*bi_Z)->wordlen = ((*bi_X)->wordlen) + 1;
		(*bi_Z)->a = (word*)calloc((*bi_Z)->wordlen, sizeof(word));
		(*bi_Z)->sign = NON_NEGATIVE;
	//}

	int borrow = 0;

	for (i = 0; i < get_word_length(bi_X); i++)
	{
		(*bi_Z)->a[i] = ((*bi_X)->a[i]) - borrow; //자동으로 모듈러해줌
		borrow = SUB_borrow((*bi_X)->a[i], borrow);
		borrow += SUB_borrow((*bi_Z)->a[i], (*bi_Y)->a[i]);
		(*bi_Z)->a[i] = (*bi_Z)->a[i] - (*bi_Y)->a[i];

		//덧셈과 별개로 최상위 워드는 따로 계산하는것은 필요없다.
	}

	refine_bigint(bi_Y);//wordlen 되돌려줌
	refine_bigint(bi_Z); //최상위 word부분에서 borrow를 빼서 0x0될 수 있으므로 refine시킨다
}

int SUB_borrow(word x, word y)
{
	if (x >= y) 
		return 0;
	
	return 1; //C<B or A<borrow

}

void multiplication(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)//pdf : 58/113
{
	delete_bigint(bi_Z);
	
	if (is_zero(bi_X) == TRUE || is_zero(bi_Y) == TRUE)
	{
		set_zero(bi_Z);
		return;
	}
	if (is_one(bi_X) == TRUE)
	{
		assign_bigint(bi_Z, bi_Y);
		return;
	}
	if (is_minus_one(bi_X) == TRUE)
	{
		assign_bigint(bi_Z, bi_Y);
		flip_sign(bi_Z);
		return;
	}
	if (is_one(bi_Y) == TRUE)
	{
		assign_bigint(bi_Z, bi_X);
		return;
	}
	if (is_minus_one(bi_Y) == TRUE)
	{
		assign_bigint(bi_Z, bi_X);
		flip_sign(bi_Z);
		return;
	}
	Schoolbook_MUL(bi_X, bi_Y, bi_Z);
}

void Schoolbook_MUL(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)
{
	int i, j;
	bigint_ptr temp = NULL;
	bigint_ptr MUL_temp = NULL;


	delete_bigint(bi_Z);
	set_zero(bi_Z);

	for (i = 0; i < get_word_length(bi_X); i++)
	{
		for (j = 0; j < get_word_length(bi_Y); j++)
		{
			//단일곱셈(MUL_temp에 임시 저장)
			MULC_word((*bi_X)->a[i], (*bi_Y)->a[j], &MUL_temp); //j번째 A와 i번째 B를 곱함(슈두코드 line4)
			//임시 저장값을 Shift이동(word_bitlen * (i+j))
			left_shift(&MUL_temp, WORD_BITLEN * (i + j));
			//c<-ADDC(C, T)
			addition(bi_Z, &MUL_temp, &temp); //line6 : c<-ADDC(C, T)
			assign_bigint(bi_Z, &temp); //line6 : c<-ADDC(C, T)
		}
	}
	//부호 결정
	if ((*bi_X)->sign != (*bi_Y)->sign)//sign이 서로 다르면 음수
		(*bi_Z)->sign = NEGATIVE;

	delete_bigint(&temp);
	delete_bigint(&MUL_temp);
	refine_bigint(bi_Z);
}


void MULC_word(word A, word B, bigint_ptr* MUL_temp)
{
	bigint_ptr T1 = NULL; //A1*B1, A0*B0 저장
	bigint_ptr T2 = NULL; //A1*B0를 저장
	bigint_ptr T3 = NULL; //A0*B1를 저장
	bigint_ptr temp = NULL; // addition할때 임시 저장을 위한 변수
	set_zero(&T1);
	set_zero(&T2);
	set_zero(&T3);
	set_zero(&temp);

	//LINE3
	//T2->a[0]<-A1*B0
	//ex)when word_bitlen = 8, a[0]<-(B&0x0f)*(A>>4)
	T2->a[0] = ((B & ((1i64 << (WORD_BITLEN / 2)) - 1)) * (A >> (WORD_BITLEN / 2)));
	//T3->a[0]<-A0*B1
	T3->a[0] = ((A & ((1i64 << (WORD_BITLEN / 2)) - 1)) * (B >> (WORD_BITLEN / 2)));

	T1->a = (word*)realloc(T1->a, 2 * (sizeof(word))); //A1*B1, A0*B0 저장을 위해 워드 2개할당
	T1->wordlen = 2;

	//LINE6
	//T->a[0]<-A0*B0
	T1->a[0] = (A & ((1i64 << (WORD_BITLEN / 2)) - 1)) * (B & ((1i64 << (WORD_BITLEN / 2)) - 1));
	//T->a[1]<-A1*B1
	T1->a[1] = (A >> (WORD_BITLEN / 2))* (B >> (WORD_BITLEN / 2));

	left_shift(&T2, WORD_BITLEN / 2); //자리수 맞춰주기
	left_shift(&T3, WORD_BITLEN / 2);

	//T1,T2,T3를 전부 더함
	addition(&T1, &T2, &temp);
	addition(&temp, &T3, MUL_temp);

	//메모리 할당 해제
	delete_bigint(&T1);
	delete_bigint(&T2);
	delete_bigint(&T3);
	delete_bigint(&temp);
}



int is_minus_one(bigint_ptr* bi_X) //multiplication함수에 사용된다.
{
	if ((*bi_X)->sign == NON_NEGATIVE || (*bi_X)->a[0] != 0x1)
		return FALSE;
	for (int i = (*bi_X)->wordlen - 1; i >= 1; i--)
	{
		if ((*bi_X)->a[i] != 0)
			return FALSE;
	}
	return TRUE;
}


void mul_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z, int* count)//1 or -1 or 0이 있는경우
{
	delete_bigint(bi_Z);
	--(*count);              //함수 시작 -> --count

	if (is_zero(bi_X) == TRUE || is_zero(bi_Y) == TRUE)
	{
		set_zero(bi_Z);
		return;
	}
	if (is_one(bi_X) == TRUE)
	{
		assign_bigint(bi_Z, bi_Y);
		return;
	}
	if (is_minus_one(bi_X) == TRUE)
	{
		assign_bigint(bi_Z, bi_Y);
		flip_sign(bi_Z);
		return;
	}
	if (is_one(bi_Y) == TRUE)
	{
		assign_bigint(bi_Z, bi_X);
		return;
	}
	if (is_minus_one(bi_Y) == TRUE)
	{
		assign_bigint(bi_Z, bi_X);
		flip_sign(bi_Z);
		return;
	}

	set_wordlen_the_same(bi_X, bi_Y); //get_word_length(bi_X)=get_word_length(bi_Y) and even
	MULC_karatsuba(bi_X, bi_Y, bi_Z, count);

	return;
}

//max(n, m) + 1
void set_wordlen_the_same(bigint_ptr* bi_X, bigint_ptr* bi_Y) //get_word_length(bi_X)=get_word_length(bi_Y) and even를 위한 함수를 호출하는 함수
{
	if (get_word_length(bi_X) == get_word_length(bi_Y)) //wordlen 같을 경우
	{
		if (get_word_length(bi_X) % 2 == 1) //when wordlen of bi_X is odd, then even이 되게 realloc해줌
			add_wordlen(bi_X, 1);

		if (get_word_length(bi_Y) % 2 == 1) //when wordlen of bi_Y is odd, then even이 되게 realloc해줌
			add_wordlen(bi_Y, 1);
	}

	if (get_word_length(bi_X) != get_word_length(bi_Y)) //wordlen 다를 경우 -> 짝수로 변환후 wordlen을 맞춰줘야함
	{
		if (get_word_length(bi_X) % 2 == 1)
			add_wordlen(bi_X, 1);

		if (get_word_length(bi_Y) % 2 == 1)
			add_wordlen(bi_Y, 1);

		if (get_word_length(bi_X) >= get_word_length(bi_Y)) //wordlen 차이만큼 realloc해줌
			add_wordlen(bi_Y, get_word_length(bi_X) - get_word_length(bi_Y));

		if (get_word_length(bi_X) <= get_word_length(bi_Y)) //wordlen 차이만큼 realloc해줌
			add_wordlen(bi_X, get_word_length(bi_Y) - get_word_length(bi_X));
	}
}

void add_wordlen(bigint_ptr* bi_X, int add_wordlen) //wordlen 늘리고 늘린 공간에 0x00으로 값설정
{
	int i;

#if WORD_BITLEN == 8
	(*bi_X)->a = (word*)realloc((*bi_X)->a, (*bi_X)->wordlen + add_wordlen); //add_wordlen 만큼 공간 늘림
	for (i = ((*bi_X)->wordlen) + add_wordlen - 1; i >= (*bi_X)->wordlen; i--) // 늘린 공간 0x00으로 값설정
	{
		(*bi_X)->a[i] = 0x00;
	}
	(*bi_X)->wordlen += add_wordlen;

#elif WORD_BITLEN == 32
	(*bi_X)->a = (word*)realloc((*bi_X)->a, 4 * ((*bi_X)->wordlen + add_wordlen));
	for (i = ((*bi_X)->wordlen) + add_wordlen - 1; i >= (*bi_X)->wordlen; i--)
	{
		(*bi_X)->a[i] = 0x00;
	}
	(*bi_X)->wordlen += add_wordlen;

#else
	(*bi_X)->a = (word*)realloc((*bi_X)->a, 8 * ((*bi_X)->wordlen + add_wordlen));
	for (i = ((*bi_X)->wordlen) + add_wordlen - 1; i >= (*bi_X)->wordlen; i--)
	{
		(*bi_X)->a[i] = 0x00;
	}
	(*bi_X)->wordlen += add_wordlen;
#endif
}

void MULC_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z, int* count)// count = 3으로 세팅하기를 권장
{ 
	delete_bigint(bi_Z);
	set_zero(bi_Z); //bi_Z를 0x00 세팅

	
	bigint_ptr A0 = NULL;
	bigint_ptr A1 = NULL;
	bigint_ptr B0 = NULL;
	bigint_ptr B1 = NULL;

	bigint_ptr T0 = NULL; //A0*B0 저장
	bigint_ptr T1 = NULL; //A1*B1 저장

	bigint_ptr R = NULL;  //T1<<(2 * l * word_bitlen) +T0 저장
	
	bigint_ptr S0 = NULL; //A0-A1
	bigint_ptr S1 = NULL; //B1-B0
	


	int count_temp = (*count);

	
	if ((*bi_X)->sign != (*bi_Y)->sign) //부호 세팅
		(*bi_Z)->sign = NEGATIVE;


	int l = (get_word_length(bi_X) / 2); // l<-(max(wordlen A or B) + 1 ) >> 1

	//LINE6, 7
	assign_bigint(&A0, bi_X);
	reduction(&A0, l * WORD_BITLEN); // A0 mod 2^lw
	assign_bigint(&A1, bi_X);
	right_shift(&A1, l * WORD_BITLEN); // A1>>lw

	assign_bigint(&B0, bi_Y);
	reduction(&B0, l * WORD_BITLEN); // B0 mod 2^lw
	assign_bigint(&B1, bi_Y);
	right_shift(&B1, l * WORD_BITLEN); // B1>>lw

	//LINE8
	if ((*count) > 0) //whem count>0, mul_karatsuba
		mul_karatsuba(&A0, &B0, &T0, count);
	if ((*count) <= 0)
		multiplication(&A0, &B0, &T0); //T0<-A0*B0

	(*count) = count_temp;

	if ((*count) > 0) //whem count>0, mul_karatsuba
		mul_karatsuba(&A1, &B1, &T1, count);
	if ((*count) <= 0)
		multiplication(&A1, &B1, &T1); //T1<-A1*B1

	//LINE9
	left_shift(&T1, 2* l * WORD_BITLEN);
	addition(&T1, &T0, &R);
	right_shift(&T1, 2 * l * WORD_BITLEN); //다시 되돌려줌
	
	//LINE10
	subtraction(&A0, &A1, &S0); //S0<-A0-A1
	subtraction(&B1, &B0, &S1); //S1<-B1-B0

	//LINE11
	//부호 결정
	if (S1->sign == S0->sign)
		(*bi_Z)->sign = NON_NEGATIVE;
	if (S1->sign != S0->sign)
		(*bi_Z)->sign = NEGATIVE;
	//mul_karatsuba
	//슈두코드에서의 S는 bi_Z로
	(*count) = count_temp;
	if ((*count) > 0) //whem count>0, mul_karatsuba
		mul_karatsuba(&S1, &S0, bi_Z, count);
	if ((*count) <= 0)
		multiplication(&S1, &S0, bi_Z);

	//LINE12~
	//T0+T1+S
	addition(&T0, &T1, &A0); //addition 함수내에서 &A0 delete_bigint해주고 메모리 할당해줌
	addition(&A0, bi_Z, &S1);
	left_shift(&S1, l * WORD_BITLEN);
	addition(&R, &S1, bi_Z);

	delete_bigint(&A0);
	delete_bigint(&A1); 
	delete_bigint(&B0);
	delete_bigint(&B1);
	delete_bigint(&T0);
	delete_bigint(&T1);
	delete_bigint(&R);
	delete_bigint(&S0);
	delete_bigint(&S1);
}

void squaring(bigint_ptr* bi_X, bigint_ptr* bi_Z) //word의 값이 -1, 0, 1일 경우 또는 wordlen = 1 인 경우
{
	delete_bigint(bi_Z);
	
	if (is_zero(bi_X) == TRUE)
	{
		set_zero(bi_Z);
		return;
	}
	if (is_one(bi_X) == TRUE)
	{
		set_one(bi_Z);
		return;
	}
	if (is_minus_one(bi_X) == TRUE)
	{
		set_one(bi_Z);
		return;
	}
	if (get_word_length(bi_X) == 1) //SUARING_C로 갈 필요가 없다.(단일 곱셈이기 때문)
	{
		SQUARING_C_ONE_WORD((*bi_X)->a[0], bi_Z);
		return;
	}
	SQUARING_C(bi_X, bi_Z); //위를 만족하지 않으면 일반적인 제곱연산
}

void SQUARING_C(bigint_ptr* bi_X, bigint_ptr* bi_Z) // pdf73/113 슈두코드 참고
{
	bigint_ptr C1 = NULL;   
	bigint_ptr C2 = NULL;   
	bigint_ptr T1 = NULL;
	bigint_ptr T2 = NULL;
	bigint_ptr Temp = NULL;
	
	//밑에 addition(&T1, &C1, &Temp)에서 첫값으로 C1이 NULL이 안들어가게 하기 위해서(초기값 설정이라고 보면된다)
	//line 1
	set_zero(&C1);
	set_zero(&C2);
	set_zero(&Temp);
	set_zero(bi_Z);

	int i, j = 0;
	for (j = 0; j < get_word_length(bi_X); j++)
	{
		SQUARING_C_ONE_WORD((*bi_X)->a[j], &T1); //단일워드 제곱
		left_shift(&T1, 2 * j * WORD_BITLEN); //line4 T1 << 2jw
		addition(&T1, &C1, &Temp); //T1 + C1
		assign_bigint(&C1, &Temp); //C1 <- Temp(=T1 + C1)

		set_zero(&Temp);

		for (i = j + 1; i < get_word_length(bi_X); i++)
		{
			MULC_word((*bi_X)->a[j], (*bi_X)->a[i], &T2); //J번째 A와 I번째 A를 단일워드 곱셈
			left_shift(&T2, (i + j) * WORD_BITLEN); //T2 << (i + j)w
			addition(&C2, &T2, &Temp); //C2 + T2
			assign_bigint(&C2, &Temp); //C2 <- Temp(=C2 + T2)

			set_zero(&Temp);
		}
	}
	left_shift(&C2, 1);
	addition(&C1, &C2, bi_Z);
	refine_bigint(bi_Z);
	
	delete_bigint(&C1);
	delete_bigint(&C2);
	delete_bigint(&T1);
	delete_bigint(&T2);
	delete_bigint(&Temp);
}

void SQUARING_C_ONE_WORD(word A, bigint_ptr* squared_A) //pdf 69/113참고
{
	bigint_ptr C = NULL;
	bigint_ptr T = NULL;
	set_zero(&C);
	set_zero(&T);
	
	delete_bigint(squared_A);
	
	//C와 T는 [0,W^2)에 속해 있으므로 공간 할당해줘야함
#if WORD_BITLEN == 8
	C->a = (word*)realloc(C->a, 2 * (sizeof(word)));
#elif WORD_BITLEN == 32
	C->a = (word*)realloc(C->a, 2 * (sizeof(word)));
#else 
	C->a = (word*)realloc(C->a, 2 * (sizeof(word)));
#endif
	C->wordlen = 2;

	word A0 = A & ((1i64 << (WORD_BITLEN / 2)) - 1);
	word A1 = A >> (WORD_BITLEN / 2);
	word C0 = A0 * A0; //C0는 [0, W)
	word C1 = A1 * A1; //C1는 [0, W)

	//C <- (C1 << w) + C0
	C->a[0] = C0; //C->a[0] <- C0
	C->a[1] = C1; //C->a[1] <-(C1 << w)

	T->a[0] = A0 * A1; //line4 T는 [0, W)
	
	left_shift(&T, (WORD_BITLEN / 2) + 1); //line5
	addition(&C, &T, squared_A); //line6
	
	//메모리 해제
	delete_bigint(&C);
	delete_bigint(&T);
}

void squaring_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Z, int* count)
{
	delete_bigint(bi_Z);
	(*count)--;

	if (is_zero(bi_X) == TRUE)
	{
		set_zero(bi_Z);
		return;
	}
	if (is_one(bi_X) == TRUE)
	{
		set_one(bi_Z);
		return;
	}
	if (is_minus_one(bi_X) == TRUE)
	{
		set_one(bi_Z);
		return;
	}
	if (get_word_length(bi_X) == 1)
	{
		SQUARING_C_ONE_WORD((*bi_X)->a[0], bi_Z);
		return;
	}
	SQUARING_C_karatsuba(bi_X, bi_Z, count);
}

void SQUARING_C_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Z, int* count)
{
	int depth = (*count);
	bigint_ptr A0 = NULL; // mod 2^lw, 뒤
	bigint_ptr A1 = NULL; // A >> lw, 앞
	bigint_ptr T0 = NULL;
	bigint_ptr T1 = NULL;
	bigint_ptr R = NULL;
	bigint_ptr S = NULL;
	
	set_zero(bi_Z);

	if (get_word_length(bi_X) % 2 == 1) //홀수일 경우
		add_wordlen(bi_X, 1); //word를 하나 추가해서 wordlen이 짝수가 되게 해줌

	int l = (get_word_length(bi_X) / 2); // l<-(max(wordlen A or B) + 1 ) >> 1

	//A0 <- A mod 2^lw
	assign_bigint(&A0, bi_X);
	reduction(&A0, (l * WORD_BITLEN));
	//A1 <- A >> lw
	assign_bigint(&A1, bi_X);
	right_shift(&A1, (l * WORD_BITLEN));


	if ((*count) > 0) //if count > 0, then Karatsuba
		squaring_karatsuba(&A0, &T0, count);
	if ((*count) <= 0) //else general squaring
		squaring(&A0, &T0);

	(*count) = depth; // 위에 조건문에서 squaring_karatsuba 호출을 이용해서 count줄였기에 
	                  //원래값으로 되돌림
	
	if ((*count) > 0) //if count > 0, then Karatsuba
		squaring_karatsuba(&A1, &T1, count);
	if ((*count) <= 0) //else general squaring
		squaring(&A1, &T1);

	left_shift(&T1, 2* l * WORD_BITLEN);// line8
	addition(&T1, &T0, &R);

	(*count) = depth; // 위에 조건문에서 squaring_karatsuba 호출을 이용해서 count줄였기에 
					  //원래값으로 되돌림
	//line9
	if ((*count) > 0)
		mul_karatsuba(&A1, &A0, &S, count);
	if ((*count) <= 0)
		multiplication(&A1, &A0, &S);

	//lin10
	left_shift(&S, (l * WORD_BITLEN) + 1);
	//line11
	addition(&R, &S, bi_Z);

	//메모리할당해제
	delete_bigint(&A0);
	delete_bigint(&A1);
	delete_bigint(&T0);
	delete_bigint(&T1);
	delete_bigint(&R);
	delete_bigint(&S);
}

//bi_X를 bi_Y로 나누었을때, 목 : bi_Q, 나머지 : bi_R
void division_binary_long(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Q, bigint_ptr* bi_R) //pdf 81/113 참고
{
	bigint_ptr T1 = NULL;
	bigint_ptr T2 = NULL;
	
	set_zero(bi_Q);
	set_zero(bi_R);
	set_zero(&T1);
	set_zero(&T2);

	if (is_zero(bi_Y) == TRUE || (*bi_Y)->sign == NEGATIVE) // bi_Y <= 0 일 경우
	{
		printf("INVALID \n");
		return;
	}

	for (int i = get_bit_length(bi_X) - 1; i >= 0; i--)
	{
		left_shift(bi_R, 1); //line3
		(*bi_R)->a[0] ^= get_j_th_bit(bi_X, i); //line3

		if (compare(bi_R, bi_Y) != -1) //R>=B일때이므로 R<B인 경우가 -1이므로 NOT으로 하면된다.
		{
			subtraction(bi_R, bi_Y, &T1); //빼주면 나머지 나옴
			assign_bigint(bi_R, &T1); //그 나머지 Temp1을 bi_R로 복사

			set_one(&T2); //뺏다는건 몫이 1이 생겨야 한다는 것
			left_shift(&T2, i); //몫이 위치할 곳으로 shift이동
			set_wordlen_the_same(bi_Q, &T2); //기존의 bi_Q를 T2와 XOR하기를 원한다. 따라서 XOR하기 전에 wordlen을 맞춰주자
			for (int j = 0; j < get_word_length(&T2); j++)
			{
				(*bi_Q)->a[j] ^= T2->a[j];
			}
		}
	}
	refine_bigint(bi_Q);
	refine_bigint(bi_R);

	delete_bigint(&T1);
	delete_bigint(&T2);
}

// bi_X = bi_Y * bi_Q + bi_R
void division_naive(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Q, bigint_ptr* bi_R)
{
	if (is_zero(bi_Y) == TRUE || (*bi_Y)->sign == NEGATIVE) // bi_Y <= 0 일 경우
	{
		printf("INVALID \n");
		return;
	}

	if (compare(bi_X, bi_Y) == -1) //bi_X < bi_Y 일 경우
	{
		//Trivial
		set_zero(bi_Q); //몫이 0
		assign_bigint(bi_R, bi_X); //나머지 bi_X
		return;
	}

	if (is_one(bi_Y) == TRUE) //bi_Y = 1 일 경우
	{
		//Trivial
		assign_bigint(bi_Q, bi_X); // 몫은 bi_Y
		set_zero(bi_R); //나머지는 0
		return;
	}

	//line 10 : (Q, R) <- (0, A)
	set_zero(bi_Q);
	assign_bigint(bi_R, bi_X);

	bigint_ptr one_bigint_for_quotient = NULL;
	bigint_ptr Temp1 = NULL;
	bigint_ptr Temp2 = NULL;

	//초기값 설정
	set_zero(&Temp1);
	set_zero(&Temp2);
	//Q <- Q + 1 위한 bigint
	set_one(&one_bigint_for_quotient);

	while (compare(bi_R, bi_Y) == 1 || compare(bi_R, bi_Y) == 0) //bi_R >= bi_Y
	{
		//Q <- Q + 1
		addition(bi_Q, &one_bigint_for_quotient, &Temp1);
		assign_bigint(bi_Q, &Temp1);

		//R <- R - B
		subtraction(bi_R, bi_Y, &Temp2);
		assign_bigint(bi_R, &Temp2);
		
		
		//printf("bi_R = ");
		//show_bigint2(bi_Q, 16);
		//printf("bi_R = ");
		//show_bigint2(bi_R, 16);
		//printf("bi_Y = ");
		//show_bigint2(bi_Y, 16);
		

		//printf("bi_R의 wordlen : %d \n",(*bi_R)->wordlen);
		//printf("bi_Y의 wordlen : %d \n", (*bi_Y)->wordlen);
	}

	delete_bigint(&one_bigint_for_quotient);
	delete_bigint(&Temp1);
	delete_bigint(&Temp2);
}

void division_multi_precision_long(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Q, bigint_ptr* bi_R)
{
	if (is_zero(bi_Y) == TRUE || (*bi_Y)->sign == NEGATIVE) // bi_Y <= 0 일 경우
	{
		printf("INVALID \n");
		return;
	}

	if (compare(bi_X, bi_Y) == -1) //bi_X < bi_Y 일 경우
	{
		//Trivial
		set_zero(bi_Q); //몫이 0
		assign_bigint(bi_R, bi_X); //나머지 bi_X
		return;
	}

	if (is_one(bi_Y) == TRUE) //bi_Y = 1 일 경우
	{
		//Trivial
		assign_bigint(bi_Q, bi_X); // 몫은 bi_Y
		set_zero(bi_R); //나머지는 0
		return;
	}

	//line 7 : (Q, R) <- (0, 0)
	set_zero(bi_Q);
	set_zero(bi_R);

	//Q는 wordlen이 bi_X와 같아야하고 모든값이 0이어야함
	//bi_Q는 현재 wordlen이 1이므로 bi_X와 wordlen이 같을려면 wordlen-1만큼 더해줘야함
	add_wordlen(bi_Q, ((*bi_X)->wordlen) - 1);

	refine_bigint(bi_Y);
	int i = 0;
	int k = 0;
	bigint_ptr To_find_k = NULL;
	set_zero(&To_find_k);

	//line8
	for (i = ((*bi_X)->wordlen) - 1; i >= 0; i--)
	{
		//printf("%d \n", i);
		left_shift(bi_R, WORD_BITLEN); //RW
		(*bi_R)->a[0] = (*bi_X)->a[i]; //R <- RW + i번째 A
		
		if(i== ((*bi_X)->wordlen) - 1)//word_bit가 8일때 예로 들면 처음에는 00000000 a[0] 이기에 refine을 시켜줌
			refine_bigint(bi_R);

		//line 4 : k를 찾는다. 2^k * m-1번째 bi_Y 가 [2^w-1, 2^w)에 속하도록 
		//k는 한번만 찾아주면된다.
		//div_core에서 이쪽으로 이동시켜줬음
		if (i == ((*bi_X)->wordlen) - 1)
		{
			To_find_k->a[0] = (*bi_Y)->a[((*bi_Y)->wordlen) - 1];
			for (int cnt_i = 0;; cnt_i++)
			{
				//m-1번째 bi_Y와 0b1000 0000 와 and연산을 해서 1000 0000 이 나오면 2^w-1 이상이므로
				//k를 찾을 수 있음
				if (((To_find_k->a[0]) & (1i64 << (WORD_BITLEN - 1))) == (1i64 << (WORD_BITLEN - 1)))
				{
					k = cnt_i;
					break;
				}
				To_find_k->a[0] <<= 1;
			}
		}	
		div_core(bi_R, bi_Y, (*bi_Q)->a, i, bi_R, k); //i번째 Q에 값이 들어가기 위해, (*bi_Q)->a와 i를 다음 함수로 넘겨줌, R에 값이 들어가기위해 R도 넘겨줌
	}

	refine_bigint(bi_Q);

	delete_bigint(&To_find_k);
}

void div_core(bigint_ptr* bi_X, bigint_ptr* bi_Y, word* Q_ith_word, int i, bigint_ptr* return_bi_R, int k)
{
	int cnt_i = 0;

	if (compare(bi_X, bi_Y) == -1) //bi_X < bi_Y 일 경우
	{
		//return (0, A)
		Q_ith_word[i] = 0x0;
	   // assign_bigint(return_bi_R, bi_X); //은 할 필요가 없음. 둘다 이전 함수에서 bi_R의 주소를 가져온것이므로
		return;
	}

	//line5
	bigint_ptr prime_bi_X = NULL;
	bigint_ptr prime_bi_Y = NULL;
	bigint_ptr prime_bi_Q = NULL;
	bigint_ptr prime_bi_R = NULL;
	
	if (k == 0)
	{
		assign_bigint(&prime_bi_X, bi_X); //prime_bi_X <- 2^k * bi_X
		assign_bigint(&prime_bi_Y, bi_Y);
	}
	if (k != 0)
	{
		left_shift(bi_X, k); //2^k * bi_X
		assign_bigint(&prime_bi_X, bi_X); //prime_bi_X <- 2^k * bi_X
		right_shift(bi_X, k); // bi_X는 되돌려줌

		left_shift(bi_Y, k); //2^k * bi_Y
		assign_bigint(&prime_bi_Y, bi_Y);
		right_shift(bi_Y, k); // bi_Y는 되돌려줌
	}
	//line6 : prime_Q, prime_R <- div_core_core(prime_A, prime_B)
	div_core_core(&prime_bi_X, &prime_bi_Y, &prime_bi_Q, &prime_bi_R);

	//line7
	Q_ith_word[i] = prime_bi_Q->a[0];
	
	if(k != 0)
		right_shift(&prime_bi_R, k);
	
	assign_bigint(return_bi_R, &prime_bi_R);

	delete_bigint(&prime_bi_X);
	delete_bigint(&prime_bi_Y);
	delete_bigint(&prime_bi_Q);
	delete_bigint(&prime_bi_R);

}

void div_core_core(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* return_bi_Q, bigint_ptr* return_bi_R)
{
	bigint_ptr Temp1 = NULL;
	bigint_ptr Temp2 = NULL;
	bigint_ptr Temp3 = NULL;

	bigint_ptr Q = NULL;
	bigint_ptr R = NULL;
	bigint_ptr one = NULL;

	int count = COUNT;

	//초기값 설정이 필요한 것들만 세팅
	set_one(&one);
	
	set_zero(&Q);
	set_zero(&R);

	set_zero(&Temp1);
	set_zero(&Temp2);
	set_zero(&Temp3);

	if ((*bi_X)->wordlen == (*bi_Y)->wordlen)
	{
		Temp1->a[0] = (*bi_X)->a[((*bi_X)->wordlen) - 1];
		Temp2->a[0] = (*bi_Y)->a[((*bi_Y)->wordlen) - 1];

		division_binary_long(&Temp1, &Temp2, &Q, &R);
		set_zero(&R);
	}

	if ((*bi_X)->wordlen == ((*bi_Y)->wordlen) + 1)
	{
		if ((*bi_X)->a[(*bi_Y)->wordlen] == (*bi_Y)->a[((*bi_Y)->wordlen) - 1])
		{
			//W - 1
#if WORD_BITLEN == 8
			Q->a[0] = 0xff;
#elif WORD_BITLEN == 32
			Q->a[0] = 0xffffffff;
#else 
			Q->a[0] = 0xffffffffffffffff;
#endif
		}
		else
		{
			Temp1->a[0] = (*bi_X)->a[(*bi_Y)->wordlen];
			left_shift(&Temp1, WORD_BITLEN);
			Temp1->a[0] = (*bi_X)->a[((*bi_Y)->wordlen) - 1];
			
			Temp2->a[0] = (*bi_Y)->a[((*bi_Y)->wordlen) - 1];

			division_binary_long(&Temp1, &Temp2, &Q, &R);
			set_zero(&R);
		}
	}

	mul_karatsuba(bi_Y, &Q, &Temp3, &count);
	
	subtraction(bi_X, &Temp3, &R);
	
	int i = 0;
	while (R->sign == NEGATIVE)//compare(&R, &zero)==-1
	{
		subtraction(&Q, &one, &Temp1);
		assign_bigint(&Q, &Temp1);
		
		addition(&R, bi_Y, &Temp2);
		assign_bigint(&R, &Temp2);
	}
	
	assign_bigint(return_bi_Q, &Q);
	assign_bigint(return_bi_R, &R);

	delete_bigint(&Temp1);
	delete_bigint(&Temp2);
	delete_bigint(&Temp3);
	delete_bigint(&Q);
	delete_bigint(&R);
	delete_bigint(&one);
	
}

void left_to_right_binary_method_EXPO(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y)
{
	return 0;
}
void left_to_right_binary_method_CM(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y)
{
	return 0;
}
void right_to_left_binary_method_EXPO(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y)
{
	return 0;
}

void right_to_left_binary_method_CM(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y)
{
	return 0;
}
void multiply_and_squaring_EXPO(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y)
{
	return 0;
}
void multiply_and_squaring_CM(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y)
{
	return 0;
}