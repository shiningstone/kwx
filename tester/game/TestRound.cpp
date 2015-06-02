
#include "./../../game/GameType.h"

typedef struct CardSeq {
    Card_t player1[14];
    Card_t player2[13];
    Card_t player3[13];
    Card_t remains[44];
}CardSeq_t;

CardSeq_t CardRoundx[] = {
    {/* »ù±¾²Ù×÷ */
        {TIAO_1,TIAO_1,TIAO_1,TIAO_3,TIAO_3,TIAO_3,TIAO_4,TIAO_5,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1},
        {TONG_1,TONG_1,TONG_1,TONG_2,TONG_2,TONG_3,TONG_3,TONG_4,TONG_6,TONG_7,TONG_8,TONG_8,TONG_9},
        {ZHONG,ZHONG,ZHONG,TONG_2,TONG_2,TONG_3,TONG_3,TONG_4,TONG_6,TONG_7,TONG_8,TONG_9,TONG_9},

        {TIAO_1,TIAO_2,TIAO_3,TIAO_4,TIAO_5,TIAO_6,TIAO_7,BAI,BAI,BAI,BAI,BAI},
    },
    {/* ???ü */
        {TIAO_1,TIAO_1,TIAO_2,TIAO_2,TIAO_3,TIAO_3,TIAO_4,TIAO_4,TIAO_5,TIAO_5,TIAO_6,TIAO_6,TIAO_7,TIAO_7},
        {TONG_1,TONG_1,TONG_1,TONG_2,TONG_2,TONG_2,TONG_3,TONG_3,TONG_3,TONG_4,TONG_4,TONG_4,BAI},
        {TONG_1,TONG_1,TONG_1,TONG_2,TONG_2,TONG_2,TONG_3,TONG_3,TONG_3,TONG_4,TONG_4,TONG_4,FA},

		{TONG_9,TONG_9,BAI,TIAO_1,TIAO_2,TIAO_3,TIAO_4,BAI,BAI,TIAO_1,TIAO_2,TIAO_3,TIAO_4},
    },
    {/* Ñ¡¸Ü */
        {TIAO_1,TIAO_1,TIAO_1,TIAO_1,TIAO_3,TIAO_3,TIAO_3,TIAO_3,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1},
        {TONG_1,TONG_1,TONG_1,TONG_2,TONG_2,TONG_3,TONG_3,TONG_4,TONG_6,TONG_7,TONG_8,TONG_8,TONG_9},
        {ZHONG,ZHONG,ZHONG,TONG_2,TONG_2,TONG_3,TONG_3,TONG_4,TONG_6,TONG_7,TONG_8,TONG_9,TONG_9},

        {TIAO_4,TIAO_5,TIAO_6,TIAO_4,TIAO_5,TIAO_6,TIAO_7,BAI,BAI,BAI,BAI,BAI},
    },
    {/* ¿ª¾Ö¸Ü */
        {TIAO_1,TIAO_1,TIAO_1,TIAO_1,TIAO_3,TIAO_3,TIAO_4,TIAO_5,TIAO_6,TIAO_7,TONG_1,TONG_2,TONG_3,TONG_4},
        {TONG_1,TONG_1,TONG_2,TONG_2,TONG_2,TONG_3,TONG_3,TONG_5,TONG_6,TONG_7,ZHONG,FA,BAI},
        {TONG_1,TONG_1,TONG_1,TONG_2,TONG_2,TONG_4,TONG_4,TONG_5,TONG_6,TONG_7,ZHONG,FA,BAI},
    
        {TIAO_9,TIAO_2,TIAO_3,TIAO_4,TIAO_5,TIAO_6,TIAO_7,BAI,BAI,BAI,BAI,BAI},
    },
    {/* other's action test */
        {TIAO_1,TIAO_1,TIAO_1,TIAO_3,TIAO_3,TIAO_3,TIAO_4,TIAO_5,TIAO_6,TIAO_7,TONG_1,TONG_2,TONG_3,TONG_4},
        {TONG_1,TONG_1,TONG_2,TONG_2,TONG_2,TONG_3,TONG_3,TONG_5,TONG_6,TONG_7,ZHONG,FA,BAI},
        {TONG_1,TONG_1,TONG_1,TONG_2,TONG_2,TONG_4,TONG_4,TONG_5,TONG_6,TONG_7,ZHONG,FA,BAI},

        {TIAO_1,TIAO_2,TIAO_3,TIAO_4,TIAO_5,TIAO_6,TIAO_7,BAI,BAI,BAI,BAI,BAI},
    },
};

void load_test_round(int roundNo,Card_t *output) {
    CardSeq_t *cardSeq = &CardRoundx[roundNo-1];
    
	int idx = 0;
	for(int i=0;i<14;i++) {
		output[idx++] = cardSeq->player1[i];
	}
    
	for(int i=0;i<13;i++) {
		output[idx++] = cardSeq->player2[i];
	}
    
	for(int i=0;i<13;i++) {
		output[idx++] = cardSeq->player3[i];
	}

    for(int i=0;i<84-40;i++) {
        output[40+i] = cardSeq->remains[i];
    }
}

