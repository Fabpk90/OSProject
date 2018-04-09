#include <stdlib.h>
#include <stdio.h>

#include "cardHandler.h"


cardHandler_t *  initCardHandler()
{
 cardHandler_t *  cardHandler =  malloc(sizeof(cardHandler_t));

  cardHandler->card = -1;
  cardHandler->next = NULL;

  return cardHandler;
}

void addCard(cardHandler_t * cardHandler, cardvalue_t card)
{
  cardHandler_t * index = cardHandler;

  cardHandler_t * addingCard = malloc(sizeof(cardHandler_t));
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
  if(cardHandler != NULL)
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

}
