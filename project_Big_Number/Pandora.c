#include "Pandora.h"

int64_t cpucycles(void)
{
	return __rdtsc();
}

void delete_bigint(bigint_ptr* bi_X) //����ü�� �Ҵ�� �޸𸮸� ������ �ʱ�ȭ
{
	if (*bi_X == NULL)
		return;
	for (int i = 0; i < (*bi_X)->wordlen; i++)
	{
		(*bi_X)->a[i] = 0x00; //�޸� �� �ʱ�ȭ
	}
	free((*bi_X)->a);
	free(*bi_X);
	*bi_X = NULL;
}

void new_bigint(bigint_ptr* bi_X, int wordlen) //���ο� bigint�� ����
{
	if (*bi_X != NULL) //�����Ͱ��� �Ҵ�Ǿ��ִٸ� ����
		delete_bigint(bi_X);

	*bi_X = (bigint*)malloc(sizeof(bigint));
	
	(*bi_X)->sign = NON_NEGATIVE;
	(*bi_X)->wordlen = wordlen;
	(*bi_X)->a = (word*)calloc(wordlen, sizeof(word));
}

void set_bigint(bigint_ptr* bi_X, word* input, int sign, int wordlen) //�Էµ� input���� ���� bigint�� ����
{
	if (*bi_X != NULL) //�����Ͱ��� �Ҵ�Ǿ��ִٸ� ����
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

void show_bigint2(bigint_ptr* X, int base) //base = hexa, �ڵ� �м���
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

void refine_bigint(bigint_ptr* bi_X) // EX) A(n��) + B(n��) = �ִ� n+1�� ����, �׷��� n+1��° ���� 0...0�̿��� n�� ����� �����ϱ� ���� �Լ�
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

void assign_bigint(bigint_ptr* bi_Y, bigint_ptr* bi_X) //X�� Y�� ����
{
	set_bigint(bi_Y, (*bi_X)->a, (*bi_X)->sign, (*bi_X)->wordlen);
}

void generate_random_bigint(bigint_ptr* bi_X, int sign, int wordlen)
{
	if (*bi_X != NULL)
		delete_bigint(bi_X);
	
	word* Input;
	Input = (word*)calloc(wordlen, sizeof(word)); //random���� �����ϱ����� �޸� �Ҵ�
	
	srand((unsigned int)time(NULL)); //�����Ҷ� ���� �ٸ� random���� �����ϱ� ����
	array_rand(Input, wordlen);
	
	set_bigint(bi_X, Input, sign, wordlen);
	refine_bigint(bi_X);
}

void array_rand(word* dst, int wordlen)//������ ä���ֱ�
{
	for (int i = 0; i < wordlen; i++)
	{
		dst[i] = one_array_rand();//�� ���徿 ������ ä���ֱ�
	}
}

#if WORD_BITLEN == 8
word one_array_rand()
{
	return (word)one_rand_8bits(); //8bit¥�� random�� ����
}
#elif WORD_BITLEN == 32
word one_array_rand()
{
	return (word)one_rand_32bits(); //32bit¥�� random�� ����
}
#else//64
word one_array_rand()
{
	return (word)one_rand_64bits(); //64bit¥�� random�� ����
}
#endif

unsigned char one_rand_8bits()
{
	return rand() & 0xff;
	//rand()�� 15bit�� return���ش�.
	//���� 8bit�� random�� ���� return���ֱ� ���ؼ��� 0xff�� &���� �����ش�.
}

unsigned int one_rand_32bits()
{
	int i = 3;
	unsigned int tmp = one_rand_8bits();
	while (i > 0) //ä���ִ°� 3�� 8bit + 3�� * 8bit = 32bit
	{
		tmp = tmp << 8; //8bit ������ �а�
		tmp = tmp ^ one_rand_8bits(); //8bit�� random�� ä���ְ�
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


int get_word_length(bigint_ptr* bi_X) //wordlen�� return����
{
	return (*bi_X)->wordlen;
}

int get_bit_length(bigint_ptr* bi_X)
{
	int total_word_bit_length = ((*bi_X)->wordlen) * WORD_BITLEN; //�� bigint�� ������ �ִ� word�� ��übit�� ���ϴ°�
	int i, cnt = 0;
	
	for (i = WORD_BITLEN; (i >= 0) && (((*bi_X)->a[(*bi_X)->wordlen - 1] >> (i - 1)) == 0); i--)
	{
		cnt++; //�ֻ��� word������ �ֻ��� bit���� -1�ϸ鼭 0�� ������ ����
	}
	return total_word_bit_length - cnt; 
}

int get_j_th_bit(bigint_ptr* bi_X, int j_th) //j = 0���� word�ǹ�� - 1 ����
{
	if (j_th >= WORD_BITLEN)
		return (((*bi_X)->a[j_th / WORD_BITLEN] >> (j_th % WORD_BITLEN)) & 1);
	
	return (((*bi_X)->a[0] >> j_th) & 1);
}

int get_sign(bigint_ptr* bi_X)
{
	return (*bi_X)->sign;
}

void flip_sign(bigint_ptr* bi_X) //����->���, ���->���� ,�� �ٲ�ġ��
{
	(*bi_X)->sign = (*bi_X)->sign ^ 1;
}

void swap_bigint(bigint_ptr* bi_X, bigint_ptr* bi_Y)
{
	bigint_ptr temp = NULL;
	temp = (bigint_ptr)calloc(1, sizeof(bigint)); //new_bigint()�Լ��� wordlen���ڸ� �޾ƾ��ϹǷ� ��������

	temp = *bi_X;
	*bi_X = *bi_Y;
	*bi_Y = temp;

}

void set_one(bigint_ptr* bi_X)//�������� ���� �ʿ�
{
	new_bigint(bi_X, 1);
	//(*bi_X)->sign = NON_NEGATIVE;  new_bigint()���� �����ϰ� ���⶧���� ���� 
	(*bi_X)->a[0] = 0x1;
}

void set_zero(bigint_ptr* bi_X)//�������� ���� �ʿ�
{
	new_bigint(bi_X, 1);
	//(*bi_X)->sign = NON_NEGATIVE;	 new_bigint()���� �����ϰ� ���⶧���� ����
	(*bi_X)->a[0] = 0x0;
}

int is_zero(bigint_ptr* bi_X) //EX)A=0�϶�, A*B=0 �Ҷ� �ʿ�
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

int is_one(bigint_ptr* bi_X) //EX)A=1�϶�, A*B=B �Ҷ� �ʿ�
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

int compare_ABS(bigint_ptr* bi_X, bigint_ptr* bi_Y) //compare()�Լ��� ���� �Լ�(X��Y�� �Ѵ� ���)
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
			(*bi_X)->a[i + (shift / WORD_BITLEN)] = (*bi_X)->a[i]; //�߰��� �迭�� �̵�(shift)
		}

		for (i = 0; i < (shift / WORD_BITLEN); i++)
		{
			(*bi_X)->a[i] = 0x0;
		}
	}
	else //shift % WORD_BITLEN != 0 ,�� ��� �������� ����
	{
#if WORD_BITLEN == 8
		(*bi_X)->a = (word*)realloc((*bi_X)->a, (((*bi_X)->wordlen) + (shift / WORD_BITLEN) + 1)); //�������� �����ϹǷ� +1�� ���־����
#elif WORD_BITLEN == 32
		(*bi_X)->a = (word*)realloc((*bi_X)->a, 4 * (((*bi_X)->wordlen) + (shift / WORD_BITLEN) + 1)); 
#else //WORD_BITLEN ==64
		(*bi_X)->a = (word*)realloc((*bi_X)->a, 8 * (((*bi_X)->wordlen) + (shift / WORD_BITLEN) + 1)); 
#endif
		
		(*bi_X)->wordlen += shift / WORD_BITLEN + 1; //���Ӱ� ���ŵ� wordlen

		for (i = (*bi_X)->wordlen - 2; i > (shift / WORD_BITLEN) - 1; i--) //��ŭ shift�̵�
		{
			(*bi_X)->a[i] = (*bi_X)->a[i - (shift / WORD_BITLEN)]; 
		}
		for (i = 0; i < (shift / WORD_BITLEN); i++) //�̵��Ϸ��� 0x0���� ä����
		{
			(*bi_X)->a[i] = 0x0;
		}
		//���� ��������ŭ�� shift �̵��� �ʿ���
		(*bi_X)->a[(*bi_X)->wordlen - 1] = (*bi_X)->a[(*bi_X)->wordlen - 2] >> (WORD_BITLEN - shift % WORD_BITLEN); //�ֻ��� word �κ�
		
		for (i = (*bi_X)->wordlen - 2; i > shift / WORD_BITLEN; i--)//�߰�word�κ�
		{
			(*bi_X)->a[i] = ((*bi_X)->a[i] << (shift % WORD_BITLEN)) | ((*bi_X)->a[i - 1] >> (WORD_BITLEN - shift % WORD_BITLEN));
		}
		//������word�κ�
		(*bi_X)->a[shift / WORD_BITLEN] = (*bi_X)->a[shift / WORD_BITLEN] << (shift % WORD_BITLEN);


		if ((*bi_X)->a[(*bi_X)->wordlen - 1] == 0x0) //������word�κ��� 0�̸� refine bigint���ش�.
		{
			refine_bigint(bi_X);
		}
	}
}

