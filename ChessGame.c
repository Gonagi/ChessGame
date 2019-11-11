#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>
#include <process.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define BACK 8
#define SPACE 32
#define mainX 24
#define mainY 10
#define WHITE 'W'
#define BLACK 'B'
#define NONE 'N'
<<<<<<< Updated upstream
#define SET_TIME (10 + 1)
=======
#define SET_TIME (300 + 1)
>>>>>>> Stashed changes

void titleDraw();   //Ÿ��Ʋ ȭ�� ���
void gotoxy(int, int);  //�ֳܼ� ��ǥ �̵�
void cursorDraw(int, int);  //Ŀ�� '>' ���
void erase(int, int);   //�ش� ��ǥ �����
void chessDraw();   //ü���� ���
void chessRule();   //ü�� ��Ģ ������ ���
void controlRule(); //���� ������ ���
void textcolor(int);    //�ֳܼ� �۾� ���� ����
void backcolor(int);    //�ֳܼ� ���� ���� ����
void pieceNowDraw();    //���õ� �� ǥ��
void pieceDeadDraw();   //���� �� ǥ��
void pieceMove(int, int);   //�� �̵� �Լ�
void gameInit();    //���� �ʱ�ȭ
void wrongMove_Alert(); //�߸��� ������ ���
void wrongOrder_Alert();    //���� ���
void whiteWin_Alert();
void blackWin_Alert();
void orderDraw();   //���� ǥ��
void gameCheck();   //���� �� üũ ���� Ȯ��
void sooDraw();     //�� ������ ǥ��
void CursorView(char); //Ŀ�� ǥ�� �Լ�
<<<<<<< Updated upstream

=======
void pieceSelect(int, int);
>>>>>>> Stashed changes
unsigned _stdcall timerDraw(void *);  //�����带 �̿��� Ÿ�̸� �Լ�
void timer_Alert();

int cursor_x = mainX, cursor_y = mainY;
<<<<<<< Updated upstream
int sel_x, sel_y;
char sel_color = NONE;
int pieceSel_check = 0;
int order = 0;    //¦���� ��, Ȧ���� ��
int check_error = 0;
int check_gameend = 0;  //1�̸� ���� ��, 2�� ���� ��
=======
int sel_x, sel_y;   //�����̽��ٷ� ����� ����� x y ��ǥ
char sel_color = NONE;
char *sel_piece;
int pieceSel_check = 0;
int order = 0;    //¦���� ��, Ȧ���� ��
int check_error = 0;
int check_winner = 0;  //1�̸� ���� ��, 2�� ���� ��
>>>>>>> Stashed changes
int soo = 1;
int check_rule = 0;
int bk_live = 0, wk_live = 0;
int bq_live = 0, wq_live = 0;
int br_live = 0, wr_live = 0;
int bn_live = 0, wn_live = 0;
int bb_live = 0, wb_live = 0;
int bp_live = 0, wp_live = 0;
int time = SET_TIME;
<<<<<<< Updated upstream

enum color { red = 12, yellow = 14, white = 7, grey = 8 };
char *chessPos[8][8] = { {"BR","BN","BB","BQ","BK","BB","BN","BR"},
						{"BP","BP","BP","BP","BP","BP","BP","BP"},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"WP","WP","WP","WP","WP","WP","WP","WP"},
						{"WR","WN","WB","WQ","WK","WB","WN","WR"} };

=======

enum color { red = 12, yellow = 14, white = 7, grey = 8 };
char *chessPos[8][8] = { {"BR1","BN1","BB1","BQ","BK","BB2","BN2","BR2"},
						{"BP1","BP2","BP3","BP4","BP5","BP6","BP7","BP8"},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"WP1","WP2","WP3","WP4","WP5","WP6","WP7","WP8"},
						{"WR1","WN1","WB1","WQ","WK","WB2","WN2","WR2"} };

>>>>>>> Stashed changes
char *chessPiece[13] = { "  ","BR","BN","BB","BQ","BK","BP","WR","WN","WB","WQ","WK","WP" };


