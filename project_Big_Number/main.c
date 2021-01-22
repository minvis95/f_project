#include "Pandora.h"


int main()
{
    //!!addition, subtraction, mul, mul_kara, squar, squar_kara�� 
    //A, B�� wordlen�� 100���� �����ϰ� ���

    //test_add_verification();
    //test_add_performance(); //totalcycles/ 100 =       1295

    //test_sub_verification();
    //test_sub_performance(); // totalcycles/ 100 =        993

    //test_mul_verification();
    //test_mul_performance(); // totalcycles/ 100 =  101210988

    //test_mul_karatsuba_verification();
    //test_mul_karatsuba_performance(); // totalcycles/ 100 =   65019430

    //test_squaring_verification();
    //test_squaring_performance(); // totalcycles/ 100 =   53701775

    //test_squaring_karatsuba_verification();
    //test_squaring_karatsuba_performance(); // totalcycles / 100 = 46849105


    //!!division���� A, B�� wordlen�� ���� 40, 10���� �����ϰ� Performance���
    //!!naive�� 4,1�� ���־���(�ʹ� �����ɸ�...��� 4, 1�� �ʹ� �����ɷ��� cnt_i�� �ѹ���..)

    //test_division_naive_verification();
    //test_division_naive_performance(); // totalcycles / 1 = 100968121042

    //test_division_binary_long_verification();
    //test_division_binary_long_performance(); // totalcycles/ 100 =   10180729

    //test_division_multi_precision_long_verification();
    //test_binary_long_division_vs_multi_precision_long_performance();
    //  totalcycles / 100 = 11063074    <- long division
    //  totalcycles / 100 = 10414615    <- multi_precision
    
    //total_vs_performance();
}

    /*
    //�Լ� test�� ���� word��
    word InputA[3] = { 0x22, 0x43, 0x24 };
    word InputB[3] = { 0x22, 0x44, 0x42 };
    word InputC[4] = { 0xf2, 0x3d, 0xea, 0x00 };
    word InputD[5] = { 0xf2, 0x3d, 0xea, 0xf1, 0xd7 };
    word InputE[3] = { 0x01, 0x02, 0x03 };
    word InputF[3] = { 0x11, 0x22, 0x00 };
    word InputG[3] = { 0x11, 0x22, 0x33 };
    word InputZ[3] = { 0x00, 0x00, 0xff };
    

    //�Լ� test�� ���� bigint ����ü�� ����Ű�� �����ͺ���
    //typedef bigint* bigint_ptr; ��� ��������
    bigint_ptr A = NULL;
    bigint_ptr B = NULL;
    bigint_ptr C = NULL;
    bigint_ptr D = NULL;
    bigint_ptr E = NULL;
    bigint_ptr F = NULL;
    bigint_ptr G = NULL;
    bigint_ptr H = NULL;
    bigint_ptr I = NULL;
    bigint_ptr J = NULL;
    bigint_ptr A1 = NULL;
    bigint_ptr A2 = NULL;
    bigint_ptr A3 = NULL;
    bigint_ptr Q = NULL;


    //3�� ���� test
    printf("test \n");
    new_bigint(&A, 3);
    set_bigint(&B, InputA, NON_NEGATIVE, 3);
    show_bigint(B, 16);
    delete_bigint(&B);
    set_bigint(&B, InputD, NON_NEGATIVE, 5);
    show_bigint(B, 16);







    
    //new_bigint(bigint_ptr* bi_X, int wordlen)
    printf("new_bigint�Լ� test \n");
    new_bigint(&A, 3);
    show_bigint(A, 16); //show_bigint(bigint* X, int base)

    printf("\n");

    //set_bigint(bigint_ptr* bi_X, word* input, int sign, int wordlen)
    printf("set_bigint�Լ� test \n");
    set_bigint(&B, InputA, NEGATIVE, 3);
    show_bigint(B, 16);

    printf("\n");

    //refine_bigint(bigint_ptr* bi_X)
    printf("refine_bigint�Լ� test \n");
    set_bigint(&C, InputC, NON_NEGATIVE, 4); //0x00 WORD�ϳ� �� �߰�������(3 -> 4)
    
    printf("before \n");
    show_bigint(C, 16);
    
    printf("after \n");
    refine_bigint(&C);
    show_bigint(C, 16);

    printf("\n");

    //assign_bigint(bigint_ptr* bi_Y, bigint* X)
    printf("assign_bigint�Լ� test \n");
    assign_bigint(&Q, &C);
    show_bigint(Q, 16);

    printf("\n");

    //generate_random_bigint(bigint_ptr* bi_X, int sign, int wordlen)
    printf("generate_random_bigint�Լ� test \n");
    generate_random_bigint(&D, NON_NEGATIVE, 3);
    show_bigint(D, 16);

    printf("\n");

    //int get_word_length(bigint_ptr* bi_X), int get_bit_length(bigint_ptr * bi_X)
    printf("get_word_length�Լ��� get_bit_length�Լ� test \n");
    printf("word length = %d \n", get_word_length(&D));
    printf("bit length = %d \n", get_bit_length(&D));

    printf("\n");

    //int get_j_th_bit(bigint_ptr* bi_X, int j_th) j=0��°����~WORD_BITLEN�� ��� - 1����
    printf("get_j_th_bit�Լ� test \n");
    printf("InputA�� word�� �� ����ü(*B)�� 17��° ��Ʈ = %d \n", get_j_th_bit(&B, 18));
    
    printf("\n");

    //int get_sign(bigint_ptr* bi_X), void flip_sign(bigint_ptr* bi_X)
    printf("get_sign�Լ��� flip_sign�Լ� test \n");
    printf("flip�� = %d \n", get_sign(&B));
    flip_sign(&B);
    printf("flip�� = %d \n", get_sign(&B));

    printf("\n");

    //swap_bigint(bigint_ptr* bi_X, bigint_ptr* bi_Y)
    printf("swap_bigint�Լ� test \n");
    printf("����Ű�� ����ü �ּҸ� ���� �ٲ۴�. \n"); 
    printf("before \n");
    show_bigint(A, 16);
    show_bigint(C, 16);

    swap_bigint(&A, &C);
    printf("after \n");
    show_bigint(A, 16);
    show_bigint(C, 16);

    printf("\n");

    //set_one(bigint_ptr* bi_X), set_zero(bigint_ptr* bi_X)
    printf("set_one�Լ��� set_zero�Լ� test \n");
    set_one(&E);
    set_zero(&F);
    show_bigint(E, 16);
    show_bigint(F, 16);

    printf("\n");

    //is_zero(bigint_ptr* bi_X), is_one(bigint_ptr* bi_X)
    printf("is_zero�Լ��� is_one�Լ� test \n");

    printf("*E�� zero? %d \n", is_zero(&E)); //0
    printf("*E�� one? %d \n", is_one(&E)); //1
    printf("*F�� zero? %d \n", is_zero(&F)); //1
    printf("*F�� one? %d \n", is_one(&F)); //0
    
    printf("\n");

    //compare(bigint_ptr* bi_X, bigint_ptr* bi_Y)
    printf("compare�Լ� test \n");

    set_bigint(&G, InputB, NON_NEGATIVE, 3);
    show_bigint(B, 16);
    show_bigint(D, 16); //*D����ü�� wordlen�� 3�� random���̴�.
    
    if (compare(&B, &D) == 1)
        printf("ù��°�� ��ũ��. \n");
    else if (compare(&B, &G) == -1)
        printf("�ι�°�� ��ũ��. \n");
    else
        printf("�Ѵ� �������̴�. \n");

    printf("\n");

    //left_shift(bigint_ptr* bi_X, int shift)
    printf("left_shift �Լ� test \n");

    show_bigint(B, 16);
    left_shift(&B, 8);
    show_bigint(B, 16);
    left_shift(&B, 1);
    show_bigint(B, 16);
    
    printf("\n");
    show_bigint(D, 16);
    left_shift(&D, 9);
    show_bigint(D, 16);
    
    printf("\n");
    set_bigint(&G, InputF, NON_NEGATIVE, 3);
    show_bigint(G, 16);
    left_shift(&G, 9);
    show_bigint(G, 16);

    printf("\n");

    //right_shift(bigint_ptr* bi_X, int shift)
    printf("right_shift�Լ� test \n");
    
    show_bigint(B, 16);//left_shift �̵��� ��ġ��ŭ right_shift ���ϸ� ���� ������ ���� ���;��Ѵ�
    right_shift(&B, 1);
    show_bigint(B, 16);
    right_shift(&B, 8);
    show_bigint(B, 16);
    
    printf("\n");
    show_bigint(D, 16);
    right_shift(&D, 9);
    show_bigint(D, 16);

    printf("\n");

    //reduction(bigint_ptr* bi_X, int power_of_2)
    printf("reduction�Լ� test \n");

    show_bigint(B, 16);
    reduction(&B, 24); //�״�ξ���
    show_bigint(B, 16);
    reduction(&B, 16); //�ֻ��� WORD�ϳ� ���������
    show_bigint(B, 16);
    reduction(&B, 9); //WORDLEN = 2, 10 0000 0000 Ob ������ ���;��� 
    show_bigint(B, 16);
    reduction(&B, 4); //WORDLEN = 1, 1 0000 Ob ������ ���;���
    show_bigint(B, 16);
    printf("\n");

    //void addition(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)
    printf("addition�Լ� test \n");

    set_bigint(&H, InputG, NON_NEGATIVE, 3);//InputX�� ���� wordlen�� ��ġ�ϰ� �־ addition�ϸ� �ǹٸ� ���� ����
    set_bigint(&I, InputZ, NON_NEGATIVE, 3);
    new_bigint(&J, 3);

    show_bigint(H, 16);
    show_bigint(I, 16);
    addition(&H, &I, &J);
    show_bigint(J, 16);

    printf("\n");
        
    //void subtraction(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)
    printf("subtraction�Լ� test \n");
    show_bigint(I, 16);
    show_bigint(H, 16);
    subtraction(&I, &H, &J);
    show_bigint(J, 16);

    printf("\n");

    

   //random bigint�� addition test
   // generate_random_bigint(&A1, NON_NEGATIVE, 5);
   // generate_random_bigint(&A2, NON_NEGATIVE, 3);
    //new_bigint(&A3, 3);
    
    //addition(&A1, &A2, &A3);
    //printf("A = ");
   // show_bigint(A1, 16);
    //printf("B = ");
   // show_bigint(A2, 16);
   // printf("C = ");
   // show_bigint(A3, 16);
   // printf("C == A + B");

    printf("\n");
    





    //delete_bigint(bigint_ptr* bi_X) �Ҵ� free
    delete_bigint(&A);
    delete_bigint(&B);
    delete_bigint(&C);
    delete_bigint(&D);
    delete_bigint(&E);
    delete_bigint(&F);
    delete_bigint(&G);
    delete_bigint(&H);
    delete_bigint(&I);
    delete_bigint(&J);
    delete_bigint(&Q);
    delete_bigint(&A);
    delete_bigint(&B);
    delete_bigint(&C);

    */