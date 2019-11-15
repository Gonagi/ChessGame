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
#define SET_TIME (300 + 1)

void titleDraw();   //Ÿ��Ʋ ȭ�� ���
void chessDraw();   //ü���� ���
void pieceNowDraw();    //���õ� �� ǥ��
void pieceDeadDraw();   //���� �� ǥ��
void cursorDraw(int, int);  //Ŀ�� '>' ���
void pieceDraw(int, int);
void orderDraw();   //���� ǥ��
void sooDraw();     //�� ������ ǥ��
void chessRule();   //ü�� ��Ģ ������ ���
void controlRule(); //���� ������ ���
unsigned _stdcall timerDraw(void *);  //�����带 �̿��� Ÿ�̸� �Լ�

void pieceMove(int, int);   //�� �̵� �Լ�
void pieceSelect(int, int);
void gameInit();    //���� �ʱ�ȭ
void gameCheck();   //���� �� üũ ���� Ȯ��
void turnPiece(int, int);

void whitePawn(int, int);   // ��� ��
void blackPawn(int, int);   // ��� ��
void whiteNight(int, int);  // ��� ����Ʈ
void blackNight(int, int);  // ��� ����Ʈ

void whitePromotion(int);
void blackPromotion(int);

void wrongMove_Alert(); //�߸��� ������ ���
void wrongOrder_Alert();    //���� ���
void whiteWin_Alert();
void blackWin_Alert();
void timer_Alert();

void CursorView(char); //Ŀ�� ���� �Լ�
void gotoxy(int, int);  //�ֳܼ� ��ǥ �̵�
void erase(int, int);   //�ش� ��ǥ �����
char invert(int);
void textcolor(int);    //�ֳܼ� �۾� ���� ����
void backcolor(int);    //�ֳܼ� ���� ���� ����

int cursor_x = mainX, cursor_y = mainY;
int sel_x, sel_y;   //�����̽��ٷ� ����� ����� x y ��ǥ
char sel_color = NONE;
char *sel_piece;
int sel_check = 0;
int order = 0;    //¦���� ��, Ȧ���� ��
int soo = 1;
int time = SET_TIME;
int check_error = 0;
int check_winner = 0;  //1�̸� ���� ��, 2�� ���� ��
int check_rule = 0;
int temp;   //�� ���� �ʱ�ȭ
int w_order;
int b_order;

int check_whitePromotion = 0;
int check_blackPromotion = 0;
int check_whiteAng = 0;
int check_blackAng = 0;

int check_wpPr = 0;
int check_bpPr = 0;
int check_wqPr = 0;
int check_bqPr = 0;
int check_wbPr = 0;
int check_bbPr = 0;
int check_wnPr = 0;
int check_bnPr = 0;
int check_wrPr = 0;
int check_brPr = 0;
int bk_live = 0, wk_live = 0;
int bq_live = 0, wq_live = 0;
int br_live = 0, wr_live = 0;
int bn_live = 0, wn_live = 0;
int bb_live = 0, wb_live = 0;
int bp_live = 0, wp_live = 0;

enum color { red = 12, yellow = 14, white = 7, grey = 8 };
char *chessPos[8][8] = { {"BR1","BN1","BB1","BQ1","BK1","BB1","BN1","BR1"}, //1�� �ѹ��� �������� ���� ��, 0�� �ѹ� �̻� ������ ��
						{"BP1","BP1","BP1","BP1","BP1","BP1","BP1","BP1"},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"WP1","WP1","WP1","WP1","WP1","WP1","WP1","WP1"},
						{"WR1","WN1","WB1","WQ1","WK1","WB1","WN1","WR1"} };

char *chessPiece[13] = { "  ","BR","BN","BB","BQ","BK","BP","WR","WN","WB","WQ","WK","WP" };

