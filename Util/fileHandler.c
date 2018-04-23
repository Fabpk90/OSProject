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

      if((valRead = readInt(fd)))
      {
        (*bank)->nbDecks = valRead;

        if((valRead = readInt(fd)))
        {

          (*bank)->nbRounds = valRead;

          //START PARSING PLAYERS
          for(i = 0; i < (*bank)->nbPlayer; i++)
          {
            printf("ii : %d\n", i);
              if((valRead = readInt(fd)))
              {
                printf("ii : %d\n", i);
                (*players)[i].money = valRead;

                if((valRead = readInt(fd)))
                {
                  printf("ii : %d\n", i);
                  (*players)[i].placing = valRead;

                  lseek(fd, -1, SEEK_CUR);
                  read(fd, &valRead, sizeof(char));


                  printf("%c\n", (char)valRead);

                  //if the gambling strat is specified
                  if((char)valRead != ';')
                  {
                    printf("strat %d\n", i);
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

                    read(fd, &valRead, sizeof(char));
                    printf("yo %c \n", valRead);
                  }

                  if((valRead = readInt(fd)))
                  {
                    (*players)[i].stopVal = valRead;

                    if((valRead = readInt(fd)))
                    {
                      (*players)[i].objMoney = valRead;
                    }
                  }

                }

              }
              else
                printf("pal,sdgfksdfknsdgf");
          }
        }
      }
    }
    return ERROR_FILE_READ;
  }
  return ERROR_FILE_OPEN;
}

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
      //printf("buf char: %c\n", buf);
      buf = ((int)buf) - 48;
    //  printf("buf int: %d\n", buf);


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
    for(; i >= 0; i--, charParsed *= 10 )
    {

      parsed += toRead[i] * (charParsed);
      //printf("%d, parsed: %d\n",toRead[i], parsed);
    }
  }

  printf("parsed: %d\n", parsed);

  return parsed;
}
