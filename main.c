<<<<<<< Updated upstream
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
    //system("mode con cols=40 lines=15"); //콘솔창 크기 설정 cols :가로, lines :세로
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
			printf("게임을 종료합니다...\n");
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
    timecheck();
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
		timecheck();
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

void cursorPosDraw(int x, int y)
{
    textcolor(white);
    char x_pos=invert(x);
    int y_pos=y+1;
    gotoxy(20,1);
    printf("x축 위치 : %c, y축 위치 :%d",x_pos,y_pos);
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
    int time = 300;
    int min;
    int sec;
    while(1)
    {
        min = time / 60;
        sec = time % 60;
        gotoxy(20, 0);
        printf("남은시간 = %2d:%2d\n", min, sec);
        Sleep(1000);
        time--;
        gotoxy(cursor_x,cursor_y);
        if(time ==0)
        {
            system("cls");
            gotoxy(7,4);
            printf("시간초과\n");
        }
    }
}
=======
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

void titleDraw();   //����씠��� �솕硫� 異쒕젰
void chessDraw();   //泥댁뒪�뙋 異쒕젰
void pieceNowDraw();    //�꽑�깮�맂 留� �몴�떆
void pieceDeadDraw();   //二쎌�� 留� �몴�떆
void cursorDraw(int, int);  //而ㅼ꽌 '>' 異쒕젰
void pieceDraw(int, int);
void orderDraw();   //�닚�꽌 �몴�떆
void sooDraw();     //紐� �닔�씤吏� �몴�떆
void chessRule();   //泥댁뒪 洹쒖튃 �꽕紐낅궡�슜 異쒕젰
void controlRule(); //議곗옉 �꽕紐낅궡�슜 異쒕젰
unsigned _stdcall timerDraw(void *);  //�뒪�젅�뱶瑜� �씠�슜�븳 ����씠癒� �븿�닔

void pieceMove(int, int);   //留� �씠�룞 �븿�닔
void pieceSelect(int, int);
void gameInit();    //寃뚯엫 珥덇린�솕
void gameCheck();   //寃뚯엫 �궡 泥댄겕 �궗�빆 �솗�씤
void turnPiece(int, int);

void whitePawn(int, int);
void blackPawn(int, int);

void whitePromotion(int);
void blackPromotion(int);

void wrongMove_Alert(); //�옒紐삳맂 ���吏곸엫 寃쎄퀬
void wrongOrder_Alert();    //�닚�꽌 寃쎄퀬
void whiteWin_Alert();
void blackWin_Alert();
void timer_Alert();

void CursorView(char); //而ㅼ꽌 �닾紐� �븿�닔
void gotoxy(int, int);  //肄섏넄�궡 醫뚰몴 �씠�룞
void erase(int, int);   //�빐�떦 醫뚰몴 吏��슦湲�
char invert(int);
void textcolor(int);    //肄섏넄�궡 湲��뵪 �깋源� 蹂�寃�
void backcolor(int);    //肄섏넄�궡 諛뷀깢 �깋源� 蹂�寃�

int cursor_x = mainX, cursor_y = mainY;
int sel_x, sel_y;   //�뒪�럹�씠�뒪諛붾줈 怨⑤옄�쓣 �떦�떆�쓽 x y 醫뚰몴
char sel_color = NONE;
char *sel_piece;
int sel_check = 0;
int order = 0;    //吏앹닔硫� 諛�, ����닔硫� �쓳
int soo = 1;
int time = SET_TIME;
int check_error = 0;
int check_winner = 0;  //1�씠硫� 諛깊�� �듅, 2硫� �쓳��� �듅
int check_rule = 0;
int temp;   //留� �떆�옉 珥덇린�솕
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
char *chessPos[8][8] = { {"BR1","BN1","BB1","BQ1","BK1","BB1","BN1","BR1"}, //1��� �븳踰덈룄 ���吏곸씠吏� �븡��� 留�, 0��� �븳踰� �씠�긽 ���吏곸씤 留�
						{"BP1","BP1","BP1","BP1","BP1","BP1","BP1","BP1"},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"WP1","WP1","WP1","WP1","WP1","WP1","WP1","WP1"},
						{"WR1","WN1","WB1","WQ1","WK1","WB1","WN1","WR1"} };

char *chessPiece[13] = { "  ","BR","BN","BB","BQ","BK","BP","WR","WN","WB","WQ","WK","WP" };