int main()
{
	system("mode con cols=67 lines=18 | title ChessGame_Team2");
Main:
    time=-2;
	CursorView(0);
	textcolor(white);
	system("cls");
	titleDraw();
	cursorDraw(cursor_x, cursor_y);

	while (1)
	{
		int key;
		key = _getch();

		if (cursor_y == mainY && key == ENTER)
		{
			system("cls");
			break;
		}
		else if (cursor_y == mainY + 1 && key == ENTER)
		{
		Rule:
			system("cls");
			cursor_x = 18, cursor_y = 6;
			check_rule = 0;
			gotoxy(18, 4);
			printf("� ������ �����ðڽ��ϱ�?");
			gotoxy(20, 6);
			printf("���� ���� ��Ģ");
			gotoxy(20, 7);
			printf("ü�� ��Ģ");
			textcolor(grey);
			gotoxy(0, 16);
			printf("����ȭ������ ���� (Backspace)");
			textcolor(white);
			cursorDraw(cursor_x, cursor_y);
			while (1) {
				key = _getch();

				if (cursor_y == 6 && key == ENTER)
				{
					system("cls");
					controlRule();
				}
				else if (cursor_y == 7 && key == ENTER)
				{
					system("cls");
					chessRule();
				}
				switch (key)
				{
				case UP:
					if (cursor_y == 6)
						continue;
					else if (check_rule == 1)
						continue;
					cursor_y--;
					erase(cursor_x, cursor_y + 1);
					cursorDraw(cursor_x, cursor_y);
					break;

				case DOWN:
					if (cursor_y == 7)
						continue;
					else if (check_rule == 1)
						continue;
					else
						cursor_y++;
					erase(cursor_x, cursor_y - 1);
					cursorDraw(cursor_x, cursor_y);
					break;
				case BACK:
					if (check_rule == 1)
						goto Rule;
					cursor_x = mainX, cursor_y = mainY + 1;
					goto Main;
					break;
				default:
					continue;
				}
			}
		}
		else if (cursor_y == mainY + 2 && key == ENTER)
		{
			system("cls");
			gotoxy(10, 10);
			printf("������ �����մϴ�...\n");
			gotoxy(0, 17);
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
			if (cursor_y == mainY + 2)
				continue;
			else
				cursor_y++;
			erase(cursor_x, cursor_y - 1);
			cursorDraw(cursor_x, cursor_y);
			break;
		}
	}

	gameInit();
	gameCheck();
	while (1)
	{
<<<<<<< Updated upstream
	    if(check_gameend==1)
=======
		CursorView(0);
		gameCheck();
		   if(check_winner==1)
>>>>>>> Stashed changes
        {
            whiteWin_Alert();
            Sleep(1000);
            break;
        }
<<<<<<< Updated upstream
        else if(check_gameend==2)
=======
        else if(check_winner==2)
>>>>>>> Stashed changes
        {
            blackWin_Alert();
            Sleep(1000);
            break;
        }
<<<<<<< Updated upstream
		CursorView(0);
		gameCheck();
=======
>>>>>>> Stashed changes
		pieceDeadDraw();
		orderDraw();
		sooDraw();
		CursorView(1);

		//gotoxy(20,6);
		//printf("Test %d",check_gameend);  //������ Ȯ�ο� (���� ��)
		gotoxy(cursor_x, cursor_y);

<<<<<<< Updated upstream
		int y_pos = cursor_x / 2;
=======
		int y_pos = cursor_x / 2;   //Ŀ�� ��ġ�� �迭 �� ���� ��ȯ
>>>>>>> Stashed changes
		int x_pos = cursor_y;
		int key;
		key = _getch();

		if (check_error == 1)
			break;

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
			cursor_x += 2;
			gotoxy(cursor_x, cursor_y);
			break;
		case BACK:
			cursor_x = mainX, cursor_y = mainY;
			gameInit();
			cursor_x = mainX, cursor_y = mainY;
			goto Main;
			break;
		case SPACE:
<<<<<<< Updated upstream
			if (pieceSel_check == 0 && chessPos[x_pos][y_pos] != chessPiece[0])
=======
			if (pieceSel_check == 0 && chessPos[x_pos][y_pos] != chessPiece[0]) //�� ������ ���߰� ������ ĭ�� ���� ��
>>>>>>> Stashed changes
			{
				if (chessPos[x_pos][y_pos][0] == 'B')
					sel_color = BLACK;
				else
					sel_color = WHITE;

<<<<<<< Updated upstream
				if ((order % 2 == 0 && sel_color == BLACK) || (order % 2 != 0 && sel_color == WHITE))
=======
				if ((order % 2 == 0 && sel_color == BLACK) || (order % 2 != 0 && sel_color == WHITE))   //���ʰ� ���ε� ���� ���� �� ���, ���ʰ� ���ε� �� ���� �� ���
>>>>>>> Stashed changes
				{
					wrongOrder_Alert();
					gotoxy(20, 2);
					printf("���� �� �� : ����       ");
				}
				else {
<<<<<<< Updated upstream
=======
                    pieceSelect(x_pos, y_pos);
>>>>>>> Stashed changes
					pieceNowDraw();
					gotoxy(1, 10);
					printf("                        ");
					pieceSel_check = 1;
					sel_x = x_pos, sel_y = y_pos;
				}
			}
<<<<<<< Updated upstream
			else if (pieceSel_check == 0 && chessPos[x_pos][y_pos] == chessPiece[0])
=======
			else if (pieceSel_check == 0 && chessPos[x_pos][y_pos] == chessPiece[0])    //�� ������ ���߰� ������ ĭ�� ��ĭ�� ��
>>>>>>> Stashed changes
			{
				gotoxy(1, 10);
				printf("                        ");
				gotoxy(cursor_x, cursor_y);
				pieceSel_check = 0;
<<<<<<< Updated upstream
				pieceNowDraw();
			}
			else if (pieceSel_check == 1 && chessPos[x_pos][y_pos] != chessPiece[0])
=======
				pieceSelect(x_pos, y_pos);
				pieceNowDraw();
			}
			else if (pieceSel_check == 1 && chessPos[x_pos][y_pos] != chessPiece[0])    //�� ������ �߰� ������ ĭ�� ���� ��
>>>>>>> Stashed changes
			{
				if (sel_color != chessPos[x_pos][y_pos][0])
				{
					pieceMove(cursor_x, cursor_y);
				}
				else if (sel_color == chessPos[x_pos][y_pos][0] && (sel_x != x_pos || sel_y != y_pos))
				{
					wrongMove_Alert();
					gotoxy(20, 2);
					printf("���� �� �� : ����       ");
					pieceSel_check = 0;
				}
<<<<<<< Updated upstream
				else if (sel_color == chessPos[x_pos][y_pos][0] && sel_x == x_pos && sel_y == y_pos)
=======
				else if (sel_color == chessPos[x_pos][y_pos][0] && sel_x == x_pos && sel_y == y_pos)    //�� ������ �߰� �ٽ� �ڱ� �ڽ��� ������ ��
>>>>>>> Stashed changes
				{
					gotoxy(1, 10);
					printf("                        ");
					gotoxy(20, 2);
					printf("���� �� �� : ����       ");
					gotoxy(cursor_x, cursor_y);
					pieceSel_check = 0;
				}
			}
<<<<<<< Updated upstream
			else if (pieceSel_check == 1 && chessPos[x_pos][y_pos] == chessPiece[0])
=======
			else if (pieceSel_check == 1 && chessPos[x_pos][y_pos] == chessPiece[0])    //�� ������ �߰� ������ ĭ�� ��ĭ�� ��
>>>>>>> Stashed changes
			{
				gotoxy(1, 10);
				printf("                        ");
				gotoxy(cursor_x, cursor_y);
				pieceMove(cursor_x, cursor_y);
			}
		default:
			continue;
		}
	}
	gotoxy(0, 17);
	return 0;
}

