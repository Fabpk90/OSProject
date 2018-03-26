#ifdef PLAYER_H
#define PLAYER_H

#include "../Util/consts.h"

typedef enum EPlayerGamblingType
{
	CONSTANT,
	PLUS,
	LESS
} EPlayerGamblingType;

typedef struct Player
{
	uint money;
	EPlayerGamblingType

} Player;


#endif // PLAYER_H
