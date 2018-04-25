#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

#include "fileHandler.h"
#include "cardHandler.h"


int initGame(const char * path, bank_t ** bank, player_t ** players)
{
  int fd = 0, i = 0, valRead = 0;

  fd = open(path, O_RDONLY);

  if(fd != -1)
  {
    (*bank) = malloc(sizeof(bank_t));
    if((valRead = readInt(fd)))
    {
      //on limite le nombre de joueurs ?
      //TODO: insert here the test for max players
      (*bank)->nbPlayer = valRead;
      (*players) = malloc(sizeof(player_t) * valRead);

      (*bank)->barrierCard = malloc(sizeof(pthread_barrier_t));
      (*bank)->barrierCardTmp = malloc(sizeof(pthread_barrier_t));
      (*bank)->barrierRound = malloc(sizeof(pthread_barrier_t));
      (*bank)->barrierRoundTmp = malloc(sizeof(pthread_barrier_t));

      pthread_barrier_init((*bank)->barrierRound, NULL, (*bank)->nbPlayer + 1);
      pthread_barrier_init((*bank)->barrierRoundTmp, NULL, (*bank)->nbPlayer + 1);
      pthread_barrier_init((*bank)->barrierCardTmp, NULL, (*bank)->nbPlayer + 1);
      pthread_barrier_init((*bank)->barrierCard, NULL, (*bank)->nbPlayer + 1);

      if((valRead = readInt(fd)))
      {
        (*bank)->nbDecks = valRead;
        if((valRead = readInt(fd)))
        {
          (*bank)->nbRounds = valRead;
          //START PARSING PLAYERS
          for(i = 0; i < (*bank)->nbPlayer; i++)
          {
            (*players)[i].hand = NULL;
              (*players)[i].id = i;
              if((valRead = readInt(fd)))
              {
                (*players)[i].money = valRead;

                if((valRead = readInt(fd)))
                {
                  (*players)[i].placingBase = valRead;

                  lseek(fd, -1, SEEK_CUR);
                  read(fd, &valRead, sizeof(char));

                  //sets the default strat
                  (*players)[i].strategy = FLAG_GAMBLING_CONST;

                  //if the gambling strat is specified
                  if((char)valRead != ';')
                  {

                    switch (valRead)
                    {
                      case '+':
                        (*players)[i].strategy = FLAG_GAMBLING_MORE;
                      break;

                      case '-':
                        (*players)[i].strategy = FLAG_GAMBLING_LESS;
                        break;
                    }
                    //read the ';'
                    read(fd, &valRead, sizeof(char));
                  }

                  if((valRead = readInt(fd)))
                  {
                    (*players)[i].stopVal = valRead;

                    if((valRead = readInt(fd)))
                    {
                      (*players)[i].objMoney = valRead;
                    }
                  }

                //  (*players)[i].barrierEndGame = (*bank)->barrierEndGame;
                  (*players)[i].barrierCard =  &((*bank)->barrierCard);
                  (*players)[i].barrierCardTmp = &((*bank)->barrierCardTmp);
                  (*players)[i].barrierRound = &((*bank)->barrierRound);
                  (*players)[i].barrierRoundTmp = &((*bank)->barrierRoundTmp);

                  (*players)[i].moneyWon = 0;
                }
              }
          }
        }
      }
    }
    return ERROR_FILE_READ;
  }
  return ERROR_FILE_OPEN;
}

// #cartes;totalJoueur;banque;totalBanque;mise;gain;nbJetons
//TODO: handle blackjack case
//enhancements: path stored in the player, batch the log
int writePlayerLog(player_t * player)
{
  char path[12] = "player";
  int fd = -1;
  char separator = CONST_SEPARATOR;

  path[6] = player->id + 48;
  path[7] = '.';
  path[8] = 'd';
  path[9] = 'a';
  path[10] = 't';
  path[11] = '\0';

  fd = open(path, O_CREAT| O_RDWR | O_APPEND, 0666);

  if(fd != -1)
  {
    writeCardsName(fd, player->hand);
    write(fd, &separator, sizeof(char));

    printInt(fd, player->cardsVal);
    write(fd, &separator, sizeof(char));

    writeCardsName(fd, (*player->bankHand));
    write(fd, &separator, sizeof(char));

    printInt(fd, getValueFromHand((*player->bankHand)));
    write(fd, &separator, sizeof(char));

    printInt(fd, player->placing);
    write(fd, &separator, sizeof(char));

    if(player->roundResult & FLAG_RESULT_WON)
    {
      printInt(fd, player->moneyWon);
      player->moneyWon = 0;
    }
    else
      write(fd, "0", sizeof(char));

    write(fd, &separator, sizeof(char));

    printInt(fd, player->money);

    write(fd, "\n", 1);

    close(fd);

    return 0;
  }
  else
    return ERROR_FILE_OPEN;
}

//prints all the cards in the specified fd
void writeCardsName(int fd, cardHandler_t * cards)
{
  cardHandler_t * index = cards;
  char cardName;
  //prints the cards' name of the player
  while(index != NULL)
  {
    if(index->cards[0] != -1)
    {
      cardName = getCardName(index->cards[0]);
      write(fd, &cardName, sizeof(char));
    }

    if(index->cards[1] != -1)
    {
      cardName = getCardName(index->cards[1]);
      write(fd, &cardName, sizeof(char));
    }

    index = index->next;
  }
}

int printInt(int fd, int val)
{
  char toWrite[32];
  int index = 0, divisor = 1;
  int i = 0, zeros = 0;

  if(val == 0)
  {
    write(fd, "0", 1);
    return 0;
  }

  while(val > 9 && index < 31)
  {
    while((val / divisor) > 9) //look for the highest base 10 pow
    {
      divisor *= 10;
    }

    //takes out the Right Most power of 10
    toWrite[index] = (val / divisor) + 48;
    index++;
    val -= (val / divisor) * divisor;

    //if the val doesn't contain zero, we proceed, if it does,
    //we keep the divisor to know how many zeros to add
    if(val != 0)
      divisor = 1;

  }

  if(val == 0)
  {
    //caluclates the zeros to add
    while(divisor / 10 > 1)
    {
      zeros++;
      divisor /= 10;
    }
    zeros++;

    //writes all the remaning zeros
      for(i = 0; i < zeros; i++, index++)
      {
        toWrite[index] = 48;
      }

    toWrite[index+1] = '\0';
    write(fd, toWrite, index);
  }
  else //writes a 'normal' number, without zero in it
  {

    toWrite[index] = val + 48;
    toWrite[index+1] = '\0';

    write(fd, toWrite, index+1);
  }

  return 0;
}

//reads an int until it finds something else
int readInt(int fd)
{
  char toRead[16];
  int i = -1, charParsed = 1;
  bool isRead = 0;
  char buf;
  int parsed = 0;

  while(!isRead)
  {
    if(read(fd, &buf, sizeof(char)))
    {
      //ascii transform
      buf = ((int)buf) - 48;

      if(buf >= 0 && buf <= 9)
      {
        i++;
        toRead[i] = buf;
      }
      else
        isRead = 1;
    }
    else
      isRead = 1;
  }

  if(i != -1)
  {
    //from an array to the actual number
    for(; i >= 0; i--, charParsed *= 10 )
    {
      parsed += toRead[i] * (charParsed);
    }
  }
  return parsed;
}
