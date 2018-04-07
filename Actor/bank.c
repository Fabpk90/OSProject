#include "../Util/deck.h"

#include "bank.h"

void bankManager(pthread_t * threads, player_t * players, pthread_barrier_t * barrier)
{
  initDeckLib();
  deck_t *deck = initDeck(P52, 3);
}
