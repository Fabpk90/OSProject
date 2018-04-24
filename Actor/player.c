#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "../Util/cardHandler.h"
#include "../Util/fileHandler.h"

void * playerManager(void * playerStruct)
{
  player_t *player = (player_t*) playerStruct;
  char resultLastRound = FLAG_RESULT_NONE;

  player->isPlaying = 1;
  player->isPlayingRound = 1;

  while(player->isPlaying)
  {
    while(player->isPlayingRound)
    {
      printf("Player: %d money: %d, placing: %d, strat: %d, stop: %d, obj: %d\n",player->id, player->money, player->placing
      , player->strategy, player->stopVal, player->objMoney);

      player->placing = getBet(resultLastRound, player->placing, player->strategy);

      //waits for his cards
      pthread_barrier_wait(player->barrierRound);

      player->cardsVal = getValueFromHand(player->hand);

      if(player->cardsVal <= player->stopVal)
      {
        player->wantCard = 1;
      }
      else
        player->wantCard = 0;

      //waits for the bank to notice his choice
      pthread_barrier_wait(player->barrierRound);

      //waits for the bank to make his choice
      pthread_barrier_wait(player->barrierRound);

      // loop until threshold reached
      while(player->wantCard && player->isPlayingRound)
      {
          //ask card
          player->wantCard = 1;
          printf("barrier player card before\n");
          printf("jattends\n");
          //notice me senpai de la part du joueur
          pthread_barrier_wait(*(player->barrierCard));

          player->cardsVal = getValueFromHand(player->hand);
          if(player->cardsVal >= player->stopVal)
            player->wantCard = 0;
          else
            player->wantCard = 1;

          printf("barrier player card \n");
          //attends l'init de la barrière par la banque
          pthread_barrier_wait(*(player->barrierCardTmp));
          player->cardsVal = getValueFromHand(player->hand); // to optimize, add up directly the val of the card given
      }

      player->wantCard = 0; //not really needed, but still
      player->isPlayingRound = 0;
      pthread_barrier_wait(player->barrierRound);

      writePlayerLog(player);
      freeCardHandler(player->hand);
    }

    //at the end, when it gets his money or not, test if he quits
    if(player->money >= player->objMoney)
      player->isPlaying = 0;
  }

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
