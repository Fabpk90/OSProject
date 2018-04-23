#include <stdlib.h>
#include <stdio.h>

#include "../Util/deck.h"

#include "bank.h"

void bankManager(bank_t * bank, pthread_t * threads, player_t * players)
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

  pthread_barrier_wait(bank->barrierRound);

  while(bank->nbRounds!=0)
  {
    for(i = 0; i < bank->nbPlayer; i++)
    {
      if(players[i].wantCard==1)
      {
        addCard(players[i].hand,5);
      }
      if(getValueFromHand(players[i].hand)==21)
      {
        printf("Win");
        bank->nbRounds=0;
      }
    }

    bank->nbRounds --;
  }


  removeDeck(decks);
}
