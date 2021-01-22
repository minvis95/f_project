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
int  test_division_naive_verification()
{
	int cnt_i = 80; //10�̸� 10�� 100�̸� 100��

	while ((cnt_i > 0))
	{
		bigint_ptr X = NULL;
		bigint_ptr Y = NULL;
		bigint_ptr Q = NULL;
		bigint_ptr R = NULL;

		generate_random_bigint(&X, NON_NEGATIVE, 11);
		generate_random_bigint(&Y, NON_NEGATIVE, 10);

		printf("x = ");
		show_bigint(X, 16);
		printf("y = ");
		show_bigint(Y, 16);

		//������ naive ����
		division_naive(&X, &Y, &Q, &R);

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
int test_division_naive_performance()
{
	unsigned long long cycles = 0, cycles1 = 0, cycles2 = 0, totalcycle = 0;

	//�Ʒ� ������ ������ ���� �����ϸ��
	int i = 1; //��� ����ϰڴ°�?
	int temp_i = i;
	int wordlen_size1 = 4;// A�� wordlen ũ��
	int wordlen_size2 = 1;// B�� wordlen ũ��
	

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
		division_naive(&A, &B, &Q, &R);
		cycles2 = cpucycles(); //���� ����

		cycles = cycles2 - cycles1; //���� - ����
		totalcycle += cycles;// totalcycle�� ������

		delete_bigint(&A);
		delete_bigint(&B);
		delete_bigint(&Q);
		delete_bigint(&R);
	}

	printf(" totalcycles/ %d = %10lld", temp_i, totalcycle / temp_i); //��հ��� ���ֱ� ���� i�� ��������

	return 0;
}
#endif