void right_shift(bigint_ptr* bi_X, int shift)
{
	int i;
	if (shift % WORD_BITLEN == 0) //������ 0�϶�
	{
		for (i = 0; i < (*bi_X)->wordlen - (shift / WORD_BITLEN); i++)
		{
			(*bi_X)->a[i] = (*bi_X)->a[i + (shift / WORD_BITLEN)]; //word �̵�
		}
		for (i = (*bi_X)->wordlen - (shift / WORD_BITLEN); i < (*bi_X)->wordlen; i++) //�̵���Ų�� ������ �ִ� ���� 0x0���� ä��(refine�� �ϱ�����)
		{
			(*bi_X)->a[i] = 0x0;
		}
		refine_bigint(bi_X); //remove zero(�޸� ���Ҵ�)
	}
	else // (shift % WORD_BITLEN(=8) != 0)
	{
		for (i = 0; i < (*bi_X)->wordlen - (shift / WORD_BITLEN); i++)//�ϴ� ��ŭ �̵�
		{
			(*bi_X)->a[i] = (*bi_X)->a[i + (shift / WORD_BITLEN)];
		}
		for (i = (*bi_X)->wordlen - (shift / WORD_BITLEN); i < (*bi_X)->wordlen; i++)
		{
			(*bi_X)->a[i] = 0x0; // ������ �κ� 0x0����
		}
		for (i = 0; i < (*bi_X)->wordlen - shift / WORD_BITLEN - 1; i++) //���� �ִ�word�κк��� �������� shifting ������
		{
			(*bi_X)->a[i] = (((*bi_X)->a[i] >> (shift % WORD_BITLEN)) | ((*bi_X)->a[i + 1] << (WORD_BITLEN - (shift % WORD_BITLEN))));
		}
		//���� �ִ� �ֻ��� word�� ���� ���� 
		(*bi_X)->a[(*bi_X)->wordlen - (shift / WORD_BITLEN) - 1] = (*bi_X)->a[(*bi_X)->wordlen - (shift / WORD_BITLEN) - 1] >> ((shift % WORD_BITLEN));

		refine_bigint(bi_X); //remove zero(�޸� ���Ҵ�)
	}
}

