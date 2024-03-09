all : mainlayout 2048 gameover

mainlayout : mainlayout.c
	gcc -o mainlayout mainlayout.c -lcurses

2048 : 2048.c
	gcc -o 2048 2048.c -lcurses

gameover : gameover.c
	gcc -o gameover gameover.c -lcurses

clean :
	rm -f mainlayout 2048 gameover
