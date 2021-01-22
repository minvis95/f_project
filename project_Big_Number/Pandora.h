#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

//수정필요한 부분
//1. dckim github 추가
//->찾았음. FDL-dc 였음
//2. subtraction에서 결과값을 나오기 위해 flip_sign(bi_X or bi_Y)를 하였는데 다시 원래의 것으로 돌려줘야함
//->수정하였음
//3. &A할당하고 &A FREE하고 다시 A를 할당하면 오류나네??
//->당연하지. 그래서 delete_bigint 함수를 만들었잖아.
//4. 덧셈 XYZ말고 ZXZ, ZZX이런것들
//->필요성을 못느낌
//5. 자세한 test_add, test_sub, test_mul, test_mul_karatsuba 필요
//->add :
//
//
//6. assign_bigint 수정 필요
//-> assign_bigint(bigint_ptr* bi_Y, bigint_ptr* bi_X)로 수정
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
void assign_bigint(bigint_ptr* bi_Y, bigint_ptr* X); //X를 Y로 복사
void generate_random_bigint(bigint_ptr* bi_X, int sign, int wordlen);
void array_rand(word* dst, int wordlen); //8bit씩 차례대로 랜덤값 채워넣기
word one_array_rand();
unsigned char one_rand_8bits();
unsigned int one_rand_32bits();
unsigned long long one_rand_64bits();
int get_word_length(bigint_ptr* bi_X); //wordlen을 return해줌
int get_bit_length(bigint_ptr* bi_X);
int get_j_th_bit(bigint_ptr* bi_X, int j_th); //j = 0부터 word의배수 - 1 까지
int get_sign(bigint_ptr* bi_X);
void flip_sign(bigint_ptr* bi_X); //음수->양수, 양수->음수 ,즉 바꿔치기
void swap_bigint(bigint_ptr* bi_X, bigint_ptr* bi_Y);
void set_one(bigint_ptr* bi_X); //지수승을 위해 필요
void set_zero(bigint_ptr* bi_X); //지수승을 위해 필요
int is_zero(bigint_ptr* bi_X); //EX)A=0일때, A*B=0 할때 필요
int is_one(bigint_ptr* bi_X); //EX)A=1일때, A*B=B 할때 필요
int is_minus_one(bigint_ptr* bi_X);
int compare_ABS(bigint_ptr* bi_X, bigint_ptr* bi_Y); //compare()함수를 위한 함수(X와Y가 둘다 양수)
int compare(bigint_ptr* bi_X, bigint_ptr* bi_Y);
void left_shift(bigint_ptr* bi_X, int shift);
void right_shift(bigint_ptr* bi_X, int shift);
void reduction(bigint_ptr* bi_X, int power_of_2); //power_of_2 는 r을 의미함

void addition(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
void ADDC(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
void ADD_carry(word z, word x, word y, int* carry);
void subtraction(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
void SUBC(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
int SUB_borrow(word x, word y);
void multiplication(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
void Schoolbook_MUL(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z);
void MULC_word(word A, word B, bigint_ptr* MUL_temp);
void mul_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z, int* count);													//1 or -1 or 0이 있는경우
void set_wordlen_the_same(bigint_ptr* bi_X, bigint_ptr* bi_Y);																	 //get_word_length(bi_X)=get_word_length(bi_Y) and even를 위한 함수를 호출하는 함수
void add_wordlen(bigint_ptr* bi_X, int add_wordlen);																		//wordlen 늘리고 늘린 공간에 0x00으로 값설정
void MULC_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z, int* count);
void squaring(bigint_ptr* bi_X, bigint_ptr* bi_Z);																			//word의 값이 -1, 0, 1일 경우 또는 wordlen = 1 인 경우
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
