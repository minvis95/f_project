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
int test_division_multi_precision_long_verification()
{
	int cnt_i = 80; //10이면 10번 100이면 100번

	while ((cnt_i > 0))
	{
		bigint_ptr X = NULL;
		bigint_ptr Y = NULL;
		bigint_ptr Q = NULL;
		bigint_ptr R = NULL;

		generate_random_bigint(&X, NON_NEGATIVE, ((rand() & 0x07) << 1) ^ 0x01);
		generate_random_bigint(&Y, NON_NEGATIVE, ((rand() & 0x07) << 1) ^ 0x01);

		printf("x = ");
		show_bigint(X, 16);
		printf("y = ");
		show_bigint(Y, 16);

		//multi_precision 나눗셈 연산
		division_multi_precision_long(&X, &Y, &Q, &R);

		printf("q = ");
		show_bigint(Q, 16);
		printf("r = ");
		show_bigint(R, 16);

		printf("print(x//y == q)\n"); //sage로 //는 몫
		printf("print(x%%y == r)\n"); //sage로 %는 나머지

		//메모리 해제
		delete_bigint(&X);
		delete_bigint(&Y);
		delete_bigint(&Q);
		delete_bigint(&R);

		delay(300); //이 line이 없으면 똑같은 random_bigint 값을 생성할 수도 있다.
		cnt_i--;
	}
	return 0;
}
#endif

//Performance
#if 0
int test_division_multi_precision_long_performance()
{
	unsigned long long cycles = 0, cycles1 = 0, cycles2 = 0, totalcycle = 0;

	//아래 세개의 변수의 값만 수정하면됌
	int i = 100; //몇번 계산하겠는가?
	int temp_i = i;
	int wordlen_size1 = 40;// A의 wordlen 크기
	int wordlen_size2 = 10;// B의 wordlen 크기
	
	while ((i >= 0)) //bigint를 생성하고 연산을 하고 메모리 해제하는 시간을 측정
	{
		i--;

		bigint_ptr A = NULL;
		bigint_ptr B = NULL;
		bigint_ptr Q = NULL;
		bigint_ptr R = NULL;

		generate_random_bigint(&A, NON_NEGATIVE, wordlen_size1);
		generate_random_bigint(&B, NON_NEGATIVE, wordlen_size2);

		cycles1 = cpucycles(); //측정 시작
		division_multi_precision_long(&A, &B, &Q, &R);
		cycles2 = cpucycles(); //측정 종료

		cycles = cycles2 - cycles1; //종료 - 시작
		totalcycle += cycles;// totalcycle에 더해줌

		delete_bigint(&A);
		delete_bigint(&B);
		delete_bigint(&Q);
		delete_bigint(&R);
	}

	printf(" totalcycles/ %d = %10lld \n", temp_i, totalcycle / temp_i); //평균값을 내주기 위해 i로 나누어줌

	return 0;
}
#endif

//binary long division과 multi_precision 비교
#if 0
int test_binary_long_division_vs_multi_precision_long_performance()
{
	unsigned long long cycles_mid1 = 0, cycles_mid2 = 0, cycles1 = 0, cycles2 = 0, cycles3 = 0, cycles4 = 0, totalcycle1 = 0, totalcycle2 = 0;

	//아래 세개의 변수의 값만 수정하면됌
	int i = 100; //몇번 계산하겠는가?
	int temp_i = i;
	int wordlen_size1 = 40;// A의 wordlen 크기
	int wordlen_size2 = 10;// B의 wordlen 크기

	while ((i >= 0)) //bigint를 생성하고 연산을 하고 메모리 해제하는 시간을 측정
	{
		i--;

		bigint_ptr A = NULL;
		bigint_ptr B = NULL;
		bigint_ptr Q = NULL;
		bigint_ptr R = NULL;

		generate_random_bigint(&A, NON_NEGATIVE, wordlen_size1);
		generate_random_bigint(&B, NON_NEGATIVE, wordlen_size2);

		cycles1 = cpucycles(); //측정 시작
		division_multi_precision_long(&A, &B, &Q, &R);
		cycles2 = cpucycles(); //측정 종료

		cycles_mid1 = cycles2 - cycles1; //종료 - 시작
		totalcycle1 += cycles_mid1;// totalcycle에 더해줌

		cycles3 = cpucycles(); //측정 시작
		division_multi_precision_long(&A, &B, &Q, &R);
		cycles4 = cpucycles(); //측정 종료

		cycles_mid2 = cycles4 - cycles3; //종료 - 시작
		totalcycle2 += cycles_mid2;// totalcycle에 더해줌

		delete_bigint(&A);
		delete_bigint(&B);
		delete_bigint(&Q);
		delete_bigint(&R);
	}

	printf(" totalcycles/ %d = %10lld \n", temp_i, totalcycle1 / temp_i); //평균값을 내주기 위해 i로 나누어줌
	printf(" totalcycles/ %d = %10lld \n", temp_i, totalcycle2 / temp_i); //평균값을 내주기 위해 i로 나누어줌
	return 0;
}
#endif