void titleDraw()
{
	printf("\n\n\n");
	printf("                ####  #  #  ####   ###   ###   \n");
	printf("               #      #  #  #     #     #      \n");
	printf("               #      ####  ####   ###   ###   \n");
	printf("               #      #  #  #         #     #  \n");
	printf("                ###   #  #  ####   ###   ###   \n");
	printf("\n");
	printf("                             2��    \n");
	printf("                          ���ӽ���   \n");
	printf("                          ���Ӽ���   \n");
	printf("                          ��������   \n");

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
	gotoxy(x, y);
	printf(">");
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
		for (j = 0; j < 8; j++)
		{
			if (j % 2 == 0 && i % 2 == 0)
				backcolor(112);
			else if (j % 2 != 0 && i % 2 == 0)
				backcolor(128);
			else if (j % 2 == 0 && i % 2 != 0)
				backcolor(128);
			else
				backcolor(112);

<<<<<<< Updated upstream
			if (chessPos[i][j] == chessPiece[1])
				printf("��");
			else if (chessPos[i][j] == chessPiece[2])
				printf("��");
			else if (chessPos[i][j] == chessPiece[3])
				printf("��");
			else if (chessPos[i][j] == chessPiece[4])
				printf("��");
			else if (chessPos[i][j] == chessPiece[5])
				printf("��");
			else if (chessPos[i][j] == chessPiece[6])
				printf("��");
			else if (chessPos[i][j] == chessPiece[7])
				printf("��");
			else if (chessPos[i][j] == chessPiece[8])
				printf("��");
			else if (chessPos[i][j] == chessPiece[9])
				printf("��");
			else if (chessPos[i][j] == chessPiece[10])
				printf("��");
			else if (chessPos[i][j] == chessPiece[11])
				printf("��");
			else if (chessPos[i][j] == chessPiece[12])
=======
			if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'R')
				printf("��");
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'N')
				printf("��");
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'B')
				printf("��");
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'Q')
				printf("��");
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'K')
				printf("��");
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'P')
				printf("��");
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'R')
				printf("��");
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'N')
				printf("��");
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'B')
				printf("��");
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'Q')
				printf("��");
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'K')
				printf("��");
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'P')
>>>>>>> Stashed changes
				printf("��");
			else if (chessPos[i][j] == chessPiece[0])
				printf("  ");
		}
		printf("\n");
	}

	for (i = 0; i < 8; i++)
	{
		gotoxy(17, i);
		textcolor(yellow);
		printf("%d", i + 1);
		textcolor(white);
	}

	backcolor(7);
	textcolor(yellow);
	printf("\n");
	printf(" a b c d e f g h\n");
	textcolor(white);
	printf("------------���â---------------------\n");
	printf("\n");
	printf("---------------------------------------\n");
	printf(" �� : ��, �� : ����Ʈ, �� : ���\n");
	printf(" �� : ��, �� : ŷ, �� : ��\n");
	textcolor(grey);
	gotoxy(0, 16);
	printf("����ȭ������ ���ư��� (Backspace)\n");
	textcolor(white);

	for (i = 0; i < 14; i++)
	{
		gotoxy(49, i);
		printf("|");
	}
}

