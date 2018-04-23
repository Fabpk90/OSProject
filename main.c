#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "Actor/bank.h"
#include "Actor/player.h"
#include "Util/cardHandler.h"
#include "Util/fileHandler.h"

int main(int argc, char **argv)
{
  /*
    create all players structs pthread,
    initiate the bank
    go into the bank
    simulation
  */
  pthread_t * threads = NULL;
  player_t * players = NULL;
  pthread_barrier_t * barrier = NULL;
  bank_t * bank = NULL;
  int i = 0;

  initGame("config.dat", &bank, &players);

  threads = malloc(sizeof(pthread_t) * bank->nbPlayer);
  barrier = malloc(sizeof(pthread_barrier_t));

  pthread_barrier_init(barrier, NULL, bank->nbPlayer + 1);

  for(i = 0; i < bank->nbPlayer; i++)
  {
    players[i].barrierRound = barrier;
    pthread_create(&threads[i], NULL, playerManager, &players[i]);
  }


  bankManager(bank, threads, players, barrier);

  for(i = 0; i < bank->nbPlayer; i++)
  {
      pthread_join(threads[i], NULL);
  }

  free(players);
  free(threads);
  pthread_barrier_destroy(barrier);
  free(barrier);
  free(bank);

  return 0;
}
