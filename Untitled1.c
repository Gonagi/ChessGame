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

void DrawTitle();                   //Ÿ��Ʋ ȭ�� ���
void DrawChess();                   //ü���� ���
void DrawNowPiece();                //���õ� �� ǥ��
void DrawDeadPiece();               //���� �� ǥ��
void DrawCursor(int, int);          //Ŀ�� '>' ���
void DrawPiece(int, int);           //ü���ǿ� �� ǥ�� �Լ�
void DrawOrder();                   //���� ǥ��
void DrawSoo();                     //�� ������ ǥ��
void ChessRule();                   //ü�� ��Ģ ������ ���
void ControlRule();                 //���� ������ ���
unsigned _stdcall timerDraw(void *);//�����带 �̿��� Ÿ�̸� �Լ�

void MovePiece(int, int);           //�� �̵� �Լ�
void SelectPiece(int, int);         //�� �����ؼ� ���� ������ �� �����ϴ� �Լ�
void InitializeGame();                    //���� �ʱ�ȭ
void CheckGame();                   //���� �� üũ ���� Ȯ��
void TurnPiece(int, int);           //������ �� �ڿ� 0 �ִ� �Լ�
void WhiteCheck();
void BlackCheck();
void CheckCheck();

void WhiteKing(int, int);           //��� ŷ
void BlackKing(int, int);           //��� ŷ
void WhiteQueen(int, int);          //��� ��
void BlackQueen(int, int);          //��� ��
void WhitePawn(int, int);           // ��� ��
void BlackPawn(int, int);           // ��� ��
void WhiteNight(int, int);          // ��� ����Ʈ
void BlackNight(int, int);          // ��� ����Ʈ
void WhiteBishop(int, int);         // ��� ���
void BlackBishop(int, int);         // ��� ���
void WhiteRook(int, int);           // ��� ��
void BlackRook(int, int);           // ��� ��

void WhitePromotion(int);           //����� ���θ�� ��
void BlackPromotion(int);           //����� ���θ�� ��

void WrongMove_Alert();             //�߸��� ������ ���
void WrongOrder_Alert();            //���� ���
void WhiteWin_Alert();              //��� �� �˸�
void BlackWin_Alert();              //��� �� �˸�
void TImer_Alert();                 //�ð� �ʰ� ���
void WhiteSur_Alert();              //��� ��� �˸�
void BlackSur_Alert();              //��� ��� �˸�
void WhiteCheck_Alert();
void WhiteCheckmate_Alert();
void BlackCheck_Alert();
void BlackCheckmate_Alert();

void ViewCursor(char);              //Ŀ�� ���� �Լ�
void Gotoxy(int, int);              //�ֳܼ� ��ǥ �̵�
void Erase(int, int);               //�ش� ��ǥ �����
char Invert(int);                   //1~8�� a~h�� �ٲٴ� �Լ�
void TextColor(int);                //�ֳܼ� �۾� ���� ����
void BackColor(int);                //�ֳܼ� ���� ���� ����

int cursor_x = mainX, cursor_y = mainY; //����ȭ�鿡���� x, y ��ǥ
int sel_x, sel_y;                   //�����̽��ٷ� ����� ����� x y ��ǥ
char sel_color = NONE;              //�����̽��ٷ� �� ���� ����
char *sel_piece;                    //�����̽��ٷ� �� ���� ����
int sel_check = 0;                  //�����ߴ� ���� ����
int order = 0;                      //¦���� ��, Ȧ���� ��
int soo = 1;                        //���� �� ��
int time = SET_TIME;                //Ÿ�̸� �ð� ����
int check_error = 0;                //���� �߻� ����
int check_winner = 0;               //1�̸� ���� ��, 2�� ���� ��
int check_rule = 0;                 //���� ������ �ڷΰ��� �����ϱ� ���� ����
int temp;                           //�� ���� �ʱ�ȭ
int w_order;                        //���Ļ� ������ ���� ���� ���� ����
int b_order;                        // ""
int check_check = 0;

int check_WhitePromotion = 0;       //����� ���θ�� ���� Ȯ��
int check_BlackPromotion = 0;       //����� ���θ�� ���� Ȯ��
int check_whiteAng = 0;             //����� ���Ļ� ���� Ȯ��
int check_blackAng = 0;             //����� ���Ļ� ���� Ȯ��

int check_wpPr = 0;                 //���θ���� ���� �ٲ� ��� ���� ����
int check_bpPr = 0;
int check_wqPr = 0;
int check_bqPr = 0;
int check_wbPr = 0;
int check_bbPr = 0;
int check_wnPr = 0;
int check_bnPr = 0;
int check_wrPr = 0;
int check_brPr = 0;
int bk_live = 0, wk_live = 0;       //���� �� ǥ�ø� ���� ���� Ȯ�� ����
int bq_live = 0, wq_live = 0;
int br_live = 0, wr_live = 0;
int bn_live = 0, wn_live = 0;
int bb_live = 0, wb_live = 0;
int bp_live = 0, wp_live = 0;
int wk_x, wk_y;
int bk_x, bk_y;
int wking_check=0;
int bking_check=0;

enum color { red = 12, yellow = 14, white = 7, grey = 8 };      //TextColor���� ���� enum ����
char *chessPos[8][8] = { {"BR1","BN1","BB1","BQ1","BK1","BB1","BN1","BR1"}, //1�� �ѹ��� �������� ���� ��, 0�� �ѹ� �̻� ������ ��
						{"BP1","BP1","BP1","BP1","BP1","BP1","BP1","BP1"},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"WP1","WP1","WP1","WP1","WP1","WP1","WP1","WP1"},
						{"WR1","WN1","WB1","WQ1","WK1","WB1","WN1","WR1"} };

char *chessPiece[13] = { "  ","BR","BN","BB","BQ","BK","BP","WR","WN","WB","WQ","WK","WP" };    //�����ͳ��� �� �ȵȴٰ� ��� ���� �� ���� ������ �迭

int movable[8][8] = {{0,0,0,0,0,0,0,0},     //ü�������� �̵� �� ���� ǥ�� �迭 -1�� ���� ���� �� 0�� �̵� �Ұ� 1�� �̵� ���� 2�� ���Ļ� 3�� Ư�� �̵�
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0} };

