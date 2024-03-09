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
#include <stdlib.h>
#include <time.h>

int board[4][4];
short int check[4];
int my_score;

int up(void);
int down(void);
int left(void);
int right(void);
int generate(int init);
int moved(int a[4][4]);
void initializeCheck(void);
void defaultPage(void);

int main(){
	int result = 0;
	int skipGenerate;
	int c;	
	
	if (generate(1) == -1){
		fprintf(stderr, "generate Error\n");
	}
	defaultPage();

	while (1){
		c = getch();
		skipGenerate = 0;
		if (c =='Q') break;
		
		else if (c == 'w') { 
			result = up(); 
			if (result != -1){// up done
				my_score += result;
				result = 0;
				initializeCheck();
			}
			else {
				check[0] = 1;
				skipGenerate = 1;
			}
		}
		else if (c == 'a') {
			result = left();
			if (result != -1){
				my_score += result;
				result = 0;
				initializeCheck();
			}
			else {
				check[1] = 1;
				skipGenerate = 1;
			}
		}
		else if (c == 's') {
			result = down();
			if (result != -1){
				my_score += result;
				result = 0;
				initializeCheck();
			}
			else {
				check[2] = 1;
				skipGenerate = 1;
			}
		}
		else if (c == 'd') {
			result = right();
			if (result != -1){
				my_score += result;
				result = 0;
				initializeCheck();
			}
			else {
				check[3] = 1;
				skipGenerate = 1;
			}
		}
		else continue;
		short int gameover = 0;
		if (check[0]*check[1]*check[2]*check[3]) { // all elements are 1 => game over
			pid_t pid = fork();

			if (pid < 0){
				fprintf(stderr, "fork failed\n");
			}
			else if (pid == 0){
				char temp[200];
				sprintf(temp, "%d", my_score);
				char *argv[] = {"./gameover", temp, NULL};
				if (execv(argv[0], argv) == -1){
					perror("execv failed");
				}
			}
			else {
				wait(NULL);
				break;
			}
		}
		if (!skipGenerate) generate(0);
		defaultPage();
		refresh();
		// redisplay
	}
	endwin();	


	return 0;
}

void initializeCheck(void){
	for (int i = 0; i < 4; i++) {
		check[i] = 0;
	}
}

void defaultPage(void){
	initscr();
	crmode();
	noecho();
	clear();
	
	for (int i = 1; i < 24; i++){
		move(i, 0); addstr("|");
		move(i, 52); addstr("|");
		if (i % 6 == 0) continue;	
		move(i, 13); addstr("|");
		move(i, 26); addstr("|");
		move(i, 39); addstr("|");
	}
	for (int i = 1; i < 52; i++){
		move(0, i); addstr("-");
		move(6, i); addstr("-");
		move(12, i); addstr("-");
		move(18, i); addstr("-");
		move(24, i); addstr("-");
	}
	
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);	
	
	char num[200];
	sprintf(num, "%d", board[0][0]); move(3, 5); if (board[0][0] != 0) addstr(num); else addstr(" ");

	sprintf(num, "%d", board[0][1]); move(3, 18); if (board[0][1] != 0) addstr(num); else addstr(" ");
	
	sprintf(num, "%d", board[0][2]); move(3, 31); if (board[0][2] != 0) addstr(num); else addstr(" ");
	
	sprintf(num, "%d", board[0][3]); move(3, 44); if (board[0][3] != 0) addstr(num); else addstr(" ");
	
	sprintf(num, "%d", board[1][0]); move(9, 5); if (board[1][0] != 0) addstr(num); else addstr(" ");

	sprintf(num, "%d", board[1][1]); move(9, 18); if (board[1][1] != 0) addstr(num); else addstr(" ");
	
	sprintf(num, "%d", board[1][2]); move(9, 31); if (board[1][2] != 0) addstr(num); else addstr(" ");
	
	sprintf(num, "%d", board[1][3]); move(9, 44); if (board[1][3] != 0) addstr(num); else addstr(" ");
	
	sprintf(num, "%d", board[2][0]); move(15, 5); if (board[2][0] != 0) addstr(num); else addstr(" ");
	
	sprintf(num, "%d", board[2][1]); move(15, 18); if (board[2][1] != 0) addstr(num); else addstr(" ");
	
	sprintf(num, "%d", board[2][2]); move(15, 31); if (board[2][2] != 0) addstr(num); else addstr(" ");
	
	sprintf(num, "%d", board[2][3]); move(15, 44); if (board[2][3] != 0) addstr(num); else addstr(" ");
	
	sprintf(num, "%d", board[3][0]); move(21, 5); if (board[3][0] != 0) addstr(num); else addstr(" ");

	sprintf(num, "%d", board[3][1]); move(21, 18); if (board[3][1] != 0) addstr(num); else addstr(" ");

	sprintf(num, "%d", board[3][2]); move(21, 31); if (board[3][2] != 0) addstr(num); else addstr(" ");

	sprintf(num, "%d", board[3][3]); move(21, 44); if (board[3][3] != 0) addstr(num); else addstr(" ");

	sprintf(num, "Score : %d", my_score);
	move(25, 22); addstr(num);
	move(26, 20); addstr("< 2048 Rules >");
	move(27, 0); addstr("When you cannot move either up, down, left, or right, the game ends.");
	move(28, 0);
	move(29, 23); addstr("BUTTONS");
	move(30, 0); addstr("UP : w   LEFT : a   DOWN : s   RIGHT : d\tEXIT : Q");
	move(32, 22); addstr("   TIPS  ");
	
	sprintf(num, "disabled to move : up %d left %d down %d right %d", check[0], check[1], check[2], check[3]);
	move(33, 0); addstr(num);
	move(34, 13); addstr("1 : disabled\t0 : can move");
	return;
}

