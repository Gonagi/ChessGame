#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define BACK 8

void titleDraw();
void gotoxy(int, int);
void cursorDraw(int, int);
void erase(int, int);
void chessDraw();
void chessRule();

int cursor_x = 13, cursor_y = 8;
char chessPos[8][8][2] = {"R","N","B","Q","K","B","N","R",
						  "P","P","P","P","P","P","P","P",
						  "*","*","*","*","*","*","*","*",
					  	  "*","*","*","*","*","*","*","*",
						  "*","*","*","*","*","*","*","*",
						  "*","*","*","*","*","*","*","*",
						  "P","P","P","P","P","P","P","P",
						  "R","N","B","Q","K","B","N","R"};

int main()
{
Main:
	system("cls");
	titleDraw();
	cursorDraw(cursor_x,cursor_y);

	while (1)
	{
		int key;
		key = _getch();

		if (cursor_y == 8 && key==ENTER)
		{
			system("cls");
			break;
		}
		else if (cursor_y == 9 && key == ENTER)
		{
			system("cls");
			chessRule();
			while (1) {
				key = _getch();
				switch (key)
				{
				case BACK:
					goto Main;
					break;
				default:
					continue;
				}
			}
		}
		else if (cursor_y == 10 && key == ENTER)
		{
			system("cls");
			gotoxy(3, 1);
			printf("게임을 종료합니다...\n");
			return 0;
		}

		switch (key)
		{
		case UP:
			if (cursor_y == 8)
				continue;
			cursor_y--;
			erase(cursor_x, cursor_y + 1);
			cursorDraw(cursor_x, cursor_y);
			break;

		case DOWN:
			if (cursor_y == 10)
				continue;
			else
			cursor_y++;
			erase(cursor_x, cursor_y - 1);
			cursorDraw(cursor_x, cursor_y);
			break;
		}
	}
	system("cls");
	chessDraw();
	cursor_x = 0, cursor_y = 0;
	gotoxy(cursor_x, cursor_y);
	while (1)
	{
		int key;
		key = _getch();
		switch (key)
		{
		case UP:
			if (cursor_y == 0)
				continue;
			cursor_y--;
			gotoxy(cursor_x, cursor_y);
			break;
		case DOWN:
			if (cursor_y == 7)
				continue;
			cursor_y++;
			gotoxy(cursor_x, cursor_y);
			break;
		case LEFT:
			if (cursor_x == 0)
				continue;
			cursor_x -= 2;
			gotoxy(cursor_x, cursor_y);
			break;
		case RIGHT:
			if (cursor_x == 14)
				continue;
			cursor_x+=2;
			gotoxy(cursor_x, cursor_y);
			break;
		case BACK:
			cursor_x = 13, cursor_y = 8;
			goto Main;
			break;
		default:
			continue;
		}
	}
	return 0;
}

void titleDraw()
{
	printf("\n");
	printf("    ####  #  #  ####   ###   ###   \n");
	printf("   #      #  #  #     #     #      \n");
	printf("   #      ####  ####   ###   ###   \n");
	printf("   #      #  #  #         #     #  \n");
	printf("    ###   #  #  ####   ###   ###   \n");
	printf("\n");
	printf("                 2조    \n");
	printf("              게임시작   \n");
	printf("              게임설명   \n");
	printf("              게임종료   \n");

}

void gotoxy(int x, int y)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleHandle, pos);
}

void cursorDraw(int x, int y)
{
	if (cursor_x == 13)
	{
		gotoxy(x, y);
		printf(">");
	}
}

void erase(int x, int y)
{
	gotoxy(x, y);
	printf(" ");
}

void chessDraw()
{
	int i, j,k;
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hC, &csbi);

	SetConsoleTextAttribute(hC, 112);
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
			for (k = 0; k < 2; k++)
				printf("%c", chessPos[i][j][k]);
		printf("\n");
	}

	SetConsoleTextAttribute(hC, 7);
	printf("-----------------------------\n");
	printf(" R : 룩, N : 나이트, B : 비숍\n");
	printf(" Q : 퀸, K : 킹, P : 폰\n");
	printf(" 메인화면으로 돌아가기 (Backspace)\n");
}

void chessRule()
{
	printf("----------------게임 방법---------------\n");
	printf("1. 백과 흑이 번갈아가며 플레이합니다.\n");
	printf("2. 백이 먼저 게임을 시작합니다.\n");
	printf("3.");
	printf("\n");
	printf("뒤로가기 (Backspace)");
}