void reduction(bigint_ptr* bi_X, int power_of_2) //power_of_2 �� r�� �ǹ���
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

		(*bi_X)->wordlen = power_of_2 / WORD_BITLEN; //wordlen ����
		return;
	} 

	//case3 : ����� �ƴѰ��
	//power_of_2 �������� ������ word�� �ֻ��� word�� �����ؽ����ָ��
	(*bi_X)->a[(power_of_2 / WORD_BITLEN)] &= (0xff >> (WORD_BITLEN - (power_of_2 % WORD_BITLEN))); //   A & (2^r' - 1) ,k���� word����

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
		ADDC(bi_X, bi_Y, bi_Z); //���⼭ ���̷� ���� ���� ������
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

	if (get_word_length(bi_X) > get_word_length(bi_Y)) //wordlen�� �Ȱ��� ���߱����� bi_X�� wordlen��ŭ ���Ҵ� ����
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
			(*bi_Y)->a[i] = 0x0; //���Ҵ�� �κ� 0x0���� �ʱ�ȭ
		}

		(*bi_Y)->wordlen = get_word_length(bi_X);//bi_X�� wordlen�� ���������Ƿ� ����
	}
	//if (*bi_Z != NULL) //bi_Z�� �Ҵ�Ǿ������� �ʱ�ȭ�� free��Ű�� �ٽ� �޸��Ҵ� ����
	//{
		delete_bigint(bi_Z);
		*bi_Z = (bigint_ptr)calloc(1, sizeof(bigint));
		(*bi_Z)->wordlen = (*bi_X)->wordlen + 1;
		(*bi_Z)->a = (word*)calloc((*bi_Z)->wordlen, sizeof(word));
		(*bi_Z)->sign = (*bi_X)->sign; // ADDC�Լ��� ������ bi_X�� bi_Y�� ��ȣ�� �Ȱ��� ������ bi_Z�� x��y�� �Ȱ���
	//}
	
	int carry = 0;

	for (i = 0; i < get_word_length(bi_X); i++)
	{
		(*bi_Z)->a[i] = (*bi_X)->a[i] + (*bi_Y)->a[i] + carry; //�ڵ����� ��ⷯ����
		ADD_carry((*bi_Z)->a[i], (*bi_X)->a[i], (*bi_Y)->a[i], &carry);
		if (i == get_word_length(bi_X) - 1)
		{
			(*bi_Z)->a[i+1] = carry; //������� �ֻ��� word�� 1�Ǵ� 0�̴�
		}
	}
	refine_bigint(bi_Y);//wordlen �ǵ�����
	refine_bigint(bi_Z); //�ֻ��� word�κ��� carry�� �ȹ����� 0x0�ϰ��̹Ƿ� refine��Ų��
}

