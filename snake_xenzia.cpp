#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdbool.h> 
#include<time.h> 
#include<string.h>
#include<thread>
//#include<unistd.h>
#define screen_x	41
#define screen_y	24
#define speed		80
#define clear_x		41
#define clear_y		22
HANDLE wHnd;
COORD bufferSize = { screen_x,screen_y };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
int setConsole(int x, int y) {
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
int x = 20, y = 10, i = 0, j, total = 2, fx, fy, last_fx = 99, last_fy = 99, bx, by, lastx, lasty, length;//left = -1, stop = 0, right = 1, 40*20
int tail[99][2];
int openfile = 0;
//int logoarr[125][2];
int score, game_over, bfood_score, drawpass1, drawpass2, stage = 0, shiftat;
char direction, ch = ' ', lastkey;
float seconds, bfood_time;
clock_t start, end, end_bfood;
char name[50], chname;
struct player {
	char n[50];
	int sc;
};
struct player p[5];
void gotoxy(int x, int y) {
	COORD c = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void setcolor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void draw_snake(int x, int y) {
	setcolor(10, 10);
	gotoxy(x, y);
	printf("#");
}
void draw_food(int x, int y) {
	setcolor(7, 7);
	gotoxy(x, y);
	printf("o");
}
void draw_bfood(int x, int y) {
	setcolor(12, 12);
	gotoxy(bx, by);
	printf("O");
}
void draw_wall(int x, int y) {
	setcolor(5, 5);
	gotoxy(x, y);
	printf("x");
}
void clear_bfood(int x, int y) {
	gotoxy(x, y);
	printf(" ");
}
void setcursor(bool visible) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void erase_background(int x, int y) {
	gotoxy(x, y);
	setcolor(0, 0);
	printf(" ");
}
void eat_status(int x, int y) {//quantity of food snake has eaten
	setcolor(7, 0);
	gotoxy(x, y);
	//printf("eat : %d", total); use for showing how many foods the snake has eaten
	printf("SCORE : %d", score);
}
void shift() {
	for (i = length; i > 0; i--) {
		for (j = 0; j < 2; j++) {
			tail[i][j] = tail[i - 1][j];
		}
	}
}
void check_gameover() {
	for (i = 1; i < total + 1; i++) {
		if (x == tail[i][0] && y == tail[i][1]) {
			gotoxy(6, 10);
			setcolor(15, 0);
			printf("G A M E  O V E R");
			Sleep(3000);
		}
	}
}
void menu(int x, int y) {
	setcolor(15, 0);
	gotoxy(x, y);
	printf("PRESS W TO START");
	setcolor(8, 0);
	gotoxy(x - 5, y + 2);
	printf("PRESS S TO SEE SCOREBOARD");
}
void entername(int x, int y) {
	setcolor(7, 0);
	gotoxy(x, y);
	printf("ENTER YOUR NAME");
	gotoxy(x + 2, y + 2);
	printf(": ");
}
void dev_name(int x, int y) {
	setcolor(8, 0);
	gotoxy(x, y);
	printf("Phatorn Pitiphat 64010667");
}
int logoarr[125][2] = { {8 + 2, 4},{8 + 3, 4},{8 + 4, 4},{8 + 6, 4},{8 + 9, 4},{8 + 12, 4},{8 + 13, 4},{8 + 16, 4},{8 + 19, 4},{8 + 21, 4},{8 + 22, 4},{8 + 23, 4},//12
						{8 + 2, 4 + 1},{8 + 6, 4 + 1},{8 + 7, 4 + 1},{8 + 9, 4 + 1},{8 + 11, 4 + 1},{8 + 14, 4 + 1},{8 + 16, 4 + 1},{8 + 18, 4 + 1},{8 + 21, 4 + 1},//9
						{8 + 2, 4 + 2},{8 + 3, 4 + 2},{8 + 4, 4 + 2},{8 + 6, 4 + 2},{8 + 7, 4 + 2},{8 + 8, 4 + 2},{8 + 9, 4 + 2},{8 + 11, 4 + 2},{8 + 12, 4 + 2},{8 + 13, 4 + 2},{8 + 14, 4 + 2},{8 + 16, 4 + 2},{8 + 17, 4 + 2},{8 + 21, 4 + 2},{8 + 22, 4 + 2},{8 + 23, 4 + 2},//16
						{8 + 4, 4 + 3},{8 + 6, 4 + 3},{8 + 8, 4 + 3},{8 + 9, 4 + 3},{8 + 11, 4 + 3},{8 + 14, 4 + 3},{8 + 16, 4 + 3},{8 + 18, 4 + 3},{8 + 21, 4 + 3},//9
						{8 + 2, 4 + 4},{8 + 3, 4 + 4},{8 + 4, 4 + 4},{8 + 6, 4 + 4},{8 + 9, 4 + 4},{8 + 11, 4 + 4},{8 + 14, 4 + 4},{8 + 16, 4 + 4},{8 + 19, 4 + 4},{8 + 21, 4 + 4},{8 + 22, 4 + 4},{8 + 23, 4 + 4},//12

						{8 + 0, 4 + 7},{8 + 2, 4 + 7},{8 + 4, 4 + 7},{8 + 5, 4 + 7},{8 + 6, 4 + 7},{8 + 8, 4 + 7},{8 + 11, 4 + 7},{8 + 13, 4 + 7},{8 + 14, 4 + 7},{8 + 15, 4 + 7},{8 + 16, 4 + 7},{8 + 18, 4 + 7},{8 + 19, 4 + 7},{8 + 20, 4 + 7},{8 + 23, 4 + 7},{8 + 24, 4 + 7},//16
						{8 + 0, 4 + 8},{8 + 2, 4 + 8},{8 + 4, 4 + 8},{8 + 8, 4 + 8},{8 + 9, 4 + 8},{8 + 11, 4 + 8},{8 + 16, 4 + 8},{8 + 19, 4 + 8},{8 + 22, 4 + 8},{8 + 25, 4 + 8},//10
						{8 + 1, 4 + 9},{8 + 4, 4 + 9},{8 + 5, 4 + 9},{8 + 6, 4 + 9},{8 + 8, 4 + 9},{8 + 9, 4 + 9},{8 + 10, 4 + 9},{8 + 11, 4 + 9},{8 + 14, 4 + 9},{8 + 15, 4 + 9},{8 + 19, 4 + 9},{8 + 22, 4 + 9},{8 + 23, 4 + 9},{8 + 24, 4 + 9},{8 + 25, 4 + 9},//15
						{8 + 0, 4 + 10},{8 + 2, 4 + 10},{8 + 4, 4 + 10},{8 + 8, 4 + 10},{8 + 10, 4 + 10},{8 + 11, 4 + 10},{8 + 13, 4 + 10},{8 + 19, 4 + 10},{8 + 22, 4 + 10},{8 + 25, 4 + 10},//10
						{8 + 0, 4 + 11},{8 + 2, 4 + 11},{8 + 4, 4 + 11},{8 + 5, 4 + 11},{8 + 6, 4 + 11},{8 + 8, 4 + 11},{8 + 11, 4 + 11},{8 + 13, 4 + 11},{8 + 14, 4 + 11},{8 + 15, 4 + 11},{8 + 16, 4 + 11},{8 + 18, 4 + 11},{8 + 19, 4 + 11},{8 + 20, 4 + 11},{8 + 22, 4 + 11},{8 + 25, 4 + 11} };//16
int wall1[120][2] = {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0},{14,0},{15,0},{16,0},{17,0},{18,0},{19,0},{20,0},{21,0},{22,0},{23,0},{24,0},{25,0},{26,0},{27,0},{28,0},{29,0},{30,0},{31,0},{32,0},{33,0},{34,0},{35,0},{36,0},{37,0},{38,0},{39,0},{40,0}
					,{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8},{0,9},{0,10},{0,11},{0,12},{0,13},{0,14},{0,15},{0,16},{0,17},{0,18},{0,19}
					,{40,1},{40,2},{40,3},{40,4},{40,5},{40,6},{40,7},{40,8},{40,9},{40,10},{40,11},{40,12},{40,13},{40,14},{40,15},{40,16},{40,17},{40,18},{40,19}
					,{0,20},{1,20},{2,20},{3,20},{4,20},{5,20},{6,20},{7,20},{8,20},{9,20},{10,20},{11,20},{12,20},{13,20},{14,20},{15,20},{16,20},{17,20},{18,20},{19,20},{20,20},{21,20},{22,20},{23,20},{24,20},{25,20},{26,20},{27,20},{28,20},{29,20},{30,20},{31,20},{32,20},{33,20},{34,20},{35,20},{36,20},{37,20},{38,20},{39,20},{40,20} };
const int wall1_q = 120;
int wall2[76][2] = {{0,0},{1,0},{2,0},{3,0},{4,0},{36,0},{37,0},{38,0},{39,0},{40,0}
					,{0,20},{1,20},{2,20},{3,20},{4,20},{36,20},{37,20},{38,20},{39,20},{40,20}
					,{0,1},{0,2},{0,3},{0,4},{0,16},{0,17},{0,18},{0,19}
					,{40,1},{40,2},{40,3},{40,4},{40,16},{40,17},{40,18},{40,19}
					,{7,6},{8,6},{9,6},{10,6},{11,6},{12,6},{13,6},{14,6},{15,6},{16,6}
					,{7,14},{8,14},{9,14},{10,14},{11,14},{12,14},{13,14},{14,14},{15,14},{16,14}
					,{24,6},{25,6},{26,6},{27,6},{28,6},{29,6},{30,6},{31,6},{32,6},{33,6}
					,{24,14},{25,14},{26,14},{27,14},{28,14},{29,14},{30,14},{31,14},{32,14},{33,14} };
const int wall2_q = 76;
int wall3[56][2] = { {13,0},{13,1},{13,2},{13,3},{13,4},{13,5},{13,6},{13,7},{13,8},{13,9},{13,10},{13,11},{13,12},{13,13}
					,{0,17},{1,17},{2,17},{3,17},{4,17},{5,17},{6,17},{7,17},{8,17},{9,17},{10,17},{11,17},{12,17},{13,17}
					,{27,3},{28,3},{29,3},{30,3},{31,3},{32,3},{33,3},{34,3},{35,3},{36,3},{37,3},{38,3},{39,3},{40,3}
					,{27,7},{27,8},{27,9},{27,10},{27,11},{27,12},{27,13},{27,14},{27,15},{27,16},{27,17},{27,18},{27,19},{27,20} };
const int wall3_q = 56;
int wall4[130][2] = { {0,0},{1,0},{2,0},{3,0},{4,0},{10,0},{11,0},{12,0},{13,0},{14,0},{15,0},{16,0},{17,0},{18,0},{19,0},{20,0},{21,0},{22,0},{23,0},{24,0},{25,0},{26,0},{27,0},{28,0},{29,0},{30,0},{31,0},{32,0},{33,0},{34,0}//35
					,{0,14},{1,14},{2,14},{3,14},{4,14},{5,14},{6,14},{7,14},{8,14},{9,14},{10,14},{11,14},{12,14},{13,14},{14,14},{15,14},{16,14},{17,14},{18,14},{19,14},{20,14},{21,14},{22,14},{23,14},{24,14},{25,14},{26,14},{27,14},{28,14},{29,14},{30,14},{31,14},{32,14},{33,14},{34,14},{35,14},{36,14},{37,14},{38,14},{39,14},{40,14}//41
					,{0,8},{1,8},{2,8},{3,8},{4,8},{5,8},{6,8},{7,8},{8,8},{9,8},{10,8},{11,8},{12,8},{13,8},{14,8},{15,8},{16,8},{17,8},{23,8},{24,8},{25,8},{26,8},{27,8},{28,8},{29,8},{30,8},{31,8},{32,8},{33,8},{34,8},{35,8},{36,8},{37,8},{38,8},{39,8},{40,8}//36
					,{0,1},{0,2},{0,3}//4
					,{17,1},{17,2},{17,3},{17,4},{17,5},{17,6},{17,7},{17,8}//8
					,{23,15},{23,16},{23,17},{23,18},{23,19},{23,20} };//6
const int wall4_q = 130;
void bfood_timer(float n) {
	if (n <= 1) bfood_score = 150;
	else if (n <= 2 && n > 1) bfood_score = 120;
	else if (n <= 3 && n > 2) bfood_score = 90;
	else if (n <= 4 && n > 3) bfood_score = 60;
	else if (n <= 5 && n > 4) bfood_score = 30;
	else if (n > 5) bfood_score = 0;
}

char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
		return '\0';
	else return buf[0];
}
int main() {
	FILE* fc;
	fc = fopen("highscore.txt", "a");
	fclose(fc);
	setConsole(screen_x, screen_y);
	int newWidth = 30, newHeight = 30;
	CONSOLE_FONT_INFOEX fontStructure = { 0 };
	fontStructure.cbSize = sizeof(fontStructure);
	fontStructure.dwFontSize.X = newWidth;
	fontStructure.dwFontSize.Y = newHeight;
	wcscpy(fontStructure.FaceName, L"Consolas");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentConsoleFontEx(hConsole, true, &fontStructure);
	setcursor(0);
	do {
		do {
			setcursor(0);
			score = 0;
			for (i = 0; i < clear_x; i++) {//clear screen
				for (j = 0; j < clear_y; j++) {
					erase_background(i, j);
				}
			}
			for (i = 0; i < 125; i++) {
				draw_snake(logoarr[i][0], logoarr[i][1]);
			}
			menu(13, 17);
			dev_name(16, 22);
			//draw_snake(40, 22); //use for test screen size
			ch = ' ', lastkey;
			ch = _getch();
			direction = ' ';
			//while (ch != 'w') {
				if (ch == 'w') {
					stage = 99;
					for (i = 0; i < clear_x; i++) {//clear screen
						for (j = 0; j < clear_y; j++) {
							erase_background(i, j);
						}
					}
				}
			//}
			if (ch == 's') {
				stage = 6;
				for (i = 0; i < clear_x; i++) {//clear screen
					for (j = 0; j < clear_y; j++) {
						erase_background(i, j);
					}
				}
			}
		} while (stage == 0);
		while (stage == 99) {
			chname = ' ';
			entername(13, 8);
			i = 0;
			chname = getchar();
			while (chname != '\n') {
				name[i] = chname;
				i++;
				chname = getchar();
			}
			name[i] = '\0';
			//printf("\n%s", name); use for debug
			if (chname == '\n') {
				//score = 0;// edit here !!!
				stage = 1;// edit here !!!! DEFAULT 1
			}
		}
		while (stage == 1 && ch != 'x') {
			for (i = 0; i < clear_x; i++) {//clear screen
				for (j = 0; j < clear_y; j++) {
					erase_background(i, j);
				}
			}
			for (i = 0; i < 120; i++) {
				draw_wall(wall1[i][0], wall1[i][1]);
			}
			ch = ' ';
			lastkey = '/';
			direction = ' ';
			int tail[99][2];
			int f = 0;
			x = 20, y = 10, i = 0, j, total = 2, fx, fy, last_fx = 99, last_fy = 99;
			drawpass1 = 1;
			drawpass2 = 1;
			score = 0;
			tail[0][0] = x;
			tail[0][1] = y;
			tail[1][0] = x;
			tail[1][1] = y + 1;
			tail[2][0] = x;
			tail[2][1] = y + 2;
			draw_snake(tail[0][0], tail[0][1]);
			draw_snake(tail[1][0], tail[1][1]);
			draw_snake(tail[2][0], tail[2][1]);
			srand((unsigned int)time(NULL));
			eat_status(1, 21);
			do {
				fx = (rand() % (38 - 2 + 1)) + 2;//number = (rand() % (upper - lower + 1)) + lower
				fy = (rand() % (18 - 2 + 1)) + 2;
				for (i = 0; i <= total + 1; i++) {
					if (fx == tail[i][0] && fy == tail[i][1]) {
						drawpass1 = 0;
						break;
					}
					else drawpass1 = 1;
				}
				for (i = 0; i < wall1_q; i++) {
					if (fx == wall1[i][0] && fy == wall1[i][1]) {
						drawpass2 = 0;
						break;
					}
					else drawpass2 = 1;
				}
			} while ((last_fx == fx && last_fy == fy) || drawpass1 == 0 || drawpass2 == 0);
			draw_food(fx, fy);
			do {
				game_over = 0;
				length = total + 1;
				if (_kbhit()) { //control
					lastkey = ch;
					ch = _getch();
					if (lastkey != ch) {
						if (ch == 'w' && direction != 's') direction = 'w';//up
						if (ch == 'a' && direction != 'd') direction = 'a';//left
						if (ch == 's' && direction != 'w' && lastkey != ' ') direction = 's';//down
						if (ch == 'd' && direction != 'a') direction = 'd';//right
					}
				}
				if (y == 20 && direction == 's') y = -1;
				if (x == 40 && direction == 'd') x = -1;
				if (y == 0 && direction == 'w') y = 21;
				if (x == 0 && direction == 'a') x = 41;
				if (direction == 'a') {//moving direction
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = --x;
					tail[0][1] = y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 'd') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = ++x;
					tail[0][1] = y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 'w') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = x;
					tail[0][1] = --y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 's') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = x;
					tail[0][1] = ++y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				for (i = 0; i < wall1_q; i++) {
					if (cursor(wall1[i][0], wall1[i][1]) == '#') {
						game_over++;
					}
				}
				if (game_over == 1) {
					std::thread q(Beep, 400, 200);
					q.detach();
					gotoxy(12, 10);
					setcolor(15, 0);
					printf("G A M E  O V E R ");
					Sleep(3000);
					stage = 6;
					break;
				}
				fflush(stdin);
				if (cursor(fx, fy) == '#') {//check if the food was eaten
					std::thread q(Beep, 700, 100);
					q.detach();
					f++;
					do {
						fx = (rand() % (38 - 2 + 1)) + 2;//number = (rand() % (upper - lower + 1)) + lower
						fy = (rand() % (18 - 2 + 1)) + 2;
						for (i = 0; i <= total + 1; i++) {
							if (fx == tail[i][0] && fy == tail[i][1]) {
								drawpass1 = 0;
								break;
							}
							else drawpass1 = 1;
						}
						for (i = 0; i < wall1_q; i++) {
							if (fx == wall1[i][0] && fy == wall1[i][1]) {
								drawpass2 = 0;
								break;
							}
							else drawpass2 = 1;
						}
					} while ((last_fx == fx && last_fy == fy) || drawpass1 == 0 || drawpass2 == 0);
					draw_food(fx, fy);
					total++;
					score = score + 5;
					last_fx = fx;
					last_fy = fy;
				}
				if (f % 5 == 0 && f != 0) {//to create bonus food every 5 normal food
					do {
						bx = (rand() % (38 - 2 + 1)) + 2;//number = (rand() % (upper - lower + 1)) + lower
						by = (rand() % (18 - 2 + 1)) + 2;
						for (i = 0; i <= total + 1; i++) {
							if (bx == tail[i][0] && by == tail[i][1]) {
								drawpass1 = 0;
								break;
							}
							else drawpass1 = 1;
						}
						for (i = 0; i < wall1_q; i++) {
							if (bx == wall1[i][0] && by == wall1[i][1]) {
								drawpass2 = 0;
								break;
							}
							else drawpass2 = 1;
						}
					} while (drawpass1 == 0 || drawpass2 == 0);
					draw_bfood(bx, by);
					start = clock();
					f = 0;
				}
				end_bfood = clock();
				bfood_time = ((float)(end_bfood - start)) / CLOCKS_PER_SEC;
				if ((int)bfood_time == 5 && cursor(bx, by) == 'O') {
					erase_background(bx, by);
				}
				if (cursor(bx, by) == '#') {
					total++;
					end = clock();
					seconds = ((float)(end - start)) / CLOCKS_PER_SEC;
					if (seconds <= 1) bfood_score = 150;
					else if (seconds <= 2 && seconds > 1) bfood_score = 120;
					else if (seconds <= 3 && seconds > 2) bfood_score = 90;
					else if (seconds <= 4 && seconds > 3) bfood_score = 60;
					else if (seconds <= 5 && seconds > 4) bfood_score = 30;
					else if (seconds > 5) bfood_score = 0;
					score = score + bfood_score;//change 150 to bfood_score
					if (bfood_score != 0) {
						std::thread q(Beep, 700, 100);
						q.detach();
					}
					bx = 99;
					by = 99;
				}
				if (score >= 500) {
					score = 500;
					stage = 2;
					break;
				}
				eat_status(1, 21);
				Sleep(speed);
			} while (ch != 'x');
		}
		while (stage == 2 && ch != 'x') {
			for (i = 0; i < clear_x; i++) {//clear screen
				for (j = 0; j < clear_y; j++) {
					erase_background(i, j);
				}
			}
			for (i = 0; i < wall2_q; i++) {
				draw_wall(wall2[i][0], wall2[i][1]);
			}
			ch = ' ';
			lastkey = '/';
			direction = ' ';
			int tail[99][2];
			int f = 0;
			x = 20, y = 10, i = 0, j, total = 2, fx, fy, last_fx = 99, last_fy = 99;
			drawpass1 = 1;
			drawpass2 = 1;
			tail[0][0] = x;
			tail[0][1] = y;
			tail[1][0] = x;
			tail[1][1] = y + 1;
			tail[2][0] = x;
			tail[2][1] = y + 2;
			draw_snake(tail[0][0], tail[0][1]);
			draw_snake(tail[1][0], tail[1][1]);
			draw_snake(tail[2][0], tail[2][1]);
			srand((unsigned int)time(NULL));
			eat_status(1, 21);
			do {
				fx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
				fy = (rand() % (19 - 0 + 1)) + 0;
				for (i = 0; i <= total + 1; i++) {
					if (fx == tail[i][0] && fy == tail[i][1]) {
						drawpass1 = 0;
						break;
					}
					else drawpass1 = 1;
				}
				for (i = 0; i < wall2_q; i++) {
					if (fx == wall2[i][0] && fy == wall2[i][1]) {
						drawpass2 = 0;
						break;
					}
					else drawpass2 = 1;
				}
			} while ((last_fx == fx && last_fy == fy) || drawpass1 == 0 || drawpass2 == 0);
			draw_food(fx, fy);
			do {
				game_over = 0;
				length = total + 1;
				if (_kbhit()) { //control
					lastkey = ch;
					ch = _getch();
					if (lastkey != ch) {
						if (ch == 'w' && direction != 's') direction = 'w';//up
						if (ch == 'a' && direction != 'd') direction = 'a';//left
						if (ch == 's' && direction != 'w' && lastkey != ' ') direction = 's';//down
						if (ch == 'd' && direction != 'a') direction = 'd';//right
					}
				}
				if (y == 20 && direction == 's') y = -1;
				if (x == 40 && direction == 'd') x = -1;
				if (y == 0 && direction == 'w') y = 21;
				if (x == 0 && direction == 'a') x = 41;
				if (direction == 'a') {//moving direction
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = --x;
					tail[0][1] = y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 'd') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = ++x;
					tail[0][1] = y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 'w') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = x;
					tail[0][1] = --y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 's') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = x;
					tail[0][1] = ++y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				for (i = 0; i < wall2_q; i++) {
					if (cursor(wall2[i][0], wall2[i][1]) == '#') {
						game_over++;
					}
				}
				if (game_over == 1) {
					std::thread q(Beep, 400, 200);
					q.detach();
					gotoxy(12, 10);
					setcolor(15, 0);
					printf("G A M E  O V E R ");
					Sleep(3000);
					stage = 6;
					break;
				}
				fflush(stdin);
				if (cursor(fx, fy) == '#') {//check if the food was eaten
					std::thread q(Beep, 700, 100);
					q.detach();
					f++;
					do {
						fx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
						fy = (rand() % (19 - 0 + 1)) + 0;
						for (i = 0; i <= total + 1; i++) {
							if (fx == tail[i][0] && fy == tail[i][1]) {
								drawpass1 = 0;
								break;
							}
							else drawpass1 = 1;
						}
						for (i = 0; i < wall2_q; i++) {
							if (fx == wall2[i][0] && fy == wall2[i][1]) {
								drawpass2 = 0;
								break;
							}
							else drawpass2 = 1;
						}
					} while ((last_fx == fx && last_fy == fy) || drawpass1 == 0 || drawpass2 == 0);
					draw_food(fx, fy);
					total++;
					score = score + 5;
					//eat_status(1, 21);
					last_fx = fx;
					last_fy = fy;
				}
				if (f % 5 == 0 && f != 0) {//to create bonus food every 5 normal food
					do {
						bx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
						by = (rand() % (19 - 0 + 1)) + 0;
						for (i = 0; i <= total + 1; i++) {
							if (bx == tail[i][0] && by == tail[i][1]) {
								drawpass1 = 0;
								break;
							}
							else drawpass1 = 1;
						}
						for (i = 0; i < wall2_q; i++) {
							if (bx == wall2[i][0] && by == wall2[i][1]) {
								drawpass2 = 0;
								break;
							}
							else drawpass2 = 1;
						}
					} while (drawpass1 == 0 || drawpass2 == 0);
					draw_bfood(bx, by);
					start = clock();
					f = 0;
				}
				end_bfood = clock();
				bfood_time = ((float)(end_bfood - start)) / CLOCKS_PER_SEC;
				if ((int)bfood_time == 5 && cursor(bx, by) == 'O') {
					erase_background(bx, by);
				}
				if (cursor(bx, by) == '#') {
					total++;
					end = clock();
					seconds = ((float)(end - start)) / CLOCKS_PER_SEC;
					if (seconds <= 1) bfood_score = 150;
					else if (seconds <= 2 && seconds > 1) bfood_score = 120;
					else if (seconds <= 3 && seconds > 2) bfood_score = 90;
					else if (seconds <= 4 && seconds > 3) bfood_score = 60;
					else if (seconds <= 5 && seconds > 4) bfood_score = 30;
					else if (seconds > 5) bfood_score = 0;
					score = score + bfood_score;//change 150 to bfood_score
					//eat_status(1, 21);
					if (bfood_score != 0) {
						std::thread q(Beep, 700, 100);
						q.detach();
					}
					bx = 99;
					by = 99;
				}
				if (score >= 1000) {
					score = 1000;
					stage = 3;
					break;
				}
				eat_status(1, 21);
				Sleep(speed);
			} while (ch != 'x');
		}
		while (stage == 3 && ch != 'x') {
			for (i = 0; i < clear_x; i++) {//clear screen
				for (j = 0; j < clear_y; j++) {
					erase_background(i, j);
				}
			}
			for (i = 0; i < wall3_q; i++) {
				draw_wall(wall3[i][0], wall3[i][1]);
			}
			ch = ' ';
			lastkey = '/';
			direction = ' ';
			int tail[99][2];
			int f = 0;
			x = 20, y = 10, i = 0, j, total = 2, fx, fy, last_fx = 99, last_fy = 99;
			drawpass1 = 1;
			drawpass2 = 1;
			tail[0][0] = x;
			tail[0][1] = y;
			tail[1][0] = x;
			tail[1][1] = y + 1;
			tail[2][0] = x;
			tail[2][1] = y + 2;
			draw_snake(tail[0][0], tail[0][1]);
			draw_snake(tail[1][0], tail[1][1]);
			draw_snake(tail[2][0], tail[2][1]);
			srand((unsigned int)time(NULL));
			eat_status(1, 21);
			do {
				fx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
				fy = (rand() % (19 - 0 + 1)) + 0;
				for (i = 0; i <= total + 1; i++) {
					if (fx == tail[i][0] && fy == tail[i][1]) {
						drawpass1 = 0;
						break;
					}
					else drawpass1 = 1;
				}
				for (i = 0; i < wall3_q; i++) {
					if (fx == wall3[i][0] && fy == wall3[i][1]) {
						drawpass2 = 0;
						break;
					}
					else drawpass2 = 1;
				}
			} while ((last_fx == fx && last_fy == fy) || drawpass1 == 0 || drawpass2 == 0);
			draw_food(fx, fy);
			do {
				game_over = 0;
				length = total + 1;
				if (_kbhit()) { //control
					lastkey = ch;
					ch = _getch();
					if (lastkey != ch) {
						if (ch == 'w' && direction != 's') direction = 'w';//up
						if (ch == 'a' && direction != 'd') direction = 'a';//left
						if (ch == 's' && direction != 'w' && lastkey != ' ') direction = 's';//down
						if (ch == 'd' && direction != 'a') direction = 'd';//right
					}
				}
				if (y == 20 && direction == 's') y = -1;
				if (x == 40 && direction == 'd') x = -1;
				if (y == 0 && direction == 'w') y = 21;
				if (x == 0 && direction == 'a') x = 41;
				if (direction == 'a') {//moving direction
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = --x;
					tail[0][1] = y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 'd') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = ++x;
					tail[0][1] = y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 'w') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = x;
					tail[0][1] = --y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 's') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = x;
					tail[0][1] = ++y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				for (i = 0; i < wall3_q; i++) {
					if (cursor(wall3[i][0], wall3[i][1]) == '#') {
						game_over++;
					}
				}
				if (game_over == 1) {
					std::thread q(Beep, 400, 200);
					q.detach();
					gotoxy(12, 10);
					setcolor(15, 0);
					printf("G A M E  O V E R ");
					Sleep(3000);
					stage = 6;
					break;
				}
				fflush(stdin);
				if (cursor(fx, fy) == '#') {//check if the food was eaten
					std::thread q(Beep, 700, 100);
					q.detach();
					f++;
					do {
						fx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
						fy = (rand() % (19 - 0 + 1)) + 0;
						for (i = 0; i <= total + 1; i++) {
							if (fx == tail[i][0] && fy == tail[i][1]) {
								drawpass1 = 0;
								break;
							}
							else drawpass1 = 1;
						}
						for (i = 0; i < wall3_q; i++) {
							if (fx == wall3[i][0] && fy == wall3[i][1]) {
								drawpass2 = 0;
								break;
							}
							else drawpass2 = 1;
						}
					} while ((last_fx == fx && last_fy == fy) || drawpass1 == 0 || drawpass2 == 0);
					draw_food(fx, fy);
					total++;
					score = score + 5;
					last_fx = fx;
					last_fy = fy;
				}
				if (f % 5 == 0 && f != 0) {//to create bonus food every 5 normal food
					do {
						bx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
						by = (rand() % (19 - 0 + 1)) + 0;
						for (i = 0; i <= total + 1; i++) {
							if (bx == tail[i][0] && by == tail[i][1]) {
								drawpass1 = 0;
								break;
							}
							else drawpass1 = 1;
						}
						for (i = 0; i < wall3_q; i++) {
							if (bx == wall3[i][0] && by == wall3[i][1]) {
								drawpass2 = 0;
								break;
							}
							else drawpass2 = 1;
						}
					} while (drawpass1 == 0 || drawpass2 == 0);
					draw_bfood(bx, by);
					start = clock();
					f = 0;
				}
				end_bfood = clock();
				bfood_time = ((float)(end_bfood - start)) / CLOCKS_PER_SEC;
				if ((int)bfood_time == 5 && cursor(bx, by) == 'O') {
					erase_background(bx, by);
				}
				if (cursor(bx, by) == '#') {
					total++;
					end = clock();
					seconds = ((float)(end - start)) / CLOCKS_PER_SEC;
					if (seconds <= 1) bfood_score = 150;
					else if (seconds <= 2 && seconds > 1) bfood_score = 120;
					else if (seconds <= 3 && seconds > 2) bfood_score = 90;
					else if (seconds <= 4 && seconds > 3) bfood_score = 60;
					else if (seconds <= 5 && seconds > 4) bfood_score = 30;
					else if (seconds > 5) bfood_score = 0;
					score = score + bfood_score;//change 150 to bfood_score
					//eat_status(1, 21);
					if (bfood_score != 0) {
						std::thread q(Beep, 700, 100);
						q.detach();
					}
					bx = 99;
					by = 99;
				}
				if (score >= 1500) {
					score = 1500;
					stage = 4;
					break;
				}
				eat_status(1, 21);
				Sleep(speed);
			} while (ch != 'x');
		}
		while (stage == 4 && ch != 'x') {
			for (i = 0; i < clear_x; i++) {//clear screen
				for (j = 0; j < clear_y; j++) {
					erase_background(i, j);
				}
			}
			for (i = 0; i < wall4_q; i++) {
				draw_wall(wall4[i][0], wall4[i][1]);
			}
			ch = ' ';
			lastkey = '/';
			direction = ' ';
			int tail[99][2];
			int f = 0;
			x = 20, y = 10, i = 0, j, total = 2, fx, fy, last_fx = 99, last_fy = 99;
			drawpass1 = 1;
			drawpass2 = 1;
			tail[0][0] = x;
			tail[0][1] = y;
			tail[1][0] = x;
			tail[1][1] = y + 1;
			tail[2][0] = x;
			tail[2][1] = y + 2;
			draw_snake(tail[0][0], tail[0][1]);
			draw_snake(tail[1][0], tail[1][1]);
			draw_snake(tail[2][0], tail[2][1]);
			srand((unsigned int)time(NULL));
			eat_status(1, 21);
			do {
				fx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
				fy = (rand() % (19 - 0 + 1)) + 0;
				for (i = 0; i <= total + 1; i++) {
					if (fx == tail[i][0] && fy == tail[i][1]) {
						drawpass1 = 0;
						break;
					}
					else drawpass1 = 1;
				}
				for (i = 0; i < wall4_q; i++) {
					if (fx == wall4[i][0] && fy == wall4[i][1]) {
						drawpass2 = 0;
						break;
					}
					else drawpass2 = 1;
				}
			} while ((last_fx == fx && last_fy == fy) || drawpass1 == 0 || drawpass2 == 0);
			draw_food(fx, fy);
			do {
				game_over = 0;
				length = total + 1;
				if (_kbhit()) { //control
					lastkey = ch;
					ch = _getch();
					if (lastkey != ch) {
						if (ch == 'w' && direction != 's') direction = 'w';//up
						if (ch == 'a' && direction != 'd') direction = 'a';//left
						if (ch == 's' && direction != 'w' && lastkey != ' ') direction = 's';//down
						if (ch == 'd' && direction != 'a') direction = 'd';//right
					}
				}
				if (y == 20 && direction == 's') y = -1;
				if (x == 40 && direction == 'd') x = -1;
				if (y == 0 && direction == 'w') y = 21;
				if (x == 0 && direction == 'a') x = 41;
				if (direction == 'a') {//moving direction
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = --x;
					tail[0][1] = y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 'd') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = ++x;
					tail[0][1] = y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 'w') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = x;
					tail[0][1] = --y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 's') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = x;
					tail[0][1] = ++y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				for (i = 0; i < wall4_q; i++) {
					if (cursor(wall4[i][0], wall4[i][1]) == '#') {
						game_over++;
					}
				}
				if (game_over == 1) {
					std::thread q(Beep, 400, 200);
					q.detach();
					gotoxy(12, 10);
					setcolor(15, 0);
					printf("G A M E  O V E R ");
					Sleep(3000);
					stage = 6;
					break;
				}
				fflush(stdin);
				if (cursor(fx, fy) == '#') {//check if the food was eaten
					std::thread q(Beep, 700, 100);
					q.detach();
					f++;
					do {
						fx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
						fy = (rand() % (19 - 0 + 1)) + 0;
						for (i = 0; i <= total + 1; i++) {
							if (fx == tail[i][0] && fy == tail[i][1]) {
								drawpass1 = 0;
								break;
							}
							else drawpass1 = 1;
						}
						for (i = 0; i < wall4_q; i++) {
							if (fx == wall4[i][0] && fy == wall4[i][1]) {
								drawpass2 = 0;
								break;
							}
							else drawpass2 = 1;
						}
					} while ((last_fx == fx && last_fy == fy) || drawpass1 == 0 || drawpass2 == 0);
					draw_food(fx, fy);
					total++;
					score = score + 5;
					last_fx = fx;
					last_fy = fy;
				}
				if (f % 5 == 0 && f != 0) {//to create bonus food every 5 normal food
					do {
						bx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
						by = (rand() % (19 - 0 + 1)) + 0;
						for (i = 0; i <= total + 1; i++) {
							if (bx == tail[i][0] && by == tail[i][1]) {
								drawpass1 = 0;
								break;
							}
							else drawpass1 = 1;
						}
						for (i = 0; i < wall4_q; i++) {
							if (bx == wall4[i][0] && by == wall4[i][1]) {
								drawpass2 = 0;
								break;
							}
							else drawpass2 = 1;
						}
					} while (drawpass1 == 0 || drawpass2 == 0);
					draw_bfood(bx, by);
					start = clock();
					f = 0;
				}
				end_bfood = clock();
				bfood_time = ((float)(end_bfood - start)) / CLOCKS_PER_SEC;
				if ((int)bfood_time == 5 && cursor(bx, by) == 'O') {
					erase_background(bx, by);
				}
				if (cursor(bx, by) == '#') {
					total++;
					end = clock();
					seconds = ((float)(end - start)) / CLOCKS_PER_SEC;
					if (seconds <= 1) bfood_score = 150;
					else if (seconds <= 2 && seconds > 1) bfood_score = 120;
					else if (seconds <= 3 && seconds > 2) bfood_score = 90;
					else if (seconds <= 4 && seconds > 3) bfood_score = 60;
					else if (seconds <= 5 && seconds > 4) bfood_score = 30;
					else if (seconds > 5) bfood_score = 0;
					score = score + bfood_score;//change 150 to bfood_score
					if (bfood_score != 0) {
						std::thread q(Beep, 700, 100);
						q.detach();
					}
					bx = 99;
					by = 99;
				}
				if (score >= 2000) {
					score = 2000;
					stage = 5;
					break;
				}
				eat_status(1, 21);
				Sleep(speed);
			} while (ch != 'x');
		}
		while (stage == 5 && ch != 'x') {
			for (i = 0; i < clear_x; i++) {//clear screen
				for (j = 0; j < clear_y; j++) {
					erase_background(i, j);
				}
			}
			ch = ' ';
			lastkey = '/';
			direction = ' ';
			int tail[99][2];
			int f = 0;
			x = 20, y = 10, i = 0, j, total = 2, fx, fy, last_fx = 99, last_fy = 99;
			drawpass1 = 1;
			drawpass2 = 1;
			tail[0][0] = x;
			tail[0][1] = y;
			tail[1][0] = x;
			tail[1][1] = y + 1;
			tail[2][0] = x;
			tail[2][1] = y + 2;
			draw_snake(tail[0][0], tail[0][1]);
			draw_snake(tail[1][0], tail[1][1]);
			draw_snake(tail[2][0], tail[2][1]);
			srand((unsigned int)time(NULL));
			eat_status(1, 21);
			do {
				fx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
				fy = (rand() % (19 - 0 + 1)) + 0;
				for (i = 0; i <= total + 1; i++) {
					if (fx == tail[i][0] && fy == tail[i][1]) {
						drawpass1 = 0;
						break;
					}
					else drawpass1 = 1;
				}
			} while ((last_fx == fx && last_fy == fy) || drawpass1 == 0 || drawpass2 == 0);
			draw_food(fx, fy);
			do {
				game_over = 0;
				length = total + 1;
				if (_kbhit()) { //control
					lastkey = ch;
					ch = _getch();
					if (lastkey != ch) {
						if (ch == 'w' && direction != 's') direction = 'w';//up
						if (ch == 'a' && direction != 'd') direction = 'a';//left
						if (ch == 's' && direction != 'w' && lastkey != ' ') direction = 's';//down
						if (ch == 'd' && direction != 'a') direction = 'd';//right
					}
				}
				if (y == 20 && direction == 's') y = -1;
				if (x == 40 && direction == 'd') x = -1;
				if (y == 0 && direction == 'w') y = 21;
				if (x == 0 && direction == 'a') x = 41;
				if (direction == 'a') {//moving direction
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = --x;
					tail[0][1] = y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 'd') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = ++x;
					tail[0][1] = y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 'w') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = x;
					tail[0][1] = --y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (direction == 's') {
					erase_background(tail[total][0], tail[total][1]);
					for (i = length; i > 0; i--) {
						for (j = 0; j < 2; j++) {
							tail[i][j] = tail[i - 1][j];
						}
					}
					tail[0][0] = x;
					tail[0][1] = ++y;
					draw_snake(x, y);
					for (i = 1; i < total + 1; i++) {
						if (x == tail[i][0] && y == tail[i][1]) {
							game_over++;
						}
					}
				}
				if (game_over == 1) {
					std::thread q(Beep, 400, 200);
					q.detach();
					gotoxy(12, 10);
					setcolor(15, 0);
					printf("G A M E  O V E R ");
					Sleep(3000);
					stage = 6;
					break;
				}
				fflush(stdin);
				if (cursor(fx, fy) == '#') {//check if the food was eaten
					std::thread q(Beep, 700, 100);
					q.detach();
					f++;
					do {
						fx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
						fy = (rand() % (19 - 0 + 1)) + 0;
						for (i = 0; i <= total + 1; i++) {
							if (fx == tail[i][0] && fy == tail[i][1]) {
								drawpass1 = 0;
								break;
							}
							else drawpass1 = 1;
						}
					} while ((last_fx == fx && last_fy == fy) || drawpass1 == 0 || drawpass2 == 0);
					draw_food(fx, fy);
					total++;
					score = score + 5;
					last_fx = fx;
					last_fy = fy;
				}
				if (f % 5 == 0 && f != 0) {//to create bonus food every 5 normal food
					do {
						bx = (rand() % (40 - 0 + 1)) + 0;//number = (rand() % (upper - lower + 1)) + lower
						by = (rand() % (19 - 0 + 1)) + 0;
						for (i = 0; i <= total + 1; i++) {
							if (bx == tail[i][0] && by == tail[i][1]) {
								drawpass1 = 0;
								break;
							}
							else drawpass1 = 1;
						}
					} while (drawpass1 == 0 || drawpass2 == 0);
					draw_bfood(bx, by);
					start = clock();
					f = 0;
				}
				end_bfood = clock();
				bfood_time = ((float)(end_bfood - start)) / CLOCKS_PER_SEC;
				if ((int)bfood_time == 5 && cursor(bx, by) == 'O') {
					erase_background(bx, by);
				}
				if (cursor(bx, by) == '#') {
					total++;
					end = clock();
					seconds = ((float)(end - start)) / CLOCKS_PER_SEC;
					if (seconds <= 1) bfood_score = 150;
					else if (seconds <= 2 && seconds > 1) bfood_score = 120;
					else if (seconds <= 3 && seconds > 2) bfood_score = 90;
					else if (seconds <= 4 && seconds > 3) bfood_score = 60;
					else if (seconds <= 5 && seconds > 4) bfood_score = 30;
					else if (seconds > 5) bfood_score = 0;
					score = score + bfood_score;//change 150 to bfood_score
					if (bfood_score != 0) {
						std::thread q(Beep, 700, 100);
						q.detach();
					}
					bx = 99;
					by = 99;
				}
				eat_status(1, 21);
				Sleep(50);
			} while (ch != 'x');
		}
		while (stage == 6) {
			FILE* fr;
			fr = fopen("highscore.txt", "r");
			for (i = 0; i < 5; i++) {
				fscanf(fr, "%s\t%d\n", p[i].n, &p[i].sc);
			}
			fclose(fr);
			for (i = 4; i >= 0; i--) {
				if (score <= p[i].sc) {
					i++;
					shiftat = i;
					break;
				}
			}
			for (i = 4; i >= shiftat; i--) {
				p[i + 1].sc = p[i].sc;
				strcpy(p[i + 1].n, p[i].n);
			}
			p[shiftat].sc = score;
			strcpy(p[shiftat].n, name);
			FILE* fp;
			fp = fopen("highscore.txt", "w");
			for (i = 0; i < 5; i++) {
				fprintf(fp, "%s\t", p[i].n);
				fprintf(fp, "%d\n", p[i].sc);
			}
			fclose(fp);
			for (i = 0; i < clear_x; i++) {//clear screen
				for (j = 0; j < clear_y; j++) {
					erase_background(i, j);
				}
			}
			stage = 7;
		}
		while (stage == 7) {
			setcolor(11, 0);
			gotoxy(12, 2);
			printf("YOUR SCORE : %d", score);
			setcolor(4, 0);
			gotoxy(15, 5);
			printf("HIGH SCORE");
			setcolor(7, 0);
			gotoxy(14, 7);
			if (p[0].sc != 0) 
				printf("%s\t%d",p[0].n, p[0].sc);
			gotoxy(14, 9);
			if (p[1].sc != 0) 
				printf("%s\t%d", p[1].n, p[1].sc);
			gotoxy(14, 11);
			if (p[2].sc != 0) 
				printf("%s\t%d", p[2].n, p[2].sc);
			gotoxy(14, 13);
			if (p[3].sc != 0) 
				printf("%s\t%d", p[3].n, p[3].sc);
			gotoxy(14, 15);
			if (p[4].sc != 0) 
				printf("%s\t%d", p[4].n, p[4].sc);
			gotoxy(8, 19);
			setcolor(8, 0);
			printf("PRESS X TO RETURN TO MENU");
			ch = _getch();
			if (ch == 'x') {
				stage = 0;
			}
		}
	} while (ch != 'p');
	return 0;
}