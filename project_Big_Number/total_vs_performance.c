#include "Pandora.h"

#if 0
void total_vs_performance()
{
	unsigned long long cycles_mid1 = 0, cycles_mid2 = 0, cycles1 = 0, cycles2 = 0, cycles3 = 0, cycles4 = 0, totalcycle1 = 0, totalcycle2 = 0;
	unsigned long long cycles_a = 0, cycles_b = 0, cycles_c = 0, cycles_d = 0, cycles_e = 0, cycles_f = 0, cycles_g = 0, cycles_h = 0, cycles_i = 0, cycles_j = 0, cycles_k = 0, cycles_l = 0;
	unsigned long long cycles_mid_a = 0, cycles_mid_b = 0, cycles_mid_c = 0, cycles_mid_d = 0, cycles_mid_e = 0, cycles_mid_f = 0;
	unsigned long long totalcycle_a = 0, totalcycle_b = 0, totalcycle_c = 0, totalcycle_d = 0, totalcycle_e = 0, totalcycle_f = 0;

	//�Ʒ� ������ ������ ���� �����ϸ��
	int i = 100; //��� ����ϰڴ°�?
	int temp_i = i;
	int wordlen_size1 = 60;// A�� wordlen ũ��
	int wordlen_size2 = 40;// B�� wordlen ũ��
	int count = COUNT;

	while ((i >= 0)) //bigint�� �����ϰ� ������ �ϰ� �޸� �����ϴ� �ð��� ����
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

		cycles_a = cpucycles(); //���� ����
		addition(&A, &B, &Z);
		cycles_b = cpucycles(); //���� ����

		cycles_mid_a = cycles_b - cycles_a; //���� - ����
		totalcycle_a += cycles_mid_a;// totalcycle�� ������

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);

		cycles_c = cpucycles(); //���� ����
		subtraction(&A, &B, &Z);
		cycles_d = cpucycles(); //���� ����

		cycles_mid_b = cycles_d - cycles_c; //���� - ����
		totalcycle_b += cycles_mid_b;// totalcycle�� ������

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Z);

		cycles_g = cpucycles(); //���� ����
		mul_karatsuba(&A, &B, &Z, &count);
		cycles_h = cpucycles(); //���� ����

		cycles_mid_d = cycles_h - cycles_g; //���� - ����
		totalcycle_d += cycles_mid_d;// totalcycle�� ������

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Z);

		cycles_e = cpucycles(); //���� ����
		multiplication(&A, &B, &Z);
		cycles_f = cpucycles(); //���� ����

		cycles_mid_c = cycles_f - cycles_e; //���� - ����
		totalcycle_c += cycles_mid_c;// totalcycle�� ������

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Z);

		cycles_i = cpucycles(); //���� ����
		squaring(&A, &Z);
		cycles_j = cpucycles(); //���� ����

		cycles_mid_e = cycles_j - cycles_i; //���� - ����
		totalcycle_e += cycles_mid_e;// totalcycle�� ������

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Z);

		cycles_k = cpucycles(); //���� ����
		squaring_karatsuba(&A, &Z, &count);
		cycles_l = cpucycles(); //���� ����

		cycles_mid_f = cycles_l - cycles_k; //���� - ����
		totalcycle_f += cycles_mid_f;// totalcycle�� ������

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Z);
		
		cycles1 = cpucycles(); //���� ����
		division_multi_precision_long(&A, &B, &Q, &R);
		cycles2 = cpucycles(); //���� ����

		cycles_mid1 = cycles2 - cycles1; //���� - ����
		totalcycle1 += cycles_mid1;// totalcycle�� ������

		assign_bigint(&A, &temp);
		assign_bigint(&B, &temp2);
		set_zero(&Q);
		set_zero(&R);
		set_zero(&Z);

		cycles3 = cpucycles(); //���� ����
		division_multi_precision_long(&A, &B, &Q, &R);
		cycles4 = cpucycles(); //���� ����

		cycles_mid2 = cycles4 - cycles3; //���� - ����
		totalcycle2 += cycles_mid2;// totalcycle�� ������

		delete_bigint(&A);
		delete_bigint(&B);
		delete_bigint(&Q);
		delete_bigint(&R);
	}

	printf("ADDITION ->         totalcycles/ %d = %10lld \n", temp_i, totalcycle_a / temp_i); //��հ��� ���ֱ� ���� i�� ��������
	printf("SUBTRACTION ->      totalcycles/ %d = %10lld \n", temp_i, totalcycle_b / temp_i); //��հ��� ���ֱ� ���� i�� ��������
	printf("MULTIPLICATION ->   totalcycles/ %d = %10lld \n", temp_i, totalcycle_c / temp_i); //��հ��� ���ֱ� ���� i�� ��������
	printf("MUL_KARATSUBA ->    totalcycles/ %d = %10lld \n", temp_i, totalcycle_d / temp_i); //��հ��� ���ֱ� ���� i�� ��������
	printf("SQUARING ->         totalcycles/ %d = %10lld \n", temp_i, totalcycle_e / temp_i); //��հ��� ���ֱ� ���� i�� ��������
	printf("SQUAR_KARATSUBA ->  totalcycles/ %d = %10lld \n", temp_i, totalcycle_f / temp_i); //��հ��� ���ֱ� ���� i�� ��������
	printf("LONG DIVISION ->    totalcycles/ %d = %10lld \n", temp_i, totalcycle1 / temp_i); //��հ��� ���ֱ� ���� i�� ��������
	printf("MULTI PRECISION ->  totalcycles/ %d = %10lld \n", temp_i, totalcycle2 / temp_i); //��հ��� ���ֱ� ���� i�� ��������

	return 0;

}
#endif