void chessRule()
{
	check_rule = 1;
	textcolor(white);
	printf("----------------���� ���---------------\n");
	printf("1. ��� ���� �����ư��� �÷����մϴ�.\n");
	printf("2. ���� ���� ������ �����մϴ�.\n");
	printf("3.  \n");
	printf("4.  \n");
	printf("\n");
	textcolor(grey);
	gotoxy(0, 16);
	printf("���ư��� (Backspace)");
	textcolor(white);
}

void controlRule()
{
	check_rule = 1;
	textcolor(white);
	printf("-------------------���� ���------------------\n\n");
	printf("1. Ű���� ����Ű�� �����Ͽ� Ŀ���� �̵��մϴ�.\n\n");
	printf("2. �̵��ϰ��� �ϴ� ���� Ŀ���� �̵���Ų ��\n");
	printf("   �����̽� �ٸ� �Է��Ͽ� ���� �����մϴ�.\n\n");
	printf("3. ���õ� ���� ���ϴ� ��ġ�� Ŀ���� �̵���Ų\n");
	printf("   �� �����̽� �ٸ� �Է� �Ͽ� �̵���ŵ�ϴ�.\n\n");
	printf("4. �� ������ ����ϰ� ���� ��� �ش� ���� \n");
	printf("   Ŀ���� �ø��� �����̽� �ٸ� �ѹ� �� ������\n");
	printf("   ����մϴ�.\n\n");
	textcolor(grey);
	gotoxy(0, 16);
	printf("���ư��� (Backspace)");
	textcolor(white);
}
void textcolor(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}

