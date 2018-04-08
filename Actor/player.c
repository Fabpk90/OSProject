#include <stdlib.h>
#include <stdio.h>

#include "player.h"

void * playerManager(void *playerStruct)
{
  player_t *player = (player_t*) playerStruct;

  //waits for his cards
  pthread_barrier_wait(player->barrierRound);

  

  return NULL;
}