int movable[8][8] = {{0,0,0,0,0,0,0,0},     //0��� �씠�룞 遺덇�� 1��� �씠�룞 媛��뒫
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
			printf("�뼱�뼡 �꽕紐낆쓣 �뱾�쑝�떆寃좎뒿�땲源�?");
			gotoxy(20, 6);
			printf("寃뚯엫 議곗옉 洹쒖튃");
			gotoxy(20, 7);
			printf("泥댁뒪 洹쒖튃");
			textcolor(grey);
			gotoxy(0, 16);
			printf("硫붿씤�솕硫댁쑝濡� 媛�湲� (Backspace)");
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
			printf("寃뚯엫�쓣 醫낅즺�빀�땲�떎...\n");
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
		printf("Test M%d %d %d C[2]%c ",movable[cursor_y][cursor_x/2],check_wpPr,check_wqPr,chessPos[cursor_y][cursor_x/2][2]);  //蹂��닔媛� �솗�씤�슜 (吏��슱 寃�)
		gotoxy(cursor_x, cursor_y);

        int x_pos = cursor_y;
		int y_pos = cursor_x / 2;   //而ㅼ꽌 �쐞移섎�� 諛곗뿴 �뻾 �뿴濡� 蹂��솚
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
			if (sel_check == 0) //1. 留� �꽑�깮�쓣 �븞�뻽�쓣 寃쎌슦
			{
			    if (chessPos[x_pos][y_pos] != chessPiece[0])    //1-1. �꽑�깮�븳 移몄뿉 留먯씠 �엳�쓣 �븣
				{
				    if (chessPos[x_pos][y_pos][0] == 'B')
					sel_color = BLACK;
                    else
					sel_color = WHITE;

                    if ((order % 2 == 0 && sel_color == BLACK) || (order % 2 != 0 && sel_color == WHITE))   //李⑤��媛� 諛깆씤�뜲 寃���� 留먯쓣 怨좊Ⅸ 寃쎌슦, 李⑤��媛� 寃��씤�뜲 �씛 留먯쓣 怨좊Ⅸ 寃쎌슦
                    {
                        wrongOrder_Alert();
                        gotoxy(20, 2);
                        printf("�쁽�옱 怨좊Ⅸ 留� : �뾾�쓬       ");
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
				else if (chessPos[x_pos][y_pos] == chessPiece[0])    //1-2. �꽑�깮�븳 移몄씠 鍮덉뭏�씪 �븣
                {
                    gotoxy(1, 10);
                    printf("                        ");
                    gotoxy(cursor_x, cursor_y);
                    sel_check = 0;
                    pieceSelect(x_pos, y_pos);
                    pieceNowDraw();
                }
			}

			else if (sel_check == 1)    //留� �꽑�깮�쓣 �뻽�쓣 寃쎌슦
			{
                if (movable[x_pos][y_pos] == 1)
                        pieceMove(x_pos, y_pos);

                else if (movable[x_pos][y_pos] == 2) //�븰�뙆�긽 �쟻�슜
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
                else if (movable[x_pos][y_pos] == 3)    //�듅�닔 猷�
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
                        printf("�쁽�옱 怨좊Ⅸ 留� : �뾾�쓬       ");
                        gotoxy(cursor_x, cursor_y);
                        sel_check = 0;
                    }
                    else
                    {
                        wrongMove_Alert();
                        chessDraw();
                        gotoxy(20, 2);
                        printf("�쁽�옱 怨좊Ⅸ 留� : �뾾�쓬       ");
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
	printf("                             2議�    \n");
	printf("                          寃뚯엫�떆�옉   \n");
	printf("                          寃뚯엫�꽕紐�   \n");
	printf("                          寃뚯엫醫낅즺   \n");

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
                    printf("\b\b쨌");
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
	printf("------------寃쎄퀬李�---------------------\n");
	printf("\n\n");
	printf("---------------------------------------\n");
	printf(" �뼚 : 猷�, �뾾 : �굹�씠�듃, �솧 : 鍮꾩닄\n");
	printf(" �솯 : ���, �쁾 : �궧, �뿈 : �룿\n");
	textcolor(grey);
	gotoxy(0, 16);
	printf("硫붿씤�솕硫댁쑝濡� �룎�븘媛�湲� (Backspace)\n");
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
		printf("�쁽�옱 怨좊Ⅸ 留� : �쓳�깋 猷�");
	else if (sel_piece == chessPiece[2])
		printf("�쁽�옱 怨좊Ⅸ 留� : �쓳�깋 �굹�씠�듃");
	else if (sel_piece == chessPiece[3])
		printf("�쁽�옱 怨좊Ⅸ 留� : �쓳�깋 鍮꾩닄");
	else if (sel_piece == chessPiece[4])
		printf("�쁽�옱 怨좊Ⅸ 留� : �쓳�깋 ���");
	else if (sel_piece == chessPiece[5])
		printf("�쁽�옱 怨좊Ⅸ 留� : �쓳�깋 �궧");
	else if (sel_piece == chessPiece[6])
		printf("�쁽�옱 怨좊Ⅸ 留� : �쓳�깋 �룿");
	else if (sel_piece == chessPiece[7])
		printf("�쁽�옱 怨좊Ⅸ 留� : 諛깆깋 猷�");
	else if (sel_piece == chessPiece[8])
		printf("�쁽�옱 怨좊Ⅸ 留� : 諛깆깋 �굹�씠�듃");
	else if (sel_piece == chessPiece[9])
		printf("�쁽�옱 怨좊Ⅸ 留� : 諛깆깋 鍮꾩닄");
	else if (sel_piece == chessPiece[10])
		printf("�쁽�옱 怨좊Ⅸ 留� : 諛깆깋 ���");
	else if (sel_piece == chessPiece[11])
		printf("�쁽�옱 怨좊Ⅸ 留� : 諛깆깋 �궧");
	else if (sel_piece == chessPiece[12])
		printf("�쁽�옱 怨좊Ⅸ 留� : 諛깆깋 �룿");
	else
		printf("�쁽�옱 怨좊Ⅸ 留� : �뾾�쓬");

	gotoxy(cursor_x, cursor_y);
}

void pieceDeadDraw()
{
	int i;

	gotoxy(50, 0);
	printf("     二쎌�� 留�");
	gotoxy(50, 1);
	printf("------諛깊��------");
	gotoxy(51, 2);
	for (i = 0; i < 8 - wp_live; i++)
		printf("�뿈");
	gotoxy(51, 3);
	for (i = 0; i < 2 - wr_live; i++)
		printf("�뼚");
	gotoxy(56, 3);
	for (i = 0; i < 2 - wn_live; i++)
		printf("�뾾");
	gotoxy(61, 3);
	for (i = 0; i < 2 - wb_live; i++)
		printf("�솧");
	gotoxy(51, 4);
	if (wq_live == 0)
		printf("�솯");
	gotoxy(54, 4);
	if (wk_live == 0)
		printf("�쁾");

	gotoxy(50, 7);
	printf("------�쓳���------");
	gotoxy(51, 8);
	for (i = 0; i < 8 - bp_live; i++)
		printf("�뿃");
	gotoxy(51, 9);
	for (i = 0; i < 2 - br_live; i++)
		printf("�뼞");
	gotoxy(56, 9);
	for (i = 0; i < 2 - bn_live; i++)
		printf("�뾿");
	gotoxy(61, 9);
	for (i = 0; i < 2 - bb_live; i++)
		printf("�솮");
	gotoxy(51, 10);
	if (bq_live == 0)
		printf("�솪");
	gotoxy(54, 10);
	if (bk_live == 0)
		printf("�쁿");

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
				printf("�뼚");
    else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'N')
				printf("�뾾");
    else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'B')
				printf("�솧");
    else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'Q')
				printf("�솯");
    else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'K')
				printf("�쁾");
    else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'P')
				printf("�뿈");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'R')
				printf("�뼞");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'N')
				printf("�뾿");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'B')
				printf("�솮");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'Q')
				printf("�솪");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'K')
				printf("�쁿");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'P')
				printf("�뿃");
    else if (chessPos[i][j] == chessPiece[0])
				printf("  ");
}