void backcolor(int color_number)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hC, &csbi);

	SetConsoleTextAttribute(hC, color_number);
}

<<<<<<< Updated upstream
void pieceNowDraw(int x, int y)
{
	int y_pos = x / 2;
	int x_pos = y;
	gotoxy(20, 2);
	printf("                         ");
	gotoxy(20, 2);
	if (chessPos[x_pos][y_pos] == chessPiece[1])
		printf("���� �� �� : ��� ��");
	else if (chessPos[x_pos][y_pos] == chessPiece[2])
		printf("���� �� �� : ��� ����Ʈ");
	else if (chessPos[x_pos][y_pos] == chessPiece[3])
		printf("���� �� �� : ��� ���");
	else if (chessPos[x_pos][y_pos] == chessPiece[4])
		printf("���� �� �� : ��� ��");
	else if (chessPos[x_pos][y_pos] == chessPiece[5])
		printf("���� �� �� : ��� ŷ");
	else if (chessPos[x_pos][y_pos] == chessPiece[6])
		printf("���� �� �� : ��� ��");
	else if (chessPos[x_pos][y_pos] == chessPiece[7])
		printf("���� �� �� : ��� ��");
	else if (chessPos[x_pos][y_pos] == chessPiece[8])
		printf("���� �� �� : ��� ����Ʈ");
	else if (chessPos[x_pos][y_pos] == chessPiece[9])
		printf("���� �� �� : ��� ���");
	else if (chessPos[x_pos][y_pos] == chessPiece[10])
		printf("���� �� �� : ��� ��");
	else if (chessPos[x_pos][y_pos] == chessPiece[11])
		printf("���� �� �� : ��� ŷ");
	else if (chessPos[x_pos][y_pos] == chessPiece[12])
=======
void pieceNowDraw()
{
	gotoxy(20, 2);
	printf("                         ");
	gotoxy(20, 2);
	if (sel_piece == "BR")
		printf("���� �� �� : ��� ��");
	else if (sel_piece == "BN")
		printf("���� �� �� : ��� ����Ʈ");
	else if (sel_piece == "BB")
		printf("���� �� �� : ��� ���");
	else if (sel_piece == "BQ")
		printf("���� �� �� : ��� ��");
	else if (sel_piece == "BK")
		printf("���� �� �� : ��� ŷ");
	else if (sel_piece == "BP")
		printf("���� �� �� : ��� ��");
	else if (sel_piece == "WR")
		printf("���� �� �� : ��� ��");
	else if (sel_piece == "WN")
		printf("���� �� �� : ��� ����Ʈ");
	else if (sel_piece == "WB")
		printf("���� �� �� : ��� ���");
	else if (sel_piece == "WQ")
		printf("���� �� �� : ��� ��");
	else if (sel_piece == "WK")
		printf("���� �� �� : ��� ŷ");
	else if (sel_piece == "WP")
>>>>>>> Stashed changes
		printf("���� �� �� : ��� ��");
	else
		printf("���� �� �� : ����");

	gotoxy(cursor_x, cursor_y);
}

void pieceDeadDraw()
{
	int i;

	gotoxy(55, 0);
	printf("���� ��");
	gotoxy(50, 1);
	printf("------����------");
	gotoxy(51, 2);
	for (i = 0; i < 8 - wp_live; i++)
		printf("��");
	gotoxy(51, 3);
	for (i = 0; i < 2 - wr_live; i++)
		printf("��");
	gotoxy(56, 3);
	for (i = 0; i < 2 - wn_live; i++)
		printf("��");
	gotoxy(61, 3);
	for (i = 0; i < 2 - wb_live; i++)
		printf("��");
	gotoxy(51, 4);
	if (wq_live == 0)
		printf("��");
	gotoxy(54, 4);
	if (wk_live == 0)
		printf("��");

	gotoxy(50, 7);
	printf("------����------");
	gotoxy(51, 8);
	for (i = 0; i < 8 - bp_live; i++)
		printf("��");
	gotoxy(51, 9);
	for (i = 0; i < 2 - br_live; i++)
		printf("��");
	gotoxy(56, 9);
	for (i = 0; i < 2 - bn_live; i++)
		printf("��");
	gotoxy(61, 9);
	for (i = 0; i < 2 - bb_live; i++)
		printf("��");
	gotoxy(51, 10);
	if (bq_live == 0)
		printf("��");
	gotoxy(54, 10);
	if (bk_live == 0)
		printf("��");
<<<<<<< Updated upstream

=======
    gotoxy(cursor_x,cursor_y);
>>>>>>> Stashed changes
}

void orderDraw()
{
	char *order_str;
	if (order % 2 == 0)
		order_str = "��";
	else
		order_str = "��";
	gotoxy(20, 0);
	printf("���� ���� : %s", order_str);
	gotoxy(cursor_x, cursor_y);
}

void sooDraw()
{
	gotoxy(20, 4);
	printf("�� %d��", soo);
	gotoxy(cursor_x, cursor_y);
}
void gameInit()
{
<<<<<<< Updated upstream
=======
    time=SET_TIME;
>>>>>>> Stashed changes
    _beginthreadex(NULL,0,timerDraw,0,0,NULL);
	int i;
	system("cls");
	chessDraw();
	textcolor(white);
	cursor_x = 4, cursor_y = 4;
	pieceSel_check = 0;
<<<<<<< Updated upstream
	time=SET_TIME;
	gotoxy(20, 2);
	printf("���� �� �� : ����");
	chessPos[0][0] = "BR";
	chessPos[0][1] = "BN";
	chessPos[0][2] = "BB";
	chessPos[0][3] = "BQ";
	chessPos[0][4] = "BK";
	chessPos[0][5] = "BB";
	chessPos[0][6] = "BN";
	chessPos[0][7] = "BR";
	for (i = 0; i <= 7; i++)
	{
		chessPos[1][i] = "BP";
=======
	check_winner=0;
	order=0;
	soo=1;
	gotoxy(20, 2);
	printf("���� �� �� : ����");
	chessPos[0][0] = "BR1";
	chessPos[0][1] = "BN1";
	chessPos[0][2] = "BB1";
	chessPos[0][3] = "BQ";
	chessPos[0][4] = "BK";
	chessPos[0][5] = "BB2";
	chessPos[0][6] = "BN2";
	chessPos[0][7] = "BR2";
	chessPos[1][0] = "BP1";
	chessPos[1][1] = "BP2";
	chessPos[1][2] = "BP3";
	chessPos[1][3] = "BP4";
	chessPos[1][4] = "BP5";
	chessPos[1][5] = "BP6";
	chessPos[1][6] = "BP7";
	chessPos[1][7] = "BP8";
	for (i = 0; i <= 7; i++)
	{
>>>>>>> Stashed changes
		chessPos[2][i] = "  ";
		chessPos[3][i] = "  ";
		chessPos[4][i] = "  ";
		chessPos[5][i] = "  ";
<<<<<<< Updated upstream
		chessPos[6][i] = "WP";
	}
	chessPos[7][0] = "WR";
	chessPos[7][1] = "WN";
	chessPos[7][2] = "WB";
	chessPos[7][3] = "WQ";
	chessPos[7][4] = "WK";
	chessPos[7][5] = "WB";
	chessPos[7][6] = "WN";
	chessPos[7][7] = "WR";

	gotoxy(cursor_x, cursor_y);
}

void gameCheck()
{
	bk_live = 0, wk_live = 0;
	bq_live = 0, wq_live = 0;
	br_live = 0, wr_live = 0;
	bn_live = 0, wn_live = 0;
	bb_live = 0, wb_live = 0;
	bp_live = 0, wp_live = 0;
	int i, j;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
		{
			if (chessPos[i][j] == chessPiece[11])
				wk_live++;
			else if (chessPos[i][j] == chessPiece[5])
				bk_live++;
			else if (chessPos[i][j] == chessPiece[10])
				wq_live++;
			else if (chessPos[i][j] == chessPiece[4])
				bq_live++;
			else if (chessPos[i][j] == chessPiece[7])
				wr_live++;
			else if (chessPos[i][j] == chessPiece[1])
				br_live++;
			else if (chessPos[i][j] == chessPiece[8])
				wn_live++;
			else if (chessPos[i][j] == chessPiece[2])
				bn_live++;
			else if (chessPos[i][j] == chessPiece[9])
				wb_live++;
			else if (chessPos[i][j] == chessPiece[3])
				bb_live++;
			else if (chessPos[i][j] == chessPiece[12])
				wp_live++;
			else if (chessPos[i][j] == chessPiece[6])
				bp_live++;
		}

	if (bk_live == 0 && wk_live == 1)
	{
		whiteWin_Alert();
	}
	else if (bk_live == 1 && wk_live == 0)
	{
		blackWin_Alert();
	}
	else if (bk_live == 0 && wk_live == 0)
	{
		gotoxy(1, 10);
		printf("                        ");
		gotoxy(1, 10);
		textcolor(red);
		printf("ġ���� ���� �߻�");
		check_error = 1;
		textcolor(white);
	}
}
void pieceMove(int x, int y)
{
	chessPos[y][x / 2] = chessPos[sel_x][sel_y];
	chessPos[sel_x][sel_y] = "  ";
	system("cls");
	chessDraw();
	pieceSel_check = 0;
	if (order % 2 != 0)
		soo++;
	order++;
	sel_color = NONE;
	time=SET_TIME;
	gotoxy(20, 2);
	printf("���� �� �� : ����");
}

void wrongMove_Alert()
{
	gotoxy(1, 10);
	printf("                        ");
	gotoxy(1, 10);
	textcolor(red);
	printf("�߸��� �̵��Դϴ�.");
	sel_color = NONE;
	textcolor(white);
	gotoxy(cursor_x, cursor_y);
}

void wrongOrder_Alert()
{
	gotoxy(1, 10);
	printf("                        ");
	gotoxy(1, 10);
	textcolor(red);
	printf("���ʰ� �ƴմϴ�.");
	sel_color = NONE;
	textcolor(white);
	gotoxy(cursor_x, cursor_y);
}

void whiteWin_Alert()
{
    gotoxy(1, 10);
    printf("                        ");
    gotoxy(1, 10);
    textcolor(red);
    printf("���� �¸�!");
    textcolor(white);
}

void blackWin_Alert()
{
    gotoxy(1, 10);
    printf("                        ");
    gotoxy(1, 10);
    textcolor(red);
    printf("���� �¸�!");
    textcolor(white);
}
void timer_Alert()
{
    gotoxy(1, 10);
	printf("                        ");
	gotoxy(1, 10);
	textcolor(red);
	if(order%2==0)
    {
        printf("���� �ð��ʰ�.");
        textcolor(white);
        Sleep(2000);
        check_gameend = 2;
    }
    else
    {
        printf("���� �ð��ʰ�.");
        textcolor(white);
        Sleep(2000);
        check_gameend = 1;
    }
	textcolor(white);
	gotoxy(cursor_x, cursor_y);
}
void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

unsigned _stdcall timerDraw(void *arg)
{
	int min, sec;
	while(1)
	{
		min=time/60;
		sec=time%60;
		CursorView(0);
		gotoxy(20,6);
		printf("�����ð� = %2d:%2d",min,sec);
		gotoxy(cursor_x,cursor_y);
		CursorView(1);
		Sleep(1000);
		time--;
		if(time==-1)
        {
        timer_Alert();
        break;
        }
        else if(time<-1)
        break;
	}
=======
	}
	chessPos[6][0] = "WP1";
	chessPos[6][1] = "WP2";
	chessPos[6][2] = "WP3";
	chessPos[6][3] = "WP4";
	chessPos[6][4] = "WP5";
	chessPos[6][5] = "WP6";
	chessPos[6][6] = "WP7";
	chessPos[6][7] = "WP8";
	chessPos[7][0] = "WR1";
	chessPos[7][1] = "WN1";
	chessPos[7][2] = "WB1";
	chessPos[7][3] = "WQ";
	chessPos[7][4] = "WK";
	chessPos[7][5] = "WB2";
	chessPos[7][6] = "WN2";
	chessPos[7][7] = "WR2";

	gotoxy(cursor_x, cursor_y);
}

void gameCheck()
{
	bk_live = 0, wk_live = 0;
	bq_live = 0, wq_live = 0;
	br_live = 0, wr_live = 0;
	bn_live = 0, wn_live = 0;
	bb_live = 0, wb_live = 0;
	bp_live = 0, wp_live = 0;
	int i, j;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
		{
			if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'K')
				wk_live++;
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'K')
				bk_live++;
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'Q')
				wq_live++;
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'Q')
				bq_live++;
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'R')
				wr_live++;
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'R')
				br_live++;
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'N')
				wn_live++;
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'N')
				bn_live++;
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'B')
				wb_live++;
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'B')
				bb_live++;
			else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'P')
				wp_live++;
			else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'P')
				bp_live++;
		}

	if (bk_live == 0 && wk_live == 1)
	{
		check_winner=1;
	}
	else if (bk_live == 1 && wk_live == 0)
	{
		check_winner=2;
	}
	else if (bk_live == 0 && wk_live == 0)
	{
		gotoxy(1, 10);
		printf("                        ");
		gotoxy(1, 10);
		textcolor(red);
		printf("ġ���� ���� �߻�");
		check_error = 1;
		textcolor(white);
	}
}
void pieceMove(int x, int y)
{
	chessPos[y][x / 2] = chessPos[sel_x][sel_y];
	chessPos[sel_x][sel_y] = "  ";
	system("cls");
	chessDraw();
	pieceSel_check = 0;
	if (order % 2 != 0)
		soo++;
	order++;
	sel_color = NONE;
	time=SET_TIME;
	gotoxy(20, 2);
	printf("���� �� �� : ����");
}

