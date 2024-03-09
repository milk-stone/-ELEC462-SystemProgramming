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

void setMainLayout(void);

int main(){
	setMainLayout();
	
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	int height, width;
	getmaxyx(stdscr, height, width);
	
	if (height < 35 || width < 70){
		endwin();
		printf("To start the game, the terminal height must be at least 35 and its width at least 70.\n");
		printf("Current Terminal Size - height : %d, width : %d\n", height, width);
		return 0;
	}

	int c;	
	
	while (1){
		c = getch();
		// G : Game Start
		if (c == 'Q') break;
		if (c == 'G'){
			pid_t pid = fork();
			if (pid < 0){
				fprintf(stderr, "fork failed\n");
				move(19, 16); addstr("fork failed!! You should EXIT");
			}
			else if (pid == 0){
				char *argv[] = {"./2048", NULL};
				if (execv(argv[0], argv) == -1){
					perror("execv failed");
				}
			}
			else {
				wait(NULL);
				endwin();
				setMainLayout();
				refresh();
			}
		}
	}
	endwin();
	return 0;
}

void setMainLayout(void){
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

	move(7, 15); addstr("-----");
	move(8, 15); addstr("    |");
	move(9, 15); addstr("-----");
	move(10, 15); addstr("|    ");
	move(11, 15); addstr("-----");
	
	move(7, 21); addstr("-----");
	move(8, 21); addstr("|   |");
	move(9, 21); addstr("|   |");
	move(10, 21); addstr("|   |");
	move(11, 21); addstr("-----");
	
	move(7, 27); addstr("|   |");
	move(8, 27); addstr("|   |");
	move(9, 27); addstr("----|");
	move(10, 27); addstr("    | ");
	move(11, 27); addstr("    |");
	
	move(7, 33); addstr("-----");
	move(8, 33); addstr("|   |");
	move(9, 33); addstr("-----");
	move(10, 33); addstr("|   |");
	move(11, 33); addstr("-----");

	
	move(15, 16); addstr("Press G to Game Start");
	move(17, 16); addstr("Press Q to    EXIT   ");

	move(0, 0);
	return;
}
