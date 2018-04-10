#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "../Util/cardHandler.h"

void * playerManager(void * playerStruct)
{
  player_t *player = (player_t*) playerStruct;
  bool stopPlaying = 0;
  int handValue = 0;
  float bet = 0;
  char resultLastRound = FLAG_RESULT_NONE;

  while(!stopPlaying)
  {
    stopPlaying = 1;

    //waits for his cards
    pthread_barrier_wait(player->barrierRound);

    bet = getBet(resultLastRound, player->placing, player->strategy);

    handValue = getValueFromHand(player->hand);

    printf("Value : %d\n", handValue);

    // loop unitl threshold reached
    while(handValue < player->stopVal)
    {
        //ask card
        player->wantCard = 1;
        pthread_barrier_wait(player->barrierCard);
        handValue = getValueFromHand(player->hand); // to optimize, stack is better i think
        //player->wantCard = 0;  <- this is done by the bank, for synchronisation purpose
    }

    printf("Value : %d\n", handValue);
    //waits for the bank to draw its cards
    //pthread_barrier_wait(player->barrierRound);


    //at the end, when it gets his money or not, test if he quits
    if(player->money >= player->objMoney)
      stopPlaying = 1;

  }

  freeCardHandler(player->hand);

  return NULL;
}

float getBet(char resultLastRound, float placing, gambling_t strategy)
{
  if(resultLastRound & FLAG_RESULT_NONE || strategy & FLAG_GAMBLING_CONST)
    return placing;

  if(strategy & FLAG_GAMBLING_MORE)
  {
    if(resultLastRound & FLAG_RESULT_LOSS)
      return placing * 2.f;

    return placing;
  }
  else // less
  {
    if(resultLastRound & FLAG_RESULT_LOSS)
      return placing / 2.f;
    return placing;
  }
}

int getValueFromHand(cardHandler_t * cards)
{
  uint val = 0;
  cardHandler_t * index = cards;

  while(index != NULL)
  {
    val += index->card;

    index = index->next;
  }

  return val;
}
