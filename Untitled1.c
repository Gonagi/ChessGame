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

void DrawTitle();                   //타이틀 화면 출력
void DrawChess();                   //체스판 출력
void DrawNowPiece();                //선택된 말 표시
void DrawDeadPiece();               //죽은 말 표시
void DrawCursor(int, int);          //커서 '>' 출력
void DrawPiece(int, int);           //체스판에 말 표시 함수
void DrawOrder();                   //순서 표시
void DrawSoo();                     //몇 수인지 표시
void ChessRule();                   //체스 규칙 설명내용 출력
void ControlRule();                 //조작 설명내용 출력
unsigned _stdcall timerDraw(void *);//스레드를 이용한 타이머 함수

void MovePiece(int, int);           //말 이동 함수
void SelectPiece(int, int);         //말 선택해서 전역 변수에 값 저장하는 함수
void InitializeGame();                    //게임 초기화
void CheckGame();                   //게임 내 체크 사항 확인
void TurnPiece(int, int);           //움직인 후 뒤에 0 넣는 함수
void WhiteCheck();
void BlackCheck();
void CheckCheck();

void WhiteKing(int, int);           //백색 킹
void BlackKing(int, int);           //흑색 킹
void WhiteQueen(int, int);          //백색 퀸
void BlackQueen(int, int);          //흑색 퀸
void WhitePawn(int, int);           // 백색 폰
void BlackPawn(int, int);           // 흑색 폰
void WhiteNight(int, int);          // 백색 나이트
void BlackNight(int, int);          // 흑색 나이트
void WhiteBishop(int, int);         // 백색 비숍
void BlackBishop(int, int);         // 흑색 비숍
void WhiteRook(int, int);           // 백색 룩
void BlackRook(int, int);           // 흑색 룩

void WhitePromotion(int);           //백색팀 프로모션 룰
void BlackPromotion(int);           //흑색팀 프로모션 룰

void WrongMove_Alert();             //잘못된 움직임 경고
void WrongOrder_Alert();            //순서 경고
void WhiteWin_Alert();              //백색 승 알림
void BlackWin_Alert();              //흑색 승 알림
void TImer_Alert();                 //시간 초과 경고
void WhiteSur_Alert();              //백색 기권 알림
void BlackSur_Alert();              //흑색 기권 알림
void WhiteCheck_Alert();
void WhiteCheckmate_Alert();
void BlackCheck_Alert();
void BlackCheckmate_Alert();

void ViewCursor(char);              //커서 투명 함수
void Gotoxy(int, int);              //콘솔내 좌표 이동
void Erase(int, int);               //해당 좌표 지우기
char Invert(int);                   //1~8을 a~h로 바꾸는 함수
void TextColor(int);                //콘솔내 글씨 색깔 변경
void BackColor(int);                //콘솔내 바탕 색깔 변경

int cursor_x = mainX, cursor_y = mainY; //메인화면에서의 x, y 좌표
int sel_x, sel_y;                   //스페이스바로 골랐을 당시의 x y 좌표
char sel_color = NONE;              //스페이스바로 고른 말의 색깔
char *sel_piece;                    //스페이스바로 고른 말의 종류
int sel_check = 0;                  //선택했는 지의 여부
int order = 0;                      //짝수면 백, 홀수면 흑
int soo = 1;                        //현재 몇 수
int time = SET_TIME;                //타이머 시간 설정
int check_error = 0;                //에러 발생 여부
int check_winner = 0;               //1이면 백팀 승, 2면 흑팀 승
int check_rule = 0;                 //게임 설명에서 뒤로가기 구현하기 위한 변수
int temp;                           //매 시작 초기화
int w_order;                        //앙파상 구현을 위한 순서 저장 변수
int b_order;                        // ""
int check_check = 0;

int check_WhitePromotion = 0;       //백색팀 프로모션 여부 확인
int check_BlackPromotion = 0;       //흑색팀 프로모션 여부 확인
int check_whiteAng = 0;             //백색팀 앙파상 여부 확인
int check_blackAng = 0;             //흑색팀 앙파상 여부 확인

