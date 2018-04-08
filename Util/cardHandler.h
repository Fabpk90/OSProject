#ifndef CARDHANDLER_H
#define CARDHANDLER_H

#include "deck.h"

typedef struct cardHandler
{
  cardvalue_t card;
  struct cardHandler * next;
} cardHandler_t;

void initCardHandler(cardHandler_t * cardHandler);
void addCard(cardHandler_t * cardHandler, cardvalue_t card);
void freeCardHandler(cardHandler_t * cardHandler);

#endif
