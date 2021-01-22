#include "Pandora.h"


int main()
{
    //!!addition, subtraction, mul, mul_kara, squar, squar_kara는 
    //A, B의 wordlen을 100으로 고정하고 계산

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


    //!!division에서 A, B의 wordlen을 각각 40, 10으로 고정하고 Performance계산
    //!!naive만 4,1로 해주었음(너무 오래걸림...사실 4, 1도 너무 오래걸려서 cnt_i도 한번만..)

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
    //함수 test를 위한 word값
    word InputA[3] = { 0x22, 0x43, 0x24 };
    word InputB[3] = { 0x22, 0x44, 0x42 };
    word InputC[4] = { 0xf2, 0x3d, 0xea, 0x00 };
    word InputD[5] = { 0xf2, 0x3d, 0xea, 0xf1, 0xd7 };
    word InputE[3] = { 0x01, 0x02, 0x03 };
    word InputF[3] = { 0x11, 0x22, 0x00 };
    word InputG[3] = { 0x11, 0x22, 0x33 };
    word InputZ[3] = { 0x00, 0x00, 0xff };
    

    //함수 test를 위한 bigint 구조체를 가르키는 포인터변수
    //typedef bigint* bigint_ptr; 라고 선언했음
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


    //3번 문제 test
    printf("test \n");
    new_bigint(&A, 3);
    set_bigint(&B, InputA, NON_NEGATIVE, 3);
    show_bigint(B, 16);
    delete_bigint(&B);
    set_bigint(&B, InputD, NON_NEGATIVE, 5);
    show_bigint(B, 16);







    
    //new_bigint(bigint_ptr* bi_X, int wordlen)
    printf("new_bigint함수 test \n");
    new_bigint(&A, 3);
    show_bigint(A, 16); //show_bigint(bigint* X, int base)

    printf("\n");

    //set_bigint(bigint_ptr* bi_X, word* input, int sign, int wordlen)
    printf("set_bigint함수 test \n");
    set_bigint(&B, InputA, NEGATIVE, 3);
    show_bigint(B, 16);

    printf("\n");

    //refine_bigint(bigint_ptr* bi_X)
    printf("refine_bigint함수 test \n");
    set_bigint(&C, InputC, NON_NEGATIVE, 4); //0x00 WORD하나 더 추가시켰음(3 -> 4)
    
    printf("before \n");
    show_bigint(C, 16);
    
    printf("after \n");
    refine_bigint(&C);
    show_bigint(C, 16);

    printf("\n");

    //assign_bigint(bigint_ptr* bi_Y, bigint* X)
    printf("assign_bigint함수 test \n");
    assign_bigint(&Q, &C);
    show_bigint(Q, 16);

    printf("\n");

    //generate_random_bigint(bigint_ptr* bi_X, int sign, int wordlen)
    printf("generate_random_bigint함수 test \n");
    generate_random_bigint(&D, NON_NEGATIVE, 3);
    show_bigint(D, 16);

    printf("\n");

    //int get_word_length(bigint_ptr* bi_X), int get_bit_length(bigint_ptr * bi_X)
    printf("get_word_length함수와 get_bit_length함수 test \n");
    printf("word length = %d \n", get_word_length(&D));
    printf("bit length = %d \n", get_bit_length(&D));

    printf("\n");

    //int get_j_th_bit(bigint_ptr* bi_X, int j_th) j=0번째부터~WORD_BITLEN의 배수 - 1까지
    printf("get_j_th_bit함수 test \n");
    printf("InputA가 word로 들어간 구조체(*B)의 17번째 비트 = %d \n", get_j_th_bit(&B, 18));
    
    printf("\n");

    //int get_sign(bigint_ptr* bi_X), void flip_sign(bigint_ptr* bi_X)
    printf("get_sign함수와 flip_sign함수 test \n");
    printf("flip전 = %d \n", get_sign(&B));
    flip_sign(&B);
    printf("flip후 = %d \n", get_sign(&B));

    printf("\n");

    //swap_bigint(bigint_ptr* bi_X, bigint_ptr* bi_Y)
    printf("swap_bigint함수 test \n");
    printf("가르키는 구조체 주소를 서로 바꾼다. \n"); 
    printf("before \n");
    show_bigint(A, 16);
    show_bigint(C, 16);

    swap_bigint(&A, &C);
    printf("after \n");
    show_bigint(A, 16);
    show_bigint(C, 16);

    printf("\n");

    //set_one(bigint_ptr* bi_X), set_zero(bigint_ptr* bi_X)
    printf("set_one함수와 set_zero함수 test \n");
    set_one(&E);
    set_zero(&F);
    show_bigint(E, 16);
    show_bigint(F, 16);

    printf("\n");

    //is_zero(bigint_ptr* bi_X), is_one(bigint_ptr* bi_X)
    printf("is_zero함수와 is_one함수 test \n");

    printf("*E는 zero? %d \n", is_zero(&E)); //0
    printf("*E는 one? %d \n", is_one(&E)); //1
    printf("*F는 zero? %d \n", is_zero(&F)); //1
    printf("*F는 one? %d \n", is_one(&F)); //0
    
    printf("\n");

    //compare(bigint_ptr* bi_X, bigint_ptr* bi_Y)
    printf("compare함수 test \n");

    set_bigint(&G, InputB, NON_NEGATIVE, 3);
    show_bigint(B, 16);
    show_bigint(D, 16); //*D구조체는 wordlen이 3인 random값이다.
    
    if (compare(&B, &D) == 1)
        printf("첫번째가 더크다. \n");
    else if (compare(&B, &G) == -1)
        printf("두번째가 더크다. \n");
    else
        printf("둘다 같은값이다. \n");

    printf("\n");

    //left_shift(bigint_ptr* bi_X, int shift)
    printf("left_shift 함수 test \n");

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
    printf("right_shift함수 test \n");
    
    show_bigint(B, 16);//left_shift 이동한 수치만큼 right_shift 취하면 순수 기존의 값이 나와야한다
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
    printf("reduction함수 test \n");

    show_bigint(B, 16);
    reduction(&B, 24); //그대로야함
    show_bigint(B, 16);
    reduction(&B, 16); //최상위 WORD하나 사라져야함
    show_bigint(B, 16);
    reduction(&B, 9); //WORDLEN = 2, 10 0000 0000 Ob 안으로 들어와야함 
    show_bigint(B, 16);
    reduction(&B, 4); //WORDLEN = 1, 1 0000 Ob 안으로 들어와야함
    show_bigint(B, 16);
    printf("\n");

    //void addition(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)
    printf("addition함수 test \n");

    set_bigint(&H, InputG, NON_NEGATIVE, 3);//InputX에 따라 wordlen을 일치하게 넣어서 addition하면 옳바른 값이 나옴
    set_bigint(&I, InputZ, NON_NEGATIVE, 3);
    new_bigint(&J, 3);

    show_bigint(H, 16);
    show_bigint(I, 16);
    addition(&H, &I, &J);
    show_bigint(J, 16);

    printf("\n");
        
    //void subtraction(bigint_ptr* bi_X, bigint_ptr* bi_Y, bigint_ptr* bi_Z)
    printf("subtraction함수 test \n");
    show_bigint(I, 16);
    show_bigint(H, 16);
    subtraction(&I, &H, &J);
    show_bigint(J, 16);

    printf("\n");

    

   //random bigint를 addition test
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
    





    //delete_bigint(bigint_ptr* bi_X) 할당 free
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