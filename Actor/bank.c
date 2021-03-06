#include <stdlib.h>

#include "../Util/deck.h"

#include "bank.h"

void bankManager(bank_t * bank, pthread_t * threads, player_t * players)
{
  uint i = 0;
  deck_t * decks = NULL;
  int playerWantCard = 0;
  bool isBlackJacking = 0;
  initDeckLib();

  //init and shuffling decks
  decks = initDeck(P52, bank->nbDecks);
  shuffleDeck(decks);

  while(bank->nbRounds!=0 )
  {
    isBlackJacking = 0;
    playerWantCard = 0;

    bank->hand = initCardHandler();

    //gives cards to all players
    for(i = 0; i < bank->nbPlayer; i++)
    {
      if(players[i].isPlaying)
      {
        players[i].hand = initCardHandler();
        addCard(players[i].hand, getValueFromCardID(drawCard(decks)));
        addCard(players[i].hand, getValueFromCardID(drawCard(decks)));

        players[i].bankHand = &bank->hand;
      }

    }

    addCard(bank->hand, getValueFromCardID(drawCard(decks)));
    addCard(bank->hand, getValueFromCardID(drawCard(decks)));

    //the barrier used for wainting the cards
    pthread_barrier_wait(bank->barrierRound);

    //barrier used for waiting players' choice
    pthread_barrier_wait(bank->barrierRound);

    //cas où BlackJack
    if(getValueFromHand(bank->hand)==21)
    {
      isBlackJacking = 1;
    }
    for(i = 0; i < bank->nbPlayer && !isBlackJacking; i++)
    {
      if( players[i].isPlaying && getValueFromHand(players[i].hand)==21)
      {
        isBlackJacking = 1;
      }
    }

    if(isBlackJacking)
    {
      checkForWinners(bank, players, 1);
    }

    playerWantCard = getNbPlayerWantCard(bank->nbPlayer, players);

    //if players need to play
    if(playerWantCard > 0)
    {
      //pthread_barrier_destroy(bank->barrierCard);
      //includes the bank in the waiters
      pthread_barrier_init(bank->barrierCard, NULL, playerWantCard + 1);
      pthread_barrier_init(bank->barrierCardTmp, NULL, playerWantCard + 1);
    }

    //barrier to notify the bank's decision
    pthread_barrier_wait(bank->barrierRound);

    while(playerWantCard > 0)
    {
      for(i = 0; i < bank->nbPlayer; i++)
      {
        if(players[i].wantCard==1)
        {
          if(!checkAvaibleCards(decks))
          {
            removeDeck(decks);
            //init and shuffling decks
            decks = initDeck(P52, bank->nbDecks);
            shuffleDeck(decks);
          }
          addCard(players[i].hand, getValueFromCardID(drawCard(decks)));

        }
      }
      //all the players wanting a card have it now
      pthread_barrier_wait(bank->barrierCard);

      //the players make a choice (wether they want another card)
      pthread_barrier_wait(bank->barrierCardTmp);

      playerWantCard = getNbPlayerWantCard(bank->nbPlayer,players);

      //Updates the card barrier
      //using a tmp barrier for synchro

      if(playerWantCard > 0)
      {
        pthread_barrier_destroy(bank->barrierCard);
        pthread_barrier_init(bank->barrierCard, NULL, playerWantCard + 1);

        pthread_barrier_wait(bank->barrierCardTmp);

        pthread_barrier_destroy(bank->barrierCardTmp);
        pthread_barrier_init(bank->barrierCardTmp, NULL, playerWantCard + 1);
      }
      else
      {
        while(getValueFromHand(bank->hand) < 17)
        {
          if(!checkAvaibleCards(decks))
          {
            removeDeck(decks);
            //init and shuffling decks
            decks = initDeck(P52, bank->nbDecks);
            shuffleDeck(decks);
          }
          addCard(bank->hand, getValueFromCardID(drawCard(decks)));

        }

        //nobody wants cards, so we init nothing
        pthread_barrier_wait(bank->barrierCardTmp);
      }
    }

    checkForWinners(bank, players, 0);
    if(bank->nbRounds - 1  == 0) // end of the game
      {
        //tell the player to stop playing
        for(i = 0; i < bank->nbPlayer; i++)
        {
          players[i].isPlaying = 0;
        }
      }

    pthread_barrier_wait(bank->barrierRound);

    pthread_barrier_wait(bank->barrierRoundTmp);

    pthread_barrier_destroy(bank->barrierRound);
    pthread_barrier_init(bank->barrierRound, NULL, getNbPlayersPlay(bank->nbPlayer, players) + 1);

    pthread_barrier_wait(bank->barrierRoundTmp);

    pthread_barrier_destroy(bank->barrierRoundTmp);
    pthread_barrier_init(bank->barrierRoundTmp,NULL, getNbPlayersPlay(bank->nbPlayer, players) + 1);

    freeCardHandler(bank->hand);

    bank->nbRounds --;
  }

  removeDeck(decks);
}

