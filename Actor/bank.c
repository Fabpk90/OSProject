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
      players[i].hand = initCardHandler();
      addCard(players[i].hand, getValueFromCardID(drawCard(decks)));
      addCard(players[i].hand, getValueFromCardID(drawCard(decks)));

      players[i].bankHand = &bank->hand;
    }


    addCard(bank->hand, getValueFromCardID(drawCard(decks)));
    addCard(bank->hand, getValueFromCardID(drawCard(decks)));

    //barrier utilisée pour les cartes, tout le monde attend
    pthread_barrier_wait(bank->barrierRound);
    printf("1\n");
    //on attend que les joueurs prennent leurs décision, jouer ou pas
    pthread_barrier_wait(bank->barrierRound);
    printf("2\n");

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
    pthread_barrier_init(bank->barrierCard, NULL, playerPlaying + 1);
    pthread_barrier_init(bank->barrierCardTmp, NULL, playerPlaying + 1);

    //la banque a pris sa décision, donc on continue
    pthread_barrier_wait(bank->barrierRound);
    printf("3\n");
    while(playerPlaying > 0)
    {
      for(i = 0; i < bank->nbPlayer; i++)
      {
        if(players[i].wantCard==1)
        {
          if(getDrawPileSize(decks)!=0)
          {
            addCard(players[i].hand, getValueFromCardID(drawCard(decks)));
            players->wantCard = 0;  // this is done , for synchronisation purpose
            printf("card\n");
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

      printf("%d doivent attendre\n", playerPlaying + 1);

      printf("jattends\n");

      //tout le monde recois des cartes ici
      pthread_barrier_wait(bank->barrierCard);
      printf("4\n");

      playerPlaying = getNbPlayersPlay(bank,players);
      pthread_barrier_init(bank->barrierCard, NULL, playerPlaying + 1);

      //tout le monde a pris leurs decision
      pthread_barrier_wait(bank->barrierCardTmp);
      printf("5\n");
      pthread_barrier_init(bank->barrierCard, NULL, playerPlaying + 1);

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

    pthread_barrier_wait(bank->barrierRound);

    bank->nbRounds --;
  }

  //pthread_wait(finGame);

  removeDeck(decks);
}

int getNbPlayersPlay(bank_t * bank, player_t * players)
{
  int i, playing = 0;
  for(i=0;i<bank->nbPlayer;i++)
  {
    if(players[i].isPlayingRound==1) playing++;
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
1
2
barrier player card before
jattends
barrier player card before
jattends
3
barrier player card before
jattends
card
card
card
4 doivent attendre
jattends
4
barrier player card
barrier player card
barrier player card
5
barrier player card before
jattends
card
4 doivent attendre
jattends


*/