int check_wpPr = 0;                 //프로모션을 통해 바뀐 목숨 관리 변수
int check_bpPr = 0;
int check_wqPr = 0;
int check_bqPr = 0;
int check_wbPr = 0;
int check_bbPr = 0;
int check_wnPr = 0;
int check_bnPr = 0;
int check_wrPr = 0;
int check_brPr = 0;
int bk_live = 0, wk_live = 0;       //죽은 말 표시를 위한 생사 확인 변수
int bq_live = 0, wq_live = 0;
int br_live = 0, wr_live = 0;
int bn_live = 0, wn_live = 0;
int bb_live = 0, wb_live = 0;
int bp_live = 0, wp_live = 0;
int wk_x, wk_y;
int bk_x, bk_y;
int wking_check=0;
int bking_check=0;

enum color { red = 12, yellow = 14, white = 7, grey = 8 };      //TextColor에서 쓰일 enum 선언
char *chessPos[8][8] = { {"BR1","BN1","BB1","BQ1","BK1","BB1","BN1","BR1"}, //1은 한번도 움직이지 않은 말, 0은 한번 이상 움직인 말
						{"BP1","BP1","BP1","BP1","BP1","BP1","BP1","BP1"},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"  ","  ","  ","  ","  ","  ","  ","  "},
						{"WP1","WP1","WP1","WP1","WP1","WP1","WP1","WP1"},
						{"WR1","WN1","WB1","WQ1","WK1","WB1","WN1","WR1"} };

char *chessPiece[13] = { "  ","BR","BN","BB","BQ","BK","BP","WR","WN","WB","WQ","WK","WP" };    //포인터끼리 비교 안된다고 경고 나올 때 쓰일 포인터 배열

int movable[8][8] = {{0,0,0,0,0,0,0,0},     //체스말들의 이동 및 상태 표시 배열 -1은 현재 선택 중 0은 이동 불가 1은 이동 가능 2는 앙파상 3은 특수 이동
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0} };