int movable[8][8] = {{0,0,0,0,0,0,0,0},     //0�� �̵� �Ұ� 1�� �̵� ����
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0} };

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
		CursorView(0);
		gameCheck();

        if(check_winner==1)
        {
            whiteWin_Alert();
            Sleep(1000);
            break;
        }
        else if(check_winner==2)
        {
            blackWin_Alert();
            Sleep(1000);
            break;
        }
        else if (check_whitePromotion == 1)
            whitePromotion(temp);

        else if (check_blackPromotion == 1)
            blackPromotion(temp);


        pieceDeadDraw();
		orderDraw();
		sooDraw();
		CursorView(1);

		gotoxy(20,7);
		printf("Test M%d %d %d C[2]%c ",movable[cursor_y][cursor_x/2],check_wpPr,check_wqPr,chessPos[cursor_y][cursor_x/2][2]);  //������ Ȯ�ο� (���� ��)
		gotoxy(cursor_x, cursor_y);

        int x_pos = cursor_y;
		int y_pos = cursor_x / 2;   //Ŀ�� ��ġ�� �迭 �� ���� ��ȯ
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
			if (sel_check == 0) //1. �� ������ ������ ���
			{
			    if (chessPos[x_pos][y_pos] != chessPiece[0])    //1-1. ������ ĭ�� ���� ���� ��
				{
				    if (chessPos[x_pos][y_pos][0] == 'B')
					sel_color = BLACK;
                    else
					sel_color = WHITE;

                    if ((order % 2 == 0 && sel_color == BLACK) || (order % 2 != 0 && sel_color == WHITE))   //���ʰ� ���ε� ���� ���� �� ���, ���ʰ� ���ε� �� ���� �� ���
                    {

                        wrongOrder_Alert();
                        gotoxy(20, 2);
                        printf("���� �� �� : ����       ");
                        chessDraw();
                    }
                    else
                    {
                        pieceSelect(x_pos, y_pos);
                        pieceNowDraw();
                        gotoxy(1, 10);
                        printf("                        ");
                        sel_check = 1;
                        sel_x = x_pos, sel_y = y_pos;
                    }
				}
				else if (chessPos[x_pos][y_pos] == chessPiece[0])    //1-2. ������ ĭ�� ��ĭ�� ��
                {
                    gotoxy(1, 10);
                    printf("                        ");
                    gotoxy(cursor_x, cursor_y);
                    sel_check = 0;
                    pieceSelect(x_pos, y_pos);
                    pieceNowDraw();
                }
			}

			else if (sel_check == 1)    //�� ������ ���� ���
			{
                if (movable[x_pos][y_pos] == 1)
                        pieceMove(x_pos, y_pos);

                else if (movable[x_pos][y_pos] == 2) //���Ļ� ����
                    {
                        if (order%2 == 0)
                        {
                            check_whiteAng = 1;
                            w_order = order;
                            pieceMove(x_pos, y_pos);
                        }
                        else
                        {
                            check_blackAng = 1;
                            b_order = order;
                            pieceMove(x_pos, y_pos);
                        }
                    }
                else if (movable[x_pos][y_pos] == 3)    //Ư�� ��
                {
                    if (sel_piece == "WP")
                    {
                        chessPos[x_pos+1][y_pos] = chessPiece[0];
                        pieceMove(x_pos, y_pos);
                    }
                    else if (sel_piece == "BP")
                    {
                        chessPos[x_pos-1][y_pos] = chessPiece[0];
                        pieceMove(x_pos, y_pos);
                    }
                }

                else if (movable[x_pos][y_pos] == 0)
                {
                    if (sel_x == x_pos && sel_y == y_pos)
                    {
                        gotoxy(1, 10);
                        printf("                        ");
                        for(int i=0; i<=7; i++)
                            for(int j=0; j<=7; j++)
                                movable[i][j]=0;
                        chessDraw();
                        gotoxy(20, 2);
                        printf("���� �� �� : ����       ");
                        gotoxy(cursor_x, cursor_y);
                        sel_check = 0;
                    }
                    else
                    {
                        wrongMove_Alert();
                        chessDraw();
                        gotoxy(20, 2);
                        printf("���� �� �� : ����       ");
                        sel_check = 0;
                    }
                }
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

void chessDraw()
{
	int i, j;

    gotoxy(0,0);
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

			pieceDraw(i,j);

            if (movable[i][j] == 1 || movable[i][j] == 2)
            {
                if (chessPos[i][j] == chessPiece[0])
                    printf("\b\b��");
                else if (chessPos[i][j] != chessPiece[0])
                {
                    backcolor(192);
                    printf("\b\b");
                    pieceDraw(i,j);
                    textcolor(white);
                }
            }
            else if (movable[i][j] == 3)
            {
                backcolor(144);
                printf("\b\b");
                pieceDraw(i,j);
                textcolor(white);
            }
		}
		printf("\n");
	}

	for (i = 0; i < 8; i++)
	{
		gotoxy(17, i);
		textcolor(yellow);
		printf("%d", 8 - i);
		textcolor(white);
	}

	backcolor(7);
	textcolor(yellow);
	printf("\n");
	printf(" a b c d e f g h\n");
	textcolor(white);
	printf("------------���â---------------------\n");
	printf("\n\n");
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

