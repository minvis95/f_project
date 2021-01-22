#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

//�����ʿ��� �κ�
//1. dckim github �߰�
//->ã����. FDL-dc ����
//2. subtraction���� ������� ������ ���� flip_sign(bi_X or bi_Y)�� �Ͽ��µ� �ٽ� ������ ������ ���������
//->�����Ͽ���
//3. &A�Ҵ��ϰ� &A FREE�ϰ� �ٽ� A�� �Ҵ��ϸ� ��������??
//->�翬����. �׷��� delete_bigint �Լ��� ������ݾ�.
//4. ���� XYZ���� ZXZ, ZZX�̷��͵�
//->�ʿ伺�� ������
//5. �ڼ��� test_add, test_sub, test_mul, test_mul_karatsuba �ʿ�
//->add :
//
//
//6. assign_bigint ���� �ʿ�
//-> assign_bigint(bigint_ptr* bi_Y, bigint_ptr* bi_X)�� ����
#include <stdio.h>
#include <stdlib.h>
#include < time.h>
#include <stdint.h>


#define NEGATIVE 1
#define NON_NEGATIVE 0
#define TRUE 1
#define FALSE 0
#define COUNT 3

#define WORD_BITLEN 8	// w = 8, 32, 64 (bit)

#if WORD_BITLEN == 64
typedef unsigned long long word;
#elif WORD_BITLEN == 32
typedef unsigned int word;
#else	// WORD_BITLEN == 8
typedef unsigned char word;
#endif

typedef struct {
	int sign;	// negative <-> sign = 1, non_negative <-> sign = 0
	int wordlen;
	word* a;
}bigint;

typedef bigint* bigint_ptr;

long long cpucycles(void);

void delete_bigint(bigint_ptr* bi_X);
void new_bigint(bigint_ptr* bi_X, int wordlen);
void set_bigint(bigint_ptr* bi_X, word* input, int sign, int wordlen);
void show_bigint(bigint* X, int base); //base = hexa, deci, binary
void refine_bigint(bigint_ptr* bi_X);
void assign_bigint(bigint_ptr* bi_Y, bigint_ptr* X); //X�� Y�� ����
void generate_random_bigint(bigint_ptr* bi_X, int sign, int wordlen);
void array_rand(word* dst, int wordlen); //8bit�� ���ʴ�� ������ ä���ֱ�
word one_array_rand();
unsigned char one_rand_8bits();
unsigned int one_rand_32bits();
unsigned long long one_rand_64bits();
int get_word_length(bigint_ptr* bi_X); //wordlen�� return����
int get_bit_length(bigint_ptr* bi_X);
int get_j_th_bit(bigint_ptr* bi_X, int j_th); //j = 0���� word�ǹ�� - 1 ����
int get_sign(bigint_ptr* bi_X);
void flip_sign(bigint_ptr* bi_X); //����->���, ���->���� ,�� �ٲ�ġ��
void swap_bigint(bigint_ptr* bi_X, bigint_ptr* bi_Y);
void set_one(bigint_ptr* bi_X); //�������� ���� �ʿ�
void set_zero(bigint_ptr* bi_X); //�������� ���� �ʿ�
int is_zero(bigint_ptr* bi_X); //EX)A=0�϶�, A*B=0 �Ҷ� �ʿ�
int is_one(bigint_ptr* bi_X); //EX)A=1�϶�, A*B=B �Ҷ� �ʿ�
int is_minus_one(bigint_ptr* bi_X);
int compare_ABS(bigint_ptr* bi_X, bigint_ptr* bi_Y); //compare()�Լ��� ���� �Լ�(X��Y�� �Ѵ� ���)
int compare(bigint_ptr* bi_X, bigint_ptr* bi_Y);
void left_shift(bigint_ptr* bi_X, int shift);
void right_shift(bigint_ptr* bi_X, int shift);
void reduction(bigint_ptr* bi_X, int power_of_2); //power_of_2 �� r�� �ǹ���

void addition(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
void ADDC(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
void ADD_carry(word z, word x, word y, int* carry);
void subtraction(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
void SUBC(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
int SUB_borrow(word x, word y);
void multiplication(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
void Schoolbook_MUL(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
void MULC_word(word A, word B, bigint_ptr* MUL_temp);
void mul_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z, int* count);													//1 or -1 or 0�� �ִ°��
void set_wordlen_the_same(bigint_ptr* bi_X, bigint_ptr* bi_Y);																	 //get_word_length(bi_X)=get_word_length(bi_Y) and even�� ���� �Լ��� ȣ���ϴ� �Լ�
void add_wordlen(bigint_ptr* bi_X, int add_wordlen);																		//wordlen �ø��� �ø� ������ 0x00���� ������
void MULC_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z, int* count);
void squaring(bigint_ptr* bi_X, bigint_ptr* bi_Z);																			//word�� ���� -1, 0, 1�� ��� �Ǵ� wordlen = 1 �� ���
void SQUARING_C(bigint_ptr* bi_X, bigint_ptr* bi_Z);
void SQUARING_C_ONE_WORD(word A, bigint_ptr* squaring_word);
void squaring_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Z, int* count);
void SQUARING_C_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Z, int* count);
void division_naive(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Q, bigint_ptr* bi_R);
void division_binary_long(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Q, bigint_ptr* bi_R);
void division_multi_precision_long(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Q, bigint_ptr* bi_R);
void div_core(bigint_ptr* bi_X, bigint_ptr* bi_Y, word* Q_ith_word, int i, bigint_ptr* return_bi_R, int k);
void div_core_core(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* return_bi_Q, bigint_ptr* return_bi_R);
void left_to_right_binary_method_EXPO(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y);
void left_to_right_binary_method_CM(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y);
void right_to_left_binary_method_EXPO(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y);
void right_to_left_binary_method_CM(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y);
void multiply_and_squaring_EXPO(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y);
void multiply_and_squaring_CM(bigint_ptr* bi_X, word* n, bigint_ptr* bi_Y);
