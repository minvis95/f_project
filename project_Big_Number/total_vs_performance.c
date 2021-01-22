#include "Pandora.h"

#if 0
void total_vs_performance()
{
	unsigned long long cycles_mid1 = 0, cycles_mid2 = 0, cycles1 = 0, cycles2 = 0, cycles3 = 0, cycles4 = 0, totalcycle1 = 0, totalcycle2 = 0;
	unsigned long long cycles_a = 0, cycles_b = 0, cycles_c = 0, cycles_d = 0, cycles_e = 0, cycles_f = 0, cycles_g = 0, cycles_h = 0, cycles_i = 0, cycles_j = 0, cycles_k = 0, cycles_l = 0;
	unsigned long long cycles_mid_a = 0, cycles_mid_b = 0, cycles_mid_c = 0, cycles_mid_d = 0, cycles_mid_e = 0, cycles_mid_f = 0;
	unsigned long long totalcycle_a = 0, totalcycle_b = 0, totalcycle_c = 0, totalcycle_d = 0, totalcycle_e = 0, totalcycle_f = 0;

	//아래 세개의 변수의 값만 수정하면됌
	int i = 100; //몇번 계산하겠는가?
	int temp_i = i;
	int wordlen_size1 = 60;// A의 wordlen 크기
	int wordlen_size2 = 40;// B의 wordlen 크기
	int count = COUNT;

	while ((i >= 0)) //bigint를 생성하고 연산을 하고 메모리 해제하는 시간을 측정
	{
		i--;


		bigint_ptr A = NULL;
		bigint_ptr B = NULL;
		bigint_ptr Z = NULL;
		bigint_ptr Q = NULL;
		bigint_ptr R = NULL;

		bigint_ptr temp = NULL;
		bigint_ptr temp2 = NULL;


		generate_random_bigint(&A, NON_NEGATIVE, wordlen_size1);
		generate_random_bigint(&B, NON_NEGATIVE, wordlen_size2);

		assign_bigint(&temp, &A);
		assign_bigint(&temp2, &B);

		cycles_a = cpucycles(); //측정 시작
		addition(&A, &B, &Z);
		cycles_b = cpucycles(); //측정 종료

		cycles_mid_a = cycles_b - cycles_a; //종료 - 시작
		totalcycle_a += cycles_mid_a;// totalcycle에 더해줌

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);

		cycles_c = cpucycles(); //측정 시작
		subtraction(&A, &B, &Z);
		cycles_d = cpucycles(); //측정 종료

		cycles_mid_b = cycles_d - cycles_c; //종료 - 시작
		totalcycle_b += cycles_mid_b;// totalcycle에 더해줌

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Z);

		cycles_g = cpucycles(); //측정 시작
		mul_karatsuba(&A, &B, &Z, &count);
		cycles_h = cpucycles(); //측정 종료

		cycles_mid_d = cycles_h - cycles_g; //종료 - 시작
		totalcycle_d += cycles_mid_d;// totalcycle에 더해줌

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Z);

		cycles_e = cpucycles(); //측정 시작
		multiplication(&A, &B, &Z);
		cycles_f = cpucycles(); //측정 종료

		cycles_mid_c = cycles_f - cycles_e; //종료 - 시작
		totalcycle_c += cycles_mid_c;// totalcycle에 더해줌

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Z);

		cycles_i = cpucycles(); //측정 시작
		squaring(&A, &Z);
		cycles_j = cpucycles(); //측정 종료

		cycles_mid_e = cycles_j - cycles_i; //종료 - 시작
		totalcycle_e += cycles_mid_e;// totalcycle에 더해줌

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Z);

		cycles_k = cpucycles(); //측정 시작
		squaring_karatsuba(&A, &Z, &count);
		cycles_l = cpucycles(); //측정 종료

		cycles_mid_f = cycles_l - cycles_k; //종료 - 시작
		totalcycle_f += cycles_mid_f;// totalcycle에 더해줌

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Z);
		
		cycles1 = cpucycles(); //측정 시작
		division_multi_precision_long(&A, &B, &Q, &R);
		cycles2 = cpucycles(); //측정 종료

		cycles_mid1 = cycles2 - cycles1; //종료 - 시작
		totalcycle1 += cycles_mid1;// totalcycle에 더해줌

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Q);
		set_zero(&R);
		set_zero(&Z);

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

	printf("ADDITION ->         totalcycles/ %d = %10lld \n", temp_i, totalcycle_a / temp_i); //평균값을 내주기 위해 i로 나누어줌
	printf("SUBTRACTION ->      totalcycles/ %d = %10lld \n", temp_i, totalcycle_b / temp_i); //평균값을 내주기 위해 i로 나누어줌
	printf("MULTIPLICATION ->   totalcycles/ %d = %10lld \n", temp_i, totalcycle_c / temp_i); //평균값을 내주기 위해 i로 나누어줌
	printf("MUL_KARATSUBA ->    totalcycles/ %d = %10lld \n", temp_i, totalcycle_d / temp_i); //평균값을 내주기 위해 i로 나누어줌
	printf("SQUARING ->         totalcycles/ %d = %10lld \n", temp_i, totalcycle_e / temp_i); //평균값을 내주기 위해 i로 나누어줌
	printf("SQUAR_KARATSUBA ->  totalcycles/ %d = %10lld \n", temp_i, totalcycle_f / temp_i); //평균값을 내주기 위해 i로 나누어줌
	printf("LONG DIVISION ->    totalcycles/ %d = %10lld \n", temp_i, totalcycle1 / temp_i); //평균값을 내주기 위해 i로 나누어줌
	printf("MULTI PRECISION ->  totalcycles/ %d = %10lld \n", temp_i, totalcycle2 / temp_i); //평균값을 내주기 위해 i로 나누어줌

	return 0;

}
#endif