int check[8][8] = {{0,0,0,0,0,0,0,0},       //ü���� üũ �Ǻ� 0�� �⺻ Ÿ�� 1�� �� �̵� �Ÿ�
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
	ViewCursor(0);
	TextColor(white);
	system("cls");
	DrawTitle();
	DrawCursor(cursor_x, cursor_y);

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
			cursor_x = 18, cursor_y = 6;
		Rule:
			system("cls");
			check_rule = 0;
			Gotoxy(18, 4);
			printf("� ������ �����ðڽ��ϱ�?");
			Gotoxy(20, 6);
			printf("���� ���� ��Ģ");
			Gotoxy(20, 7);
			printf("ü�� ��Ģ");
			TextColor(grey);
			Gotoxy(0, 16);
			printf("����ȭ������ ���� (Backspace)");
			TextColor(white);
			DrawCursor(cursor_x, cursor_y);

			while (1)
            {
                key = _getch();

                if (cursor_y == 6 && key == ENTER)
                {
                    system("cls");
                    ControlRule();
                }

                else if (cursor_y == 7 && key == ENTER)
                {
                    system("cls");
                    ChessRule();
                }

                switch (key)
                {
                case UP:
                    if (cursor_y == 6)
                        continue;

                    else if (check_rule == 1)
                        continue;

                    cursor_y--;
                    Erase(cursor_x, cursor_y + 1);
                    DrawCursor(cursor_x, cursor_y);
                    break;

                case DOWN:

                    if (cursor_y == 7)
                        continue;

                    else if (check_rule == 1)
                        continue;

                    else
                        cursor_y++;

                    Erase(cursor_x, cursor_y - 1);
                    DrawCursor(cursor_x, cursor_y);
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
            Gotoxy(10, 10);
            printf("������ �����մϴ�...\n");
            Gotoxy(0, 17);
            return 0;
        }

        switch (key)
        {
        case UP:

            if (cursor_y == mainY)
                continue;

            cursor_y--;
            Erase(cursor_x, cursor_y + 1);
            DrawCursor(cursor_x, cursor_y);
            break;

        case DOWN:

            if (cursor_y == mainY + 2)
                continue;

            else
                cursor_y++;

            Erase(cursor_x, cursor_y - 1);
            DrawCursor(cursor_x, cursor_y);
            break;
        }
    }
    InitializeGame();
    CheckGame();

    while (1)   //���� ���� while��
    {
        ViewCursor(0);
        CheckGame();
        CheckCheck();

        if(check_winner == 1)
        {
            WhiteWin_Alert();
            Sleep(1000);
            break;
        }

        else if(check_winner == 2)
        {
            BlackWin_Alert();
            Sleep(1000);
            break;
        }

        else if (check_WhitePromotion == 1)
            WhitePromotion(temp);

        else if (check_BlackPromotion == 1)
            BlackPromotion(temp);

        DrawDeadPiece();
        DrawOrder();
        DrawSoo();
        ViewCursor(1);
        Gotoxy(20,7);
        printf("%d %d %d,%d",wking_check, check[cursor_y][cursor_x/2], wk_x, wk_y);  //������ Ȯ�ο� (���� ��)
        Gotoxy(cursor_x, cursor_y);
        int x_pos = cursor_y;
        int y_pos = cursor_x / 2;   //Ŀ�� ��ġ�� �迭 �� ���� ��ȯ
        int key;
        key = _getch();

        if (check_error == 1)
            break;

        switch (key)
        {
        case 's':

            if (order%2 == 0)
                WhiteSur_Alert();

            else
                BlackSur_Alert();

        case 'S':

            if (order%2 == 0)
                WhiteSur_Alert();

            else
                BlackSur_Alert();

        case UP:

            if (cursor_y == 0)
                continue;

            cursor_y--;
            Gotoxy(cursor_x, cursor_y);
            break;

        case DOWN:

            if (cursor_y == 7)
                continue;

            cursor_y++;
            Gotoxy(cursor_x, cursor_y);
            break;

        case LEFT:

            if (cursor_x == 0)
                continue;

            cursor_x -= 2;
            Gotoxy(cursor_x, cursor_y);
            break;

        case RIGHT:

            if (cursor_x == 14)
                continue;

            cursor_x += 2;
            Gotoxy(cursor_x, cursor_y);
            break;

        case BACK:
            cursor_x = mainX, cursor_y = mainY;
            InitializeGame();
            cursor_x = mainX, cursor_y = mainY;
            goto Main;
            break;

        case SPACE:

            if (sel_check == 0) //1. �� ������ ������ ���
            {
                if (chessPos[x_pos][y_pos] != chessPiece[0])    //1-1. ������ ĭ�� ���� ���� ��
                {
                    if (chessPos[x_pos][y_pos][0] == BLACK)
                        sel_color = BLACK;

                    else
                        sel_color = WHITE;

                    if ((order % 2 == 0 && sel_color == BLACK) || (order % 2 != 0 && sel_color == WHITE))   //���ʰ� ���ε� ���� ���� �� ���, ���ʰ� ���ε� �� ���� �� ���
                    {
                        WrongOrder_Alert();
                        Gotoxy(20, 2);
                        printf("���� �� �� : ����       ");
                        DrawChess();
                    }

                    else
                    {
                        SelectPiece(x_pos, y_pos);
                        DrawNowPiece();
                        Gotoxy(1, 10);
                        printf("                        ");
                        sel_check = 1;
                        sel_x = x_pos, sel_y = y_pos;
                    }
                }

                else if (chessPos[x_pos][y_pos] == chessPiece[0])    //1-2. ������ ĭ�� ��ĭ�� ��
                {
                    Gotoxy(1, 10);
                    printf("                        ");
                    Gotoxy(cursor_x, cursor_y);
                    sel_check = 0;
                    SelectPiece(x_pos, y_pos);
                    DrawNowPiece();
                }
            }

            else if (sel_check == 1)    //2.�� ������ ���� ���
            {
                if (movable[x_pos][y_pos] == 1)     //2-1. �̵��ϰ��� �ϴ� ��ġ�� movable = 1�̸�
                    MovePiece(x_pos, y_pos);

                else if (movable[x_pos][y_pos] == 2) //���Ļ� ����
                {
                    if (order%2 == 0)
                    {
                        check_whiteAng = 1;
                        w_order = order;
                        MovePiece(x_pos, y_pos);
                    }

                    else
                    {
                        check_blackAng = 1;
                        b_order = order;
                        MovePiece(x_pos, y_pos);
                    }
                }

                else if (movable[x_pos][y_pos] == 3)    //Ư�� ��
                {
                    if (sel_piece == "WP")              //���Ļ�
                    {
                        chessPos[x_pos+1][y_pos] = chessPiece[0];
                        MovePiece(x_pos, y_pos);
                    }

                    else if (sel_piece == "BP")
                    {
                        chessPos[x_pos-1][y_pos] = chessPiece[0];
                        MovePiece(x_pos, y_pos);
                    }

                    else if (sel_piece == "WK")         //ĳ����
                    {
                        if (y_pos == 6)
                        {
                            chessPos[x_pos][y_pos-1] = "WR1";
                            chessPos[x_pos][y_pos+1] = chessPiece[0];
                            MovePiece(x_pos, y_pos);
                        }

                        else
                        {
                            chessPos[x_pos][y_pos+1] = "WR1";
                            chessPos[x_pos][y_pos-2] = chessPiece[0];
                            MovePiece(x_pos, y_pos);
                        }
                    }

                    else if (sel_piece == "BK")
                    {
                        if (y_pos == 6)
                        {
                            chessPos[x_pos][y_pos-1] = "BR1";
                            chessPos[x_pos][y_pos+1] = chessPiece[0];
                            MovePiece(x_pos, y_pos);
                        }

                        else
                        {
                            chessPos[x_pos][y_pos+1] = "BR1";
                            chessPos[x_pos][y_pos-2] = chessPiece[0];
                            MovePiece(x_pos, y_pos);
                        }
                    }
                }
                else if (movable[x_pos][y_pos] == -1)
                {
                    Gotoxy(1, 10);
                    printf("                        ");

                    for(int i=0; i<=7; i++)
                        for(int j=0; j<=7; j++)
                            movable[i][j]=0;

                    DrawChess();
                    Gotoxy(20, 2);
                    printf("���� �� �� : ����       ");
                    Gotoxy(cursor_x, cursor_y);
                    sel_check = 0;
                }

                else if (movable[x_pos][y_pos] == 0)        //�̵��ϰ��� �ϴ� ��ġ�� movable = 0�̸�
                {
                    WrongMove_Alert();
                    DrawChess();
                    Gotoxy(20, 2);
                    printf("���� �� �� : ����       ");
                    sel_check = 0;
                }
            }

        default:
            continue;
        }
    }

    Gotoxy(0, 17);
        return 0;
}


void DrawTitle()
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


void DrawChess()
{
	int i, j;
	Gotoxy(0,0);

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (j % 2 == 0 && i % 2 == 0)
				BackColor(112);

			else if (j % 2 != 0 && i % 2 == 0)
				BackColor(128);

			else if (j % 2 == 0 && i % 2 != 0)
				BackColor(128);

			else
				BackColor(112);

			DrawPiece(i,j);

            if (movable[i][j] == 1 || movable[i][j] == 2)
            {
                if (chessPos[i][j] == chessPiece[0])
                    printf("\b\b��");

                else if (chessPos[i][j] != chessPiece[0])
                {
                    BackColor(192);
                    printf("\b\b");
                    DrawPiece(i,j);
                    TextColor(white);
                }
            }

            else if (movable[i][j] == 3)
            {
                BackColor(144);
                printf("\b\b");
                DrawPiece(i,j);
                TextColor(white);
            }

            else if (movable[i][j] == -1)
            {
                BackColor(224);
                printf("\b\b");
                DrawPiece(i,j);
                TextColor(white);
            }
		}
		printf("\n");
	}

	for (i = 0; i < 8; i++)
	{
		Gotoxy(17, i);
		TextColor(yellow);
		printf("%d", 8 - i);
		TextColor(white);
	}

	BackColor(7);
	TextColor(yellow);
	printf("\n");
	printf(" a b c d e f g h\n");
	TextColor(white);
	printf("------------���â---------------------\n");
	printf("\n\n");
	printf("---------------------------------------\n");
	printf(" �� : ��, �� : ����Ʈ, �� : ���\n");
	printf(" �� : ��, �� : ŷ, �� : ��\n");
	TextColor(grey);
	Gotoxy(0, 16);
	printf("����ȭ������ ���ư��� (Backspace)\n");
	TextColor(white);

	for (i = 0; i < 14; i++)
	{
		Gotoxy(49, i);
		printf("|");
	}
}


void DrawNowPiece()
{
	Gotoxy(20, 2);
	printf("                         ");
	Gotoxy(20, 2);

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

	Gotoxy(cursor_x, cursor_y);
}


void DrawDeadPiece()
{
	int i;
	Gotoxy(50, 0);
	printf("     ���� ��");
	Gotoxy(50, 1);
	printf("------����------");
	Gotoxy(51, 2);

	for (i = 0; i < 8 - wp_live; i++)
		printf("��");

	Gotoxy(51, 3);

	for (i = 0; i < 2 - wr_live; i++)
		printf("��");

	Gotoxy(56, 3);

	for (i = 0; i < 2 - wn_live; i++)
		printf("��");

	Gotoxy(61, 3);

	for (i = 0; i < 2 - wb_live; i++)
		printf("��");

	Gotoxy(51, 4);

	if (wq_live == 0)
		printf("��");

	Gotoxy(54, 4);

	if (wk_live == 0)
		printf("��");

	Gotoxy(50, 7);
	printf("------����------");
	Gotoxy(51, 8);

	for (i = 0; i < 8 - bp_live; i++)
		printf("��");

	Gotoxy(51, 9);

	for (i = 0; i < 2 - br_live; i++)
		printf("��");

	Gotoxy(56, 9);

	for (i = 0; i < 2 - bn_live; i++)
		printf("��");

	Gotoxy(61, 9);

	for (i = 0; i < 2 - bb_live; i++)
		printf("��");

	Gotoxy(51, 10);

	if (bq_live == 0)
		printf("��");

	Gotoxy(54, 10);

	if (bk_live == 0)
		printf("��");

    Gotoxy(cursor_x,cursor_y);
}


void DrawCursor(int x, int y)
{
	Gotoxy(x, y);
	printf(">");
}


