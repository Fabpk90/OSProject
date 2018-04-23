#ifndef FILEHANDLER_H
#define FILEHANDLER_H


#include "consts.h"
#include "../Actor/bank.h"
#include "../Actor/player.h"

#define ERROR_FILE_OPEN 1
#define ERROR_FILE_READ 2


int initGame(const char * path, bank_t ** bank, player_t ** players);

int readInt(int fd);

int writePlayerLog(player_t * player);

#endif
