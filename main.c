#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "Actor/bank.h"
#include "Actor/player.h"
#include "Util/cardHandler.h"

int main(int argc, char **argv)
{
  /*
    create all players structs pthread,
    initiate the bank
    go into the bank
    simulation
  */
  int numPlayer = 4;
  pthread_t * threads = malloc(sizeof(pthread_t) * numPlayer);
  player_t * players = malloc(sizeof(player_t) * numPlayer);
  pthread_barrier_t * barrier  = malloc(sizeof(pthread_barrier_t));
  bank_t * bank = malloc(sizeof(bank_t));
  int i = 0;

  pthread_barrier_init(barrier, NULL, numPlayer + 1);

  for(i = 0; i < numPlayer; i++)
  {
    players[i].id = i+1;
    players[i].barrierRound = barrier;
    players[i].strategy = FLAG_GAMBLING_CONST;

    pthread_create(&threads[i], NULL, playerManager, &players[i]);
  }

  bank->nbPlayer = numPlayer;
  bank->nbDecks = 3;
  bank->nbRounds = 20;

  bankManager(bank, threads, players, barrier);

  for(i = 0; i < numPlayer; i++)
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