void DrawPiece(int i, int j)
{
    if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'R')
				printf("��");

    else if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'N')
				printf("��");

    else if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'B')
				printf("��");

    else if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'Q')
				printf("��");

    else if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'K')
				printf("��");

    else if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'P')
				printf("��");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'R')
				printf("��");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'N')
				printf("��");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'B')
				printf("��");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'Q')
				printf("��");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'K')
				printf("��");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'P')
				printf("��");

    else if (chessPos[i][j] == chessPiece[0])
				printf("  ");
}


void DrawOrder()
{
	char *order_str;

	if (order % 2 == 0)
		order_str = "��";

	else
		order_str = "��";

	Gotoxy(20, 0);
	printf("���� ���� : %s", order_str);
	Gotoxy(cursor_x, cursor_y);
}


void DrawSoo()
{
	Gotoxy(20, 4);
	printf("�� %d��", soo);
	Gotoxy(cursor_x, cursor_y);
}


void ChessRule()
{
	check_rule = 1;
	TextColor(white);
	printf("----------------���� ���---------------\n");
	printf("1. ��� ���� �����ư��� �÷����մϴ�.\n");
	printf("2. ���� ���� ������ �����մϴ�.\n");
	printf("3.  \n");
	printf("4.  \n");
	printf("\n");
	TextColor(grey);
	Gotoxy(0, 16);
	printf("���ư��� (Backspace)");
	TextColor(white);
}


void ControlRule()
{
	check_rule = 1;
	TextColor(white);
	printf("-------------------���� ���------------------\n\n");
	printf("1. Ű���� ����Ű�� �����Ͽ� Ŀ���� �̵��մϴ�.\n\n");
	printf("2. �̵��ϰ��� �ϴ� ���� Ŀ���� �̵���Ų ��\n");
	printf("   �����̽� �ٸ� �Է��Ͽ� ���� �����մϴ�.\n\n");
	printf("3. ���õ� ���� ���ϴ� ��ġ�� Ŀ���� �̵���Ų\n");
	printf("   �� �����̽� �ٸ� �Է� �Ͽ� �̵���ŵ�ϴ�.\n\n");
	printf("4. �� ������ ����ϰ� ���� ��� �ش� ���� \n");
	printf("   Ŀ���� �ø��� �����̽� �ٸ� �ѹ� �� ������\n");
	printf("   ����մϴ�.\n\n");
	TextColor(grey);
	Gotoxy(0, 16);
	printf("���ư��� (Backspace)");
	TextColor(white);
}


unsigned _stdcall timerDraw(void *arg)
{
	int min, sec;

	while(1)
	{
		min=time/60;
		sec=time%60;
		ViewCursor(0);
		Gotoxy(20,6);
		printf("�����ð� = %2d:%2d",min,sec);
		Gotoxy(cursor_x,cursor_y);
		ViewCursor(1);
		Sleep(1000);
		time--;

		if(time==-1)
        {
            TImer_Alert();
            break;
        }

        else if(time<-1)
            break;
	}

	_endthreadex(0);
	return 0;
}


void MovePiece(int x, int y)
{
    int i,j;
	chessPos[x][y] = chessPos[sel_x][sel_y];
	chessPos[sel_x][sel_y] = "  ";
	TurnPiece(x, y);    //�ٲ�
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

    DrawChess();
	Gotoxy(20, 2);
	printf("���� �� �� : ����");
}


void SelectPiece(int x_pos, int y_pos)
{
	if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'R')
    {
        sel_piece = "BR";
        movable[x_pos][y_pos] = -1;
        BlackRook(x_pos, y_pos);
        DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'N')
    {
		sel_piece = "BN";
		movable[x_pos][y_pos] = -1;
		BlackNight(x_pos, y_pos);
		DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'B')
    {
        sel_piece = "BB";
        movable[x_pos][y_pos] = -1;
        BlackBishop(x_pos, y_pos);
        DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'Q')
    {
        sel_piece = "BQ";
        movable[x_pos][y_pos] = -1;
        BlackQueen(x_pos, y_pos);
        DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'K')
    {
        sel_piece = "BK";
        movable[x_pos][y_pos] = -1;
        BlackKing(x_pos, y_pos);
        DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'P')
    {
        sel_piece = "BP";
        movable[x_pos][y_pos] = -1;
        BlackPawn(x_pos, y_pos);
        DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'R')
    {
        sel_piece = "WR";
        movable[x_pos][y_pos] = -1;
        WhiteRook(x_pos, y_pos);
        DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'N')
    {
		sel_piece = "WN";
		movable[x_pos][y_pos] = -1;
		WhiteNight(x_pos, y_pos);
		DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'B')
    {
		sel_piece = "WB";
		movable[x_pos][y_pos] = -1;
		WhiteBishop(x_pos, y_pos);
		DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'Q')
    {
        sel_piece = "WQ";
        movable[x_pos][y_pos] = -1;
        WhiteQueen(x_pos, y_pos);
        DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'K')
    {
        sel_piece = "WK";
        movable[x_pos][y_pos] = -1;
        WhiteKing(x_pos, y_pos);
        DrawChess();
    }

	else if (chessPos[x_pos][y_pos][0] == 'W' && chessPos[x_pos][y_pos][1] == 'P')
    {
        sel_piece = "WP";
        movable[x_pos][y_pos] = -1;
        WhitePawn(x_pos, y_pos);
        DrawChess();
    }

	else
		sel_piece = "  ";
}


void InitializeGame()
{
    time=SET_TIME;
    //_beginthreadex(NULL,0,timerDraw,0,0,NULL);      //Ÿ�̸� ǥ��
	int i,j;
	system("cls");
	DrawChess();
	TextColor(white);
	cursor_x = 4, cursor_y = 4;
	temp = 0;
	sel_check = 0;
	check_winner=0;
	check_BlackPromotion=0;
	check_WhitePromotion=0;
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
	Gotoxy(20, 2);
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

	Gotoxy(cursor_x, cursor_y);
}


void CheckGame()
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

    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'K')
            {
                wk_x = i;
                wk_y = j;
            }

            if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'K')
            {
                bk_x = i;
                bk_y = j;
            }
        }
    }

    if (order%2==0)
    {
        check_check=1;
        WhiteCheck();
        check_check=0;
    }

    else
    {
        check_check=1;
        BlackCheck();
        check_check=0;
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
                check_WhitePromotion = 1;
            }

            else if (chessPos[7][j] == "BP0")
            {
                temp = j;
                check_BlackPromotion = 1;
            }
		}

    if (bk_live == 0 && wk_live == 1)
        check_winner=1;

    else if (bk_live == 1 && wk_live == 0)
        check_winner=2;

    else if (bk_live == 0 && wk_live == 0)
    {
        Gotoxy(1, 10);
        printf("                        ");
        Gotoxy(1, 10);
        TextColor(red);
        printf("ġ���� ���� �߻�");
        check_error = 1;
        TextColor(white);
    }
}


void TurnPiece(int x, int y)
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


void WrongMove_Alert()
{
    int i,j;
	Gotoxy(1, 10);
	printf("                        ");
	Gotoxy(1, 10);
	TextColor(red);
	printf("�̵��� �� �����ϴ�.");
	sel_color = NONE;
	TextColor(white);

	for(i=0; i<=7; i++)
        for(j=0; j<=7; j++)
            movable[i][j]=0;

	Gotoxy(cursor_x, cursor_y);
}


void WrongOrder_Alert()
{
    int i,j;
	Gotoxy(1, 10);
	printf("                        ");
	Gotoxy(1, 10);
	TextColor(red);
	printf("���ʰ� �ƴմϴ�.");
	sel_color = NONE;
	TextColor(white);

	for(i=0; i<=7; i++)
        for(j=0; j<=7; j++)
            movable[i][j]=0;

	Gotoxy(cursor_x, cursor_y);
}


void WhiteWin_Alert()
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("���� �¸�!");
    TextColor(white);
}


void WhiteCheck_Alert()
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("���� üũ");
    TextColor(white);
}


void WhiteCheckmate_Alert()
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("���� üũ����Ʈ!");
    Sleep(3000);
    TextColor(white);
    check_winner = 2;
}


void BlackWin_Alert()
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("���� �¸�!");
    TextColor(white);
}


void BlackCheck_Alert()
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("���� üũ");
    TextColor(white);
}


void BlackCheckmate_Alert()
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("���� üũ����Ʈ!");
    Sleep(3000);
    TextColor(white);
    check_winner = 1;
}


void TImer_Alert()
{
    Gotoxy(1, 10);
	printf("                        ");
	Gotoxy(1, 10);
	TextColor(red);

	if(order%2==0)
    {
        printf("���� �ð��ʰ�.");
        TextColor(white);
        Sleep(2000);
        check_winner = 2;
    }

    else
    {
        printf("���� �ð��ʰ�.");
        TextColor(white);
        Sleep(2000);
        check_winner = 1;
    }

	TextColor(white);
	Gotoxy(cursor_x, cursor_y);
}


void WhiteSur_Alert()
{
    int check = 1;

    while(check)
    {
        char answer;
        Gotoxy(1, 10);
        printf("                        ");
        Gotoxy(1, 10);
        TextColor(red);
        printf("������ ����߽��ϴ�.");
        TextColor(white);
        Gotoxy(1, 11);
        printf("�³��Ϸ��� y, �ƴϸ� n�� �Է� : ");
        scanf("%c",&answer);

        switch(answer)
        {
        case 'y':
            check_winner = 2;
            check = 0;
            break;

        case 'n':
            check = 0;
            break;

        case 'Y':
            check_winner = 2;
            check = 0;
            break;

        case 'N':
            check = 0;
            break;

        default:
            Gotoxy(1, 11);
            printf("�³��Ϸ��� y, �ƴϸ� n�� �Է� :      ");
            continue;
        }
    }

    Gotoxy(1, 10);
    printf("                                          ");
    Gotoxy(1, 11);
    printf("                                          ");
    DrawChess();
    Gotoxy(cursor_x, cursor_y);
}


