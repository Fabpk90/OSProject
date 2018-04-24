#include <stdlib.h>
#include <stdio.h>

#include "../Util/deck.h"

#include "bank.h"

void bankManager(bank_t * bank, pthread_t * threads, player_t * players)
{
  uint i = 0;
  deck_t * decks = NULL;
  int playerPlaying = 0;
  initDeckLib();

  //init and shuffling decks
  decks = initDeck(P52, bank->nbDecks);
  shuffleDeck(decks);

  while(bank->nbRounds!=0 )
  {
    playerPlaying = 0;

    bank->hand = initCardHandler();

    //gives cards to all players
    for(i = 0; i < bank->nbPlayer; i++)
    {
      if(players[i].hand != NULL)
        freeCardHandler(players[i].hand);

      players[i].hand = initCardHandler();
      addCard(players[i].hand, getValueFromCardID(drawCard(decks)));
      addCard(players[i].hand, getValueFromCardID(drawCard(decks)));

      players[i].bankHand = &bank->hand;
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
      printf("Bank do a BlackJack\n");
      //blackjack=1; créer une variable bool
    }
    for(i = 0; i < bank->nbPlayer; i++)
    {
      if(players[i].wantCard==0 && getValueFromHand(players[i].hand)==21) printf("Player do a BlackJack\n");
      //if(balckjack==1) Doublemise ou triple;
    }

    playerPlaying = getNbPlayersPlay(bank, players);
    //if players need to play
    if(playerPlaying > 0)
    {
      //pthread_barrier_destroy(bank->barrierCard);
      //includes the bank in the waiters
      pthread_barrier_init(bank->barrierCard, NULL, playerPlaying + 1);
      pthread_barrier_init(bank->barrierCardTmp, NULL, playerPlaying + 1);
    }

    //barrier to notify the bank's decision
    pthread_barrier_wait(bank->barrierRound);

    while(playerPlaying > 0)
    {
      //wait for the players who wants their cards
      pthread_barrier_wait(bank->barrierCard);
      for(i = 0; i < bank->nbPlayer; i++)
      {
        if(players[i].wantCard==1)
        {
          if(getDrawPileSize(decks)!=0)
          {
            addCard(players[i].hand, getValueFromCardID(drawCard(decks)));
            players->wantCard = 0;  // this is done , for synchronisation purpose
          }
          else
          {
            removeDeck(decks);
            //init and shuffling decks
            decks = initDeck(P52, bank->nbDecks);
            shuffleDeck(decks);
          }

        }
      }
      //all the players wanting a card have it now
      pthread_barrier_wait(bank->barrierCard);

      //the players make a choice (wether they want another card)
      pthread_barrier_wait(bank->barrierCardTmp);

      playerPlaying = getNbPlayersPlay(bank,players);

      //Updates the card barrier
      //using a tmp barrier for synchro

      if(playerPlaying > 0)
      {
        pthread_barrier_destroy(bank->barrierCard);
        pthread_barrier_init(bank->barrierCard, NULL, playerPlaying + 1);

        pthread_barrier_wait(bank->barrierCardTmp);
        pthread_barrier_destroy(bank->barrierCardTmp);
        pthread_barrier_init(bank->barrierCardTmp, NULL, playerPlaying + 1);
      }
      else //if no player wants to play, reset the barrier to default
      {
        pthread_barrier_destroy(bank->barrierCard);
        pthread_barrier_init(bank->barrierCard, NULL, bank->nbPlayer + 1);

        //tout le monde a pris leurs decision
        pthread_barrier_wait(bank->barrierCardTmp);

        pthread_barrier_destroy(bank->barrierCardTmp);
        pthread_barrier_init(bank->barrierCardTmp, NULL, bank->nbPlayer + 1);
      }

      //whoWin(bank,players);
      /*for(i = 0; i < bank->nbPlayer; i++)
      {
        if(getValueFromHand(players[i].hand)==21 || getValueFromHand(bank->hand)==21)
        {
          printf("Win");
          bank->nbRounds=0;
        }
      }*/
    }

    if(bank->nbRounds - 1  == 0) // end of the game
      {
        //tell the player to stop playing
        for(i = 0; i < bank->nbPlayer; i++)
        {
          players[i].isPlayingRound = 0;
          players[i].isPlaying = 0;
        }
      }
    pthread_barrier_wait(bank->barrierRound);

    bank->nbRounds --;
  }



printf("bye bye\n");

  removeDeck(decks);
}

int getNbPlayersPlay(bank_t * bank, player_t * players)
{
  int i, playing = 0;
  for(i=0;i<bank->nbPlayer;i++)
  {
    if(players[i].wantCard==1) playing++;
  }

  return playing;
}

void whoWin(bank_t * bank, player_t * players)
{
  int i;

  for(i=0;i<bank->nbPlayer;i++)
  {
    if(getValueFromHand(players[i].hand)==21)
    {
      printf("Player win\n");
      //gérer mise
    }
  }
  for(i=0;i<bank->nbPlayer;i++)
  {
    players[i].isPlaying=0;
  }

  if(getValueFromHand(bank->hand)==21)
  {
    printf("Bank win\n");
  }
}
/*

*/
