#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "../Util/cardHandler.h"

void * playerManager(void * playerStruct)
{
  player_t *player = (player_t*) playerStruct;
  char stopPlaying = 0;

  while(!stopPlaying)
  {
    stopPlaying = 1;
    //waits for his cards
    pthread_barrier_wait(player->barrierRound);

    player->hand = initCardHandler();

    addCard(player->hand, 5);
    addCard(player->hand, 6);
    freeCardHandler(player->hand);
  }

  return NULL;
}

int getValueFromHand(cardHandler_t * cards)
{
  int val = 0;
  cardHandler_t * index = cards;

  while(index != NULL)
  {
    val += index->card;

    index = index->next;
  }

  return val;
}
