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
    printf("���� ���� �� : %s",cursorPos);
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
    int time = 300;
    int min;
    int sec;
    while(1)
    {
        min = time / 60;
        sec = time % 60;
        gotoxy(20, 0);
        printf("�����ð� = %2d:%2d\n", min, sec);
        Sleep(1000);
        time--;
        gotoxy(cursor_x,cursor_y);
        if(time ==0)
        {
            system("cls");
            gotoxy(7,4);
            printf("�ð��ʰ�\n");
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

void titleDraw();   //타이틀 화면 출력
void chessDraw();   //체스판 출력
void pieceNowDraw();    //선택된 말 표시
void pieceDeadDraw();   //죽은 말 표시
void cursorDraw(int, int);  //커서 '>' 출력
void pieceDraw(int, int);
void orderDraw();   //순서 표시
void sooDraw();     //몇 수인지 표시
void chessRule();   //체스 규칙 설명내용 출력
void controlRule(); //조작 설명내용 출력
unsigned _stdcall timerDraw(void *);  //스레드를 이용한 타이머 함수

void pieceMove(int, int);   //말 이동 함수
void pieceSelect(int, int);
void gameInit();    //게임 초기화
void gameCheck();   //게임 내 체크 사항 확인
void turnPiece(int, int);

void whitePawn(int, int);
void blackPawn(int, int);

void whitePromotion(int);
void blackPromotion(int);

void wrongMove_Alert(); //잘못된 움직임 경고
void wrongOrder_Alert();    //순서 경고
void whiteWin_Alert();
void blackWin_Alert();
void timer_Alert();

void CursorView(char); //커서 투명 함수
void gotoxy(int, int);  //콘솔내 좌표 이동
void erase(int, int);   //해당 좌표 지우기
char invert(int);
void textcolor(int);    //콘솔내 글씨 색깔 변경
void backcolor(int);    //콘솔내 바탕 색깔 변경

int cursor_x = mainX, cursor_y = mainY;
int sel_x, sel_y;   //스페이스바로 골랐을 당시의 x y 좌표
char sel_color = NONE;
char *sel_piece;
int sel_check = 0;
int order = 0;    //짝수면 백, 홀수면 흑
int soo = 1;
int time = SET_TIME;
int check_error = 0;
int check_winner = 0;  //1이면 백팀 승, 2면 흑팀 승
int check_rule = 0;
int temp;   //매 시작 초기화
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
char *chessPos[8][8] = { {"BR1","BN1","BB1","BQ1","BK1","BB1","BN1","BR1"}, //1은 한번도 움직이지 않은 말, 0은 한번 이상 움직인 말
						{"BP1","BP1","BP1","BP1","BP1","BP1","BP1","BP1"},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"WP1","WP1","WP1","WP1","WP1","WP1","WP1","WP1"},
						{"WR1","WN1","WB1","WQ1","WK1","WB1","WN1","WR1"} };

char *chessPiece[13] = { "  ","BR","BN","BB","BQ","BK","BP","WR","WN","WB","WQ","WK","WP" };

int movable[8][8] = {{0,0,0,0,0,0,0,0},     //0은 이동 불가 1은 이동 가능
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
			printf("어떤 설명을 들으시겠습니까?");
			gotoxy(20, 6);
			printf("게임 조작 규칙");
			gotoxy(20, 7);
			printf("체스 규칙");
			textcolor(grey);
			gotoxy(0, 16);
			printf("메인화면으로 가기 (Backspace)");
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
			printf("게임을 종료합니다...\n");
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
		printf("Test M%d %d %d C[2]%c ",movable[cursor_y][cursor_x/2],check_wpPr,check_wqPr,chessPos[cursor_y][cursor_x/2][2]);  //변수값 확인용 (지울 것)
		gotoxy(cursor_x, cursor_y);

        int x_pos = cursor_y;
		int y_pos = cursor_x / 2;   //커서 위치를 배열 행 열로 변환
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
			if (sel_check == 0) //1. 말 선택을 안했을 경우
			{
			    if (chessPos[x_pos][y_pos] != chessPiece[0])    //1-1. 선택한 칸에 말이 있을 때
				{
				    if (chessPos[x_pos][y_pos][0] == 'B')
					sel_color = BLACK;
                    else
					sel_color = WHITE;

                    if ((order % 2 == 0 && sel_color == BLACK) || (order % 2 != 0 && sel_color == WHITE))   //차례가 백인데 검은 말을 고른 경우, 차례가 검인데 흰 말을 고른 경우
                    {
                        wrongOrder_Alert();
                        gotoxy(20, 2);
                        printf("현재 고른 말 : 없음       ");
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
				else if (chessPos[x_pos][y_pos] == chessPiece[0])    //1-2. 선택한 칸이 빈칸일 때
                {
                    gotoxy(1, 10);
                    printf("                        ");
                    gotoxy(cursor_x, cursor_y);
                    sel_check = 0;
                    pieceSelect(x_pos, y_pos);
                    pieceNowDraw();
                }
			}

			else if (sel_check == 1)    //말 선택을 했을 경우
			{
                if (movable[x_pos][y_pos] == 1)
                        pieceMove(x_pos, y_pos);

                else if (movable[x_pos][y_pos] == 2) //앙파상 적용
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
                else if (movable[x_pos][y_pos] == 3)    //특수 룰
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
                        printf("현재 고른 말 : 없음       ");
                        gotoxy(cursor_x, cursor_y);
                        sel_check = 0;
                    }
                    else
                    {
                        wrongMove_Alert();
                        chessDraw();
                        gotoxy(20, 2);
                        printf("현재 고른 말 : 없음       ");
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
	printf("                             2조    \n");
	printf("                          게임시작   \n");
	printf("                          게임설명   \n");
	printf("                          게임종료   \n");

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
                    printf("\b\b·");
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
	printf("------------경고창---------------------\n");
	printf("\n\n");
	printf("---------------------------------------\n");
	printf(" ■ : 룩, ◆ : 나이트, ♠ : 비숍\n");
	printf(" ♥ : 퀸, ★ : 킹, ● : 폰\n");
	textcolor(grey);
	gotoxy(0, 16);
	printf("메인화면으로 돌아가기 (Backspace)\n");
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
		printf("현재 고른 말 : 흑색 룩");
	else if (sel_piece == chessPiece[2])
		printf("현재 고른 말 : 흑색 나이트");
	else if (sel_piece == chessPiece[3])
		printf("현재 고른 말 : 흑색 비숍");
	else if (sel_piece == chessPiece[4])
		printf("현재 고른 말 : 흑색 퀸");
	else if (sel_piece == chessPiece[5])
		printf("현재 고른 말 : 흑색 킹");
	else if (sel_piece == chessPiece[6])
		printf("현재 고른 말 : 흑색 폰");
	else if (sel_piece == chessPiece[7])
		printf("현재 고른 말 : 백색 룩");
	else if (sel_piece == chessPiece[8])
		printf("현재 고른 말 : 백색 나이트");
	else if (sel_piece == chessPiece[9])
		printf("현재 고른 말 : 백색 비숍");
	else if (sel_piece == chessPiece[10])
		printf("현재 고른 말 : 백색 퀸");
	else if (sel_piece == chessPiece[11])
		printf("현재 고른 말 : 백색 킹");
	else if (sel_piece == chessPiece[12])
		printf("현재 고른 말 : 백색 폰");
	else
		printf("현재 고른 말 : 없음");

	gotoxy(cursor_x, cursor_y);
}

void pieceDeadDraw()
{
	int i;

	gotoxy(50, 0);
	printf("     죽은 말");
	gotoxy(50, 1);
	printf("------백팀------");
	gotoxy(51, 2);
	for (i = 0; i < 8 - wp_live; i++)
		printf("●");
	gotoxy(51, 3);
	for (i = 0; i < 2 - wr_live; i++)
		printf("■");
	gotoxy(56, 3);
	for (i = 0; i < 2 - wn_live; i++)
		printf("◆");
	gotoxy(61, 3);
	for (i = 0; i < 2 - wb_live; i++)
		printf("♠");
	gotoxy(51, 4);
	if (wq_live == 0)
		printf("♥");
	gotoxy(54, 4);
	if (wk_live == 0)
		printf("★");

	gotoxy(50, 7);
	printf("------흑팀------");
	gotoxy(51, 8);
	for (i = 0; i < 8 - bp_live; i++)
		printf("○");
	gotoxy(51, 9);
	for (i = 0; i < 2 - br_live; i++)
		printf("□");
	gotoxy(56, 9);
	for (i = 0; i < 2 - bn_live; i++)
		printf("◇");
	gotoxy(61, 9);
	for (i = 0; i < 2 - bb_live; i++)
		printf("♤");
	gotoxy(51, 10);
	if (bq_live == 0)
		printf("♡");
	gotoxy(54, 10);
	if (bk_live == 0)
		printf("☆");

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
				printf("■");
    else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'N')
				printf("◆");
    else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'B')
				printf("♠");
    else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'Q')
				printf("♥");
    else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'K')
				printf("★");
    else if (chessPos[i][j][0] == 'B' && chessPos[i][j][1] == 'P')
				printf("●");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'R')
				printf("□");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'N')
				printf("◇");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'B')
				printf("♤");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'Q')
				printf("♡");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'K')
				printf("☆");
    else if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'P')
				printf("○");
    else if (chessPos[i][j] == chessPiece[0])
				printf("  ");
}