void ADD_carry(word z, word x, word y, int* carry)
{
	if (z < x || z < y) //x=0x01, y=0xff, carry=0x01�̸�, z = x+y+carry = 0x01 = x �̹Ƿ� y���� ���ϵ��� ���Խ����� 
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

	//���� ��� ������ �������� �ʴ´ٸ� bi_X=bi_Y�� �Ҹ��̴�.
	//���� ������� 0�̴�.
	set_zero(bi_Z);
	return;
}

void SUBC(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)
{
	int i;

	if (get_word_length(bi_X) > get_word_length(bi_Y)) //wordlen�� �Ȱ��� ���߱����� bi_X�� wordlen��ŭ ���Ҵ� ����
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
			(*bi_Y)->a[i] = 0x0; //���Ҵ�� �κ� 0x0���� �ʱ�ȭ
		}
		(*bi_Y)->wordlen = get_word_length(bi_X);//bi_X�� wordlen�� ���������Ƿ� ����
	}
	/*
	if (get_word_length(bi_X) < get_word_length(bi_Y)) //wordlen�� �Ȱ��� ���߱����� bi_X�� wordlen��ŭ ���Ҵ� ����
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
			(*bi_X)->a[i] = 0x0; //���Ҵ�� �κ� 0x0���� �ʱ�ȭ
		}
		(*bi_X)->wordlen = get_word_length(bi_Y);//bi_X�� wordlen�� ���������Ƿ� ����
	}
	*/
	//if (*bi_Z != NULL) //bi_Z�� �Ҵ�Ǿ������� �ʱ�ȭ�� free��Ű�� �ٽ� �޸��Ҵ� ����
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
		(*bi_Z)->a[i] = ((*bi_X)->a[i]) - borrow; //�ڵ����� ��ⷯ����
		borrow = SUB_borrow((*bi_X)->a[i], borrow);
		borrow += SUB_borrow((*bi_Z)->a[i], (*bi_Y)->a[i]);
		(*bi_Z)->a[i] = (*bi_Z)->a[i] - (*bi_Y)->a[i];

		//������ ������ �ֻ��� ����� ���� ����ϴ°��� �ʿ����.
	}

	refine_bigint(bi_Y);//wordlen �ǵ�����
	refine_bigint(bi_Z); //�ֻ��� word�κп��� borrow�� ���� 0x0�� �� �����Ƿ� refine��Ų��
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
			//���ϰ���(MUL_temp�� �ӽ� ����)
			MULC_word((*bi_X)->a[i], (*bi_Y)->a[j], &MUL_temp); //j��° A�� i��° B�� ����(�����ڵ� line4)
			//�ӽ� ���尪�� Shift�̵�(word_bitlen * (i+j))
			left_shift(&MUL_temp, WORD_BITLEN * (i + j));
			//c<-ADDC(C, T)
			addition(bi_Z, &MUL_temp, &temp); //line6 : c<-ADDC(C, T)
			assign_bigint(bi_Z, &temp); //line6 : c<-ADDC(C, T)
		}
	}
	//��ȣ ����
	if ((*bi_X)->sign != (*bi_Y)->sign)//sign�� ���� �ٸ��� ����
		(*bi_Z)->sign = NEGATIVE;

	delete_bigint(&temp);
	delete_bigint(&MUL_temp);
	refine_bigint(bi_Z);
}


