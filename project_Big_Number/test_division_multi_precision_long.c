#include "Pandora.h"

//Verification and Performance check!!

#if 0
//�Ȱ��� random_bigint���� ������ �� ���� delay�Լ��� �߰�
void delay(clock_t n)
{
	clock_t start = clock();

	while (clock() - start < n);

}

//Verification
int test_division_multi_precision_long_verification()
{
	int cnt_i = 80; //10�̸� 10�� 100�̸� 100��

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

		//multi_precision ������ ����
		division_multi_precision_long(&X, &Y, &Q, &R);

		printf("q = ");
		show_bigint(Q, 16);
		printf("r = ");
		show_bigint(R, 16);

		printf("print(x//y == q)\n"); //sage�� //�� ��
		printf("print(x%%y == r)\n"); //sage�� %�� ������

		//�޸� ����
		delete_bigint(&X);
		delete_bigint(&Y);
		delete_bigint(&Q);
		delete_bigint(&R);

		delay(300); //�� line�� ������ �Ȱ��� random_bigint ���� ������ ���� �ִ�.
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

	//�Ʒ� ������ ������ ���� �����ϸ��
	int i = 100; //��� ����ϰڴ°�?
	int temp_i = i;
	int wordlen_size1 = 40;// A�� wordlen ũ��
	int wordlen_size2 = 10;// B�� wordlen ũ��
	
	while ((i >= 0)) //bigint�� �����ϰ� ������ �ϰ� �޸� �����ϴ� �ð��� ����
	{
		i--;

		bigint_ptr A = NULL;
		bigint_ptr B = NULL;
		bigint_ptr Q = NULL;
		bigint_ptr R = NULL;

		generate_random_bigint(&A, NON_NEGATIVE, wordlen_size1);
		generate_random_bigint(&B, NON_NEGATIVE, wordlen_size2);

		cycles1 = cpucycles(); //���� ����
		division_multi_precision_long(&A, &B, &Q, &R);
		cycles2 = cpucycles(); //���� ����

		cycles = cycles2 - cycles1; //���� - ����
		totalcycle += cycles;// totalcycle�� ������

		delete_bigint(&A);
		delete_bigint(&B);
		delete_bigint(&Q);
		delete_bigint(&R);
	}

	printf(" totalcycles/ %d = %10lld \n", temp_i, totalcycle / temp_i); //��հ��� ���ֱ� ���� i�� ��������

	return 0;
}
#endif

//binary long division�� multi_precision ��
#if 0
int test_binary_long_division_vs_multi_precision_long_performance()
{
	unsigned long long cycles_mid1 = 0, cycles_mid2 = 0, cycles1 = 0, cycles2 = 0, cycles3 = 0, cycles4 = 0, totalcycle1 = 0, totalcycle2 = 0;

	//�Ʒ� ������ ������ ���� �����ϸ��
	int i = 100; //��� ����ϰڴ°�?
	int temp_i = i;
	int wordlen_size1 = 40;// A�� wordlen ũ��
	int wordlen_size2 = 10;// B�� wordlen ũ��

	while ((i >= 0)) //bigint�� �����ϰ� ������ �ϰ� �޸� �����ϴ� �ð��� ����
	{
		i--;

		bigint_ptr A = NULL;
		bigint_ptr B = NULL;
		bigint_ptr Q = NULL;
		bigint_ptr R = NULL;

		generate_random_bigint(&A, NON_NEGATIVE, wordlen_size1);
		generate_random_bigint(&B, NON_NEGATIVE, wordlen_size2);

		cycles1 = cpucycles(); //���� ����
		division_multi_precision_long(&A, &B, &Q, &R);
		cycles2 = cpucycles(); //���� ����

		cycles_mid1 = cycles2 - cycles1; //���� - ����
		totalcycle1 += cycles_mid1;// totalcycle�� ������

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

	printf(" totalcycles/ %d = %10lld \n", temp_i, totalcycle1 / temp_i); //��հ��� ���ֱ� ���� i�� ��������
	printf(" totalcycles/ %d = %10lld \n", temp_i, totalcycle2 / temp_i); //��հ��� ���ֱ� ���� i�� ��������
	return 0;
}
#endif
