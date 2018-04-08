#ifndef PLAYER_H
#define PLAYER_H

#include "../Util/consts.h"
#include <pthread.h>

typedef enum EPlayerGamblingType
{
	CONSTANT,
	MORE,
	LESS
} EPlayerGamblingType_t;

typedef struct player
{
	uint money;
	uint placing;
	EPlayerGamblingType_t strategy;
	uint stopVal;
	uint objMoney;
	pthread_barrier_t * barrierRound; //used for waitin that everyone has played
	//à utiliser pour ditribuer les cartes -> tous les joueurs attendent la banque(distribution de cartes)
	//et après la barrière resert pour attendre que tout le monde ait choisi une action
} player_t;

void * playerManager(void *playerStruct);

#endif // PLAYER_H
