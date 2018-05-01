#include <stdlib.h>

#include "player.h"
#include "../Util/cardHandler.h"
#include "../Util/fileHandler.h"

void * playerManager(void * playerStruct)
{
  player_t *player = (player_t*) playerStruct;

  player->isPlaying = 1;
  player->roundResult = FLAG_RESULT_NONE;

//  printf("Player: %d money: %d, placing: %d, strat: %d, stop: %d, obj: %d\n",player->id, player->money, player->placing
//  , player->strategy, player->stopVal, player->objMoney);

  while(player->isPlaying)
  {
    player->placing = getBet(player->roundResult, player->placing, player->placingBase, player->strategy);

    //if he wants to all in, go on
    if(player->placing >= player->money)
      player->placing = player->money;

    //waits for his cards
    pthread_barrier_wait(*(player->barrierRound));

    player->cardsVal = getValueFromHand(player->hand);

    if(player->cardsVal <= player->stopVal)
    {
      player->wantCard = 1;
    }
    else
    {
      player->wantCard = 0;
    }

    //waits for the bank to notice his choice
    pthread_barrier_wait(*(player->barrierRound));

    //waits for the bank to make its choice
    pthread_barrier_wait(*(player->barrierRound));

    // loop until threshold reached
    while(player->wantCard)
    {

        //tell the bank he wants some
        pthread_barrier_wait(*(player->barrierCard));

        //checks if he needs some more
        player->cardsVal = getValueFromHand(player->hand);
        if(player->cardsVal >= player->stopVal)
          player->wantCard = 0;
        else
          player->wantCard = 1;

        //the bank need to update the barrierCard
        //the tmp barrier is here for that

        pthread_barrier_wait(*(player->barrierCardTmp));

        //the bank has updated the barrierCard
        pthread_barrier_wait(*(player->barrierCardTmp));
    }

    //end of the round

    //the bank check for winners
    pthread_barrier_wait(*(player->barrierRound));

    player->cardsVal = getValueFromHand(player->hand);
    writePlayerLog(player);

    //at the end, when it gets his money or not, test if he quits
    if(player->money >= player->objMoney)
      player->isPlaying = 0;

    //the bnk updates the barrierRound
    pthread_barrier_wait(*(player->barrierRoundTmp));
    freeCardHandler(player->hand);
    pthread_barrier_wait(*(player->barrierRoundTmp));
  }
  
  return NULL;
}

uint getBet(char resultLastRound, uint placing, uint placingBase ,gambling_t strategy)
{
  if(resultLastRound & FLAG_RESULT_NONE || strategy & FLAG_GAMBLING_CONST)
    return placingBase;

  if(strategy & FLAG_GAMBLING_MORE)
  {
    if(resultLastRound & FLAG_RESULT_LOSS)
    {
      return placing << 1;
    }
      return placingBase;
  }
  else // less
  {
    if(resultLastRound & FLAG_RESULT_LOSS)
    {
      if(placing << 1 != 0)
        return placing >> 1;
      return 1;
    }
    return placingBase;
  }
}
