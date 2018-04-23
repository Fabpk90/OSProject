run: compile
	./BlackJack.paf

compile:
	gcc -g3 -Wall Util/fileHandler.c Util/cardHandler.c Actor/bank.c Util/deck.c Actor/player.c main.c -pthread -o BlackJack.paf
