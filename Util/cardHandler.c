#include <stdlib.h>
#include <stdio.h>

#include "cardHandler.h"
#include "consts.h"


cardHandler_t *  initCardHandler()
{
 cardHandler_t *  cardHandler =  malloc(sizeof(cardHandler_t));

  cardHandler->cards[0] = -1;
  cardHandler->cards[1] = -1;
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
    if(index->cards[0] == -1)
    {
      added = 1;
      index->cards[0] = card;
    }
    else if(index->cards[1] == -1)
    {
      added = 1;
      index->cards[1] = card;
    }
    else if(index->next == NULL)
    {
      addCardStruct = malloc(sizeof(cardHandler_t));

      addCardStruct->next = NULL;
      addCardStruct->cards[0] = card;
      addCardStruct->cards[1] = -1;

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

char getCardName(cardvalue_t card)
{
  switch (card)
  {
    case VA:
      return 'A';
    break;
    case VX:
      return 'X';
    break;
  	case VJ:
      return 'J';
    break;						//< Valet
  	case VQ:
      return 'Q';
    break;						//< Dame
  	case VK:
      return 'K';
    break;

    default:
      return ((int) card) + 48;
  }
}

uint getValueFromHand(cardHandler_t * cards)
{
  uint val = 0;
  cardHandler_t * index = cards;

  while(index != NULL)
  {
    if(index->cards[0] != -1)
      val += index->cards[0];
    if(index->cards[1] != -1)
      val += index->cards[1];

    index = index->next;
  }

  printf("valcards %d\n", val);

  return val;
}