void BlackSur_Alert()
{
    int check = 1;

    while(check)
    {
        char answer;
        Gotoxy(1, 10);
        printf("                        ");
        Gotoxy(1, 10);
        TextColor(red);
        printf("������ ����߽��ϴ�.");
        TextColor(white);
        Gotoxy(1, 11);
        printf("�³��Ϸ��� y, �ƴϸ� n�� �Է� : ");
        scanf("%c",&answer);

        switch(answer)
        {
        case 'y':
            check_winner = 1;
            check = 0;
            break;

        case 'n':
            check = 0;
            break;

        case 'Y':
            check_winner = 1;
            check = 0;
            break;

        case 'N':
            check = 0;
            break;

        default:
            Gotoxy(1, 11);
            printf("�³��Ϸ��� y, �ƴϸ� n�� �Է� :      ");
            continue;
        }
    }

    Gotoxy(1, 10);
    printf("                                          ");
    Gotoxy(1, 11);
    printf("                                          ");
    DrawChess();
    Gotoxy(cursor_x, cursor_y);
}


void WhitePromotion(int x)
{
    int check = 1;
    char a = Invert(x*2);

    while(check)
    {
        int promotion = 0;
        Gotoxy(1, 10);
        printf("                        ");
        Gotoxy(1, 10);
        TextColor(red);
        printf("%c8ĭ ���θ�� 1 �� 2 ��� 3 ����Ʈ 4 ��\n",a);
        TextColor(white);
        Gotoxy(1, 11);
        printf("�Է��ϼ��� : ");
        scanf("%d",&promotion);

        switch(promotion)
        {
        case 1:
            chessPos[0][x] = "WQ0";
            check = 0;
            check_WhitePromotion = 0;
            check_wpPr++;
            check_wqPr++;
            break;

        case 2:
            chessPos[0][x] = "WB0";
            check = 0;
            check_WhitePromotion = 0;
            check_wpPr++;
            check_wbPr++;
            break;

        case 3:
            chessPos[0][x] = "WN0";
            check = 0;
            check_WhitePromotion = 0;
            check_wpPr++;
            check_wnPr++;
            break;

        case 4:
            chessPos[0][x] = "WR0";
            check = 0;
            check_WhitePromotion = 0;
            check_wpPr++;
            check_wrPr++;
            break;

        default:
            Gotoxy(1, 11);
            printf("�Է��ϼ��� :             ");
            continue;
        }
    }

    Gotoxy(1, 10);
    printf("                                          ");
    Gotoxy(1, 11);
    printf("                                          ");
    DrawChess();
    Gotoxy(cursor_x, cursor_y);
}


void BlackPromotion(int x)
{
    int check = 1;
    char a = Invert(x*2);

    while(check)
    {
        int promotion = 0;
        Gotoxy(1, 10);
        printf("                        ");
        Gotoxy(1, 10);
        TextColor(red);
        printf("%c1ĭ ���θ�� 1 �� 2 ��� 3 ����Ʈ 4 ��\n",a);
        TextColor(white);
        Gotoxy(1, 11);
        printf("�Է��ϼ��� : ");
        scanf("%d",&promotion);

        switch(promotion)
        {
        case 1:
            chessPos[7][x] = "BQ0";
            check = 0;
            check_BlackPromotion = 0;
            check_bpPr++;
            check_bqPr++;
            break;

        case 2:
            chessPos[7][x] = "BB0";
            check = 0;
            check_BlackPromotion = 0;
            check_bpPr++;
            check_bbPr++;
            break;

        case 3:
            chessPos[7][x] = "BN0";
            check = 0;
            check_BlackPromotion = 0;
            check_bpPr++;
            check_bnPr++;
            break;

        case 4:
            chessPos[7][x] = "BR0";
            check = 0;
            check_BlackPromotion = 0;
            check_bpPr++;
            check_brPr++;
            break;

        default:
            Gotoxy(1, 11);
            printf("�Է��ϼ��� :           ");
            continue;
        }
    }

    Gotoxy(1, 10);
    printf("                                          ");
    Gotoxy(1, 11);
    printf("                                          ");
    DrawChess();
    Gotoxy(cursor_x, cursor_y);
}


void WhiteKing(int x, int y)
{
    if (x-1 >= 0)
    {
        if (y-1 >= 0)
        {
            if (chessPos[x-1][y-1] == chessPiece[0] || chessPos[x-1][y-1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x-1][y-1] = 1;

                else
                    movable[x-1][y-1] = 1;
            }
        }

        if (y+1 <= 7)
        {
            if (chessPos[x-1][y+1] == chessPiece[0] || chessPos[x-1][y+1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x-1][y+1] = 1;

                else
                    movable[x-1][y+1] = 1;
            }
        }

        if (chessPos[x-1][y] == chessPiece[0] || chessPos[x-1][y][0] == BLACK)
        {
                if (check_check == 1)
                    check[x-1][y] = 1;

                else
                    movable[x-1][y] = 1;
            }
    }

    if (x+1 <= 7)
    {
        if (y-1 >= 0)
        {
            if (chessPos[x+1][y-1] == chessPiece[0] || chessPos[x+1][y-1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x+1][y-1] = 1;

                else
                    movable[x+1][y-1] = 1;
            }
        }

        if (y+1 <= 7)
        {
            if (chessPos[x+1][y+1] == chessPiece[0] || chessPos[x+1][y+1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x+1][y+1] = 1;

                else
                    movable[x+1][y+1] = 1;
            }
        }

        if (chessPos[x+1][y] == chessPiece[0] || chessPos[x+1][y][0] == BLACK)
        {
            if (check_check == 1)
                check[x+1][y] = 1;

            else
                movable[x+1][y] = 1;
        }
    }

    if (y-1 >= 0)
    {
        if (chessPos[x][y-1] == chessPiece[0] || chessPos[x][y-1][0] == BLACK)
        {
            if (check_check == 1)
                check[x][y-1] = 1;

            else
                movable[x][y-1] = 1;
        }
    }

    if (y+1 <= 7)
    {
        if (chessPos[x][y+1] == chessPiece[0] || chessPos[x][y+1][0] == BLACK)
        {
            if (check_check == 1)
                check[x][y+1] = 1;

            else
                movable[x][y+1] = 1;
        }
    }

    if (check_check == 0)
        for (int i=0; i<8; i++)
            for (int j=0; j<8; j++)
                if (check[i][j] == 1)
                    movable[i][j] = 0;

    if (chessPos[x][y] == "WK1")        //ĳ����
    {
        if (chessPos[7][5] == chessPiece[0] && chessPos[7][6] == chessPiece[0] && chessPos[7][7] == "WR1")
            movable[x][y+2] = 3;

        if (chessPos[7][3] == chessPiece[0] && chessPos[7][2] == chessPiece[0] && chessPos[7][1] == chessPiece[0] && chessPos[7][0] == "WR1")
            movable [x][y-2] = 3;
    }
}


void BlackKing(int x, int y)
{
    if (x-1 >= 0)
    {
        if (y-1 >= 0)
        {
            if (chessPos[x-1][y-1] == chessPiece[0] || chessPos[x-1][y-1][0] == WHITE)
            {
                if (check_check == 1)
                    check[x-1][y-1] = 1;

                else
                    movable[x-1][y-1] = 1;
            }
        }

        if (y+1 <= 7)
        {
            if (chessPos[x-1][y+1] == chessPiece[0] || chessPos[x-1][y+1][0] == WHITE)
            {
                if (check_check == 1)
                    check[x-1][y+1] = 1;

                else
                    movable[x-1][y+1] = 1;
            }
        }

        if (chessPos[x-1][y] == chessPiece[0] || chessPos[x-1][y][0] == WHITE)
        {
            if (check_check == 1)
                check[x-1][y] = 1;

            else
                movable[x-1][y] = 1;
        }
    }

    if (x+1 <= 7)
    {
        if (y-1 >= 0)
        {
            if (chessPos[x+1][y-1] == chessPiece[0] || chessPos[x+1][y-1][0] == WHITE)
            {
                if (check_check == 1)
                    check[x+1][y-1] = 1;

                else
                    movable[x+1][y-1] = 1;
            }
        }

        if (y+1 <= 7)
        {
            if (chessPos[x+1][y+1] == chessPiece[0] || chessPos[x+1][y+1][0] == WHITE)
            {
                if (check_check == 1)
                    check[x+1][y+1] = 1;

                else
                    movable[x+1][y+1] = 1;
            }
        }

        if (chessPos[x+1][y] == chessPiece[0] || chessPos[x+1][y][0] == WHITE)
        {
            if (check_check == 1)
                check[x+1][y] = 1;

            else
                movable[x+1][y] = 1;
        }
    }

    if (y-1 >= 0)
    {
        if (chessPos[x][y-1] == chessPiece[0] || chessPos[x][y-1][0] == WHITE)
        {
            if (check_check == 1)
                check[x][y-1] = 1;

            else
                movable[x][y-1] = 1;
        }
    }

    if (y+1 <= 7)
    {
        if (chessPos[x][y+1] == chessPiece[0] || chessPos[x][y+1][0] == WHITE)
        {
            if (check_check == 1)
                check[x][y+1] = 1;

            else
                movable[x][y+1] = 1;
        }
    }

    if (check_check == 0)
        for (int i=0; i<8; i++)
            for (int j=0; j<8; j++)
                if (check[i][j] == 1)
                    movable[i][j] = 0;

    if (chessPos[x][y] == "BK1")
    {
        if (chessPos[0][5] == chessPiece[0] && chessPos[0][6] == chessPiece[0] && chessPos[0][7] == "BR1")
            movable[x][y+2] = 3;

        if (chessPos[0][3] == chessPiece[0] && chessPos[0][2] == chessPiece[0] && chessPos[0][1] == chessPiece[0] && chessPos[0][0] == "BR1")
            movable [x][y-2] = 3;
    }
}


