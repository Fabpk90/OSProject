compile:
	gcc -Wall Util/deck.c Actor/player.c main.c -pthread -o BlackJack.paf

run: compile
	./BlackJack.paf
