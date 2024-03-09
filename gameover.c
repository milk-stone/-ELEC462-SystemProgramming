#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char my_Score[200];

void setGameoverLayout(void);

int main(int argc, char *argv[]){
	printf("%d %s %s\n", argc, argv[0], argv[1]);
	strcpy(my_Score, "Score : ");
	strcat(my_Score, argv[1]);
	setGameoverLayout();
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	
	int c;	
	
	while (1){
		c = getch();
		if (c == 'Q') break;
	}
	endwin();
	return 0;
}

void setGameoverLayout(void){
	initscr();
	crmode();
	noecho();
	clear();
	for (int i = 1; i < 24; i++){
		move(i, 0); addstr("|");
		move(i, 52); addstr("|");
	}
	for (int i = 1; i < 52; i++){
		move(0, i); addstr("-");
		move(24, i); addstr("-");
	}

	move(9, 22); addstr("GAME OVER");
	
	move(12, 21); addstr(my_Score);

	move(17, 16); addstr("Press Q to  Main Page");

	move(0, 0);
	return;
}