void WhiteQueen(int x, int y)
{
    WhiteRook(x, y);
    WhiteBishop(x, y);
}


void BlackQueen(int x, int y)
{
    BlackRook(x, y);
    BlackBishop(x, y);
}


void WhitePawn(int x, int y)
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


void BlackPawn(int x, int y)
{
    if (chessPos[x][y][2] == '1')
        if (chessPos[x+2][y] == chessPiece[0])
            movable[x+2][y] = 2;

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


void WhiteNight(int x, int y)   // ��� ����Ʈ
{
    if (x-2 >= 0)
    {
        if (y-1 >= 0)
        {
            if (chessPos[x-2][y-1] == chessPiece[0] || chessPos[x-2][y-1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x-2][y-1] = 1;

                else
                    movable[x-2][y-1] = 1;
            }
        }

        if (y+1 <= 7)
        {
            if (chessPos[x-2][y+1] == chessPiece[0] || chessPos[x-2][y+1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x-2][y+1] = 1;

                else
                    movable[x-2][y+1] = 1;
            }
        }
    }

    if (x-1 >= 0)
    {
        if (y-2 >= 0)
        {
            if (chessPos[x-1][y-2] == chessPiece[0] || chessPos[x-1][y-2][0] == BLACK)
            {
                if (check_check == 1)
                    check[x-1][y-2] = 1;

                else
                    movable[x-1][y-2] = 1;
            }
        }

        if (y+2 <= 7)
        {
            if (chessPos[x-1][y+2] == chessPiece[0] || chessPos[x-1][y+2][0] == BLACK)
            {
                if (check_check == 1)
                    check[x-1][y+2] = 1;

                else
                    movable[x-1][y+2] = 1;
            }
        }
    }

    if (x+1 <= 7)
    {
        if (y-2 >= 0)
        {
            if (chessPos[x+1][y-2] == chessPiece[0] || chessPos[x+1][y-2][0] == BLACK)
            {
                if (check_check == 1)
                    check[x+1][y-2] = 1;

                else
                    movable[x+1][y-2] = 1;
            }
        }

        if (y+2 <= 7)
        {
            if (chessPos[x+1][y+2] == chessPiece[0] || chessPos[x+1][y+2][0] == BLACK)
            {
                if (check_check == 1)
                    check[x+1][y+2] = 1;

                else
                    movable[x+1][y+2] = 1;
            }
        }
    }

    if (x+2 <= 7)
    {
        if (y-1 >= 0)
        {
            if (chessPos[x+2][y-1] == chessPiece[0] || chessPos[x+2][y-1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x+2][y-1] = 1;

                else
                    movable[x+2][y-1] = 1;
            }
        }

        if (y+1 <= 7)
        {
            if (chessPos[x+2][y+1] == chessPiece[0] || chessPos[x+2][y+1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x+2][y+1] = 1;

                else
                    movable[x+2][y+1] = 1;
            }
        }
    }
}


void BlackNight(int x, int y)
{
    if (x-2 >= 0)
    {
        if (y-1 >= 0)
        {
            if (chessPos[x-2][y-1] == chessPiece[0] || chessPos[x-2][y-1][0] == WHITE)
            {
                if (check_check == 1)
                    check[x-2][y-1] = 1;

                else
                    movable[x-2][y-1] = 1;
            }
        }

        if (y+1 <= 7)
        {
            if (chessPos[x-2][y+1] == chessPiece[0] || chessPos[x-2][y+1][0] == WHITE)
            {
                if (check_check == 1)
                    check[x-2][y+1] = 1;

                else
                    movable[x-2][y+1] = 1;
            }
        }
    }

    if (x-1 >= 0)
    {
        if (y-2 >= 0)
        {
            if (chessPos[x-1][y-2] == chessPiece[0] || chessPos[x-1][y-2][0] == WHITE)
            {
                if (check_check == 1)
                    check[x-1][y-2] = 1;

                else
                    movable[x-1][y-2] = 1;
            }
        }

        if (y+2 <= 7)
        {
            if (chessPos[x-1][y+2] == chessPiece[0] || chessPos[x-1][y+2][0] == WHITE)
            {
                if (check_check == 1)
                    check[x-1][y+2] = 1;

                else
                    movable[x-1][y+2] = 1;
            }
        }
    }

    if (x+1 <= 7)
    {
        if (y-2 >= 0)
        {
            if (chessPos[x+1][y-2] == chessPiece[0] || chessPos[x+1][y-2][0] == WHITE)
            {
                if (check_check == 1)
                    check[x+1][y-2] = 1;

                else
                    movable[x+1][y-2] = 1;
            }
        }

        if (y+2 <= 7)
        {
            if (chessPos[x+1][y+2] == chessPiece[0] || chessPos[x+1][y+2][0] == WHITE)
            {
                if (check_check == 1)
                    check[x+1][y+2] = 1;

                else
                    movable[x+1][y+2] = 1;
            }
        }
    }

    if (x+2 <= 7)
    {
        if (y-1 >= 0)
        {
            if (chessPos[x+2][y-1] == chessPiece[0] || chessPos[x+2][y-1][0] == WHITE)
            {
                if (check_check == 1)
                    check[x+2][y-1] = 1;

                else
                    movable[x+2][y-1] = 1;
            }
        }

        if (y+1 <= 7)
        {
            if (chessPos[x+2][y+1] == chessPiece[0] || chessPos[x+2][y+1][0] == WHITE)
            {
                if (check_check == 1)
                    check[x+2][y+1] = 1;

                else
                    movable[x+2][y+1] = 1;
            }
        }
    }
}


void WhiteRook(int x, int y){

    if(x != 0)
    {
        for(int i = 1; i <= x; i++)
        {
            if(chessPos[x-i][y][0] == WHITE)
                break;

            if(chessPos[x-i][y] == chessPiece[0] || chessPos[x-i][y][0] == BLACK)
            {
                movable[x-i][y]=1;
                if(chessPos[x-i][y][0] == BLACK)
                    break;
            }
        }
    }

    if(x != 7)
    {
        for(int i = 1; i < 8-x; i++)
        {
            if(chessPos[x+i][y][0] == WHITE)
                break;

            if(chessPos[x+i][y] == chessPiece[0] || chessPos[x+i][y][0] == BLACK)
            {
                movable[x+i][y]=1;

                if(chessPos[x+i][y][0] == BLACK)
                    break;
            }
        }
    }

    if(y != 0)
    {
        for(int i = 1; i <= y; i++)
        {
            if(chessPos[x][y-i][0] == WHITE)
                break;

            if(chessPos[x][y-i] == chessPiece[0] || chessPos[x][y-i][0] == BLACK)
            {
                movable[x][y-i]=1;

                if(chessPos[x][y-i][0] == BLACK)
                    break;
            }
        }
    }

    if(y != 7)
    {
        for(int i = 1; i < 8-y; i++)
        {
            if(chessPos[x][y+i][0] == WHITE)
                break;

            if(chessPos[x][y+i] == chessPiece[0] || chessPos[x][y+i][0] == BLACK)
            {
                movable[x][y+i]=1;

                if(chessPos[x][y+i][0] == BLACK)
                    break;
            }
        }
    }
}


void BlackRook(int x, int y)
{

    if(x != 0)
    {
        for(int i = 1; i <= x; i++)
        {
            if(chessPos[x-i][y][0] == BLACK)
                break;

            if(chessPos[x-i][y] == chessPiece[0] || chessPos[x-i][y][0] == WHITE)
            {
                movable[x-i][y]=1;

                if(chessPos[x-i][y][0] == WHITE)
                    break;
            }
        }
    }

    if(x != 7)
    {
        for(int i = 1; i < 8-x; i++)
        {
            if(chessPos[x+i][y][0] == BLACK)
                break;

            if(chessPos[x+i][y] == chessPiece[0] || chessPos[x+i][y][0] == WHITE)
            {
                movable[x+i][y]=1;

                if(chessPos[x+i][y][0] == WHITE)
                    break;
            }
        }
    }

    if(y != 0)
    {
        for(int i = 1;i <= y; i++)
        {
            if(chessPos[x][y-i][0] == BLACK)
                break;

            if(chessPos[x][y-i] == chessPiece[0] || chessPos[x][y-i][0] == WHITE)
            {
                movable[x][y-i]=1;

                if(chessPos[x][y-i][0] == WHITE)
                    break;
            }
        }
    }

    if(y != 7)
    {
        for(int i = 1; i < 8-y ; i++)
        {
            if(chessPos[x][y+i][0] == BLACK)
                break;

            if(chessPos[x][y+i] == chessPiece[0]||chessPos[x][y+i][0] == WHITE)
            {
                movable[x][y+i]=1;

                if(chessPos[x][y+i][0] == WHITE)
                    break;
            }
        }
    }
}


void BlackBishop(int x,int y)
{
    int i;

    for(i = 1; i < 8; i++)
    {
        if((x-i) < 0 || (y-i) < 0)
            break;

        else if(chessPos[x-i][y-i][0] == BLACK)
            break;

        movable[x-i][y-i] = 1;

        if(chessPos[x-i][y-i][0] == WHITE)
            break;
    }

    for(i = 1; i < 8; i++)
    {
        if((x-i) < 0 || (y+i) > 7)
            break;

        else if(chessPos[x-i][y+i][0] == BLACK)
            break;

        movable[x-i][y+i]=1;

        if(chessPos[x-i][y+i][0]==WHITE)
            break;
    }

    for(i = 1; i < 8; i++)
    {
        if((x+i) > 7 || (y-i) < 0)
            break;

        else if(chessPos[x+i][y-i][0] == BLACK)
            break;

        movable[x+i][y-i]=1;

        if(chessPos[x+i][y-i][0] == WHITE)
            break;
    }

    for(i = 1; i < 8; i++)
    {
        if((x+i) > 7 || (y+i) > 7)
            break;

        else if(chessPos[x+i][y+i][0] == BLACK)
            break;

        movable[x+i][y+i]=1;

        if(chessPos[x+i][y+i][0] == WHITE)
            break;
    }
}


void WhiteBishop(int x,int y)
{
    int i;

    for(i = 1; i < 8; i++)
    {
        if((x-i) < 0 || (y-i) < 0)
            break;

        else if(chessPos[x-i][y-i][0] == WHITE)
            break;

        movable[x-i][y-i]=1;

        if(chessPos[x-i][y-i][0] == BLACK)
            break;
    }

    for(i = 1; i < 8; i++)
    {
        if((x-i) < 0 || (y+i) > 7)
            break;

        else if(chessPos[x-i][y+i][0] == WHITE)
            break;

        movable[x-i][y+i]=1;

        if(chessPos[x-i][y+i][0] == BLACK)
            break;
    }

    for(i = 1; i < 8; i++)
    {
        if((x+i) > 7 || (y-i) < 0)
            break;

        else if(chessPos[x+i][y-i][0] == WHITE)
            break;

        movable[x+i][y-i]=1;

        if(chessPos[x+i][y-i][0] == BLACK)
            break;
    }

    for(i = 1; i < 8; i++)
    {
        if((x+i) > 7 || (y+i) > 7)
            break;

        else if(chessPos[x+i][y+i][0] == WHITE)
            break;

        movable[x+i][y+i]=1;

        if(chessPos[x+i][y+i][0] == BLACK)
            break;
    }
}


void Gotoxy(int x, int y)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleHandle, pos);
}


