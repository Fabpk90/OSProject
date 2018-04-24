#ifndef CARDHANDLER_H
#define CARDHANDLER_H

#include "deck.h"

typedef struct cardHandler
{
  cardvalue_t cards[2];
  struct cardHandler * next;
} cardHandler_t;

cardHandler_t *  initCardHandler();
void addCard(cardHandler_t * cardHandler, cardvalue_t card);
void freeCardHandler(cardHandler_t * cardHandler);

char getCardName(cardvalue_t card);

uint getValueFromHand(cardHandler_t * cards);

#endif
