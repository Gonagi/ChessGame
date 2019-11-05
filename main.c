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

void titleDraw();
void gotoxy(int, int);
void cursorDraw(int, int);
void erase(int, int);
void chessDraw();
void chessRule();
void textcolor(int);
void backcolor(int);
void cursorPosNow(int, int);
void cursorPosDraw(int, int);
void pieceNowDraw();
void timecheck();
char invert(int);

int cursor_x = 13, cursor_y = 8;
enum color {red=12, yellow=14, white=7, grey=8};
char cursorPos[2];
char *chessPos[8][8] = {"R","N","B","Q","K","B","N","R",
                        "P","P","P","P","P","P","P","P",
                        " "," "," "," "," "," "," "," ",
                        " "," "," "," "," "," "," "," ",
                        " "," "," "," "," "," "," "," ",
                        " "," "," "," "," "," "," "," ",
                        "P","P","P","P","P","P","P","P",
                        "R","N","B","Q","K","B","N","R"};

int main()
{
    //system("mode con cols=40 lines=15"); //�ܼ�â ũ�� ���� cols :����, lines :����
Main:
    textcolor(white);
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
			gotoxy(3, 5);
			printf("������ �����մϴ�...\n");
			gotoxy(0,10);
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
	cursorPosNow(cursor_x,cursor_y);
	pieceNowDraw();
	cursorPosDraw(cursor_x,cursor_y);
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
        case SPACE:
            if(cursor_x)
		default:
			continue;
		}
		cursorPosNow(cursor_x,cursor_y);
		pieceNowDraw();
		cursorPosDraw(cursor_x,cursor_y);
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
	printf("                 2��    \n");
	printf("              ���ӽ���   \n");
	printf("              ���Ӽ���   \n");
	printf("              ��������   \n");

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


	for (i = 0; i < 8; i++)
	{
	    textcolor(white);
		for (j = 0; j < 8; j++)
			for (k = 0; k < 2; k++)
            {
                if(j%2==0&&i%2==0)
                    backcolor(112);
                else if(j%2!=0&&i%2==0)
                    backcolor(7);
                else if(j%2==0&&i%2!=0)
                    backcolor(7);
                else
                    backcolor(112);
                printf("%c", chessPos[i][j][k]);
            }
        textcolor(yellow);
        printf(" %d",i+1);
		printf("\n");
	}

	backcolor(7);
	textcolor(yellow);
	printf("a b c d e f g h\n");
	textcolor(white);
	printf("-----------------------------\n");
	printf(" R : ��, N : ����Ʈ, B : ���\n");
	printf(" Q : ��, K : ŷ, P : ��\n");
	textcolor(grey);
	printf("\n\n");
	printf(" ����ȭ������ ���ư��� (Backspace)\n");
}

void chessRule()
{
	printf("----------------���� ���---------------\n");
	printf("1. ��� ���� �����ư��� �÷����մϴ�.\n");
	printf("2. ���� ���� ������ �����մϴ�.\n");
	printf("3.  \n");
	printf("4.  \n");
	printf("\n");
	textcolor(grey);
	printf("�ڷΰ��� (Backspace)");
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
    printf("���� �� �� : %s",cursorPos);
    gotoxy(cursor_x,cursor_y);
}

void cursorPosDraw(int x, int y)
{
    textcolor(white);
    char x_pos=invert(x);
    int y_pos=y+1;
    gotoxy(20,1);
    printf("x�� ��ġ : %c, y�� ��ġ :%d",x_pos,y_pos);
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
void timecheck()
{
    void main()
    {
        int time = 30;
        int min = time / 60;
        int sec = time % 60;
        while(time > 0)
        {
            gotoxy(0, 20);
            printf("�����ð� = %2d:%2d\n", min, sec);
            Sleep(1000);
            time--;
            for (int i = 20; i<33; i++)
                erase(0, i);
        }
        system("cls");
        gotoxy(5, 5);
        printf("�ð��ʰ�\n");

    }
}
