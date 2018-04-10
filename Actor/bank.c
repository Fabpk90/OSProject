#include <stdlib.h>

#include "../Util/deck.h"

#include "bank.h"

void bankManager(bank_t * bank, pthread_t * threads, player_t * players, pthread_barrier_t * barrier)
{
  uint i = 0;
  deck_t * decks = NULL;
  initDeckLib();

  //init and shuffling decks
  decks = initDeck(P52, bank->nbDecks);
  shuffleDeck(decks);

  //gives cards to all players
  for(i = 0; i < bank->nbPlayer; i++)
  {
    players[i].hand = initCardHandler();
    addCard(players[i].hand, 5);
  }

  pthread_barrier_wait(barrier);

  removeDeck(decks);
}