void pieceNowDraw()
{
	gotoxy(20, 2);
	printf("                         ");
	gotoxy(20, 2);
	if (sel_piece == chessPiece[1])
		printf("���� �� �� : ��� ��");
	else if (sel_piece == chessPiece[2])
		printf("���� �� �� : ��� ����Ʈ");
	else if (sel_piece == chessPiece[3])
		printf("���� �� �� : ��� ���");
	else if (sel_piece == chessPiece[4])
		printf("���� �� �� : ��� ��");
	else if (sel_piece == chessPiece[5])
		printf("���� �� �� : ��� ŷ");
	else if (sel_piece == chessPiece[6])
		printf("���� �� �� : ��� ��");
	else if (sel_piece == chessPiece[7])
		printf("���� �� �� : ��� ��");
	else if (sel_piece == chessPiece[8])
		printf("���� �� �� : ��� ����Ʈ");
	else if (sel_piece == chessPiece[9])
		printf("���� �� �� : ��� ���");
	else if (sel_piece == chessPiece[10])
		printf("���� �� �� : ��� ��");
	else if (sel_piece == chessPiece[11])
		printf("���� �� �� : ��� ŷ");
	else if (sel_piece == chessPiece[12])
		printf("���� �� �� : ��� ��");
	else
		printf("���� �� �� : ����");

	gotoxy(cursor_x, cursor_y);
}

void pieceDeadDraw()
{
	int i;

	gotoxy(50, 0);
	printf("     ���� ��");
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

    gotoxy(cursor_x,cursor_y);
}

void cursorDraw(int x, int y)
{
	gotoxy(x, y);
	printf(">");
}

void pieceDraw(int i, int j)
{
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
				printf("��");
    else if (chessPos[i][j] == chessPiece[0])
				printf("  ");
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
	_endthreadex(0);
	return 0;
}

void pieceMove(int x, int y)
{
    int i,j;
	chessPos[x][y] = chessPos[sel_x][sel_y];
	chessPos[sel_x][sel_y] = "  ";
	turnPiece(x, y);    //�ٲ�
	system("cls");
	sel_check = 0;
	if (order % 2 != 0)
		soo++;
	order++;
	sel_color = NONE;
	time=SET_TIME;
	for(i=0; i<=7; i++)
        for(j=0; j<=7; j++)
            movable[i][j]=0;
    chessDraw();
	gotoxy(20, 2);
	printf("���� �� �� : ����");
}

void pieceSelect(int x_pos, int y_pos)
{
	if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'R')
        sel_piece = "BR";
	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'N')
    {
		sel_piece = "BN";
		blackNight(x_pos, y_pos);
		chessDraw();
    }
	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'B')
		sel_piece = "BB";
	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'Q')
		sel_piece = "BQ";
	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'K')
		sel_piece = "BK";
	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'P')
    {
        sel_piece = "BP";
        blackPawn(x_pos, y_pos);
        chessDraw();
    }
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'R')
		sel_piece = "WR";
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'N')
    {
		sel_piece = "WN";
		whiteNight(x_pos, y_pos);
		chessDraw();
    }
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'B')
		sel_piece = "WB";
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'Q')
		sel_piece = "WQ";
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'K')
		sel_piece = "WK";
	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'P')
    {
        sel_piece = "WP";
        whitePawn(x_pos, y_pos);
        chessDraw();
    }
	else
		sel_piece = "  ";

}