bool checkAvaibleCards(deck_t * deck)
{
  if(getDrawPileSize(deck)!=0)
    return 1;

  return 0;
}

int getNbPlayersPlay(uint nb, player_t * players)
{
  int i, playing = 0;
  for(i=0;i<nb;i++)
  {
    if(players[i].isPlaying==1) playing++;
  }

  return playing;
}

int getNbPlayerWantCard(uint nb, player_t * players)
{
  int i = 0, wanna = 0;

  for(i = 0; i < nb; i++)
  {
    wanna += players[i].wantCard;
  }

  return wanna;
}

void checkForWinners(bank_t * bank, player_t * players, bool firstDraw)
{
  int i, playerMax = 0, playerMaxIndex = 0;
  uint valHand = 0, valHandBank = 0;

  int * checked = calloc(bank->nbPlayer, sizeof(int));

  //finds the highest score
  //and handles hands > 21
  for(i=0;i<bank->nbPlayer;i++)
  {
    if(players[i].isPlaying)
    {
      valHand = getValueFromHand(players[i].hand);
      if(valHand > playerMax && valHand <= 21)
      {
        playerMax = valHand;
        playerMaxIndex = i;
      }
      else if(valHand > 21)
      {
        if(players[i].money - players[i].placing >= 0)
        {
          players[i].money -= players[i].placing;
          players[i].roundResult = FLAG_RESULT_LOSS;
        }
        else
        {
          players[i].money = 0;
          players[i].isPlaying = 0;
        }

        players[i].roundResult = FLAG_RESULT_LOSS;
        checked[i] = 1;
      }
    }

  }

  valHandBank = getValueFromHand(bank->hand);

  if(valHand > valHandBank || valHandBank > 21)
  {
    if(firstDraw)
    {
      players[playerMaxIndex].money += players[playerMaxIndex].placing * 3;
      players[playerMaxIndex].moneyWon = players[playerMaxIndex].placing * 3;
    }
    else
    {
        players[playerMaxIndex].money += players[playerMaxIndex].placing;
        players[playerMaxIndex].moneyWon = players[playerMaxIndex].placing;
    }

    players[playerMaxIndex].roundResult = FLAG_RESULT_WON;
  }
  else if(valHand == valHandBank)
  {
    players[playerMaxIndex].roundResult = FLAG_RESULT_NONE;
  }

  checked[playerMaxIndex] = 1;

  for(i=0;i<bank->nbPlayer;i++)
  {
    //if not checked yet
    if(!(checked[i] & 1) && players[i].isPlaying)
    {
      valHand = getValueFromHand(players[i].hand);
      if(valHand > valHandBank || valHandBank > 21)
      {
        players[i].money += players[i].placing;
        players[i].moneyWon = players[i].placing;
        players[i].roundResult = FLAG_RESULT_WON;
      }
      else if(valHand == valHandBank)
      {
        players[i].roundResult = FLAG_RESULT_NONE;
      }
      else
      {
        if(players[i].money - players[i].placing >= 0)
        {
          players[i].money -= players[i].placing;
          players[i].roundResult = FLAG_RESULT_LOSS;
        }
        else
        {
          players[i].money = 0;
          players[i].isPlaying = 0;
        }
      }
    }
  }

  free(checked);
}
