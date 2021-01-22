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
int test_squaring_verification()
{
	int cnt_i = 80; //10�̸� 10�� 100�̸� 100��
	int random_sign = 0;

	while ((cnt_i > 0))
	{
		bigint_ptr X = NULL;
		bigint_ptr Z = NULL;

		//NON_NEGATIVE�� NEGATIVE �����ϰ�
		random_sign = rand() & 0x01;

		//wordlen�� ������ 1�̻�
		generate_random_bigint(&X, random_sign, ((rand() & 0x07) << 1) ^ 0x01);

		printf("x = ");
		show_bigint(X, 16);
		
		//���� ����
		squaring(&X, &Z);

		printf("z = ");
		show_bigint(Z, 16);

		printf("print(x * x == z)\n");

		//�ʱ�ȭ
		random_sign = 0;

		//�޸� ����
		delete_bigint(&X);
		delete_bigint(&Z);

		delay(300); //�� line�� ������ �Ȱ��� random_bigint ���� ������ ���� �ִ�.
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

	//�Ʒ� ������ ������ ���� �����ϸ��
	int i = 100; //��� ����ϰڴ°�?
	int temp_i = i;
	int wordlen_size1 = 100;// A�� wordlen ũ��


	while ((i >= 0)) //bigint�� �����ϰ� ������ �ϰ� �޸� �����ϴ� �ð��� ����
	{
		i--;

		bigint_ptr A = NULL;
		bigint_ptr C = NULL;

		generate_random_bigint(&A, NON_NEGATIVE, wordlen_size1);

		cycles1 = cpucycles(); //���� ����
		squaring(&A, &C);
		cycles2 = cpucycles(); //���� ����

		cycles = cycles2 - cycles1; //���� - ����
		totalcycle += cycles;// totalcycle�� ������

		delete_bigint(&A);
		delete_bigint(&C);

	}

	printf(" totalcycles/ %d = %10lld", temp_i, totalcycle / temp_i); //��հ��� ���ֱ� ���� i�� ��������

	return 0;
}
#endif