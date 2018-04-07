#ifndef BANK_H
#define BANK_H

#include "../Util/consts.h"
#include <pthread.h>
#include "player.h"

typedef struct Bank
{
  uint nbPlayer;
  uint nbDecks;
  uint nbRounds;
} Bank;

void bankManager(pthread_t * threads, player_t * players, pthread_barrier_t* barrier);

#endif
