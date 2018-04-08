#include <stdlib.h>

#include "cardHandler.h"


void initCardHandler(cardHandler_t * cardHandler)
{
  cardHandler = (cardHandler_t*) malloc(sizeof(cardHandler_t));

  cardHandler->card = -1;
  cardHandler->next = NULL;
}

void addCard(cardHandler_t * cardHandler, cardvalue_t card)
{
  cardHandler_t * index = cardHandler;

  cardHandler_t * addingCard = (cardHandler_t*) malloc(sizeof(cardHandler_t));
  addingCard->next = NULL;
  addingCard->card = card;

  while(index->next != NULL)
  {
    index = index->next;
  }

  index->next = addingCard;
}
void freeCardHandler(cardHandler_t * cardHandler)
{
  cardHandler_t * index = cardHandler;
  cardHandler_t * next = cardHandler->next;

  while(next != NULL)
  {
    free(index);
    index = next;
    next = next->next;
  }

  free(index);
}
