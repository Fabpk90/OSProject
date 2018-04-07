#include <stdlib.h>
#include <stdio.h>

#include "player.h"

void * playerManager(void *playerStruct)
{
  player_t *player = (player_t*) playerStruct;

  printf("money %u\n", player->money);

  return NULL;
}
