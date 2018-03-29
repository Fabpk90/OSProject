#ifndef PLAYER_H
#define PLAYER_H

#include "../Util/consts.h"

typedef enum EPlayerGamblingType
{
	CONSTANT,
	MORE,
	LESS
} EPlayerGamblingType;

typedef struct player
{
	uint money;
	uint placing;
	EPlayerGamblingType strategy;
	uint stopVal;
	uint objMoney;
} Player;

void * playerManager(void *playerStruct);

#endif // PLAYER_H