void orderDraw()
{
	char *order_str;
	if (order % 2 == 0)
		order_str = "諛�";
	else
		order_str = "�쓳";
	gotoxy(20, 0);
	printf("�쁽�옱 李⑤�� : %s", order_str);
	gotoxy(cursor_x, cursor_y);
}

void sooDraw()
{
	gotoxy(20, 4);
	printf("�젣 %d�닔", soo);
	gotoxy(cursor_x, cursor_y);
}

void chessRule()
{
	check_rule = 1;
	textcolor(white);
	printf("----------------寃뚯엫 諛⑸쾿---------------\n");
	printf("1. 諛깃낵 �쓳�씠 踰덇컝�븘媛�硫� �뵆�젅�씠�빀�땲�떎.\n");
	printf("2. 諛깆씠 癒쇱�� 寃뚯엫�쓣 �떆�옉�빀�땲�떎.\n");
	printf("3.  \n");
	printf("4.  \n");
	printf("\n");
	textcolor(grey);
	gotoxy(0, 16);
	printf("�룎�븘媛�湲� (Backspace)");
	textcolor(white);
}

void controlRule()
{
	check_rule = 1;
	textcolor(white);
	printf("-------------------議곗옉 諛⑸쾿------------------\n\n");
	printf("1. �궎蹂대뱶 諛⑺뼢�궎瑜� 議곗옉�븯�뿬 而ㅼ꽌瑜� �씠�룞�빀�땲�떎.\n\n");
	printf("2. �씠�룞�븯怨좎옄 �븯�뒗 留먯뿉 而ㅼ꽌瑜� �씠�룞�떆�궓 �썑\n");
	printf("   �뒪�럹�씠�뒪 諛붾�� �엯�젰�븯�뿬 留먯쓣 �꽑�깮�빀�땲�떎.\n\n");
	printf("3. �꽑�깮�맂 留먯쓣 �썝�븯�뒗 �쐞移섎줈 而ㅼ꽌瑜� �씠�룞�떆�궓\n");
	printf("   �썑 �뒪�럹�씠�뒪 諛붾�� �엯�젰 �븯�뿬 �씠�룞�떆�궢�땲�떎.\n\n");
	printf("4. 留� �꽑�깮�쓣 痍⑥냼�븯怨� �떢�쓣 寃쎌슦 �빐�떦 留먯뿉 \n");
	printf("   而ㅼ꽌瑜� �삱由ш퀬 �뒪�럹�씠�뒪 諛붾�� �븳踰� �뜑 �닃�윭�꽌\n");
	printf("   痍⑥냼�빀�땲�떎.\n\n");
	textcolor(grey);
	gotoxy(0, 16);
	printf("�룎�븘媛�湲� (Backspace)");
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
		printf("�궓����떆媛� = %2d:%2d",min,sec);
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
	turnPiece(x, y);    //諛붾��
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
	printf("�쁽�옱 怨좊Ⅸ 留� : �뾾�쓬");
}

