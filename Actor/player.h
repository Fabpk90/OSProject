#ifndef PLAYER_H
#define PLAYER_H

#include <pthread.h>

#include "../Util/consts.h"
#include "../Util/cardHandler.h"

#define FLAG_GAMBLING_CONST 1
#define FLAG_GAMBLING_MORE 	2
#define FLAG_GAMBLING_LESS 	4

#define FLAG_RESULT_WON 	1
#define FLAG_RESULT_WON_BJ 2
#define FLAG_RESULT_LOSS 	4
#define FLAG_RESULT_NONE 	8

typedef bool gambling_t;
typedef bool result_t;

typedef struct player
{
	uint id;
	uint money;
	uint placingBase;
	uint placing;
	gambling_t strategy;
	uint stopVal;
	uint objMoney;
	bool wantCard;
	bool isPlaying; //he's playing, enough money to
	result_t roundResult;
	uint moneyWon;
	//pthread_barrier_t * barrierEndGame; //used for waitin that everyone has played/ at the start of the round
	pthread_barrier_t ** barrierRound;  //used for waiting for cards
	pthread_barrier_t ** barrierRoundTmp;
	pthread_barrier_t ** barrierCard;  //used for waiting for cards
	pthread_barrier_t ** barrierCardTmp;
	cardHandler_t * hand;
	uint cardsVal;
	cardHandler_t ** bankHand; //for printing log at each round end
} player_t;

void * playerManager(void *playerStruct);

int getBet(char resultLastRound, uint placing, uint placingBase ,gambling_t strategy);

#endif // PLAYER_H