void Erase(int x, int y)
{
	Gotoxy(x, y);
	printf(" ");
}


char Invert(int x)
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


void TextColor(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}


void BackColor(int color_number)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hC, &csbi);
    SetConsoleTextAttribute(hC, color_number);
}


void ViewCursor(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}


void WhiteCheck()
{
    int i,j;

    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            check[i][j] = 0;

    for(i = 0; i < 8; i++)
        for(j = 0; j < 8; j++)
        {
            if (chessPos[i][j][0] == WHITE)
                if (chessPos[i][j][1] != 'K')
                    check[i][j] = 1;

            if (chessPos[i][j][0] == BLACK)
            {
                if (chessPos[i][j][1] == 'P')
                    if (i < 7)
                    {
                        if (chessPos[i+1][j-1][0] == WHITE || chessPos[i+1][j-1] == chessPiece[0])
                            check[i+1][j-1] = 1;

                        if (chessPos[i+1][j+1][0] == WHITE || chessPos[i+1][j+1] == chessPiece[0])
                            check[i+1][j+1] = 1;
                    }

                if (chessPos[i][j][1] == 'N')
                    BlackNight(i, j);

                if (chessPos[i][j][1] == 'R')
                {
                    if (i != 0)
                    {
                        for (int k = 0; k <= i; k++)
                        {
                            if (chessPos[i-k][j] == chessPiece[0])
                                check[i-k][j] = 1;

                            if (chessPos[i-k][j][0] == BLACK || chessPos[i-k][j][0] == WHITE)
                            {
                                if (chessPos[i-k][j][0] == WHITE && chessPos[i-k][j][1] == 'K')
                                {
                                    check[i-k][j] = 1;
                                    if (chessPos[i-k-1][j] == chessPiece[0])
                                        check[i-k-1][j] = 1;
                                }
                            break;
                            }
                        }
                    }

                    if (i != 7)
                    {
                        for (int k = 1; k < 8-i; k++)
                        {
                            if (chessPos[i+k][j] == chessPiece[0])
                                check[i+k][j] = 1;

                            if (chessPos[i+k][j][0] == BLACK || chessPos[i+k][j][0] == WHITE)
                            {
                                if (chessPos[i+k][j][0] == WHITE && chessPos[i+k][j][1] == 'K')
                                {
                                    check[i+k][j] = 1;

                                    if (chessPos[i+k+1][j] == chessPiece[0])
                                        check[i+k+1][j] = 1;
                                }
                            break;
                            }
                        }
                    }

                    if (j != 0)
                    {
                        for (int k = 1; k <= j; k++)
                        {
                            if (chessPos[i][j-k] == chessPiece[0])
                                check[i][j-k] = 1;

                            if (chessPos[i][j-k][0] == BLACK || chessPos[i][j-k][0] == WHITE)
                            {
                                if (chessPos[i][j-k][0] == WHITE && chessPos[i][j-k][1] == 'K')
                                {
                                    check[i][j-k] = 1;

                                    if (chessPos[i][j-k-1] == chessPiece[0])
                                        check[i][j-k-1] = 1;
                                }
                                break;
                            }
                        }
                    }

                    if (j != 7)
                    {
                        for (int k = 1; k <= 8-j; k++)
                        {
                            if (chessPos[i][j+k] == chessPiece[0])
                                check[i][j+k] = 1;

                            if (chessPos[i][j+k][0] == BLACK || chessPos[i][j+k][0] == WHITE)
                            {
                                if (chessPos[i][j+k][0] == WHITE && chessPos[i][j+k][1] == 'K')
                                {
                                    check[i][j+k] = 1;

                                    if (chessPos[i][j+k+1] == chessPiece[0])
                                        check[i][j+k+1] = 1;
                                }
                                break;
                            }
                        }
                    }
                }

                if (chessPos[i][j][1] == 'B')
                {
                    for (int k = 1; k < 8; k++)
                    {
                        if ((i-k < 0) || (j-k < 0))
                            break;

                        if (chessPos[i-k][j-k][0] == BLACK)
                            break;

                        check[i-k][j-k] = 1;

                        if (chessPos[i-k][j-k][0] == WHITE)
                        {
                            if (chessPos[i-k][j-k][1] == 'K')
                            {
                                check[i-k][j-k] = 1;

                                if (chessPos[i-k-1][j-k-1] == chessPiece[0])
                                    check[i-k-1][j-k-1] = 1;
                            }
                        break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i-k < 0) || (j+k < 0))
                            break;

                        if (chessPos[i-k][j+k][0] == BLACK)
                            break;

                        check[i-k][j+k] = 1;

                        if (chessPos[i-k][j+k][0] == WHITE)
                        {
                            if (chessPos[i-k][j+k][1] == 'K')
                            {
                                check[i-k][j+k] = 1;

                                if (chessPos[i-k-1][j+k+1] == chessPiece[0])
                                    check[i-k-1][j+k+1] = 1;
                            }
                        break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i+k < 0) || (j-k < 0))
                            break;

                        if (chessPos[i+k][j-k][0] == BLACK)
                            break;

                        check[i+k][j-k] = 1;

                        if (chessPos[i+k][j-k][0] == WHITE)
                        {
                            if (chessPos[i+k][j-k][1] == 'K')
                            {
                                check[i+k][j-k] = 1;

                                if (chessPos[i+k+1][j-k-1] == chessPiece[0])
                                    check[i+k+1][j-k-1] = 1;
                            }
                        break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i+k < 0) || (j+k < 0))
                            break;

                        if (chessPos[i+k][j+k][0] == BLACK)
                            break;

                        check[i+k][j+k] = 1;

                        if (chessPos[i+k][j+k][0] == WHITE)
                        {
                            if (chessPos[i+k][j+k][1] == 'K')
                            {
                                check[i+k][j+k] = 1;

                                if (chessPos[i+k+1][j+k+1] == chessPiece[0])
                                    check[i+k+1][j+k+1] = 1;
                            }
                        break;
                        }
                    }
                }

                if (chessPos[i][j][1] == 'Q')
                {
                    if (i != 0)
                    {
                        for (int k = 0; k <= i; k++)
                        {
                            if (chessPos[i-k][j] == chessPiece[0])
                                check[i-k][j] = 1;

                            if (chessPos[i-k][j][0] == BLACK || chessPos[i-k][j][0] == WHITE)
                            {
                                if (chessPos[i-k][j][0] == WHITE && chessPos[i-k][j][1] == 'K')
                                {
                                    check[i-k][j] = 1;

                                    if (chessPos[i-k-1][j] == chessPiece[0])
                                        check[i-k-1][j] = 1;
                                }
                                break;
                            }
                        }
                    }

                    if (i != 7)
                    {
                        for (int k = 1; k < 8-i; k++)
                        {
                            if (chessPos[i+k][j] == chessPiece[0])
                                check[i+k][j] = 1;

                            if (chessPos[i+k][j][0] == BLACK || chessPos[i+k][j][0] == WHITE)
                            {
                                if (chessPos[i+k][j][0] == WHITE && chessPos[i+k][j][1] == 'K')
                                {
                                    check[i+k][j] = 1;
                                    if (chessPos[i+k+1][j] == chessPiece[0])
                                        check[i+k+1][j] = 1;
                                }
                                break;
                            }
                        }
                    }

                    if (j != 0)
                    {
                        for (int k = 1; k <= j; k++)
                        {
                            if (chessPos[i][j-k] == chessPiece[0])
                                check[i][j-k] = 1;

                            if (chessPos[i][j-k][0] == BLACK || chessPos[i][j-k][0] == WHITE)
                            {
                                if (chessPos[i][j-k][0] == WHITE && chessPos[i][j-k][1] == 'K')
                                {
                                    check[i][j-k] = 1;

                                    if (chessPos[i][j-k-1] == chessPiece[0])
                                        check[i][j-k-1] = 1;
                                }
                                break;
                            }
                        }
                    }

                    if (j != 7)
                    {
                        for (int k = 1; k <= 8-j; k++)
                        {
                            if (chessPos[i][j+k] == chessPiece[0])
                                check[i][j+k] = 1;

                            if (chessPos[i][j+k][0] == BLACK || chessPos[i][j+k][0] == WHITE)
                            {
                                if (chessPos[i][j+k][0] == WHITE && chessPos[i][j+k][1] == 'K')
                                {
                                    check[i][j+k] = 1;
                                    if (chessPos[i][j+k+1] == chessPiece[0])
                                        check[i][j+k+1] = 1;
                                }
                                break;
                            }
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i-k < 0) || (j-k < 0))
                            break;

                        if (chessPos[i-k][j-k][0] == BLACK)
                            break;

                        check[i-k][j-k] = 1;

                        if (chessPos[i-k][j-k][0] == WHITE)
                        {
                            if (chessPos[i-k][j-k][1] == 'K')
                            {
                                check[i-k][j-k] = 1;

                                if (chessPos[i-k-1][j-k-1] == chessPiece[0])
                                    check[i-k-1][j-k-1] = 1;
                            }
                        break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i-k < 0) || (j+k < 0))
                            break;

                        if (chessPos[i-k][j+k][0] == BLACK)
                            break;

                        check[i-k][j+k] = 1;

                        if (chessPos[i-k][j+k][0] == WHITE)
                        {
                            if (chessPos[i-k][j+k][1] == 'K')
                            {
                                check[i-k][j+k] = 1;

                                if (chessPos[i-k-1][j+k+1] == chessPiece[0])
                                    check[i-k-1][j+k+1] = 1;
                            }
                            break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i+k < 0) || (j-k < 0))
                            break;

                        if (chessPos[i+k][j-k][0] == BLACK)
                            break;

                        check[i+k][j-k] = 1;

                        if (chessPos[i+k][j-k][0] == WHITE)
                        {
                            if (chessPos[i+k][j-k][1] == 'K')
                            {
                                check[i+k][j-k] = 1;

                                if (chessPos[i+k+1][j-k-1] == chessPiece[0])
                                    check[i+k+1][j-k-1] = 1;
                            }
                        break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i+k < 0) || (j+k < 0))
                            break;

                        if (chessPos[i+k][j+k][0] == BLACK)
                            break;

                        check[i+k][j+k] = 1;

                        if (chessPos[i+k][j+k][0] == WHITE)
                        {
                            if (chessPos[i+k][j+k][1] == 'K')
                            {
                                check[i+k][j+k] = 1;

                                if (chessPos[i+k+1][j+k+1] == chessPiece[0])
                                    check[i+k+1][j+k+1] = 1;
                            }
                        break;
                        }
                    }
                }

                if (chessPos[i][j][1] == 'K')
                    BlackKing(i, j);
        }
    }
}