void MULC_word(word A, word B, bigint_ptr* MUL_temp)
{
	bigint_ptr T1 = NULL; //A1*B1, A0*B0 ����
	bigint_ptr T2 = NULL; //A1*B0�� ����
	bigint_ptr T3 = NULL; //A0*B1�� ����
	bigint_ptr temp = NULL; // addition�Ҷ� �ӽ� ������ ���� ����
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

	T1->a = (word*)realloc(T1->a, 2 * (sizeof(word))); //A1*B1, A0*B0 ������ ���� ���� 2���Ҵ�
	T1->wordlen = 2;

	//LINE6
	//T->a[0]<-A0*B0
	T1->a[0] = (A & ((1i64 << (WORD_BITLEN / 2)) - 1)) * (B & ((1i64 << (WORD_BITLEN / 2)) - 1));
	//T->a[1]<-A1*B1
	T1->a[1] = (A >> (WORD_BITLEN / 2))* (B >> (WORD_BITLEN / 2));

	left_shift(&T2, WORD_BITLEN / 2); //�ڸ��� �����ֱ�
	left_shift(&T3, WORD_BITLEN / 2);

	//T1,T2,T3�� ���� ����
	addition(&T1, &T2, &temp);
	addition(&temp, &T3, MUL_temp);

	//�޸� �Ҵ� ����
	delete_bigint(&T1);
	delete_bigint(&T2);
	delete_bigint(&T3);
	delete_bigint(&temp);
}



int is_minus_one(bigint_ptr* bi_X) //multiplication�Լ��� ���ȴ�.
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


