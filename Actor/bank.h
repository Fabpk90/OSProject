#ifndef BANK_H
#define BANK_H

#include "../Util/consts.h"
#include <pthread.h>
#include "player.h"

typedef struct Bank
{
  uint nbPlayer;
  uint nbDecks;
  uint nbRounds;
  //pthread_barrier_t * barrierEndGame;
  pthread_barrier_t * barrierCard;
  pthread_barrier_t * barrierCardTmp;
  pthread_barrier_t * barrierRound;
  pthread_barrier_t * barrierRoundTmp;
  cardHandler_t * hand;
} bank_t;

void bankManager(bank_t * bank, pthread_t * threads, player_t * players);

int getNbPlayersPlay(uint nb, player_t * players);
int getNbPlayerWantCard(uint nb, player_t * players);

bool checkAvaibleCards(deck_t * deck);

void checkForWinners(bank_t * bank, player_t * players, bool firstDraw);

#endif