void gameInit()
{
    time=SET_TIME;
    //_beginthreadex(NULL,0,timerDraw,0,0,NULL);
	int i,j;
	system("cls");
	chessDraw();
	textcolor(white);
	cursor_x = 4, cursor_y = 4;
	temp = 0;
	sel_check = 0;
	check_winner=0;
	check_blackPromotion=0;
	check_whitePromotion=0;
	check_blackAng=0;
	check_whiteAng=0;
	check_wpPr = 0;
    check_bpPr = 0;
    check_wqPr = 0;
    check_bqPr = 0;
    check_wbPr = 0;
    check_bbPr = 0;
    check_wnPr = 0;
    check_bnPr = 0;
    check_wrPr = 0;
    check_brPr = 0;
	order=0;
	soo=1;
	gotoxy(20, 2);
	printf("���� �� �� : ����");
	chessPos[0][0] = "BR1";
	chessPos[0][1] = "BN1";
	chessPos[0][2] = "BB1";
	chessPos[0][3] = "BQ1";
	chessPos[0][4] = "BK1";
	chessPos[0][5] = "BB1";
	chessPos[0][6] = "BN1";
	chessPos[0][7] = "BR1";
	for (i = 0; i <= 7; i++)
	{
        chessPos[1][i] = "BP1";
		chessPos[2][i] = "  ";
		chessPos[3][i] = "  ";
		chessPos[4][i] = "  ";
		chessPos[5][i] = "  ";
		chessPos[6][i] = "WP1";
	}
	chessPos[7][0] = "WR1";
	chessPos[7][1] = "WN1";
	chessPos[7][2] = "WB1";
	chessPos[7][3] = "WQ1";
	chessPos[7][4] = "WK1";
	chessPos[7][5] = "WB1";
	chessPos[7][6] = "WN1";
	chessPos[7][7] = "WR1";

	for(i=0; i<=7; i++)
        for(j=0; j<=7; j++)
            movable[i][j]=0;

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

	if (check_wpPr != 0)
        wp_live+= check_wpPr;
    else if (check_bqPr != 0)
        bp_live+= check_bpPr;
    else if (check_wqPr != 0)
        wq_live-= check_wqPr;
    else if (check_bqPr != 0)
        bq_live-= check_bqPr;
    else if (check_wbPr != 0)
        wb_live-= check_wbPr;
    else if (check_bbPr != 0)
        bb_live-= check_bbPr;
    else if (check_wrPr != 0)
        wr_live-= check_wrPr;
    else if (check_brPr != 0)
        br_live-= check_brPr;
    else if (check_wnPr != 0)
        wn_live-= check_wnPr;
    else if (check_bnPr != 0)
        bn_live-= check_bnPr;

    else if (order - w_order == 2)
    {
        for (int i=0; i<8; i++)
            for(int j=0; j<8; j++)
        {
            if (chessPos[i][j] == "WP2")
                chessPos[i][j] = "WP0";
        }
    }
    else if (order - b_order == 2)
    {
        for (int i=0; i<8; i++)
            for(int j=0; j<8; j++)
        {
            if (chessPos[i][j] == "BP2")
                chessPos[i][j] = "BP0";
        }
    }

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

            if (chessPos[0][j] == "WP0")
            {
                temp = j;
                check_whitePromotion = 1;
            }
            else if (chessPos[7][j] == "BP0")
            {
                temp = j;
                check_blackPromotion = 1;
            }

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

void turnPiece(int x, int y)
{
    if (sel_piece == chessPiece[1])
		chessPos[x][y] = "BR0";
	else if (sel_piece == chessPiece[2])
		chessPos[x][y] = "BN0";
	else if (sel_piece == chessPiece[3])
		chessPos[x][y] = "BB0";
	else if (sel_piece == chessPiece[4])
		chessPos[x][y] = "BQ0";
	else if (sel_piece == chessPiece[5])
		chessPos[x][y] = "BK0";
	else if (sel_piece == chessPiece[6])
	{
        if (check_blackAng == 1)
        {
            chessPos[x][y] = "BP2";
            check_blackAng = 0;
        }
        else
            chessPos[x][y] = "BP0";
    }
	else if (sel_piece == chessPiece[7])
		chessPos[x][y] = "WR0";
	else if (sel_piece == chessPiece[8])
		chessPos[x][y] = "WN0";
	else if (sel_piece == chessPiece[9])
		chessPos[x][y] = "WB0";
	else if (sel_piece == chessPiece[10])
		chessPos[x][y] = "WQ0";
	else if (sel_piece == chessPiece[11])
		chessPos[x][y] = "WK0";
	else if (sel_piece == chessPiece[12])
    {
        if (check_whiteAng == 1)
        {
            chessPos[x][y] = "WP2";
            check_whiteAng = 0;
        }
        else
            chessPos[x][y] = "WP0";
    }
}

void wrongMove_Alert()
{
    int i,j;
	gotoxy(1, 10);
	printf("                        ");
	gotoxy(1, 10);
	textcolor(red);
	printf("�̵��� �� �����ϴ�.");
	sel_color = NONE;
	textcolor(white);
	for(i=0; i<=7; i++)
        for(j=0; j<=7; j++)
            movable[i][j]=0;
	gotoxy(cursor_x, cursor_y);
}

void wrongOrder_Alert()
{
    int i,j;
	gotoxy(1, 10);
	printf("                        ");
	gotoxy(1, 10);
	textcolor(red);
	printf("���ʰ� �ƴմϴ�.");
	sel_color = NONE;
	textcolor(white);
	for(i=0; i<=7; i++)
        for(j=0; j<=7; j++)
            movable[i][j]=0;
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

void whitePromotion(int x)
{
    int check = 1;
    char a = invert(x*2);
    while(check)
    {
        int promotion = 0;

        gotoxy(1, 10);
        printf("                        ");
        gotoxy(1, 10);
        textcolor(red);
        printf("%c8ĭ ���θ�� 1 �� 2 ��� 3 ����Ʈ 4 ��\n",a);
        textcolor(white);
        gotoxy(1, 11);
        printf("�Է��ϼ��� : ");
        scanf("%d",&promotion);
        switch(promotion)
        {
        case 1:
            chessPos[0][x] = "WQ0";
            check = 0;
            check_whitePromotion = 0;
            check_wpPr++;
            check_wqPr++;
            break;
        case 2:
            chessPos[0][x] = "WB0";
            check = 0;
            check_whitePromotion = 0;
            check_wpPr++;
            check_wbPr++;
            break;
        case 3:
            chessPos[0][x] = "WN0";
            check = 0;
            check_whitePromotion = 0;
            check_wpPr++;
            check_wnPr++;
            break;
        case 4:
            chessPos[0][x] = "WR0";
            check = 0;
            check_whitePromotion = 0;
            check_wpPr++;
            check_wrPr++;
            break;
        default:
            gotoxy(1, 11);
            printf("�Է��ϼ��� :             ");
            continue;
        }
    }
    gotoxy(1, 10);
    printf("                                          ");
    gotoxy(1, 11);
    printf("                                          ");
    chessDraw();
    gotoxy(cursor_x, cursor_y);
}

void blackPromotion(int x)
{
    int check = 1;
    char a = invert(x*2);
    while(check)
    {
        int promotion = 0;

        gotoxy(1, 10);
        printf("                        ");
        gotoxy(1, 10);
        textcolor(red);
        printf("%c1ĭ ���θ�� 1 �� 2 ��� 3 ����Ʈ 4 ��\n",a);
        textcolor(white);
        gotoxy(1, 11);
        printf("�Է��ϼ��� : ");
        scanf("%d",&promotion);
        switch(promotion)
        {
        case 1:
            chessPos[7][x] = "BQ0";
            check = 0;
            check_blackPromotion = 0;
            check_bpPr++;
            check_bqPr++;
            break;
        case 2:
            chessPos[7][x] = "BB0";
            check = 0;
            check_blackPromotion = 0;
            check_bpPr++;
            check_bbPr++;
            break;
        case 3:
            chessPos[7][x] = "BN0";
            check = 0;
            check_blackPromotion = 0;
            check_bpPr++;
            check_bnPr++;
            break;
        case 4:
            chessPos[7][x] = "BR0";
            check = 0;
            check_blackPromotion = 0;
            check_bpPr++;
            check_brPr++;
            break;
        default:
            gotoxy(1, 11);
            printf("�Է��ϼ��� :           ");
            continue;
        }
    }
    gotoxy(1, 10);
    printf("                                          ");
    gotoxy(1, 11);
    printf("                                          ");
    chessDraw();
    gotoxy(cursor_x, cursor_y);
}

void whitePawn(int x, int y)
{
    if (chessPos[x][y][2] == '1')   //�ѹ��� �� ����������
    {
        if (chessPos[x-2][y] == chessPiece[0])
            movable[x-2][y] = 2;    //���Ļ�
    }

    if(x>0)
    {
        if (chessPos[x-1][y]==chessPiece[0])
            movable[x-1][y]=1;
        if (chessPos[x-1][y-1][0]==BLACK)
            movable[x-1][y-1]=1;
        if (chessPos[x-1][y+1][0]==BLACK)
            movable[x-1][y+1]=1;
        if (chessPos[x][y-1][2] == '2')
            movable[x-1][y-1]=3;
        if (chessPos[x][y+1][2] == '2')
            movable[x-1][y+1]=3;
    }
}

void blackPawn(int x, int y)
{
    if (chessPos[x][y][2] == '1')
    {
        if (chessPos[x+2][y] == chessPiece[0])
            movable[x+2][y] = 2;
    }

    if(x<7)
    {
        if(chessPos[x+1][y]==chessPiece[0])
            movable[x+1][y]=1;
        if(chessPos[x+1][y+1][0]==WHITE)
            movable[x+1][y+1]=1;
        if(chessPos[x+1][y-1][0]==WHITE)
            movable[x+1][y-1]=1;
        if (chessPos[x][y-1][2] == '2')
            movable[x+1][y-1]=3;
        if (chessPos[x][y+1][2] == '2')
            movable[x+1][y+1]=3;
    }
}
void whiteNight(int x, int y)   // ��� ����Ʈ
{

        if(((chessPos[x-2][y-1] == chessPiece[0]) || (chessPos[x-2][y-1][0] == BLACK)) && (2 <= x <= 7) && (1 <= y <= 7))
                movable[x-2][y-1] = 1;

        else if(((chessPos[x-1][y-2] == chessPiece[0]) || (chessPos[x-1][y-2][0] == BLACK)) && (1 <= x <= 7) && (2 <= y <= 7))
                movable[x-1][y-2] = 1;

        else if(((chessPos[x+1][y-2] == chessPiece[0]) || (chessPos[x+1][y-2][0] == BLACK)) && (0 <= x <= 6) && (2 <= y <= 7))
                movable[x+1][y-2] = 1;

        else if(((chessPos[x+2][y-1] == chessPiece[0]) || (chessPos[x+2][y-1][0] == BLACK)) && (0 <= x <= 5) && (1 <= y <= 7))
                movable[x+2][y-1] = 1;

        else if(((chessPos[x+2][y+1] == chessPiece[0]) || (chessPos[x+2][y+1][0] == BLACK)) && (0 <= x <= 5) && (0 <= y <= 6))
                movable[x+2][y+1] = 1;

        else if(((chessPos[x+1][y+2] == chessPiece[0]) || (chessPos[x+1][y+2][0] == BLACK)) && (0 <= x <= 6) && (0 <= y <= 5))
                movable[x+1][y+2] = 1;

        else if(((chessPos[x-1][y+2] == chessPiece[0]) || (chessPos[x-1][y+2][0] == BLACK)) && (1 <= x <= 7) && (0 <= y <= 5))
                movable[x-1][y+2] = 1;

        else if(((chessPos[x-2][y+1] == chessPiece[0]) || (chessPos[x-2][y+1][0] == BLACK)) && (2 <= x <= 7) && (0 <= y <= 6))
                movable[x-2][y+1] = 1;
        else
            wrongMove_Alert();

}

void blackNight(int x, int y)
{

        if(((chessPos[x-2][y-1] == chessPiece[0]) || (chessPos[x-2][y-1][0] == WHITE)) && (2 <= x <= 7) &&(1 <= y <= 7))
                movable[x-2][y-1] = 1;

        else if(((chessPos[x-1][y-2] == chessPiece[0]) || (chessPos[x-1][y-2][0] == WHITE)) && (1 <= x <= 7) && (2 <= y <= 7))
                movable[x-1][y-2] = 1;

        else if(((chessPos[x+1][y-2] == chessPiece[0]) || (chessPos[x+1][y-2][0] == WHITE)) && (0 <= x <= 6) && (2 <= y <= 7))
                movable[x+1][y-2] = 1;

        else if(((chessPos[x+2][y-1] == chessPiece[0]) || (chessPos[x+2][y-1][0] == WHITE)) && (0 <= x <= 5) && (1 <= y <= 7))
                movable[x+2][y-1] = 1;

        else if(((chessPos[x+2][y+1] == chessPiece[0]) || (chessPos[x+2][y+1][0] == WHITE)) && (0 <= x <= 5) && (0 <= y <= 6))
                movable[x+2][y+1] = 1;

        else if(((chessPos[x+1][y+2] == chessPiece[0]) || (chessPos[x+1][y+2][0] == WHITE)) && (0 <= x <= 6) && (0 <= y <= 5))
                movable[x+1][y+2] = 1;

        else if(((chessPos[x-1][y+2] == chessPiece[0]) || (chessPos[x-1][y+2][0] == WHITE)) && (1 <= x <= 7) && (0 <= y <= 5))
                movable[x-1][y+2] = 1;

        else if(((chessPos[x-2][y+1] == chessPiece[0]) || (chessPos[x-2][y+1][0] == WHITE)) && (2 <= x <= 7) && (0 <= y <= 6))
                movable[x-2][y+1] = 1;

        else
            wrongMove_Alert();

}

void gotoxy(int x, int y)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleHandle, pos);
}

void erase(int x, int y)
{
	gotoxy(x, y);
	printf(" ");
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


void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
