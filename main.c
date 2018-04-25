#include <stdlib.h>

#include <pthread.h>

#include "Actor/bank.h"
#include "Actor/player.h"
#include "Util/cardHandler.h"
#include "Util/fileHandler.h"

int main(int argc, char **argv)
{
  pthread_t * threads = NULL;
  player_t * players = NULL;
  bank_t * bank = NULL;
  int i = 0;

  initGame("config.dat", &bank, &players);

  threads = malloc(sizeof(pthread_t) * bank->nbPlayer);

  for(i = 0; i < bank->nbPlayer; i++)
  {
    pthread_create(&threads[i], NULL, playerManager, &players[i]);
  }


  bankManager(bank, threads, players);

  for(i = 0; i < bank->nbPlayer; i++)
  {
      pthread_join(threads[i], NULL);
  }

  free(players);
  free(threads);
  pthread_barrier_destroy(bank->barrierRound);
  pthread_barrier_destroy(bank->barrierCard);
  pthread_barrier_destroy(bank->barrierRoundTmp);
  pthread_barrier_destroy(bank->barrierCardTmp);
  free(bank->barrierRound);
  free(bank->barrierRoundTmp);
  free(bank->barrierCard);
  free(bank->barrierCardTmp);
  free(bank);

  return 0;
}
