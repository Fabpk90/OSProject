#ifndef CARDHANDLER_H
#define CARDHANDLER_H

#include "deck.h"

typedef struct cardHandler
{
  cardvalue_t card;
  struct cardHandler * next;
} cardHandler_t;

cardHandler_t *  initCardHandler();
void addCard(cardHandler_t * cardHandler, cardvalue_t card);
void freeCardHandler(cardHandler_t * cardHandler);

#endif