void BlackCheck()
{
    int i,j;

    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            check[i][j] = 0;

    for(i = 0; i < 8; i++)
        for(j = 0; j < 8; j++)
        {
            if (chessPos[i][j][0] == BLACK)
            {
                if (chessPos[i][j][1] != 'K')
                    check[i][j] = 1;
            }

            if (chessPos[i][j][0] == WHITE)
            {
                if (chessPos[i][j][1] == 'P')
                    if (i > 0)
                    {
                        if (chessPos[i-1][j-1][0] == BLACK || chessPos[i-1][j-1] == chessPiece[0])
                            check[i-1][j-1] = 1;

                        if (chessPos[i-1][j+1][0] == BLACK || chessPos[i-1][j+1] == chessPiece[0])
                            check[i-1][j+1] = 1;
                    }

                if (chessPos[i][j][1] == 'N')
                    WhiteNight(i, j);

                if (chessPos[i][j][1] == 'R')
                {
                    if (i != 0)
                    {
                        for (int k = 0; k <= i; k++)
                        {
                            if (chessPos[i-k][j] == chessPiece[0])
                                check[i-k][j] = 1;

                            if (chessPos[i-k][j][0] == BLACK || chessPos[i-k][j][0] == WHITE)
                            {
                                if (chessPos[i-k][j][0] == BLACK && chessPos[i-k][j][1] == 'K')
                                {
                                    check[i-k][j] = 1;
                                    if (chessPos[i-k-1][j] == chessPiece[0])
                                        check[i-k-1][j] = 1;
                                }
                            break;
                            }
                        }
                    }

                    if (i != 7)
                    {
                        for (int k = 1; k < 8-i; k++)
                        {
                            if (chessPos[i+k][j] == chessPiece[0])
                                check[i+k][j] = 1;

                            if (chessPos[i+k][j][0] == BLACK || chessPos[i+k][j][0] == WHITE)
                            {
                                if (chessPos[i+k][j][0] == BLACK && chessPos[i+k][j][1] == 'K')
                                {
                                    check[i+k][j] = 1;

                                    if (chessPos[i+k+1][j] == chessPiece[0])
                                        check[i+k+1][j] = 1;
                                }
                            break;
                            }
                        }
                    }

                    if (j != 0)
                    {
                        for (int k = 1; k <= j; k++)
                        {
                            if (chessPos[i][j-k] == chessPiece[0])
                                check[i][j-k] = 1;

                            if (chessPos[i][j-k][0] == BLACK || chessPos[i][j-k][0] == WHITE)
                            {
                                if (chessPos[i][j-k][0] == BLACK && chessPos[i][j-k][1] == 'K')
                                {
                                    check[i][j-k] = 1;

                                    if (chessPos[i][j-k-1] == chessPiece[0])
                                            check[i][j-k-1] = 1;
                                }
                                break;
                            }
                        }
                    }

                    if (j != 7)
                    {
                        for (int k = 1; k <= 8-j; k++)
                        {
                            if (chessPos[i][j+k] == chessPiece[0])
                                check[i][j+k] = 1;

                            if (chessPos[i][j+k][0] == BLACK || chessPos[i][j+k][0] == WHITE)
                            {
                                if (chessPos[i][j+k][0] == BLACK && chessPos[i][j+k][1] == 'K')
                                {
                                    check[i][j+k] = 1;

                                    if (chessPos[i][j+k+1] == chessPiece[0])
                                        check[i][j+k+1] = 1;
                                }
                                break;
                            }
                        }
                    }
                }

                if (chessPos[i][j][1] == 'B')
                {
                    for (int k = 1; k < 8; k++)
                    {
                        if ((i-k < 0) || (j-k < 0))
                            break;

                        if (chessPos[i-k][j-k][0] == WHITE)
                            break;

                        check[i-k][j-k] = 1;

                        if (chessPos[i-k][j-k][0] == BLACK)
                        {
                            if (chessPos[i-k][j-k][1] == 'K')
                            {
                                check[i-k][j-k] = 1;
                                if (chessPos[i-k-1][j-k-1] == chessPiece[0])
                                    check[i-k-1][j-k-1] = 1;
                            }
                        break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i-k < 0) || (j+k < 0))
                            break;

                        if (chessPos[i-k][j+k][0] == WHITE)
                            break;

                        check[i-k][j+k] = 1;

                        if (chessPos[i-k][j+k][0] == BLACK)
                        {
                            if (chessPos[i-k][j+k][1] == 'K')
                            {
                                check[i-k][j+k] = 1;

                                if (chessPos[i-k-1][j+k+1] == chessPiece[0])
                                    check[i-k-1][j+k+1] = 1;
                            }
                        break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i+k < 0) || (j-k < 0))
                            break;

                        if (chessPos[i+k][j-k][0] == WHITE)
                            break;

                        check[i+k][j-k] = 1;

                        if (chessPos[i+k][j-k][0] == BLACK)
                        {
                            if (chessPos[i+k][j-k][1] == 'K')
                            {
                                check[i+k][j-k] = 1;

                                if (chessPos[i+k+1][j-k-1] == chessPiece[0])
                                    check[i+k+1][j-k-1] = 1;
                            }
                        break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i+k < 0) || (j+k < 0))
                            break;

                        if (chessPos[i+k][j+k][0] == WHITE)
                            break;

                        check[i+k][j+k] = 1;

                        if (chessPos[i+k][j+k][0] == BLACK)
                        {
                            if (chessPos[i+k][j+k][1] == 'K')
                            {
                                check[i+k][j+k] = 1;

                                if (chessPos[i+k+1][j+k+1] == chessPiece[0])
                                    check[i+k+1][j+k+1] = 1;
                            }
                        break;
                        }
                    }
                }

                if (chessPos[i][j][1] == 'Q')
                {
                    if (i != 0)
                    {
                        for (int k = 0; k <= i; k++)
                        {
                            if (chessPos[i-k][j] == chessPiece[0])
                                check[i-k][j] = 1;

                            if (chessPos[i-k][j][0] == BLACK || chessPos[i-k][j][0] == WHITE)
                            {
                                if (chessPos[i-k][j][0] == BLACK && chessPos[i-k][j][1] == 'K')
                                {
                                    check[i-k][j] = 1;

                                    if (chessPos[i-k-1][j] == chessPiece[0])
                                        check[i-k-1][j] = 1;
                                }
                                break;
                            }
                        }
                    }

                    if (i != 7)
                    {
                        for (int k = 1; k < 8-i; k++)
                        {
                            if (chessPos[i+k][j] == chessPiece[0])
                                check[i+k][j] = 1;

                            if (chessPos[i+k][j][0] == BLACK || chessPos[i+k][j][0] == WHITE)
                            {
                                if (chessPos[i+k][j][0] == BLACK && chessPos[i+k][j][1] == 'K')
                                {
                                    check[i+k][j] = 1;
                                    if (chessPos[i+k+1][j] == chessPiece[0])
                                        check[i+k+1][j] = 1;
                                }
                                break;
                            }
                        }
                    }

                    if (j != 0)
                    {
                        for (int k = 1; k <= j; k++)
                        {
                            if (chessPos[i][j-k] == chessPiece[0])
                                check[i][j-k] = 1;

                            if (chessPos[i][j-k][0] == BLACK || chessPos[i][j-k][0] == WHITE)
                            {
                                if (chessPos[i][j-k][0] == BLACK && chessPos[i][j-k][1] == 'K')
                                {
                                    check[i][j-k] = 1;
                                    if (chessPos[i][j-k-1] == chessPiece[0])
                                        check[i][j-k-1] = 1;
                                }
                                break;
                            }
                        }
                    }

                    if (j != 7)
                    {
                        for (int k = 1; k <= 8-j; k++)
                        {
                            if (chessPos[i][j+k] == chessPiece[0])
                                check[i][j+k] = 1;

                            if (chessPos[i][j+k][0] == BLACK || chessPos[i][j+k][0] == WHITE)
                            {
                                if (chessPos[i][j+k][0] == BLACK && chessPos[i][j+k][1] == 'K')
                                {
                                    check[i][j+k] = 1;
                                    if (chessPos[i][j+k+1] == chessPiece[0])
                                        check[i][j+k+1] = 1;
                                }
                                break;
                            }
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i-k < 0) || (j-k < 0))
                            break;

                        if (chessPos[i-k][j-k][0] == WHITE)
                            break;

                        check[i-k][j-k] = 1;

                        if (chessPos[i-k][j-k][0] == BLACK)
                        {
                            if (chessPos[i-k][j-k][1] == 'K')
                            {
                                check[i-k][j-k] = 1;

                                if (chessPos[i-k-1][j-k-1] == chessPiece[0])
                                    check[i-k-1][j-k-1] = 1;
                            }
                        break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i-k < 0) || (j+k < 0))
                            break;

                        if (chessPos[i-k][j+k][0] == WHITE)
                            break;

                        check[i-k][j+k] = 1;

                        if (chessPos[i-k][j+k][0] == BLACK)
                        {
                            if (chessPos[i-k][j+k][1] == 'K')
                            {
                                check[i-k][j+k] = 1;

                                if (chessPos[i-k-1][j+k+1] == chessPiece[0])
                                    check[i-k-1][j+k+1] = 1;
                            }
                            break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i+k < 0) || (j-k < 0))
                            break;

                        if (chessPos[i+k][j-k][0] == WHITE)
                            break;

                        check[i+k][j-k] = 1;

                        if (chessPos[i+k][j-k][0] == BLACK)
                        {
                            if (chessPos[i+k][j-k][1] == 'K')
                            {
                                check[i+k][j-k] = 1;
                                if (chessPos[i+k+1][j-k-1] == chessPiece[0])
                                    check[i+k+1][j-k-1] = 1;
                            }
                        break;
                        }
                    }

                    for (int k = 1; k < 8; k++)
                    {
                        if ((i+k < 0) || (j+k < 0))
                            break;

                        if (chessPos[i+k][j+k][0] == WHITE)
                            break;

                        check[i+k][j+k] = 1;

                        if (chessPos[i+k][j+k][0] == BLACK)
                        {
                            if (chessPos[i+k][j+k][1] == 'K')
                            {
                                check[i+k][j+k] = 1;

                                if (chessPos[i+k+1][j+k+1] == chessPiece[0])
                                    check[i+k+1][j+k+1] = 1;
                            }
                        break;
                        }
                    }
                }

                if (chessPos[i][j][1] == 'K')
                    WhiteKing(i, j);
        }
    }
}


