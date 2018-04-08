run: compile
	./BlackJack.paf

compile:
	gcc -Wall Util/cardHandler.c Actor/bank.c Util/deck.c Actor/player.c main.c -pthread -o BlackJack.paf
