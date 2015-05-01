
#include "./../../utils/BasicType.h"

void load_test_round_1(int *output) {
	int Seq1[14]={TIAO_1,TIAO_1,TIAO_1,TIAO_3,TIAO_3,TIAO_3,TIAO_4,TIAO_5,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1};
	int Seq2[13]={TONG_1,TONG_1,TONG_1,TONG_2,TONG_2,TONG_3,TONG_3,TONG_4,TONG_6,TONG_7,TONG_8,TONG_8,TONG_9};
	int Seq3[13]={TONG_1,TONG_1,TONG_1,TONG_2,TONG_2,TONG_3,TONG_3,TONG_4,TONG_6,TONG_7,TONG_8,TONG_9,TONG_9};
    
	int c = 0;
	for(int i=0;i<14;i++) {
		output[c++]=Seq1[i];
	}
    
	for(int i=0;i<13;i++) {
		output[c++]=Seq2[i];
	}
    
	for(int i=0;i<13;i++) {
		output[c++]=Seq3[i];
	}

	output[40] = TIAO_1;
	output[41] = TIAO_2;
	output[42] = TIAO_3;
	output[43] = TIAO_4;
	output[44] = TIAO_5;
	output[45] = TIAO_6;
	output[46] = TIAO_7;
	output[47] = BAI;
	output[48] = BAI;
	output[49] = BAI;
	output[50] = BAI;
	output[51] = BAI;
}

void load_test_round(int idx,int *output) {
    load_test_round_1(output);
}

