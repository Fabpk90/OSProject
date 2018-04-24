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
    pthread_barrier_init(*(bank->barrierCard), NULL, playerPlaying + 1);
    pthread_barrier_init(*(bank->barrierCardTmp), NULL, playerPlaying + 1);

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

      //tout le monde recois les cartes ici
      pthread_barrier_wait(bank->barrierCard);
      printf("4\n");

      playerPlaying = getNbPlayersPlay(bank,players);
      pthread_barrier_init((*bank->barrierCard), NULL, playerPlaying + 1);

      //tout le monde a pris leurs decision
      pthread_barrier_wait((*bank->barrierCardTmp));
      printf("5\n");
      pthread_barrier_init((*bank->barrierCard), NULL, playerPlaying + 1);

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
gcc -g3 -Wall Util/fileHandler.c Util/cardHandler.c Actor/bank.c Util/deck.c Actor/player.c main.c -pthread -o BlackJack.paf
Util/fileHandler.c: Dans la fonction « initGame »:
Util/fileHandler.c:90:54: error: « *bank » est un pointeur; vouliez-vous utiliser « -> » ?
                   (*players)[i].barrierCard =  &(bank->barrierCard);
                                                      ^~
                                                      ->
Util/fileHandler.c:91:56: error: « *bank » est un pointeur; vouliez-vous utiliser « -> » ?
                   (*players)[i].barrierCardTmp = &(bank->barrierCardTmp);
                                                        ^~
                                                        ->
Actor/bank.c: Dans la fonction « bankManager »:
Actor/bank.c:58:26: error: type incompatible pour l'argument 1 de « pthread_barrier_init »
     pthread_barrier_init(*(bank->barrierCard), NULL, playerPlaying + 1);
                          ^
In file included from Actor/bank.h:5:0,
                 from Actor/bank.c:6:
/usr/include/pthread.h:1069:12: note: « pthread_barrier_t * restrict {alias union <anonyme> * restrict} » attendu mais l'argument est de type « pthread_barrier_t {alias union <anonyme>} »
 extern int pthread_barrier_init (pthread_barrier_t *__restrict __barrier,
            ^~~~~~~~~~~~~~~~~~~~
Actor/bank.c:59:26: error: type incompatible pour l'argument 1 de « pthread_barrier_init »
     pthread_barrier_init(*(bank->barrierCardTmp), NULL, playerPlaying + 1);
                          ^
In file included from Actor/bank.h:5:0,
                 from Actor/bank.c:6:
/usr/include/pthread.h:1069:12: note: « pthread_barrier_t * restrict {alias union <anonyme> * restrict} » attendu mais l'argument est de type « pthread_barrier_t {alias union <anonyme>} »
 extern int pthread_barrier_init (pthread_barrier_t *__restrict __barrier,
            ^~~~~~~~~~~~~~~~~~~~
Actor/bank.c:92:28: error: type incompatible pour l'argument 1 de « pthread_barrier_init »
       pthread_barrier_init((*bank->barrierCard), NULL, playerPlaying + 1);
                            ^
In file included from Actor/bank.h:5:0,
                 from Actor/bank.c:6:
/usr/include/pthread.h:1069:12: note: « pthread_barrier_t * restrict {alias union <anonyme> * restrict} » attendu mais l'argument est de type « pthread_barrier_t {alias union <anonyme>} »
 extern int pthread_barrier_init (pthread_barrier_t *__restrict __barrier,
            ^~~~~~~~~~~~~~~~~~~~
Actor/bank.c:95:28: error: type incompatible pour l'argument 1 de « pthread_barrier_wait »
       pthread_barrier_wait((*bank->barrierCardTmp));
                            ^
In file included from Actor/bank.h:5:0,
                 from Actor/bank.c:6:
/usr/include/pthread.h:1079:12: note: « pthread_barrier_t * {alias union <anonyme> *} » attendu mais l'argument est de type « pthread_barrier_t {alias union <anonyme>} »
 extern int pthread_barrier_wait (pthread_barrier_t *__barrier)
            ^~~~~~~~~~~~~~~~~~~~
Actor/bank.c:97:28: error: type incompatible pour l'argument 1 de « pthread_barrier_init »
       pthread_barrier_init((*bank->barrierCard), NULL, playerPlaying + 1);
                            ^
In file included from Actor/bank.h:5:0,
                 from Actor/bank.c:6:
/usr/include/pthread.h:1069:12: note: « pthread_barrier_t * restrict {alias union <anonyme> * restrict} » attendu mais l'argument est de type « pthread_barrier_t {alias union <anonyme>} »
 extern int pthread_barrier_init (pthread_barrier_t *__restrict __barrier,
            ^~~~~~~~~~~~~~~~~~~~
Actor/player.c: Dans la fonction « playerManager »:
Actor/player.c:48:32: warning: passage de l'argument 1 de « pthread_barrier_wait » depuis un type pointeur incompatible [-Wincompatible-pointer-types]
           pthread_barrier_wait(player->barrierCard);
                                ^~~~~~
In file included from Actor/player.h:4:0,
                 from Actor/player.c:4:
/usr/include/pthread.h:1079:12: note: « pthread_barrier_t * {alias union <anonyme> *} » attendu mais l'argument est de type « pthread_barrier_t ** {alias union <anonyme> **} »
 extern int pthread_barrier_wait (pthread_barrier_t *__barrier)
            ^~~~~~~~~~~~~~~~~~~~
Actor/player.c:50:32: warning: passage de l'argument 1 de « pthread_barrier_wait » depuis un type pointeur incompatible [-Wincompatible-pointer-types]
           pthread_barrier_wait(player->barrierCardTmp);
                                ^~~~~~
In file included from Actor/player.h:4:0,
                 from Actor/player.c:4:
/usr/include/pthread.h:1079:12: note: « pthread_barrier_t * {alias union <anonyme> *} » attendu mais l'argument est de type « pthread_barrier_t ** {alias union <anonyme> **} »
 extern int pthread_barrier_wait (pthread_barrier_t *__barrier)
            ^~~~~~~~~~~~~~~~~~~~
make: *** [Makefile:9: compile] Error 1

*/