int check[8][8] = {{0,0,0,0,0,0,0,0},       //체스말 체크 판별 0은 기본 타일 1은 말 이동 거리
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
			printf("어떤 설명을 들으시겠습니까?");
			Gotoxy(20, 6);
			printf("게임 조작 규칙");
			Gotoxy(20, 7);
			printf("체스 규칙");
			TextColor(grey);
			Gotoxy(0, 16);
			printf("메인화면으로 가기 (Backspace)");
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
            printf("게임을 종료합니다...\n");
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

    while (1)   //게임 메인 while문
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
        printf("%d %d %d,%d",wking_check, check[cursor_y][cursor_x/2], wk_x, wk_y);  //변수값 확인용 (지울 것)
        Gotoxy(cursor_x, cursor_y);
        int x_pos = cursor_y;
        int y_pos = cursor_x / 2;   //커서 위치를 배열 행 열로 변환
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

            if (sel_check == 0) //1. 말 선택을 안했을 경우
            {
                if (chessPos[x_pos][y_pos] != chessPiece[0])    //1-1. 선택한 칸에 말이 있을 때
                {
                    if (chessPos[x_pos][y_pos][0] == BLACK)
                        sel_color = BLACK;

                    else
                        sel_color = WHITE;

                    if ((order % 2 == 0 && sel_color == BLACK) || (order % 2 != 0 && sel_color == WHITE))   //차례가 백인데 검은 말을 고른 경우, 차례가 검인데 흰 말을 고른 경우
                    {
                        WrongOrder_Alert();
                        Gotoxy(20, 2);
                        printf("현재 고른 말 : 없음       ");
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

                else if (chessPos[x_pos][y_pos] == chessPiece[0])    //1-2. 선택한 칸이 빈칸일 때
                {
                    Gotoxy(1, 10);
                    printf("                        ");
                    Gotoxy(cursor_x, cursor_y);
                    sel_check = 0;
                    SelectPiece(x_pos, y_pos);
                    DrawNowPiece();
                }
            }

            else if (sel_check == 1)    //2.말 선택을 했을 경우
            {
                if (movable[x_pos][y_pos] == 1)     //2-1. 이동하고자 하는 위치가 movable = 1이면
                    MovePiece(x_pos, y_pos);

                else if (movable[x_pos][y_pos] == 2) //앙파상 적용
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

                else if (movable[x_pos][y_pos] == 3)    //특수 룰
                {
                    if (sel_piece == "WP")              //앙파상
                    {
                        chessPos[x_pos+1][y_pos] = chessPiece[0];
                        MovePiece(x_pos, y_pos);
                    }

                    else if (sel_piece == "BP")
                    {
                        chessPos[x_pos-1][y_pos] = chessPiece[0];
                        MovePiece(x_pos, y_pos);
                    }

                    else if (sel_piece == "WK")         //캐슬링
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
                    printf("현재 고른 말 : 없음       ");
                    Gotoxy(cursor_x, cursor_y);
                    sel_check = 0;
                }

                else if (movable[x_pos][y_pos] == 0)        //이동하고자 하는 위치가 movable = 0이면
                {
                    WrongMove_Alert();
                    DrawChess();
                    Gotoxy(20, 2);
                    printf("현재 고른 말 : 없음       ");
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
	printf("                             2조    \n");
	printf("                          게임시작   \n");
	printf("                          게임설명   \n");
	printf("                          게임종료   \n");
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
                    printf("\b\b·");

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
	printf("------------경고창---------------------\n");
	printf("\n\n");
	printf("---------------------------------------\n");
	printf(" ■ : 룩, ◆ : 나이트, ♠ : 비숍\n");
	printf(" ♥ : 퀸, ★ : 킹, ● : 폰\n");
	TextColor(grey);
	Gotoxy(0, 16);
	printf("메인화면으로 돌아가기 (Backspace)\n");
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

	Gotoxy(cursor_x, cursor_y);
}


void DrawDeadPiece()
{
	int i;
	Gotoxy(50, 0);
	printf("     죽은 말");
	Gotoxy(50, 1);
	printf("------백팀------");
	Gotoxy(51, 2);

	for (i = 0; i < 8 - wp_live; i++)
		printf("●");

	Gotoxy(51, 3);

	for (i = 0; i < 2 - wr_live; i++)
		printf("■");

	Gotoxy(56, 3);

	for (i = 0; i < 2 - wn_live; i++)
		printf("◆");

	Gotoxy(61, 3);

	for (i = 0; i < 2 - wb_live; i++)
		printf("♠");

	Gotoxy(51, 4);

	if (wq_live == 0)
		printf("♥");

	Gotoxy(54, 4);

	if (wk_live == 0)
		printf("★");

	Gotoxy(50, 7);
	printf("------흑팀------");
	Gotoxy(51, 8);

	for (i = 0; i < 8 - bp_live; i++)
		printf("○");

	Gotoxy(51, 9);

	for (i = 0; i < 2 - br_live; i++)
		printf("□");

	Gotoxy(56, 9);

	for (i = 0; i < 2 - bn_live; i++)
		printf("◇");

	Gotoxy(61, 9);

	for (i = 0; i < 2 - bb_live; i++)
		printf("♤");

	Gotoxy(51, 10);

	if (bq_live == 0)
		printf("♡");

	Gotoxy(54, 10);

	if (bk_live == 0)
		printf("☆");

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
				printf("■");

    else if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'N')
				printf("◆");

    else if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'B')
				printf("♠");

    else if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'Q')
				printf("♥");

    else if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'K')
				printf("★");

    else if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'P')
				printf("●");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'R')
				printf("□");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'N')
				printf("◇");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'B')
				printf("♤");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'Q')
				printf("♡");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'K')
				printf("☆");

    else if (chessPos[i][j][0] == WHITE && chessPos[i][j][1] == 'P')
				printf("○");

    else if (chessPos[i][j] == chessPiece[0])
				printf("  ");
}


