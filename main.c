#include <stdio.h>
#include <stdlib.h>

#include "Util/deck.h"
#include "Actor/player.h"

int main(int argc, char **argv)
{
  Player *player = malloc(sizeof(Player));
  player->money = 10;

  printf("%u\n", player->money);

  return 0;
}