void mul_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z, int* count)//1 or -1 or 0�� �ִ°��
{
	delete_bigint(bi_Z);
	--(*count);              //�Լ� ���� -> --count

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
void set_wordlen_the_same(bigint_ptr* bi_X, bigint_ptr* bi_Y) //get_word_length(bi_X)=get_word_length(bi_Y) and even�� ���� �Լ��� ȣ���ϴ� �Լ�
{
	if (get_word_length(bi_X) == get_word_length(bi_Y)) //wordlen ���� ���
	{
		if (get_word_length(bi_X) % 2 == 1) //when wordlen of bi_X is odd, then even�� �ǰ� realloc����
			add_wordlen(bi_X, 1);

		if (get_word_length(bi_Y) % 2 == 1) //when wordlen of bi_Y is odd, then even�� �ǰ� realloc����
			add_wordlen(bi_Y, 1);
	}

	if (get_word_length(bi_X) != get_word_length(bi_Y)) //wordlen �ٸ� ��� -> ¦���� ��ȯ�� wordlen�� ���������
	{
		if (get_word_length(bi_X) % 2 == 1)
			add_wordlen(bi_X, 1);

		if (get_word_length(bi_Y) % 2 == 1)
			add_wordlen(bi_Y, 1);

		if (get_word_length(bi_X) >= get_word_length(bi_Y)) //wordlen ���̸�ŭ realloc����
			add_wordlen(bi_Y, get_word_length(bi_X) - get_word_length(bi_Y));

		if (get_word_length(bi_X) <= get_word_length(bi_Y)) //wordlen ���̸�ŭ realloc����
			add_wordlen(bi_X, get_word_length(bi_Y) - get_word_length(bi_X));
	}
}

void add_wordlen(bigint_ptr* bi_X, int add_wordlen) //wordlen �ø��� �ø� ������ 0x00���� ������
{
	int i;

#if WORD_BITLEN == 8
	(*bi_X)->a = (word*)realloc((*bi_X)->a, (*bi_X)->wordlen + add_wordlen); //add_wordlen ��ŭ ���� �ø�
	for (i = ((*bi_X)->wordlen) + add_wordlen - 1; i >= (*bi_X)->wordlen; i--) // �ø� ���� 0x00���� ������
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

void MULC_karatsuba(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z, int* count)// count = 3���� �����ϱ⸦ ����
{ 
	delete_bigint(bi_Z);
	set_zero(bi_Z); //bi_Z�� 0x00 ����

	
	bigint_ptr A0 = NULL;
	bigint_ptr A1 = NULL;
	bigint_ptr B0 = NULL;
	bigint_ptr B1 = NULL;

	bigint_ptr T0 = NULL; //A0*B0 ����
	bigint_ptr T1 = NULL; //A1*B1 ����

	bigint_ptr R = NULL;  //T1<<(2 * l * word_bitlen) +T0 ����
	
	bigint_ptr S0 = NULL; //A0-A1
	bigint_ptr S1 = NULL; //B1-B0
	


	int count_temp = (*count);

	
	if ((*bi_X)->sign != (*bi_Y)->sign) //��ȣ ����
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
	right_shift(&T1, 2 * l * WORD_BITLEN); //�ٽ� �ǵ�����
	
	//LINE10
	subtraction(&A0, &A1, &S0); //S0<-A0-A1
	subtraction(&B1, &B0, &S1); //S1<-B1-B0

	//LINE11
	//��ȣ ����
	if (S1->sign == S0->sign)
		(*bi_Z)->sign = NON_NEGATIVE;
	if (S1->sign != S0->sign)
		(*bi_Z)->sign = NEGATIVE;
	//mul_karatsuba
	//�����ڵ忡���� S�� bi_Z��
	(*count) = count_temp;
	if ((*count) > 0) //whem count>0, mul_karatsuba
		mul_karatsuba(&S1, &S0, bi_Z, count);
	if ((*count) <= 0)
		multiplication(&S1, &S0, bi_Z);

	//LINE12~
	//T0+T1+S
	addition(&T0, &T1, &A0); //addition �Լ������� &A0 delete_bigint���ְ� �޸� �Ҵ�����
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

void squaring(bigint_ptr* bi_X, bigint_ptr* bi_Z) //word�� ���� -1, 0, 1�� ��� �Ǵ� wordlen = 1 �� ���
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
	if (get_word_length(bi_X) == 1) //SUARING_C�� �� �ʿ䰡 ����.(���� �����̱� ����)
	{
		SQUARING_C_ONE_WORD((*bi_X)->a[0], bi_Z);
		return;
	}
	SQUARING_C(bi_X, bi_Z); //���� �������� ������ �Ϲ����� ��������
}

void SQUARING_C(bigint_ptr* bi_X, bigint_ptr* bi_Z) // pdf73/113 �����ڵ� ����
{
	bigint_ptr C1 = NULL;   
	bigint_ptr C2 = NULL;   
	bigint_ptr T1 = NULL;
	bigint_ptr T2 = NULL;
	bigint_ptr Temp = NULL;
	
	//�ؿ� addition(&T1, &C1, &Temp)���� ù������ C1�� NULL�� �ȵ��� �ϱ� ���ؼ�(�ʱⰪ �����̶�� ����ȴ�)
	//line 1
	set_zero(&C1);
	set_zero(&C2);
	set_zero(&Temp);
	set_zero(bi_Z);

	int i, j = 0;
	for (j = 0; j < get_word_length(bi_X); j++)
	{
		SQUARING_C_ONE_WORD((*bi_X)->a[j], &T1); //���Ͽ��� ����
		left_shift(&T1, 2 * j * WORD_BITLEN); //line4 T1 << 2jw
		addition(&T1, &C1, &Temp); //T1 + C1
		assign_bigint(&C1, &Temp); //C1 <- Temp(=T1 + C1)

		set_zero(&Temp);

		for (i = j + 1; i < get_word_length(bi_X); i++)
		{
			MULC_word((*bi_X)->a[j], (*bi_X)->a[i], &T2); //J��° A�� I��° A�� ���Ͽ��� ����
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

void SQUARING_C_ONE_WORD(word A, bigint_ptr* squared_A) //pdf 69/113����
{
	bigint_ptr C = NULL;
	bigint_ptr T = NULL;
	set_zero(&C);
	set_zero(&T);
	
	delete_bigint(squared_A);
	
	//C�� T�� [0,W^2)�� ���� �����Ƿ� ���� �Ҵ��������
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
	word C0 = A0 * A0; //C0�� [0, W)
	word C1 = A1 * A1; //C1�� [0, W)

	//C <- (C1 << w) + C0
	C->a[0] = C0; //C->a[0] <- C0
	C->a[1] = C1; //C->a[1] <-(C1 << w)

	T->a[0] = A0 * A1; //line4 T�� [0, W)
	
	left_shift(&T, (WORD_BITLEN / 2) + 1); //line5
	addition(&C, &T, squared_A); //line6
	
	//�޸� ����
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
	bigint_ptr A0 = NULL; // mod 2^lw, ��
	bigint_ptr A1 = NULL; // A >> lw, ��
	bigint_ptr T0 = NULL;
	bigint_ptr T1 = NULL;
	bigint_ptr R = NULL;
	bigint_ptr S = NULL;
	
	set_zero(bi_Z);

	if (get_word_length(bi_X) % 2 == 1) //Ȧ���� ���
		add_wordlen(bi_X, 1); //word�� �ϳ� �߰��ؼ� wordlen�� ¦���� �ǰ� ����

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

	(*count) = depth; // ���� ���ǹ����� squaring_karatsuba ȣ���� �̿��ؼ� count�ٿ��⿡ 
	                  //���������� �ǵ���
	
	if ((*count) > 0) //if count > 0, then Karatsuba
		squaring_karatsuba(&A1, &T1, count);
	if ((*count) <= 0) //else general squaring
		squaring(&A1, &T1);

	left_shift(&T1, 2* l * WORD_BITLEN);// line8
	addition(&T1, &T0, &R);

	(*count) = depth; // ���� ���ǹ����� squaring_karatsuba ȣ���� �̿��ؼ� count�ٿ��⿡ 
					  //���������� �ǵ���
	//line9
	if ((*count) > 0)
		mul_karatsuba(&A1, &A0, &S, count);
	if ((*count) <= 0)
		multiplication(&A1, &A0, &S);

	//lin10
	left_shift(&S, (l * WORD_BITLEN) + 1);
	//line11
	addition(&R, &S, bi_Z);

	//�޸��Ҵ�����
	delete_bigint(&A0);
	delete_bigint(&A1);
	delete_bigint(&T0);
	delete_bigint(&T1);
	delete_bigint(&R);
	delete_bigint(&S);
}

//bi_X�� bi_Y�� ����������, �� : bi_Q, ������ : bi_R
void division_binary_long(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Q, bigint_ptr* bi_R) //pdf 81/113 ����
{
	bigint_ptr T1 = NULL;
	bigint_ptr T2 = NULL;
	
	set_zero(bi_Q);
	set_zero(bi_R);
	set_zero(&T1);
	set_zero(&T2);

	if (is_zero(bi_Y) == TRUE || (*bi_Y)->sign == NEGATIVE) // bi_Y <= 0 �� ���
	{
		printf("INVALID \n");
		return;
	}

	for (int i = get_bit_length(bi_X) - 1; i >= 0; i--)
	{
		left_shift(bi_R, 1); //line3
		(*bi_R)->a[0] ^= get_j_th_bit(bi_X, i); //line3

		if (compare(bi_R, bi_Y) != -1) //R>=B�϶��̹Ƿ� R<B�� ��찡 -1�̹Ƿ� NOT���� �ϸ�ȴ�.
		{
			subtraction(bi_R, bi_Y, &T1); //���ָ� ������ ����
			assign_bigint(bi_R, &T1); //�� ������ Temp1�� bi_R�� ����

			set_one(&T2); //���ٴ°� ���� 1�� ���ܾ� �Ѵٴ� ��
			left_shift(&T2, i); //���� ��ġ�� ������ shift�̵�
			set_wordlen_the_same(bi_Q, &T2); //������ bi_Q�� T2�� XOR�ϱ⸦ ���Ѵ�. ���� XOR�ϱ� ���� wordlen�� ��������
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
	if (is_zero(bi_Y) == TRUE || (*bi_Y)->sign == NEGATIVE) // bi_Y <= 0 �� ���
	{
		printf("INVALID \n");
		return;
	}

	if (compare(bi_X, bi_Y) == -1) //bi_X < bi_Y �� ���
	{
		//Trivial
		set_zero(bi_Q); //���� 0
		assign_bigint(bi_R, bi_X); //������ bi_X
		return;
	}

	if (is_one(bi_Y) == TRUE) //bi_Y = 1 �� ���
	{
		//Trivial
		assign_bigint(bi_Q, bi_X); // ���� bi_Y
		set_zero(bi_R); //�������� 0
		return;
	}

	//line 10 : (Q, R) <- (0, A)
	set_zero(bi_Q);
	assign_bigint(bi_R, bi_X);

	bigint_ptr one_bigint_for_quotient = NULL;
	bigint_ptr Temp1 = NULL;
	bigint_ptr Temp2 = NULL;

	//�ʱⰪ ����
	set_zero(&Temp1);
	set_zero(&Temp2);
	//Q <- Q + 1 ���� bigint
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
		

		//printf("bi_R�� wordlen : %d \n",(*bi_R)->wordlen);
		//printf("bi_Y�� wordlen : %d \n", (*bi_Y)->wordlen);
	}

	delete_bigint(&one_bigint_for_quotient);
	delete_bigint(&Temp1);
	delete_bigint(&Temp2);
}

void division_multi_precision_long(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Q, bigint_ptr* bi_R)
{
	if (is_zero(bi_Y) == TRUE || (*bi_Y)->sign == NEGATIVE) // bi_Y <= 0 �� ���
	{
		printf("INVALID \n");
		return;
	}

	if (compare(bi_X, bi_Y) == -1) //bi_X < bi_Y �� ���
	{
		//Trivial
		set_zero(bi_Q); //���� 0
		assign_bigint(bi_R, bi_X); //������ bi_X
		return;
	}

	if (is_one(bi_Y) == TRUE) //bi_Y = 1 �� ���
	{
		//Trivial
		assign_bigint(bi_Q, bi_X); // ���� bi_Y
		set_zero(bi_R); //�������� 0
		return;
	}

	//line 7 : (Q, R) <- (0, 0)
	set_zero(bi_Q);
	set_zero(bi_R);

	//Q�� wordlen�� bi_X�� ���ƾ��ϰ� ��簪�� 0�̾����
	//bi_Q�� ���� wordlen�� 1�̹Ƿ� bi_X�� wordlen�� �������� wordlen-1��ŭ ���������
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
		(*bi_R)->a[0] = (*bi_X)->a[i]; //R <- RW + i��° A
		
		if(i== ((*bi_X)->wordlen) - 1)//word_bit�� 8�϶� ���� ��� ó������ 00000000 a[0] �̱⿡ refine�� ������
			refine_bigint(bi_R);

		//line 4 : k�� ã�´�. 2^k * m-1��° bi_Y �� [2^w-1, 2^w)�� ���ϵ��� 
		//k�� �ѹ��� ã���ָ�ȴ�.
		//div_core���� �������� �̵���������
		if (i == ((*bi_X)->wordlen) - 1)
		{
			To_find_k->a[0] = (*bi_Y)->a[((*bi_Y)->wordlen) - 1];
			for (int cnt_i = 0;; cnt_i++)
			{
				//m-1��° bi_Y�� 0b1000 0000 �� and������ �ؼ� 1000 0000 �� ������ 2^w-1 �̻��̹Ƿ�
				//k�� ã�� �� ����
				if (((To_find_k->a[0]) & (1i64 << (WORD_BITLEN - 1))) == (1i64 << (WORD_BITLEN - 1)))
				{
					k = cnt_i;
					break;
				}
				To_find_k->a[0] <<= 1;
			}
		}	
		div_core(bi_R, bi_Y, (*bi_Q)->a, i, bi_R, k); //i��° Q�� ���� ���� ����, (*bi_Q)->a�� i�� ���� �Լ��� �Ѱ���, R�� ���� �������� R�� �Ѱ���
	}

	refine_bigint(bi_Q);

	delete_bigint(&To_find_k);
}

void div_core(bigint_ptr* bi_X, bigint_ptr* bi_Y, word* Q_ith_word, int i, bigint_ptr* return_bi_R, int k)
{
	int cnt_i = 0;

	if (compare(bi_X, bi_Y) == -1) //bi_X < bi_Y �� ���
	{
		//return (0, A)
		Q_ith_word[i] = 0x0;
	   // assign_bigint(return_bi_R, bi_X); //�� �� �ʿ䰡 ����. �Ѵ� ���� �Լ����� bi_R�� �ּҸ� �����°��̹Ƿ�
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
		right_shift(bi_X, k); // bi_X�� �ǵ�����

		left_shift(bi_Y, k); //2^k * bi_Y
		assign_bigint(&prime_bi_Y, bi_Y);
		right_shift(bi_Y, k); // bi_Y�� �ǵ�����
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

	//�ʱⰪ ������ �ʿ��� �͵鸸 ����
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