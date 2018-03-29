#include <stdlib.h>
#include <stdio.h>

#include "player.h"

void * playerManager(void *playerStruct)
{
  Player *player = (Player*) playerStruct;

  player->money = 50;

  printf("money %u\n", player->money);

  return NULL;
}
