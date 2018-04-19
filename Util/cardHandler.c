#include <stdlib.h>
#include <stdio.h>

#include "cardHandler.h"
#include "consts.h"


cardHandler_t *  initCardHandler()
{
 cardHandler_t *  cardHandler =  malloc(sizeof(cardHandler_t));

  cardHandler->card[0] = -1;
  cardHandler->card[1] = -1;
  cardHandler->next = NULL;

  return cardHandler;
}

void addCard(cardHandler_t * cardHandler, cardvalue_t card)
{
  cardHandler_t * index = cardHandler;
  cardHandler_t * addCardStruct = NULL;
  bool added = 0;

  while(!added)
  {
    if(index->card[0] == -1)
    {
      added = 1;
      index->card[0] = card;
    }
    else if(index->card[1] == -1)
    {
      added = 1;
      index->card[1] = card;
    }
    else if(index->next == NULL)
    {
      addCardStruct = malloc(sizeof(cardHandler_t));

      addCardStruct->next = NULL;
      addCardStruct->card[0] = card;
      addCardStruct->card[1] = -1;

      index->next = addCardStruct;
      added = 1;
    }
    else
      index = index->next;
  }
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