void DrawOrder()
{
	char *order_str;

	if (order % 2 == 0)
		order_str = "백";

	else
		order_str = "흑";

	Gotoxy(20, 0);
	printf("현재 차례 : %s", order_str);
	Gotoxy(cursor_x, cursor_y);
}


void DrawSoo()
{
	Gotoxy(20, 4);
	printf("제 %d수", soo);
	Gotoxy(cursor_x, cursor_y);
}


void ChessRule()
{
	check_rule = 1;
	TextColor(white);
	printf("----------------게임 방법---------------\n");
	printf("1. 백과 흑이 번갈아가며 플레이합니다.\n");
	printf("2. 백이 먼저 게임을 시작합니다.\n");
	printf("3.  \n");
	printf("4.  \n");
	printf("\n");
	TextColor(grey);
	Gotoxy(0, 16);
	printf("돌아가기 (Backspace)");
	TextColor(white);
}


void ControlRule()
{
	check_rule = 1;
	TextColor(white);
	printf("-------------------조작 방법------------------\n\n");
	printf("1. 키보드 방향키를 조작하여 커서를 이동합니다.\n\n");
	printf("2. 이동하고자 하는 말에 커서를 이동시킨 후\n");
	printf("   스페이스 바를 입력하여 말을 선택합니다.\n\n");
	printf("3. 선택된 말을 원하는 위치로 커서를 이동시킨\n");
	printf("   후 스페이스 바를 입력 하여 이동시킵니다.\n\n");
	printf("4. 말 선택을 취소하고 싶을 경우 해당 말에 \n");
	printf("   커서를 올리고 스페이스 바를 한번 더 눌러서\n");
	printf("   취소합니다.\n\n");
	TextColor(grey);
	Gotoxy(0, 16);
	printf("돌아가기 (Backspace)");
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
		printf("남은시간 = %2d:%2d",min,sec);
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
	TurnPiece(x, y);    //바뀜
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
	printf("현재 고른 말 : 없음");
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
    //_beginthreadex(NULL,0,timerDraw,0,0,NULL);      //타이머 표시
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
        printf("치명적 오류 발생");
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
	printf("이동할 수 없습니다.");
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
	printf("차례가 아닙니다.");
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
    printf("백팀 승리!");
    TextColor(white);
}


void WhiteCheck_Alert()
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("백팀 체크");
    TextColor(white);
}


void WhiteCheckmate_Alert()
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("백팀 체크메이트!");
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
    printf("흑팀 승리!");
    TextColor(white);
}


void BlackCheck_Alert()
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("흑팀 체크");
    TextColor(white);
}


void BlackCheckmate_Alert()
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("흑팀 체크메이트!");
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
        printf("백팀 시간초과.");
        TextColor(white);
        Sleep(2000);
        check_winner = 2;
    }

    else
    {
        printf("흑팀 시간초과.");
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
        printf("백팀이 기권했습니다.");
        TextColor(white);
        Gotoxy(1, 11);
        printf("승낙하려면 y, 아니면 n을 입력 : ");
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
            printf("승낙하려면 y, 아니면 n을 입력 :      ");
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
        printf("흑팀이 기권했습니다.");
        TextColor(white);
        Gotoxy(1, 11);
        printf("승낙하려면 y, 아니면 n을 입력 : ");
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
            printf("승낙하려면 y, 아니면 n을 입력 :      ");
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
        printf("%c8칸 프로모션 1 퀸 2 비숍 3 나이트 4 룩\n",a);
        TextColor(white);
        Gotoxy(1, 11);
        printf("입력하세요 : ");
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
            printf("입력하세요 :             ");
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
        printf("%c1칸 프로모션 1 퀸 2 비숍 3 나이트 4 룩\n",a);
        TextColor(white);
        Gotoxy(1, 11);
        printf("입력하세요 : ");
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
            printf("입력하세요 :           ");
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

    if (chessPos[x][y] == "WK1")        //캐슬링
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


void WhiteNight(int x, int y)   // 백색 나이트
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
