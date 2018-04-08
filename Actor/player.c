#include <stdlib.h>
#include <stdio.h>

#include "player.h"

void * playerManager(void *playerStruct)
{
  player_t *player = (player_t*) playerStruct;

  pthread_barrier_wait(player->barrierRound);
  printf("test\n");

  return NULL;
}
