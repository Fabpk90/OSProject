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

//  printf("Player: %d money: %d, placing: %d, strat: %d, stop: %d, obj: %d\n",player->id, player->money, player->placing
//  , player->strategy, player->stopVal, player->objMoney);

  while(player->isPlaying)
  {
      player->isPlayingRound = 1;
    while(player->isPlayingRound)
    {

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

      //waits for the bank to make its choice
      pthread_barrier_wait(player->barrierRound);

      // loop until threshold reached
      while(player->wantCard == 1)
      {
          //ask card
          player->wantCard = 1;
          printf("barrier player card before\n");
          printf("jattends joueur\n");

          //notice me senpai de la part du joueur
          pthread_barrier_wait(*(player->barrierCard));
          printf("carte en arriv\n");

          //la bank balance les cartes
          pthread_barrier_wait(*(player->barrierCard));
          printf("carte arrivee\n");

          player->cardsVal = getValueFromHand(player->hand);
          if(player->cardsVal >= player->stopVal)
            player->wantCard = 0;
          else
            player->wantCard = 1;

          printf("barrier player card qqqqqq\n");
          //attends l'init de la barrière par la banque
          pthread_barrier_wait(*(player->barrierCardTmp));

          printf("barrier player card aaa %d\n", player->wantCard);

          //attends l'init de la barrière par la banque
          pthread_barrier_wait(*(player->barrierCardTmp));
          printf("paf\n");
      }

      player->wantCard = 0; //not really needed, but still
      player->isPlayingRound = 0;
      printf("j'attends la fin du round\n");
      pthread_barrier_wait(player->barrierRound);
      printf("attente fin du round fini joueur %d\n", player->isPlayingRound);

      writePlayerLog(player);
    }

    //at the end, when it gets his money or not, test if he quits
    if(player->money >= player->objMoney)
      player->isPlaying = 0;
  }

    freeCardHandler(player->hand);

  pthread_exit(0);
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
