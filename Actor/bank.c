#include "../Util/deck.h"

#include "bank.h"

void bankManager(bank_t * bank, pthread_t * threads, player_t * players, pthread_barrier_t * barrier)
{
  uint i = 0;
  deck_t * deck = NULL;
  initDeckLib();
  deck = initDeck(P52, bank->nbDecks);

  //shuffling decks
  for(i = 0; i < bank->nbDecks; i++)
  {
    shuffleDeck(&deck[i]);
  }

  //gives cards to all players
  for(i = 0; i < bank->nbPlayer; i++)
  {
    
  }
}
