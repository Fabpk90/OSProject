#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

#include "fileHandler.h"


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

      (*bank)->barrierRound = malloc(sizeof(pthread_t));
      (*bank)->barrierCard = malloc(sizeof(pthread_t));

      pthread_barrier_init((*bank)->barrierRound, NULL, (*bank)->nbPlayer + 1);
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
              if((valRead = readInt(fd)))
              {
                (*players)[i].money = valRead;

                if((valRead = readInt(fd)))
                {
                  (*players)[i].placing = valRead;

                  lseek(fd, -1, SEEK_CUR);
                  read(fd, &valRead, sizeof(char));

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

                      default:
                        (*players)[i].strategy = FLAG_GAMBLING_CONST;
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

                  (*players)[i].barrierRound = (*bank)->barrierRound;
                  (*players)[i].barrierCard = (*bank)->barrierCard;

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

int writePlayerLog(player_t * player)
{
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
