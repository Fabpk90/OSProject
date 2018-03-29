#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "Util/deck.h"
#include "Actor/player.h"

int main(int argc, char **argv)
{
  /*
  create all players structs pthread,
  initiate the bank
  go into the bank
  simulation
  */
  int numPlayer = 4;
  pthread_t *threads = malloc(sizeof(pthread_t) * numPlayer);
  Player *players = malloc(sizeof(Player) * numPlayer);

  for(int i = 0; i < numPlayer; i++)
  {
    players[i].money = 10;

    pthread_create(&threads[i], NULL
      ,playerManager, &players[i]);
  }

  for(int i = 0; i < numPlayer; i++)
  {
      pthread_join(threads[i], NULL);
  }

  free(players);
  free(threads);

  return 0;
}