void pieceSelect(int x_pos, int y_pos)
{
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
    {
        sel_piece = "BP";
        blackPawn(x_pos, y_pos);
        chessDraw();
    }
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
	printf("�쁽�옱 怨좊Ⅸ 留� : �뾾�쓬");
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
		printf("移섎챸�쟻 �삤瑜� 諛쒖깮");
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
	printf("�씠�룞�븷 �닔 �뾾�뒿�땲�떎.");
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
	printf("李⑤��媛� �븘�떃�땲�떎.");
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
    printf("諛깊�� �듅由�!");
    textcolor(white);
}

void blackWin_Alert()
{
    gotoxy(1, 10);
    printf("                        ");
    gotoxy(1, 10);
    textcolor(red);
    printf("�쓳��� �듅由�!");
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
        printf("諛깊�� �떆媛꾩큹怨�.");
        textcolor(white);
        Sleep(2000);
        check_winner = 2;
    }
    else
    {
        printf("�쓳��� �떆媛꾩큹怨�.");
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
        printf("%c8移� �봽濡쒕え�뀡 1 ��� 2 鍮꾩닄 3 �굹�씠�듃 4 猷�\n",a);
        textcolor(white);
        gotoxy(1, 11);
        printf("�엯�젰�븯�꽭�슂 : ");
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
            printf("�엯�젰�븯�꽭�슂 :             ");
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
        printf("%c1移� �봽濡쒕え�뀡 1 ��� 2 鍮꾩닄 3 �굹�씠�듃 4 猷�\n",a);
        textcolor(white);
        gotoxy(1, 11);
        printf("�엯�젰�븯�꽭�슂 : ");
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
            printf("�엯�젰�븯�꽭�슂 :           ");
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
    if (chessPos[x][y][2] == '1')   //�븳踰덈룄 �븞 ���吏곸���쑝硫�
    {
        if (chessPos[x-2][y] == chessPiece[0])
            movable[x-2][y] = 2;    //�븰�뙆�긽
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
>>>>>>> Stashed changes
