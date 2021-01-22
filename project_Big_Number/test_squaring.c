#include "Pandora.h"

//Verification and Performance check!!

#if 0
//똑같은 random_bigint값을 생성할 수 없게 delay함수를 추가
void delay(clock_t n)
{
	clock_t start = clock();

	while (clock() - start < n);

}

//Verification
int test_squaring_verification()
{
	int cnt_i = 80; //10이면 10번 100이면 100번
	int random_sign = 0;

	while ((cnt_i > 0))
	{
		bigint_ptr X = NULL;
		bigint_ptr Z = NULL;

		//NON_NEGATIVE와 NEGATIVE 랜덤하게
		random_sign = rand() & 0x01;

		//wordlen은 무조건 1이상
		generate_random_bigint(&X, random_sign, ((rand() & 0x07) << 1) ^ 0x01);

		printf("x = ");
		show_bigint(X, 16);
		
		//제곱 연산
		squaring(&X, &Z);

		printf("z = ");
		show_bigint(Z, 16);

		printf("print(x * x == z)\n");

		//초기화
		random_sign = 0;

		//메모리 해제
		delete_bigint(&X);
		delete_bigint(&Z);

		delay(300); //이 line이 없으면 똑같은 random_bigint 값을 생성할 수도 있다.
		cnt_i--;
	}
	return 0;
}
#endif

//Performance
#if 0
int test_squaring_performance()
{
	unsigned long long cycles = 0, cycles1 = 0, cycles2 = 0, totalcycle = 0;

	//아래 세개의 변수의 값만 수정하면됌
	int i = 100; //몇번 계산하겠는가?
	int temp_i = i;
	int wordlen_size1 = 100;// A의 wordlen 크기


	while ((i >= 0)) //bigint를 생성하고 연산을 하고 메모리 해제하는 시간을 측정
	{
		i--;

		bigint_ptr A = NULL;
		bigint_ptr C = NULL;

		generate_random_bigint(&A, NON_NEGATIVE, wordlen_size1);

		cycles1 = cpucycles(); //측정 시작
		squaring(&A, &C);
		cycles2 = cpucycles(); //측정 종료

		cycles = cycles2 - cycles1; //종료 - 시작
		totalcycle += cycles;// totalcycle에 더해줌

		delete_bigint(&A);
		delete_bigint(&C);

	}

	printf(" totalcycles/ %d = %10lld", temp_i, totalcycle / temp_i); //평균값을 내주기 위해 i로 나누어줌

	return 0;
}
#endif