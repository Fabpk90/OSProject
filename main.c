#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "Actor/bank.h"
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
  pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * numPlayer);
  player_t *players = (player_t*)malloc(sizeof(player_t) * numPlayer);
  pthread_barrier_t * barrier  = (pthread_barrier_t*) malloc(sizeof(pthread_barrier_t));
  bank_t * bank = (bank_t*) malloc(sizeof(bank_t));
  int i = 0;

  pthread_barrier_init(barrier, NULL, numPlayer);

  for(i = 0; i < numPlayer; i++)
  {
    players[i].id = i+1;
    players[i].barrierRound = barrier;
    players[i].strategy = CONSTANT;
  //  players[i].money = 10;

    pthread_create(&threads[i], NULL, playerManager, &players[i]);
  }

  bankManager(bank, threads, players, barrier);

  for(i = 0; i < numPlayer; i++)
  {
      pthread_join(threads[i], NULL);
  }

  free(players);
  free(threads);
  pthread_barrier_destroy(barrier);
  free(bank);

  return 0;
}
