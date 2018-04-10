#ifndef PLAYER_H
#define PLAYER_H

#include <pthread.h>

#include "../Util/consts.h"
#include "../Util/cardHandler.h"

#define FLAG_GAMBLING_CONST 1
#define FLAG_GAMBLING_MORE 	2
#define FLAG_GAMBLING_LESS 	4

#define FLAG_RESULT_WON 	1
#define FLAG_RESULT_LOSS 	2
#define FLAG_RESULT_NONE 	4


typedef bool gambling_t;

typedef struct player
{
	uint id;
	float money;
	uint placing;
	gambling_t strategy;
	uint stopVal;
	uint objMoney;
	bool wantCard;
	pthread_barrier_t * barrierRound; //used for waitin that everyone has played/ at the start of the round
	pthread_barrier_t * barrierCard;  //used for waiting for cards
	cardHandler_t * hand;
	cardHandler_t * bankHand; //for printing log at each round end
} player_t;

void * playerManager(void *playerStruct);

int getValueFromHand(cardHandler_t * cards);

float getBet(char resultLastRound, float placing, gambling_t strategy);

#endif // PLAYER_H
