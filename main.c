#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define BACK 8
#define SPACE 32
#define mainX 16
#define mainY 9

void titleDraw();
void gotoxy(int, int);
void cursorDraw(int, int);
void erase(int, int);
void chessDraw();
void chessRule();
void textcolor(int);
void backcolor(int);
void cursorPosNow(int, int);
void pieceNowDraw();
void timerDraw();
char invert(int);

int cursor_x = mainX, cursor_y = mainY;
enum color {red=12, yellow=14, white=7, grey=8};
char cursorPos[2];
char *chessPos[8][8] = {"BR","BN","BB","BQ","BK","BB","BN","BR",
                        "BP","BP","BP","BP","BP","BP","BP","BP",
                        "  ","  ","  ","  ","  ","  ","  ","  ",
                        "  ","  ","  ","  ","  ","  ","  ","  ",
                        "  ","  ","  ","  ","  ","  ","  ","  ",
                        "  ","  ","  ","  ","  ","  ","  ","  ",
                        "WP","WP","WP","WP","WP","WP","WP","WP",
                        "WR","WN","WB","WQ","WK","WB","WN","WR"};

int main()
{
    system("mode con cols=80 lines=20"); //콘솔창 크기 설정 cols :가로, lines :세로
Main:
    textcolor(white);
	system("cls");
	titleDraw();
	cursorDraw(cursor_x,cursor_y);

	while (1)
	{
		int key;
		key = _getch();

		if (cursor_y == mainY && key==ENTER)
		{
			system("cls");
			break;
		}
		else if (cursor_y == mainY+1 && key == ENTER)
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
		else if (cursor_y == mainY+2 && key == ENTER)
		{
			system("cls");
			gotoxy(3, 5);
			printf("게임을 종료합니다...\n");
			gotoxy(0,10);
			return 0;
		}

		switch (key)
		{
		case UP:
			if (cursor_y == mainY)
				continue;
			cursor_y--;
			erase(cursor_x, cursor_y + 1);
			cursorDraw(cursor_x, cursor_y);
			break;

		case DOWN:
			if (cursor_y == mainY+2)
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
	cursorPosNow(cursor_x,cursor_y);
	pieceNowDraw();
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
			cursor_x = mainX, cursor_y = mainY;
			goto Main;
			break;
        case SPACE:
            if(cursor_x)
		default:
			continue;
		}
		cursorPosNow(cursor_x,cursor_y);
		pieceNowDraw();
	}
	return 0;
}

void titleDraw()
{
	printf("\n\n");
	printf("       ####  #  #  ####   ###   ###   \n");
	printf("      #      #  #  #     #     #      \n");
	printf("      #      ####  ####   ###   ###   \n");
	printf("      #      #  #  #         #     #  \n");
	printf("       ###   #  #  ####   ###   ###   \n");
	printf("\n");
	printf("                     2조    \n");
	printf("                  게임시작   \n");
	printf("                  게임설명   \n");
	printf("                  게임종료   \n");

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
	if (cursor_x == 16)
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
	int i, j;


	for (i = 0; i < 8; i++)
	{
	    textcolor(white);
		for (j = 0; j < 8; j++)
        {
            char piece=chessPos[i][j];
            if(j%2==0&&i%2==0)
                backcolor(112);
            else if(j%2!=0&&i%2==0)
                backcolor(7);
            else if(j%2==0&&i%2!=0)
                backcolor(7);
            else
                backcolor(112);

            if(chessPos[i][j]=="BR")
                printf("R ");
            else if(chessPos[i][j]=="BN")
                printf("N ");
            else if(chessPos[i][j]=="BB")
                printf("B ");
            else if(chessPos[i][j]=="BQ")
                printf("Q ");
            else if(chessPos[i][j]=="BK")
                printf("K ");
            else if(chessPos[i][j]=="BP")
                printf("P ");
            else if(chessPos[i][j]=="WR")
                printf("R ");
            else if(chessPos[i][j]=="WN")
                printf("N ");
            else if(chessPos[i][j]=="WB")
                printf("B ");
            else if(chessPos[i][j]=="WQ")
                printf("Q ");
            else if(chessPos[i][j]=="WK")
                printf("K ");
            else if(chessPos[i][j]=="WP")
                printf("P ");
            else if(chessPos[i][j]=="  ")
                printf("  ");
        }
		printf("\n");
	}

	for(i=0; i<8; i++)
    {
        gotoxy(17,i);
        textcolor(yellow);
        printf("%d",i+1);
    }

	backcolor(7);
	textcolor(yellow);
	printf("\n");
	printf(" a b c d e f g h\n");
	textcolor(white);
	printf("-----------------------------\n");
	printf(" R : 룩, N : 나이트, B : 비숍\n");
	printf(" Q : 퀸, K : 킹, P : 폰\n");
	textcolor(grey);
	printf("\n\n");
	printf(" 메인화면으로 돌아가기 (Backspace)\n");
}

void chessRule()
{
	printf("----------------게임 방법---------------\n");
	printf("1. 백과 흑이 번갈아가며 플레이합니다.\n");
	printf("2. 백이 먼저 게임을 시작합니다.\n");
	printf("3.  \n");
	printf("4.  \n");
	printf("\n");
	textcolor(grey);
	printf("뒤로가기 (Backspace)");
}

void textcolor(int color_number)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color_number);
}

void backcolor(int color_number)
{
    HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hC, &csbi);

	SetConsoleTextAttribute(hC, color_number);
}

void cursorPosNow(int x, int y)
{
    int y_pos=x/2;
    int x_pos=y;
    strcpy(cursorPos,chessPos[x_pos][y_pos]);
}

void pieceNowDraw()
{
    textcolor(white);
    gotoxy(20, 4);

    printf("현재 고른 말 : %s",cursorPos);
    gotoxy(cursor_x,cursor_y);
}

char invert(int x)
{
    switch(x)
    {
    case 0:
        return 'a';
    case 2:
        return 'b';
    case 4:
        return 'c';
    case 6:
        return 'd';
    case 8:
        return 'e';
    case 10:
        return 'f';
    case 12:
        return 'g';
    case 14:
        return 'h';
    }
}

void timerDraw()
{
    int time = 300;
    int min=time/60;
    int sec=time%60;
    while(1)
    {
        min=time/60;
        sec=time%60;
        gotoxy(20,0);
        printf("남은시간 = %2d:%2d",min,sec);
        gotoxy(cursor_x,cursor_y);
        Sleep(1000);
        time--;

        if(time<=0)
        {
        system("cls");
        gotoxy(7,4);
        printf("시간초과");
        break;
        }
    }
}
