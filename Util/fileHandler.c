#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fileHandler.h"


int initGame(const char * path, bank_t * bank, player_t * players)
{
  int fd = 0, i = 0, valRead = 0;

  fd = open(path, O_RDONLY);

  if(fd != -1)
  {
    if(read(fd, &valRead, sizeof(char)) == sizeof(char))
    {

      //on limite le nombre de joueurs ?
      //TODO: insert here the test for max players
      bank->nbPlayer = valRead;
      players = malloc(sizeof(player_t) * bank->nbPlayer);

      //black magic, reads the ;
      read(fd, NULL, sizeof(char));

      if(read(fd, &valRead, sizeof(int)))
      {
        bank->nbDecks = valRead;

        read(fd, NULL, sizeof(char));
        if(read(fd, &valRead, sizeof(int)))
        {
          bank->nbRounds = valRead;

          //read the /n this time
          read(fd, NULL, sizeof(char));

          //START PARSING PLAYERS
          for(i = 0; i < bank->nbPlayer; i++)
          {
              if(read(fd, &valRead, sizeof(int)))
              {
                players[i].money = valRead;
                read(fd, NULL, sizeof(char));

                if(read(fd, &valRead, sizeof(int)))
                {
                  players[i].placing = valRead;

                  read(fd, &valRead, sizeof(char));

                  //if no gambling strat are specified, go on with the parse
                  if((char)valRead == ';')
                  {
                    if(read(fd, &valRead, sizeof(int)))
                    {
                      players[i].stopVal = valRead;

                      read(fd, NULL, sizeof(char));

                      if(read(fd, &valRead, sizeof(int)))
                      {
                        players[i].objMoney = valRead;
                      }
                    }
                  }
                  else // the gambling strat is specified
                  {

                  }
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