int moved(int a[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (a[i][j] != board[i][j])
                return 1;
        }
    }
    return 0;
}

int up() {
    int i, j, k;
    int next_state[4][4] = { 0 };
    int score = 0;
    int temp[4][4];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            next_state[i][j] = board[i][j];
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[i][j] = 0;
        }
    }

    // Slide up operation
    for (j = 0; j < 4; j++) {
        int pos = 0;
        for (i = 0; i < 4; i++) {
            if (next_state[i][j] != 0) {
                if (temp[pos][j] == 0) {
                    temp[pos][j] = next_state[i][j];
                }
                else if (temp[pos][j] == next_state[i][j]) {
                    score += temp[pos][j];
		    temp[pos][j] *= 2;
                    pos++;
                }
                else {
                    pos++;
                    temp[pos][j] = next_state[i][j];
                }
            }
        }
    }

    // Copy temp array to next_state array
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            next_state[i][j] = temp[i][j];
        }
    }

    if (moved(next_state)) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                board[i][j] = next_state[i][j];
            }
        }
        return score;
    }
    else {
        return -1;
    }
}

int down() {
    int i, j, k;
    int next_state[4][4] = { 0 };
    int score = 0;
    int temp[4][4];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            next_state[i][j] = board[i][j];
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[i][j] = 0;
        }
    }

    // Slide down operation
    for (j = 0; j < 4; j++) {
        int pos = 3;
        for (i = 3; i >= 0; i--) {
            if (next_state[i][j] != 0) {
                if (temp[pos][j] == 0) {
                    temp[pos][j] = next_state[i][j];
                }
                else if (temp[pos][j] == next_state[i][j]) {
                    score += temp[pos][j];
		    temp[pos][j] *= 2;
                    pos--;
                }
                else {
                    pos--;
                    temp[pos][j] = next_state[i][j];
                }
            }
        }
    }

    // Copy temp array to next_state array
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            next_state[i][j] = temp[i][j];
        }
    }

    if (moved(next_state)) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                board[i][j] = next_state[i][j];
            }
        }
        return score;
    }
    else {
        return -1;
    }
}

int left() {
    int i, j, k;
    int next_state[4][4] = { 0 };
    int score = 0;
    int temp[4][4];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            next_state[i][j] = board[i][j];
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[i][j] = 0;
        }
    }

    // Slide left operation
    for (i = 0; i < 4; i++) {
        int pos = 0;
        for (j = 0; j < 4; j++) {
            if (next_state[i][j] != 0) {
                if (temp[i][pos] == 0) {
                    temp[i][pos] = next_state[i][j];
                }
                else if (temp[i][pos] == next_state[i][j]) {
                    score += temp[i][pos];
		    temp[i][pos] *= 2;
                    pos++;
                }
                else {
                    pos++;
                    temp[i][pos] = next_state[i][j];
                }
            }
        }
    }

    // Copy temp array to next_state array
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            next_state[i][j] = temp[i][j];
        }
    }

    if (moved(next_state)) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                board[i][j] = next_state[i][j];
            }
        }
        return score;
    }
    else {
        return -1;
    }
}

int right() {
    int i, j, k;
    int next_state[4][4] = { 0 };
    int score = 0;
    int temp[4][4];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            next_state[i][j] = board[i][j];
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[i][j] = 0;
        }
    }

    // Slide right operation
    for (i = 0; i < 4; i++) {
        int pos = 3;
        for (j = 3; j >= 0; j--) {
            if (next_state[i][j] != 0) {
                if (temp[i][pos] == 0) {
                    temp[i][pos] = next_state[i][j];
                }
                else if (temp[i][pos] == next_state[i][j]) {
                    score += temp[i][pos];
		    temp[i][pos] *= 2;
                    pos--;
                }
                else {
                    pos--;
                    temp[i][pos] = next_state[i][j];
                }
            }
        }
    }

    // Copy temp array to next_state array
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            next_state[i][j] = temp[i][j];
        }
    }

    if (moved(next_state)) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                board[i][j] = next_state[i][j];
            }
        }
        return score;
    }
    else {
        return -1;
    }
}


int generate(int init){
	int toAppend = 2;
	int generated[16] = { 0 };
	srand(time(NULL));
	int temp = 0;
	if (init == 1){// initial board setting
		for (int i = 0; i < 3; i++) {
			if (i == 2) toAppend = 4;
			temp = rand() % 16;
			if (generated[temp]) {
				i--;
				continue;
			}
			generated[temp] = 1;
			board[temp / 4][temp % 4] = toAppend;
		}
	}
	else { // one block append
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				if (board[i][j] != 0){
					generated[i*4 + j] = 1;
				}
			}
		}
		// full board -> game over
		int cant = 1;
		for (int i = 0; i < 16; i++) {
			cant *= generated[i];
		}
		if (cant) return -1;

		// 10 percents : 4 / 90 percents : 2
		if ((rand()%10) % 9 == 0){
			toAppend = 4;
		}
		else {
			toAppend = 2;
		}

		for (int i = 0; i < 1; i++){
			temp = rand() % 16;
			if (generated[temp]) {
				i--;
				continue;
			}
			generated[temp] = 1;
			board[temp / 4][temp % 4] = toAppend;
			break;
		}
	}
	return 0;
}