void CheckCheck()
{
    wking_check = 0;
    bking_check = 0;

    if (wk_x == 0)
    {
        if (wk_y == 0)
            wking_check = 5;

        if (wk_y > 0 && wk_y <8)
            wking_check = 3;

        if (wk_y == 7)
            wking_check = 5;
    }

    else if (wk_x == 7)
    {
        if (wk_y == 0)
            wking_check = 5;

        if (wk_y > 0 && wk_y < 8)
            wking_check = 3;

        if (wk_y == 7)
            wking_check = 5;
    }

    else if (wk_y == 0)
    {
        if (wk_x == 0)
            wking_check = 5;

        if (wk_x > 0 && wk_x <8)
            wking_check = 3;

        if (wk_x == 7)
            wking_check = 5;
    }

    else if (wk_y == 7)
    {
        if (wk_x == 0)
            wking_check = 5;

        if (wk_x > 0 && wk_x < 8)
            wking_check = 3;

        if (wk_x == 7)
            wking_check = 5;
    }

    else if (bk_x == 0)
    {
        if (bk_y == 0)
            bking_check = 5;

        if (bk_y > 0 && bk_y <8)
            bking_check = 3;

        if (bk_y == 7)
            bking_check = 5;
    }

    else if (bk_x == 7)
    {
        if (bk_y == 0)
            bking_check = 5;

        if (bk_y > 0 && bk_y < 8)
            bking_check = 3;

        if (bk_y == 7)
            bking_check = 5;
    }

    else if (bk_y == 0)
    {
        if (bk_x == 0)
            bking_check = 5;

        if (bk_x > 0 && bk_x <8)
            bking_check = 3;

        if (bk_x == 7)
            bking_check = 5;
    }

    else if (bk_y == 7)
    {
        if (bk_x == 0)
            bking_check = 5;

        if (bk_x > 0 && bk_x < 8)
            bking_check = 3;

        if (bk_x == 7)
            bking_check = 5;
    }

    if (check[wk_x][wk_y] == 1)
    {
        if (wk_x+1 <=7)
        {
            if (wk_y+1 <=7)
                if (check[wk_x+1][wk_y+1] == 1)
                    wking_check++;

            if (wk_y-1 >= 0)
                if (check[wk_x+1][wk_y-1] == 1)
                    wking_check++;

            if (check[wk_x+1][wk_y] == 1)
                    wking_check++;
        }

        if (wk_x-1 >= 0)
        {
            if (wk_y+1 <= 7)
                if (check[wk_x-1][wk_y+1] == 1)
                    wking_check++;

            if (wk_y-1 >= 0)
                if (check[wk_x-1][wk_y-1] == 1)
                    wking_check++;

            if (check[wk_x-1][wk_y] == 1)
                wking_check++;
        }
        if (wk_y+1 <= 7)
            if (check[wk_x][wk_y+1] == 1)
                wking_check++;

        if (wk_y-1 >= 0)
            if (check[wk_x][wk_y-1] == 1)
                wking_check++;

        if (wking_check == 8)
            WhiteCheckmate_Alert();

        else
            WhiteCheck_Alert();
    }

    if (check[bk_x][bk_y] == 1)
    {
        if (bk_x+1 <= 7)
        {
            if (bk_y+1 <=7)
                if (check[bk_x+1][bk_y+1] == 1)
                    bking_check++;

            if (bk_y-1 >= 0)
                if (check[bk_x+1][bk_y-1] == 1)
                    bking_check++;

            if (check[bk_x+1][bk_y] == 1)
                    bking_check++;
        }

        if (bk_x-1 >= 0)
        {
            if (bk_y+1 <= 7)
                if (check[bk_x-1][bk_y+1] == 1)
                    bking_check++;
            if (bk_y-1 >= 0)
                if (check[bk_x-1][bk_y-1] == 1)
                    bking_check++;

            if (check[bk_x-1][bk_y] == 1)
                bking_check++;
        }

        if (bk_y+1 <=7)
            if (check[bk_x][bk_y+1] == 1)
                bking_check++;

        if (bk_y-1 >= 0)
            if (check[bk_x][bk_y-1] == 1)
                bking_check++;

        if (bking_check == 8)
            BlackCheckmate_Alert();

        else
            BlackCheck_Alert();
        }
}