void orderDraw()
{
	char *order_str;
	if (order % 2 == 0)
		order_str = "백";
	else
		order_str = "흑";
	gotoxy(20, 0);
	printf("현재 차례 : %s", order_str);
	gotoxy(cursor_x, cursor_y);
}

void sooDraw()
{
	gotoxy(20, 4);
	printf("제 %d수", soo);
	gotoxy(cursor_x, cursor_y);
}

void chessRule()
{
	check_rule = 1;
	textcolor(white);
	printf("----------------게임 방법---------------\n");
	printf("1. 백과 흑이 번갈아가며 플레이합니다.\n");
	printf("2. 백이 먼저 게임을 시작합니다.\n");
	printf("3.  \n");
	printf("4.  \n");
	printf("\n");
	textcolor(grey);
	gotoxy(0, 16);
	printf("돌아가기 (Backspace)");
	textcolor(white);
}

void controlRule()
{
	check_rule = 1;
	textcolor(white);
	printf("-------------------조작 방법------------------\n\n");
	printf("1. 키보드 방향키를 조작하여 커서를 이동합니다.\n\n");
	printf("2. 이동하고자 하는 말에 커서를 이동시킨 후\n");
	printf("   스페이스 바를 입력하여 말을 선택합니다.\n\n");
	printf("3. 선택된 말을 원하는 위치로 커서를 이동시킨\n");
	printf("   후 스페이스 바를 입력 하여 이동시킵니다.\n\n");
	printf("4. 말 선택을 취소하고 싶을 경우 해당 말에 \n");
	printf("   커서를 올리고 스페이스 바를 한번 더 눌러서\n");
	printf("   취소합니다.\n\n");
	textcolor(grey);
	gotoxy(0, 16);
	printf("돌아가기 (Backspace)");
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
		printf("남은시간 = %2d:%2d",min,sec);
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
	turnPiece(x, y);    //바뀜
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
	printf("현재 고른 말 : 없음");
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
	printf("현재 고른 말 : 없음");
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
		printf("치명적 오류 발생");
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
	printf("이동할 수 없습니다.");
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
	printf("차례가 아닙니다.");
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
    printf("백팀 승리!");
    textcolor(white);
}

void blackWin_Alert()
{
    gotoxy(1, 10);
    printf("                        ");
    gotoxy(1, 10);
    textcolor(red);
    printf("흑팀 승리!");
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
        printf("백팀 시간초과.");
        textcolor(white);
        Sleep(2000);
        check_winner = 2;
    }
    else
    {
        printf("흑팀 시간초과.");
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
        printf("%c8칸 프로모션 1 퀸 2 비숍 3 나이트 4 룩\n",a);
        textcolor(white);
        gotoxy(1, 11);
        printf("입력하세요 : ");
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
            printf("입력하세요 :             ");
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
        printf("%c1칸 프로모션 1 퀸 2 비숍 3 나이트 4 룩\n",a);
        textcolor(white);
        gotoxy(1, 11);
        printf("입력하세요 : ");
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
            printf("입력하세요 :           ");
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
    if (chessPos[x][y][2] == '1')   //한번도 안 움직였으면
    {
        if (chessPos[x-2][y] == chessPiece[0])
            movable[x-2][y] = 2;    //앙파상
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
