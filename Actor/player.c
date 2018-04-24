#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "../Util/cardHandler.h"
#include "../Util/fileHandler.h"

void * playerManager(void * playerStruct)
{
  player_t *player = (player_t*) playerStruct;
  bool stopPlaying = 0;
  char resultLastRound = FLAG_RESULT_NONE;

  while(!stopPlaying)
  {
    stopPlaying = 1;

    printf("money: %d, placing: %d, strat: %d, stop: %d, obj: %d\n", player->money, player->placing
    , player->strategy, player->stopVal, player->objMoney);

    player->placing = getBet(resultLastRound, player->placing, player->strategy);
    //waits for his cards
    pthread_barrier_wait(player->barrierRound);

    player->cardsVal = getValueFromHand(player->hand);

    // loop until threshold reached
  /*  while(handValue <= player->stopVal)
    {
        //ask card
        player->wantCard = 1;
        pthread_barrier_wait(player->barrierCard);
        player->cardsVal = getValueFromHand(player->hand); // to optimize, add up directly the val of the card given
        //player->wantCard = 0;  <- this is done by the bank, for synchronisation purpose
    }*/

  //  printf("Value : %d\n", handValue);

    player->wantCard = 0; //not really needed, but still
    //pthread_barrier_wait(player->barrierRound);

    writePlayerLog(player);

    //at the end, when it gets his money or not, test if he quits
    if(player->money >= player->objMoney)
      stopPlaying = 1;

  }

  freeCardHandler(player->hand);

  return NULL;
}

int getBet(char resultLastRound, int placing, gambling_t strategy)
{
  if(resultLastRound & FLAG_RESULT_NONE || strategy & FLAG_GAMBLING_CONST)
    return placing;

  if(strategy & FLAG_GAMBLING_MORE)
  {
    if(resultLastRound & FLAG_RESULT_LOSS)
      return placing << 1;

    return placing;
  }
  else // less
  {
    if(resultLastRound & FLAG_RESULT_LOSS)
      return placing >> 1;
    return placing;
  }
}
