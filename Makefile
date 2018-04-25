clean:
	rm -f player*.dat
	make run

run: compile
	./BlackJack.paf config.dat

compile:
	gcc -g3 -Wall Util/fileHandler.c Util/cardHandler.c Actor/bank.c Util/deck.c Actor/player.c main.c -pthread -o BlackJack.paf