void wrongMove_Alert()
{
	gotoxy(1, 10);
	printf("                        ");
	gotoxy(1, 10);
	textcolor(red);
	printf("�߸��� �̵��Դϴ�.");
	sel_color = NONE;
	textcolor(white);
	gotoxy(cursor_x, cursor_y);
}

void wrongOrder_Alert()
{
	gotoxy(1, 10);
	printf("                        ");
	gotoxy(1, 10);
	textcolor(red);
	printf("���ʰ� �ƴմϴ�.");
	sel_color = NONE;
	textcolor(white);
	gotoxy(cursor_x, cursor_y);
}

void whiteWin_Alert()
{
    gotoxy(1, 10);
    printf("                        ");
    gotoxy(1, 10);
    textcolor(red);
    printf("���� �¸�!");
    textcolor(white);
}

void blackWin_Alert()
{
    gotoxy(1, 10);
    printf("                        ");
    gotoxy(1, 10);
    textcolor(red);
    printf("���� �¸�!");
    textcolor(white);
}
void timer_Alert()
{
    gotoxy(1, 10);
	printf("                        ");
	gotoxy(1, 10);
	textcolor(red);
	if(order%2==0)
    {
        printf("���� �ð��ʰ�.");
        textcolor(white);
        Sleep(2000);
        check_winner = 2;
    }
    else
    {
        printf("���� �ð��ʰ�.");
        textcolor(white);
        Sleep(2000);
        check_winner = 1;
    }
	textcolor(white);
	gotoxy(cursor_x, cursor_y);
}
void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void pieceSelect(int x, int y)
{
    int y_pos = y;
	int x_pos = x;
	if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'R')
		sel_piece = "BR";
	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'N')
		sel_piece = "BN";
	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'B')
		sel_piece = "BB";
	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'Q')
		sel_piece = "BQ";
	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'K')
		sel_piece = "BK";
	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'P')
		sel_piece = "BP";
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'R')
		sel_piece = "WR";
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'N')
		sel_piece = "WN";
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'B')
		sel_piece = "WB";
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'Q')
		sel_piece = "WQ";
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'K')
		sel_piece = "WK";
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'P')
		sel_piece = "WP";
	else
		sel_piece = "  ";

}

unsigned _stdcall timerDraw(void *arg)
{
	int min, sec;
	while(1)
	{
		min=time/60;
		sec=time%60;
		CursorView(0);
		gotoxy(20,6);
		printf("�����ð� = %2d:%2d",min,sec);
		gotoxy(cursor_x,cursor_y);
		CursorView(1);
		Sleep(1000);
		time--;
		if(time==-1)
        {
        timer_Alert();
        break;
        }
        else if(time<-1)
        break;
	}
>>>>>>> Stashed changes
	_endthreadex(0);
	return 0;
}
