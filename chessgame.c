#include <stdio.h>
#include <Windows.h>		//gotoxy함수 구현, 창 크기 설정, 텍스트 색상, 배경 색상 구현을 위한 헤더파일
#include <conio.h>		    //getch()를 사용하기 위한 헤더파일
#include <string.h>		    //문자열 관련 함수를 사용하기 위한 헤더파일
#include <process.h>		//타이머 구현을 위한 스레드 헤더파일

#define UP 72       //화살표 위 매크로
#define DOWN 80     //화살표 아래 매크로
#define LEFT 75     //화살표 왼쪽 매크로
#define RIGHT 77    //화살표 오른쪽 매크로
#define ENTER 13    //키보드 엔터 매크로
#define BACK 8      //키보드 백스페이스 매크로
#define SPACE 32    //키보드 스페이스 매크로
#define mainX 24    //메인화면에서의 x축 기본값
#define mainY 10    //메인화면에서의 y축 기본값
#define WHITE 'W'   //'W'를 WHITE로 나타내기 위한 매크로
#define BLACK 'B'   //'B'를 BLACK으로 나타내기 위한 매크로
#define NONE 'N'    //'N'을 NONE으로 나타내기 위한 매크로
#define SET_TIME (300 + 1)  //타이머 기본 설정 값 매크로

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
unsigned _stdcall Drawtimer(void *);//스레드를 이용한 타이머 함수

void MovePiece(int, int);           //말 이동 함수
void SelectPiece(int, int);         //말 선택해서 전역 변수에 값 저장하는 함수
void InitializeGame();              //게임 초기화
void CheckGame();                   //게임 내 체크 사항 확인
void TurnPiece(int, int);           //움직인 후 뒤에 0 넣는 함수
void WhiteCheck();                  //백색팀의 체크상황 판별 함수
void BlackCheck();                  //흑색팀의 체크상황 판별 함수
void CheckCheck();                  //체크 상황 판별 함수

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
void Draw_Alert();                  //무승부 알림
void Timer_Alert();                 //시간 초과 경고
void WhiteSur_Alert();              //백색 기권 알림
void BlackSur_Alert();              //흑색 기권 알림
void WhiteCheck_Alert();            //백팀 체크상황 경고
void WhiteCheckmate_Alert();        //백팀 체크메이트 알림
void BlackCheck_Alert();            //흑팀 체크상황 경고
void BlackCheckmate_Alert();        //흑팀 체크메이트 알림
void DrawSug_Alert();               //무승부 신청 알림
void Stalemate_Alert();             //스테일메이트 알림

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
int check_winner = 0;               //게임 승패로 인한 종료 체크 변수 (1이면 백팀 승, 2면 흑팀 승 3이면 무승부)
int check_rule = 0;                 //게임 설명에서 뒤로가기 구현하기 위한 변수
int temp;                           //매 시작 초기화
int w_order;                        //앙파상 구현을 위한 순서 저장 변수
int b_order;                        //앙파상 구현을 위한 순서 저장 변수
int check_check = 0;                //체스말 이동 함수에서 체크 변수를 수정하기 위한 체크 변수

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
int wk_x, wk_y;                 //백색 킹 x y 좌표
int bk_x, bk_y;                 //흑색 킹 x y 좌표
int wking_check = 0;              //백색 킹 체크메이트 구현을 위한 변수 (8개 채워지면 체크메이트)
int bking_check = 0;              //흑색 킹 체크메이트 구현을 위한 변수
int wmv_cnt = 0;                  //스테일메이트 구현을 위한 백색 말 움직인 횟수 변수
int bmv_cnt = 0;                  //스테일메이트 구현을 위한 흑색 말 움직인 횟수 변수

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

int main()      //main 함수
{
	system("mode con cols=67 lines=18 | title ChessGame_Team2");    //콘솔 창 크기 조정 및 이름
Main:       //메인화면
    time=-2;        //타이머 끔
	ViewCursor(0);  //깜빡이는 커서 끔
	TextColor(white);   //텍스트 색상 기본값(흰색)
	system("cls");      //창 초기화
	DrawTitle();        //타이틀 화면 불러오는 함수
	DrawCursor(cursor_x, cursor_y);     //메인화면 커서를 불러오는 함수

	while (1)       //메인화면 while문
	{
		int key;        //키보드 입력값 받아올 key 변수
		key = _getch();

		if (cursor_y == mainY && key == ENTER)      //만약 게임시작에 커서가 있고 엔터키를 누르면
		{
			system("cls");      //창 초기화
			break;              //메인화면 while문 빠져나옴
		}

		else if (cursor_y == mainY + 1 && key == ENTER)     //만약 게임 설명에서 엔터키를 입력하면
		{
			cursor_x = 18, cursor_y = 6;        //기존의 커서 좌표 저장
		Rule:           //규칙 설명 화면
			system("cls");      //창 초기화
			check_rule = 0;     //백스페이스를 눌렀을 때 메인화면으로 돌아가기 위한 저장 변수
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

			while (1)       //규칙 설명 화면의 while문
            {
                key = _getch();

                if (cursor_y == 6 && key == ENTER)      //조작 설명에서 엔터키를 눌렀으면
                {
                    system("cls");
                    ControlRule();      //조작 설명 불러오는 함수
                }

                else if (cursor_y == 7 && key == ENTER)     //게임 규칙 설명에서 엔터키를 눌렀으면
                {
                    system("cls");
                    ChessRule();        //게임 규칙 설명 불러오는 함수
                }

                switch (key)
                {
                case UP:
                    if (cursor_y == 6)      //커서가 최대 위치를 벗어나면
                        continue;

                    else if (check_rule == 1)
                        continue;

                    cursor_y--;     //커서가 위로 올라감
                    Erase(cursor_x, cursor_y + 1);
                    DrawCursor(cursor_x, cursor_y);
                    break;

                case DOWN:

                    if (cursor_y == 7)      //커서가 최대 위치를 벗어나면
                        continue;

                    else if (check_rule == 1)
                        continue;

                    else
                        cursor_y++;

                    Erase(cursor_x, cursor_y - 1);
                    DrawCursor(cursor_x, cursor_y);     //커서가 아래로 내려감
                    break;

                case BACK:

                    if (check_rule == 1)        //조작 설명 또는 규칙 설명창에서 다시 게임 설명창으로 돌아가기
                        goto Rule;

                    cursor_x = mainX, cursor_y = mainY + 1;
                    goto Main;
                    break;

                default:
                    continue;
                }
            }
        }

        else if (cursor_y == mainY + 2 && key == ENTER)     //만약 게임 종료에서 엔터를 눌르면
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

            if (cursor_y == mainY)      //커서가 최대 위치를 벗어나면
                continue;

            cursor_y--;     //커서 위로 올림
            Erase(cursor_x, cursor_y + 1);
            DrawCursor(cursor_x, cursor_y);
            break;

        case DOWN:

            if (cursor_y == mainY + 2)      //커서가 최대 위치를 벗어나면
                continue;

            else
                cursor_y++;

            Erase(cursor_x, cursor_y - 1);
            DrawCursor(cursor_x, cursor_y);     //커서 아래로 내림
            break;
        }
    }
    InitializeGame();       //새 게임 시작할 때 초기화 함수
    CheckGame();            //게임을 진행하면서 체크해야 할 상황 체크하는 함수

    while (1)   //게임 메인 while문
    {
        ViewCursor(0);      //커서 안보이게 함
        CheckGame();        //게임을 진행하면서 체크해야 할 상황 체크하는 함수
        CheckCheck();       //게임을 진행하면서 체크 상황인지 아닌지 체크하는 함수

        if(check_winner == 1)       //만약 백팀이 이겼다면
        {
            WhiteWin_Alert();       //백팀 승리 알림
            Sleep(1000);            //1초 쉬기
            break;                  //게임 while문 벗어남
        }

        else if(check_winner == 2)  //만약 흑팀이 이겼다면
        {
            BlackWin_Alert();       //흑팀 승리 알림
            Sleep(1000);
            break;
        }

        else if (check_winner == 3)     //만약 무승부 상황이라면
        {
            Draw_Alert();               //무승부 알림
            Sleep(1000);
            break;
        }

        else if (check_WhitePromotion == 1)     //만약 백색 폰이 프로모션 상황이면
            WhitePromotion(temp);               //프로모션 구현 함수 실행

        else if (check_BlackPromotion == 1)     //만약 흑색 폰이 프로모션 상황이면
            BlackPromotion(temp);               //프로모션 구현 함수 실행

        DrawDeadPiece();        //죽은 말 표시하는 함수
        DrawOrder();            //순서 표시하는 함수
        DrawSoo();              //몇 수 인지 표시하는 함수
        ViewCursor(1);          //다시 커서 보이게 하는 함수

        int x_pos = cursor_y;   //커서 위치를 배열 행 열로 변환
        int y_pos = cursor_x / 2;
        int key;        //키보드 입력 값 받는 key 변수
        key = _getch();

        if (check_error == 1)       //만약 게임에 에러가 발생했다면 (양 쪽 킹이 없는 경우)
            break;

        switch (key)
        {
        case 's':       //기권 버튼 s

            if (order%2 == 0)       //만약 현재 차례가 백이면
                WhiteSur_Alert();   //백색 기권 알림

            else
                BlackSur_Alert();   //아니면 흑색 기권 알림

        case 'S':       //기권 버튼 S (대 소문자 고려)

            if (order%2 == 0)
                WhiteSur_Alert();

            else
                BlackSur_Alert();

        case 'd':       //무승부 신청 버튼 d
            if (soo >= 20)          //만약 20 수 이상으로 진행됐다면
                DrawSug_Alert();    //무승부 신청 알림
            else
                continue;
        case 'D':       //무승부 신청 버튼 D (대 소문자 고려)
            if (soo >= 20)
                DrawSug_Alert();
            else
                continue;

        case UP:        //조작키 UP

            if (cursor_y == 0)      //만약 최대 높이를 벗어나서 위로 올라가려 하면
                continue;

            cursor_y--;     //커서 위로 올림
            Gotoxy(cursor_x, cursor_y);
            break;

        case DOWN:      //조작키 DOWN

            if (cursor_y == 7)      //만약 최대 높이를 벗어나서 아래로 내려가려 하면
                continue;

            cursor_y++;     //커서 아래로 내림
            Gotoxy(cursor_x, cursor_y);
            break;

        case LEFT:      //조작키 LEFT

            if (cursor_x == 0)      //만약 최대 위치를 벗어나서 왼쪽으로 이동하려 하면
                continue;

            cursor_x -= 2;      //커서 왼쪽으로 이동 (2칸이 기본값)
            Gotoxy(cursor_x, cursor_y);
            break;

        case RIGHT:     //조작키 RIGHT

            if (cursor_x == 14)     //만약 최대 위치를 벗어나서 오른쪽으로 이동하려 하면
                continue;

            cursor_x += 2;      //커서 오른쪽으로 이동
            Gotoxy(cursor_x, cursor_y);
            break;

        case BACK:      //게임 도중 백스페이스로 게임 종료하는 키 (메인화면으로 이동)
            cursor_x = mainX, cursor_y = mainY;     //커서 위치를 메인화면에 맞게 수정
            InitializeGame();       //새 게임으로 변수들 초기화
            cursor_x = mainX, cursor_y = mainY;
            goto Main;      //메인 화면으로 이동
            break;

        case SPACE:

            if (sel_check == 0) //1. 말 선택을 안했을 경우
            {
                if (chessPos[x_pos][y_pos] != chessPiece[0])    //1-1. 선택한 칸에 말이 있을 때
                {
                    if (chessPos[x_pos][y_pos][0] == BLACK)     //만약 선택한 말이 흑색이면
                        sel_color = BLACK;      //선택한 색상 변수에 흑색 저장

                    else
                        sel_color = WHITE;      //선택한 색상 변수에 백색 저장

                    if ((order % 2 == 0 && sel_color == BLACK) || (order % 2 != 0 && sel_color == WHITE))   //차례가 백인데 검은 말을 고른 경우, 차례가 검인데 흰 말을 고른 경우
                    {
                        WrongOrder_Alert();     //잘못된 차례 경고
                        Gotoxy(20, 2);
                        printf("현재 고른 말 : 없음       ");      //말 선택 취소
                        DrawChess();        //체스판 불러옴
                    }

                    else        //정상적인 선택 상황일 경우
                    {
                        SelectPiece(x_pos, y_pos);      //커서 위치의 자표를 매개변수로 넘기고 말 선택 함수 불러옴
                        DrawNowPiece();     //현재 고른 말 표시하는 함수
                        Gotoxy(1, 10);
                        printf("                        ");
                        sel_check = 1;      //선택했다는 상황 체크 변수 1 (선택함)
                        sel_x = x_pos, sel_y = y_pos;       //선택했을 당시의 좌표 넘겨줌
                    }
                }

                else if (chessPos[x_pos][y_pos] == chessPiece[0])    //1-2. 선택한 칸이 빈칸일 때
                {
                    Gotoxy(1, 10);
                    printf("                        ");
                    Gotoxy(cursor_x, cursor_y);
                    sel_check = 0;      //선택했다는 상황 체크 변수 0 (선택안함)
                    SelectPiece(x_pos, y_pos);      //선택한 말 없음
                    DrawNowPiece();     //현재 선택한 말 표시하는 함수
                }
            }

            else if (sel_check == 1)    //2.말 선택을 했을 경우
            {
                if (movable[x_pos][y_pos] == 1)     //2-1. 이동하고자 하는 위치가 movable = 1이면
                    MovePiece(x_pos, y_pos);        //말 움직이게 하는 함수

                else if (movable[x_pos][y_pos] == 2) //앙파상 적용
                {
                    if (order%2 == 0)       //만약 현재 차례가 백이면
                    {
                        check_whiteAng = 1;     //앙파상 체크 변수 1 (켜짐)
                        w_order = order;        //앙파상 구현을 위한 차례 저장 변수
                        MovePiece(x_pos, y_pos);
                    }

                    else
                    {
                        check_blackAng = 1;     //앙파상 체크 변수 1
                        b_order = order;        //앙파상 구현을 위한 차례 저장 변수
                        MovePiece(x_pos, y_pos);
                    }
                }

                else if (movable[x_pos][y_pos] == 3 || movable[x_pos][y_pos] == 4)    //특수 룰
                {
                    if (sel_piece == "WP")              //앙파상
                    {
                        chessPos[x_pos+1][y_pos] = chessPiece[0];       //앙파상 특수 룰로 말 이동 및 잡기
                        MovePiece(x_pos, y_pos);
                    }

                    else if (sel_piece == "BP")
                    {
                        chessPos[x_pos-1][y_pos] = chessPiece[0];
                        MovePiece(x_pos, y_pos);
                    }

                    else if (sel_piece == "WK")         //캐슬링
                    {
                        if (y_pos == 6)     //만약 캐슬링이 가능한 위치라면
                        {
                            chessPos[x_pos][y_pos-1] = "WR1";       //캐슬링 구현
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
                else if (movable[x_pos][y_pos] == -1)       //만약 movable변수가 -1이면 (현재 말 선택중)
                {
                    Gotoxy(1, 10);
                    printf("                        ");

                    for(int i=0; i<=7; i++)
                        for(int j=0; j<=7; j++)
                            movable[i][j]=0;

                    DrawChess();
                    Gotoxy(20, 2);
                    printf("현재 고른 말 : 없음       ");      //선택을 취소함
                    Gotoxy(cursor_x, cursor_y);
                    sel_check = 0;      //선택 체크 함수 0
                }

                else if (movable[x_pos][y_pos] == 0)        //이동하고자 하는 위치가 movable = 0이면
                {
                    WrongMove_Alert();      //잘못된 움직임 경고
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


void DrawTitle()        //타이틀 불러오는 함수
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


void DrawChess()        //기물 표시하는 함수
{
	int i, j;
	Gotoxy(0,0);

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (j % 2 == 0 && i % 2 == 0)       //체스판 무늬 흑백 번갈아 가면서 표시
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
                if (chessPos[i][j] == chessPiece[0])        //움직일 수 있는 위치는 점으로 표시
                    printf("\b\b·");

                else if (chessPos[i][j] != chessPiece[0])   //잡을 수 있는 말 빨간색 배경으로 표시
                {
                    BackColor(192);
                    printf("\b\b");
                    DrawPiece(i,j);
                    TextColor(white);
                }
            }

            else if (movable[i][j] == 3 && order%2 == 0)        //백팀 특수룰일 경우 파란색 배경으로 표시
            {
                BackColor(144);
                printf("\b\b");
                DrawPiece(i,j);
                TextColor(white);
            }

            else if (movable[i][j] == 4 && order%2 != 0)        //흑팀 특수룰일 경우 파란색 배경으로 표시
            {
                BackColor(144);
                printf("\b\b");
                DrawPiece(i,j);
                TextColor(white);
            }

            else if (movable[i][j] == -1)       //현재 선택된 말을 노란색 배경으로 표시
            {
                BackColor(224);
                printf("\b\b");
                DrawPiece(i,j);
                TextColor(white);
            }
		}
		printf("\n");
	}

	for (i = 0; i < 8; i++)     //옆에 노란색으로 좌표 숫자 표시 1~8
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

	for (i = 0; i < 14; i++)        //인터페이스 경계선 표시
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

	if (sel_piece == chessPiece[1])     //만약 고른 말이 ~이면
		printf("현재 고른 말 : 흑색 룩");       //현재 고른 말 : ~

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


void DrawDeadPiece()        //죽은 말 표시하는 함수
{
	int i;
	Gotoxy(50, 0);
	printf("     죽은 말");
	Gotoxy(50, 1);
	printf("------백팀------");
	Gotoxy(51, 2);

	for (i = 0; i < 8 - wp_live; i++)   //죽은 말을 표시
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


void DrawCursor(int x, int y)       //메인화면에서 커서 표시하는 함수
{
	Gotoxy(x, y);
	printf(">");
}


void DrawPiece(int i, int j)
{
    if (chessPos[i][j][0] == BLACK && chessPos[i][j][1] == 'R')     //만약 chessPos 배열에서 ~색 ~이면
				printf("■");    //말 표시

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


void DrawOrder()        //차례 표시하는 함수
{
	char *order_str;

	if (order % 2 == 0)     //현재 차례가 백이면
		order_str = "백";

	else                    //현재 차례가 흑이면
		order_str = "흑";

	Gotoxy(20, 0);
	printf("현재 차례 : %s", order_str);
	Gotoxy(cursor_x, cursor_y);
}


void DrawSoo()      //수 표시하는 함수
{
	Gotoxy(20, 4);
	printf("제 %d수", soo);       //몇 수 인지 표시
	Gotoxy(cursor_x, cursor_y);
}


void ChessRule()        //체스 규칙 표시하는 함수
{
	check_rule = 1;     //백스페이스 눌렀을 경우 이전 화면으로 이동시키기 위한 체크 변수
	TextColor(white);
	printf("----------------게임 방법----------------------\n");
	printf("1. 백과 흑이 번갈아가며 플레이합니다.\n\n");
	printf("2. 백이 먼저 게임을 시작합니다.\n\n");
	printf("3. 체크 상황은 킹이 죽을 위기를 의미합니다.\n\n");
	printf("4. 체크메이트는 킹이 더 이상 움직일 수 없는 상황\n");
	printf("   을 의미합니다.\n\n");
	printf("5. 양 쪽 킹 중 하나가 죽으면 게임이 끝납니다.\n\n");
	printf("6. 기권을 할 수 있으며, 기권을 하는 순간 승부가\n");
	printf("   결정납니다.");
	TextColor(grey);
	Gotoxy(0, 16);
	printf("돌아가기 (Backspace)");
	TextColor(white);

}


void ControlRule()      //조작 규칙 표시하는 함수
{
	check_rule = 1;     //백스페이스바 눌렀을 경우 다시 규칙 선택창으로 이동시키기 위한 체크 변수
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
	printf("5. 게임 도중 백스페이스 바를 누르면 메인 화면\n");
	printf("   으로 이동 합니다.");
	TextColor(grey);
	Gotoxy(0, 16);
	printf("돌아가기 (Backspace)");
	TextColor(white);
}


unsigned _stdcall Drawtimer(void *arg)      //타이머 구현 함수
{
	int min, sec;

	while(1)
	{
		min=time/60;    //설정된 시간에서 60으로 나눈 몫은 분
		sec=time%60;    //60으로 나눈 나머지는 초
		ViewCursor(0);
		Gotoxy(20,6);
		printf("남은시간 = %2d:%2d",min,sec);
		Gotoxy(cursor_x,cursor_y);
		ViewCursor(1);
		Sleep(1000);    //1초 간격으로
		time--;         //시간을 1씩 줄임

		if(time==-1)    //만약 시간이 초과되면
        {
            Timer_Alert();      //시간 초과 경고
            break;
        }

        else if(time<-1)
            break;      //while문 빠져나옴
	}

	_endthreadex(0);        //스레드 종료
	return 0;
}


void MovePiece(int x, int y)        //말 이동 구현 함수
{
    int i,j;
	chessPos[x][y] = chessPos[sel_x][sel_y];    //이동하려는 위치를 체스말로 표시
	chessPos[sel_x][sel_y] = "  ";      //이전 위치는 공백
	TurnPiece(x, y);    //움직인 후 뒤에 0으로 표시해줌
	system("cls");      //창 초기화
	sel_check = 0;      //선택 안함 상태로 설정

	if (order % 2 != 0) //만약 흑팀 차례로 끝나면
		soo++;          //수 증가

	order++;        //차례 변수 증가
	sel_color = NONE;   //선택한 색상 NONE 값
	time=SET_TIME;      //타이머 초기화

	for(i=0; i<=7; i++)
        for(j=0; j<=7; j++)
            movable[i][j]=0;    //movable변수 초기화

    DrawChess();        //체스판 불러옴
	Gotoxy(20, 2);
	printf("현재 고른 말 : 없음");
}


void SelectPiece(int x_pos, int y_pos)
{
	if (chessPos[x_pos][y_pos][0] == 'B' && chessPos[x_pos][y_pos][1] == 'R')       //만약 선택한 색상이 흑색이고 체스말이 룩이라면
    {
        sel_piece = "BR";       //선택한 말 흑색 룩
        movable[x_pos][y_pos] = -1;     //선택한 상황이라는 movable 변수 -1
        BlackRook(x_pos, y_pos);        //행마법 구현하기 위한 흑색룩 함수
        DrawChess();        //체스판 불러옴
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
		sel_piece = "  ";       //빈칸이라면 공백
}


void InitializeGame()       //게임 변수들 초기화
{
    time=SET_TIME;      //시간은 설정된 시간
    //_beginthreadex(NULL,0,Drawtimer,0,0,NULL);      //타이머 표시
	int i,j;
	system("cls");      //창 초기화
	DrawChess();        //체스판 불러옴
	TextColor(white);   //텍스트 색상 기본값
	cursor_x = 4, cursor_y = 4;     //처음 체스 시작했을 때 위치 기본값
	temp = 0;       //temp 변수 초기화
	sel_check = 0;  //선택 여부 체크 변수 초기화
	check_winner=0;     //승자 체크 변수 초기화
	check_BlackPromotion=0;     //흑색 프로모션 상황 체크 변수 초기화
	check_WhitePromotion=0;     //백색 프로모션 상황 체크 변수 초기화
	check_blackAng=0;       //흑색 앙파상 상황 체크 변수 초기화
	check_whiteAng=0;       //백색 앙파상 상황 체크 변수 초기화
	check_wpPr = 0;     //백색 폰으로 프로모션 체크 변수 초기화 (이하 동일)
    check_bpPr = 0;
    check_wqPr = 0;
    check_bqPr = 0;
    check_wbPr = 0;
    check_bbPr = 0;
    check_wnPr = 0;
    check_bnPr = 0;
    check_wrPr = 0;
    check_brPr = 0;
	order=0;        //순서 변수 기본값
	soo=1;          //수는 1 수로 초기화
	Gotoxy(20, 2);
	printf("현재 고른 말 : 없음");

    chessPos[0][0] = "BR1";     //chessPos 배열에 처음 시작할 때 말 배열로 초기화
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

	Gotoxy(cursor_x, cursor_y);
}


void CheckGame()        //게임 진행 중 체크해야 할 체크 변수 체크
{
	bk_live = 0, wk_live = 0;   //모든 말들 목숨 개수 초기화
	bq_live = 0, wq_live = 0;
	br_live = 0, wr_live = 0;
	bn_live = 0, wn_live = 0;
	bb_live = 0, wb_live = 0;
	bp_live = 0, wp_live = 0;

	if (check_wpPr != 0)        //프로모션을 한 상황에서 죽은 말 표시를 수정하기 위한 if문
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

    else if (order - w_order == 2)      //앙파상 구현하기 위한 if문 (2칸 앞으로 나간 후 턴이 다시 돌아오면 앙파상 초기화)
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

    for (int i=0; i<8; i++)     //양 측의 킹의 x y 좌표 갱신하는 for문
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

    if (order%2==0)     //만약 백팀의 차례라면
    {
        check_check=1;      //행마법 구현 함수 내에서 체크 변수만을 수정하기 위한 체크 값 1
        WhiteCheck();       //백팀의 체크 상황 구현
        check_check=0;
    }

    else        //만약 흑팀의 차례라면
    {
        check_check=1;
        BlackCheck();
        check_check=0;
    }

	int i, j;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
		{
			if (chessPos[i][j][0] == 'W' && chessPos[i][j][1] == 'K')       //죽은 말 표시를 위한 목숨값 설정
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

            if (chessPos[0][j] == "WP0")    //만약 백색 폰이 8칸 전진했다면
            {
                temp = j;       //해당 위치 temp값에 저장
                check_WhitePromotion = 1;       //백색 프로모션 체크 변수 1 (켜짐)
            }

            else if (chessPos[7][j] == "BP0")       //만약 흑색 폰이 8칸 전진했다면
            {
                temp = j;
                check_BlackPromotion = 1;
            }
		}

    if (bk_live == 0 && wk_live == 1)       //만약 흑색 킹이 죽었고 백색 킹은 살았다면
        check_winner=1;     //백팀 승리 체크 변수 1

    else if (bk_live == 1 && wk_live == 0)  //만약 흑색 킹이 살았고 백색 킹은 죽었다면
        check_winner=2;     //흑색 승리 체크 변수 2

    else if (bk_live == 0 && wk_live == 0)      //만약 양 측의 킹이 모두 죽었다면
    {
        Gotoxy(1, 10);
        printf("                        ");
        Gotoxy(1, 10);
        TextColor(red);
        printf("치명적 오류 발생");        //오류 발생 경고창
        check_error = 1;        //오류 체크 변수 1
        TextColor(white);
    }

    else if (wk_live == 1 && bk_live == 1)      //킹과 킹이 남거나 킹과 비숍이 남아 기물 부족 상황일 경우
        if (wb_live >= 0 && bb_live >= 0)
            if (wr_live == 0 && wn_live == 0 && wq_live == 0 && wp_live == 0)
                if (br_live == 0 && bn_live == 0 && bq_live == 0 && bp_live == 0)
    {
        Draw_Alert();       //무승부 알림
        check_winner = 3;       //게임 종료 체크 변수 (3은 무승부)
    }

    else if (wk_live == 1 && bk_live == 1)      //킹과 킹이 남거나 킹과 나이트가 남아 기물 부족 상황일 경우
        if (wn_live >= 0 && bn_live >= 0)
            if (wr_live == 0 && wb_live == 0 && wq_live == 0 && wp_live == 0)
                if (br_live == 0 && bb_live == 0 && bq_live == 0 && bp_live == 0)
    {
        Draw_Alert();       //무승부 알림
        check_winner = 3;       //게임 종료 체크 변수 (3은 무승부)
    }
}


void TurnPiece(int x, int y)        //움직인 말을 0으로 설정해주는 함수 (한 번 이상 움직인 말은 0)
{
    if (sel_piece == chessPiece[1])     //만약 선택했던 말이 흑색 룩이라면
		chessPos[x][y] = "BR0";     //흑색룩의 말 0으로 설정

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
        if (check_blackAng == 1)        //만약 흑색 앙파상 상황 체크 변수가 켜졌다면
        {
            chessPos[x][y] = "BP2";     //말 뒤에 숫자를 2로 설정 (앙파상)
            check_blackAng = 0;     //앙파상 상황 종료
        }

        else
            chessPos[x][y] = "BP0";     //움직였으므로 0으로 설정
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
        if (check_whiteAng == 1)        //만약 백색 앙파상 상황 체크 변수가 켜졌다면
        {
            chessPos[x][y] = "WP2";
            check_whiteAng = 0;
        }

        else
            chessPos[x][y] = "WP0";
    }
}


void WrongMove_Alert()      //잘못된 움직임 경고 함수
{
    int i,j;
	Gotoxy(1, 10);
	printf("                        ");
	Gotoxy(1, 10);
	TextColor(red);
	printf("이동할 수 없습니다.");      //경고 메세지 출력
	sel_color = NONE;       //선택한 색상 NONE 값
	TextColor(white);

	for(i=0; i<=7; i++)
        for(j=0; j<=7; j++)
            movable[i][j]=0;        //movable 변수 초기화

	Gotoxy(cursor_x, cursor_y);
}


void WrongOrder_Alert()     //잘못된 차례경고 함수
{
    int i,j;
	Gotoxy(1, 10);
	printf("                        ");
	Gotoxy(1, 10);
	TextColor(red);
	printf("차례가 아닙니다.");        //경고 메세지 출력
	sel_color = NONE;
	TextColor(white);

	for(i=0; i<=7; i++)
        for(j=0; j<=7; j++)
            movable[i][j]=0;

	Gotoxy(cursor_x, cursor_y);
}


void WhiteWin_Alert()       //백팀 승리 알림 함수
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("백팀 승리!");
    TextColor(white);
}


void WhiteCheck_Alert()     //백팀 체크 경고 함수
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("백팀 체크");
    TextColor(white);
}


void WhiteCheckmate_Alert()     //백팀 체크메이트 알림 함수
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("백팀 체크메이트!");
    Sleep(3000);        //3초 기다림
    TextColor(white);
    check_winner = 2;       //승리 체크 변수 2 (흑팀 승)
}


void BlackWin_Alert()       //흑팀 승리 알림 함수
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("흑팀 승리!");
    TextColor(white);
}


void BlackCheck_Alert()     //흑팀 체크 경고 함수
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("흑팀 체크");
    TextColor(white);
}


void BlackCheckmate_Alert()     //흑팀 체크메이트 알림 함수
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("흑팀 체크메이트!");
    Sleep(3000);        //3초 기다림
    TextColor(white);
    check_winner = 1;       //승리 체크 변수 1 (백팀 승)
}

void Stalemate_Alert()      //스테일메이트 알림 함수
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("스테일메이트!");
    Sleep(2000);        //3초 기다림
    TextColor(white);
    check_winner = 3;       //승리 체크 변수 3 (무승부)
}

void Timer_Alert()      //타이머 알림
{
    Gotoxy(1, 10);
	printf("                        ");
	Gotoxy(1, 10);
	TextColor(red);

	if(order%2==0)      //만약 백팀 차례면
    {
        printf("백팀 시간초과.");     //경고 메세지 출력
        TextColor(white);
        Sleep(2000);        //2초 기다림
        check_winner = 2;       //승리 체크 변수 2 (흑팀 승)
    }

    else
    {
        printf("흑팀 시간초과.");
        TextColor(white);
        Sleep(2000);
        check_winner = 1;       //승리 체크 변수 1 (백팀 승)
    }

	TextColor(white);
	Gotoxy(cursor_x, cursor_y);
}


void WhiteSur_Alert()       //백팀 기권 알림 함수
{
    int check = 1;  //while문 시작

    while(check)
    {
        char answer;        //대답 변수
        Gotoxy(1, 10);
        printf("                        ");
        Gotoxy(1, 10);
        TextColor(red);
        printf("백팀이 기권했습니다.");      //메세지 출력
        TextColor(white);
        Gotoxy(1, 11);
        printf("승낙하려면 y, 아니면 n을 입력 : ");
        scanf("%c",&answer);        //answer값 입력받음

        switch(answer)
        {
        case 'y':       //소문자
            check_winner = 2;       //흑팀 승리
            check = 0;
            break;

        case 'n':
            check = 0;
            break;

        case 'Y':       //대문자
            check_winner = 2;
            check = 0;
            break;

        case 'N':
            check = 0;
            break;

        default:        //다른 값을 입력받았으면
            Gotoxy(1, 11);
            printf("승낙하려면 y, 아니면 n을 입력 :      ");       //다시 한번 출력
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


void BlackSur_Alert()       //흑팀 기권 알림 함수
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
            check_winner = 1;       //백팀 승
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

void Draw_Alert()       //무승부 알림
{
    Gotoxy(1, 10);
    printf("                        ");
    Gotoxy(1, 10);
    TextColor(red);
    printf("무승부!");
    TextColor(white);
}

void DrawSug_Alert()        //무승부 신청 알림
{
    int check = 1;      //while문 시작 변수

    while(check)
    {
        char answer;        //응답을 받아들이는 문자형 변수
        Gotoxy(1, 10);
        printf("                        ");
        Gotoxy(1, 10);
        TextColor(red);
        printf("상대가 무승부를 신청했습니다.");
        TextColor(white);
        Gotoxy(1, 11);
        printf("승낙하려면 y, 아니면 n을 입력 : ");
        scanf("%c",&answer);

        switch(answer)
        {
        case 'y':       //y를 눌러 승낙
            Draw_Alert();           //무승부 알림
            check_winner = 3;       //무승부
            check = 0;
            break;

        case 'n':       //n을 눌러 거절
            check = 0;
            break;

        case 'Y':
            Draw_Alert();
            check_winner = 3;
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
    printf("                               ");
    Gotoxy(1, 11);
    printf("                                 ");
}

void WhitePromotion(int x)      //백팀 프로모션 구현 및 알림 함수
{
    int check = 1;      //while문 시작 변수
    char a = Invert(x*2);       //숫자를 알파벳으로 변환하는 함수 호출

    while(check)
    {
        int promotion = 0;      //프로모션으로 바뀌는 말 체크 변수
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
        case 1:         //백색 퀸으로 프로모션
            chessPos[0][x] = "WQ0";
            check = 0;          //while문 벗어남
            check_WhitePromotion = 0;       //프로모션 상황 체크 변수 꺼짐 0
            check_wpPr++;           //죽은 말 표시에 프로모션된 말 반영하는 체크 변수 백색 폰
            check_wqPr++;           //  "   백색 퀸
            break;

        case 2:         //백색 비숍으로 프로모션
            chessPos[0][x] = "WB0";
            check = 0;
            check_WhitePromotion = 0;
            check_wpPr++;
            check_wbPr++;
            break;

        case 3:         //백색 나이트로 프로모션
            chessPos[0][x] = "WN0";
            check = 0;
            check_WhitePromotion = 0;
            check_wpPr++;
            check_wnPr++;
            break;

        case 4:         //백색 룩으로 프로모션
            chessPos[0][x] = "WR0";
            check = 0;
            check_WhitePromotion = 0;
            check_wpPr++;
            check_wrPr++;
            break;

        default:        //그 외의 값
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

void BlackPromotion(int x)      //흑팀 프로모션 구현 및 알림 함수
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
        case 1:         //흑색 퀸으로 프로모션
            chessPos[7][x] = "BQ0";
            check = 0;
            check_BlackPromotion = 0;
            check_bpPr++;
            check_bqPr++;
            break;

        case 2:         //흑색 비숍으로 프로모션
            chessPos[7][x] = "BB0";
            check = 0;
            check_BlackPromotion = 0;
            check_bpPr++;
            check_bbPr++;
            break;

        case 3:         //흑색 나이트로 프로모션
            chessPos[7][x] = "BN0";
            check = 0;
            check_BlackPromotion = 0;
            check_bpPr++;
            check_bnPr++;
            break;

        case 4:         //흑색 룩으로 프로모션
            chessPos[7][x] = "BR0";
            check = 0;
            check_BlackPromotion = 0;
            check_bpPr++;
            check_brPr++;
            break;

        default:        //그 외의 값
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

//행마법
void WhiteKing(int x, int y)        //백색 킹 움직임 함수 (x, y값은 행과 열의 개념이다)
{
    if (x-1 >= 0)       //만약 행 방향으로 앞으로 한 칸 갈 수 있다면
    {
        if (y-1 >= 0)       //만약 열방향으로 왼쪽으로 한 칸 갈 수 있다면 (좌상향 대각선으로 이동)
        {
            if (chessPos[x-1][y-1] == chessPiece[0] || chessPos[x-1][y-1][0] == BLACK)      //만약 좌상향 대각선이 빈칸이거나 상대 팀 말일 경우
            {
                if (check_check == 1)       //만약 체크 상황을 판별해야 하는 상황일 경우
                    check[x-1][y-1] = 1;        //이동하려는 위치의 체크 배열을 1로 저장

                else                        //위의 상황이 아니라면 (그냥 이동해야 하는 상황)
                    movable[x-1][y-1] = 1;      //이동하려는 위치의 movable 배열을 1로 저장
            }
        }

        if (y+1 <= 7)       //만약 열방향으로 오른쪽으로 한 칸 갈 수 있다면 (우상향 대각선)
        {
            if (chessPos[x-1][y+1] == chessPiece[0] || chessPos[x-1][y+1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x-1][y+1] = 1;

                else
                    movable[x-1][y+1] = 1;
            }
        }

        if (chessPos[x-1][y] == chessPiece[0] || chessPos[x-1][y][0] == BLACK)      //앞으로 한칸 이동
        {
                if (check_check == 1)
                    check[x-1][y] = 1;

                else
                    movable[x-1][y] = 1;
            }
    }

    if (x+1 <= 7)       //만약 행 방향으로 뒤로 한 칸 갈 수 있다면
    {
        if (y-1 >= 0)       //좌하향 대각선
        {
            if (chessPos[x+1][y-1] == chessPiece[0] || chessPos[x+1][y-1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x+1][y-1] = 1;

                else
                    movable[x+1][y-1] = 1;
            }
        }

        if (y+1 <= 7)       //우하향 대각선
        {
            if (chessPos[x+1][y+1] == chessPiece[0] || chessPos[x+1][y+1][0] == BLACK)
            {
                if (check_check == 1)
                    check[x+1][y+1] = 1;

                else
                    movable[x+1][y+1] = 1;
            }
        }

        if (chessPos[x+1][y] == chessPiece[0] || chessPos[x+1][y][0] == BLACK)      //아래로 한 칸 이동
        {
            if (check_check == 1)
                check[x+1][y] = 1;

            else
                movable[x+1][y] = 1;
        }
    }

    if (y-1 >= 0)       //왼쪽으로 이동
    {
        if (chessPos[x][y-1] == chessPiece[0] || chessPos[x][y-1][0] == BLACK)
        {
            if (check_check == 1)
                check[x][y-1] = 1;

            else
                movable[x][y-1] = 1;
        }
    }

    if (y+1 <= 7)       //오른쪽으로 이동
    {
        if (chessPos[x][y+1] == chessPiece[0] || chessPos[x][y+1][0] == BLACK)
        {
            if (check_check == 1)
                check[x][y+1] = 1;

            else
                movable[x][y+1] = 1;
        }
    }

    if (check_check == 0)       //만약 체크 판별 상황이 아니라면
        for (int i=0; i<8; i++)
            for (int j=0; j<8; j++)
                if (check[i][j] == 1)       //좌표가 체크 1 이라면 (상대 말의 공격 범위)
                    movable[i][j] = 0;      //movable 0 (킹은 상대 말의 공격 범위로 이동할 수 없음)

    if (chessPos[x][y] == "WK1")        //캐슬링
    {
        if (chessPos[7][5] == chessPiece[0] && chessPos[7][6] == chessPiece[0] && chessPos[7][7] == "WR1")  //킹사이드 캐슬링
            movable[x][y+2] = 3;

        if (chessPos[7][3] == chessPiece[0] && chessPos[7][2] == chessPiece[0] && chessPos[7][1] == chessPiece[0] && chessPos[7][0] == "WR1")       //퀸사이드 캐슬링
            movable [x][y-2] = 3;
    }
}

void BlackKing(int x, int y)        //흑색 킹 움직임 함수 (백색 킹과 동일)
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
            movable[x][y+2] = 4;

        if (chessPos[0][3] == chessPiece[0] && chessPos[0][2] == chessPiece[0] && chessPos[0][1] == chessPiece[0] && chessPos[0][0] == "BR1")
            movable [x][y-2] = 4;
    }
}

void WhiteQueen(int x, int y)       //백색 퀸 움직임 함수
{
    WhiteRook(x, y);        //백색 룩 움직임 + 백색 비숍 움직임
    WhiteBishop(x, y);
}

void BlackQueen(int x, int y)       //흑색 퀸 움직임 함수 (백색 퀸과 동일)
{
    BlackRook(x, y);
    BlackBishop(x, y);
}

void WhitePawn(int x, int y)        //백색 폰 움직임 함수
{
    if (chessPos[x][y][2] == '1')   //만약 한번도 안 움직였다면
    {
        if (chessPos[x-2][y] == chessPiece[0])      //두 칸 앞이 빈칸이면
            movable[x-2][y] = 2;    //앙파상 판별 값으로 movable 배열 설정
    }

    if(x>0)     //만약 앞으로 전진 가능하다면
    {
        if (chessPos[x-1][y]==chessPiece[0])    //만약 한 칸 앞이 빈칸이면
            movable[x-1][y] = 1;        //움직일 수 있음

        if (chessPos[x-1][y-1][0]==BLACK)       //만약 좌상향 대각선 앞이 상대 말이면
            movable[x-1][y-1] = 1;      //공격할 수 있음

        if (chessPos[x-1][y+1][0]==BLACK)       //만약 우상향 대각선 앞이 상대 말이면
            movable[x-1][y+1] = 1;      //공격할 수 있음

        if (chessPos[x][y-1][2] == '2')     //만약 옆에 있는 말이 두 칸 전진했다면 (앙파상 상황)
            movable[x-1][y-1] = 3;      //좌상향 대각선으로 특수 이동 적용

        if (chessPos[x][y+1][2] == '2')
            movable[x-1][y+1] = 3;      //우상향 대각선으로 특수 이동 적용
    }
}


void BlackPawn(int x, int y)        //흑색 폰 움직임 함수 (백색 폰과 동일)
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
            movable[x+1][y-1]=4;

        if (chessPos[x][y+1][2] == '2')
            movable[x+1][y+1]=4;
    }
}


void WhiteNight(int x, int y)   // 백색 나이트 움직임 함수
{
    if (x-2 >= 0)       //만약 앞으로 두 칸 이동 가능하면
    {
        if (y-1 >= 0)   //만약 왼쪽으로 한 칸 이동 가능하면
        {
            if (chessPos[x-2][y-1] == chessPiece[0] || chessPos[x-2][y-1][0] == BLACK)      //만약 앞으로 두 칸, 왼쪽으로 한 칸이 비어있거나 적팀 기물이면
            {
                if (check_check == 1)       //만약 체크 판별 상황이라면
                {
                    check[x-2][y-1] = 1;    //체크 1
                    wmv_cnt++;      //스테일메이트 판별 용 백색 이동 가능 변수 +1
                }

                else
                    movable[x-2][y-1] = 1;      //체크 판별 상황이 아니면 움직임 이동
            }
        }

        if (y+1 <= 7)       //만약 오른쪽으로 한 칸 이동 가능하면
        {
            if (chessPos[x-2][y+1] == chessPiece[0] || chessPos[x-2][y+1][0] == BLACK)
            {
                if (check_check == 1)
                {
                    check[x-2][y+1] = 1;
                    wmv_cnt++;
                }

                else
                    movable[x-2][y+1] = 1;
            }
        }
    }

    if (x-1 >= 0)       //만약 앞으로 한 칸 이동 가능하면
    {
        if (y-2 >= 0)       //만약 왼쪽으로 두 칸 이동 가능하면
        {
            if (chessPos[x-1][y-2] == chessPiece[0] || chessPos[x-1][y-2][0] == BLACK)
            {
                if (check_check == 1)
                {
                    check[x-1][y-2] = 1;
                    wmv_cnt++;
                }

                else
                    movable[x-1][y-2] = 1;
            }
        }

        if (y+2 <= 7)       //만약 오른쪽으로 두 칸 이동 가능하면
        {
            if (chessPos[x-1][y+2] == chessPiece[0] || chessPos[x-1][y+2][0] == BLACK)
            {
                if (check_check == 1)
                {
                    check[x-1][y+2] = 1;
                    wmv_cnt++;
                }

                else
                    movable[x-1][y+2] = 1;
            }
        }
    }

    if (x+1 <= 7)       //만약 뒤로 한 칸 이동 가능 하면
    {
        if (y-2 >= 0)       //만약 왼쪽으로 두 칸 이동 가능 하면
        {
            if (chessPos[x+1][y-2] == chessPiece[0] || chessPos[x+1][y-2][0] == BLACK)
            {
                if (check_check == 1)
                {
                    check[x+1][y-2] = 1;
                    wmv_cnt++;
                }

                else
                    movable[x+1][y-2] = 1;
            }
        }

        if (y+2 <= 7)       //만약 오른쪽으로 두 칸 이동 가능 하면
        {
            if (chessPos[x+1][y+2] == chessPiece[0] || chessPos[x+1][y+2][0] == BLACK)
            {
                if (check_check == 1)
                {
                    check[x+1][y+2] = 1;
                    wmv_cnt++;
                }

                else
                    movable[x+1][y+2] = 1;
            }
        }
    }

    if (x+2 <= 7)       //만약 뒤로 두 칸 이동 가능하면
    {
        if (y-1 >= 0)       //만약 왼쪽으로 한 칸 이동 가능하면
        {
            if (chessPos[x+2][y-1] == chessPiece[0] || chessPos[x+2][y-1][0] == BLACK)
            {
                if (check_check == 1)
                {
                    check[x+2][y-1] = 1;
                    wmv_cnt++;
                }

                else
                    movable[x+2][y-1] = 1;
            }
        }

        if (y+1 <= 7)       //만약 오른쪽으로 한 칸 이동 가능 하면
        {
            if (chessPos[x+2][y+1] == chessPiece[0] || chessPos[x+2][y+1][0] == BLACK)
            {
                if (check_check == 1)
                {
                    check[x+2][y+1] = 1;
                    wmv_cnt++;
                }

                else
                    movable[x+2][y+1] = 1;
            }
        }
    }
}


void BlackNight(int x, int y)       //흑색 나이트 움직임 함수 (백색 나이트와 동일)
{
    if (x-2 >= 0)
    {
        if (y-1 >= 0)
        {
            if (chessPos[x-2][y-1] == chessPiece[0] || chessPos[x-2][y-1][0] == WHITE)
            {
                if (check_check == 1)
                {
                    check[x-2][y-1] = 1;
                    bmv_cnt++;
                }

                else
                    movable[x-2][y-1] = 1;
            }
        }

        if (y+1 <= 7)
        {
            if (chessPos[x-2][y+1] == chessPiece[0] || chessPos[x-2][y+1][0] == WHITE)
            {
                if (check_check == 1)
                {
                    check[x-2][y+1] = 1;
                    bmv_cnt++;
                }

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
                {
                    check[x-1][y-2] = 1;
                    bmv_cnt++;
                }

                else
                    movable[x-1][y-2] = 1;
            }
        }

        if (y+2 <= 7)
        {
            if (chessPos[x-1][y+2] == chessPiece[0] || chessPos[x-1][y+2][0] == WHITE)
            {
                if (check_check == 1)
                {
                    check[x-1][y+2] = 1;
                    bmv_cnt++;
                }

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
                {
                    check[x+1][y-2] = 1;
                    bmv_cnt++;
                }

                else
                    movable[x+1][y-2] = 1;
            }
        }

        if (y+2 <= 7)
        {
            if (chessPos[x+1][y+2] == chessPiece[0] || chessPos[x+1][y+2][0] == WHITE)
            {
                if (check_check == 1)
                {
                    check[x+1][y+2] = 1;
                    bmv_cnt++;
                }

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
                {
                    check[x+2][y-1] = 1;
                    bmv_cnt++;
                }
                else
                    movable[x+2][y-1] = 1;
            }
        }

        if (y+1 <= 7)
        {
            if (chessPos[x+2][y+1] == chessPiece[0] || chessPos[x+2][y+1][0] == WHITE)
            {
                if (check_check == 1)
                {
                    check[x+2][y+1] = 1;
                    bmv_cnt++;
                }

                else
                    movable[x+2][y+1] = 1;
            }
        }
    }
}


void WhiteRook(int x, int y){       //백색 룩 움직임 함수

    if(x != 0) //위로 이동
    {
        for(int i = 1; i <= x; i++)
        {
            if(chessPos[x-i][y][0] == WHITE)
                break;

            if(chessPos[x-i][y] == chessPiece[0] || chessPos[x-i][y][0] == BLACK)
            {
                movable[x-i][y]=1;
                if(chessPos[x-i][y][0] == BLACK) // 만약 상대기물이 앞에 있다면 갈 수 없음
                    break;
            }
        }
    }

    if(x != 7)// 밑으로 이동
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

    if(y != 0) // 왼쪽으로 이동
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

    if(y != 7) // 오른쪽으로 이동
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


void BlackRook(int x, int y) // 블랙룩 이동 함수
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


void BlackBishop(int x,int y)//블랙 비숍움직임
{
    int i;

    for(i = 1; i < 8; i++)
    {
        if((x-i) < 0 || (y-i) < 0) //왼쪽 위 대각선 말이 막히면 이동 불가
            break;

        else if(chessPos[x-i][y-i][0] == BLACK)
            break;

        movable[x-i][y-i] = 1; //비숍이 이동할 수 있는 곳을 1로 바꿔줌

        if(chessPos[x-i][y-i][0] == WHITE)
            break;
    }

    for(i = 1; i < 8; i++) //오른쪽 위로 대각선 말이 막히면 이동 불가
    {
        if((x-i) < 0 || (y+i) > 7)
            break;

        else if(chessPos[x-i][y+i][0] == BLACK)
            break;

        movable[x-i][y+i]=1;

        if(chessPos[x-i][y+i][0]==WHITE)
            break;
    }

    for(i = 1; i < 8; i++)//왼쪽 아래로 대각선 말이 막히면 이동 불가
    {
        if((x+i) > 7 || (y-i) < 0)
            break;

        else if(chessPos[x+i][y-i][0] == BLACK)
            break;

        movable[x+i][y-i]=1;

        if(chessPos[x+i][y-i][0] == WHITE)
            break;
    }

    for(i = 1; i < 8; i++)// 오른쪽 아래로 대각선 말이 막히면 이동 불가
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


void WhiteBishop(int x,int y)//화이트 비숍 움직임 블랙 비숍과 같음
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


void Gotoxy(int x, int y)       //좌표를 이동시키는 함수
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleHandle, pos);
}


void Erase(int x, int y)        //좌표를 지우는 함수
{
	Gotoxy(x, y);
	printf(" ");
}


char Invert(int x)      //숫자를 입력받아 알파벳으로 변환하는 함수
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


void TextColor(int color_number)        //텍스트 색상을 바꾸는 함수
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}


void BackColor(int color_number)        //배경 색상을 바꾸는 함수
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hC, &csbi);
    SetConsoleTextAttribute(hC, color_number);
}


void ViewCursor(char show)      //커서 표시하는 함수
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}


void WhiteCheck()       //백팀 체크 상황 판별 함수
{
    int i,j;

    for (i = 0; i < 8; i++)     //체크 변수 초기화
        for (j = 0; j < 8; j++)
            check[i][j] = 0;

    wmv_cnt = 0;        //백색 움직임 횟수 변수 초기화

    for(i = 0; i < 8; i++)      //체스판 전체를 체크
        for(j = 0; j < 8; j++)
        {
            if (chessPos[i][j][0] == WHITE)     //만약 백색 말이면
            {
                if (chessPos[i][j][1] != 'K')   //만약 킹이 아닌 다른 기물이면
                    check[i][j] = 1;        //체크 변수 1

                if (chessPos[i][j][1] == 'P')       //만약 백색 폰이면 (스테일메이트 판별 함수)
                    if (i > 0)
                    {
                        if (chessPos[i-1][j-1][0] == BLACK || chessPos[i-1][j-1] == chessPiece[0])
                            wmv_cnt++;      //백색 움직임 횟수 변수 +1

                        if (chessPos[i-1][j+1][0] == BLACK || chessPos[i-1][j+1] == chessPiece[0])
                            wmv_cnt++;
                    }

                if (chessPos[i][j][1] == 'N')       //만약 백색 나이트면
                    WhiteNight(i, j);

                if (chessPos[i][j][1] == 'R')       //만약 백색 룩이면
                {
                    if (i != 0)
                    {
                        for (int k = 0; k <= i; k++)
                        {
                            if (chessPos[i-k][j] == chessPiece[0])
                                wmv_cnt++;

                            if (chessPos[i-k][j][0] == BLACK || chessPos[i-k][j][0] == WHITE)
                            {
                                if (chessPos[i-k][j][0] == BLACK && chessPos[i-k][j][1] == 'K')
                                {
                                    wmv_cnt++;
                                    if (chessPos[i-k-1][j] == chessPiece[0])
                                        wmv_cnt++;
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
                                wmv_cnt++;

                            if (chessPos[i+k][j][0] == BLACK || chessPos[i+k][j][0] == WHITE)
                            {
                                if (chessPos[i+k][j][0] == BLACK && chessPos[i+k][j][1] == 'K')
                                {
                                    wmv_cnt++;

                                    if (chessPos[i+k+1][j] == chessPiece[0])
                                        wmv_cnt++;
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
                                wmv_cnt++;

                            if (chessPos[i][j-k][0] == BLACK || chessPos[i][j-k][0] == WHITE)
                            {
                                if (chessPos[i][j-k][0] == BLACK && chessPos[i][j-k][1] == 'K')
                                {
                                    wmv_cnt++;

                                    if (chessPos[i][j-k-1] == chessPiece[0])
                                            wmv_cnt++;
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
                                wmv_cnt++;

                            if (chessPos[i][j+k][0] == BLACK || chessPos[i][j+k][0] == WHITE)
                            {
                                if (chessPos[i][j+k][0] == BLACK && chessPos[i][j+k][1] == 'K')
                                {
                                    wmv_cnt++;

                                    if (chessPos[i][j+k+1] == chessPiece[0])
                                        wmv_cnt++;
                                }
                                break;
                            }
                        }
                    }
                }

                if (chessPos[i][j][1] == 'B')       //만약 백색 비숍이면
                {
                    for (int k = 1; k < 8; k++)
                    {
                        if ((i-k < 0) || (j-k < 0))
                            break;

                        if (chessPos[i-k][j-k][0] == WHITE)
                            break;

                        wmv_cnt++;

                        if (chessPos[i-k][j-k][0] == BLACK)
                        {
                            if (chessPos[i-k][j-k][1] == 'K')
                            {
                                wmv_cnt++;
                                if (chessPos[i-k-1][j-k-1] == chessPiece[0])
                                    wmv_cnt++;
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

                        wmv_cnt++;

                        if (chessPos[i-k][j+k][0] == BLACK)
                        {
                            if (chessPos[i-k][j+k][1] == 'K')
                            {
                                wmv_cnt++;

                                if (chessPos[i-k-1][j+k+1] == chessPiece[0])
                                    wmv_cnt++;
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

                        wmv_cnt++;

                        if (chessPos[i+k][j-k][0] == BLACK)
                        {
                            if (chessPos[i+k][j-k][1] == 'K')
                            {
                                wmv_cnt++;

                                if (chessPos[i+k+1][j-k-1] == chessPiece[0])
                                    wmv_cnt++;
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

                        wmv_cnt++;

                        if (chessPos[i+k][j+k][0] == BLACK)
                        {
                            if (chessPos[i+k][j+k][1] == 'K')
                            {
                                wmv_cnt++;

                                if (chessPos[i+k+1][j+k+1] == chessPiece[0])
                                    wmv_cnt++;
                            }
                        break;
                        }
                    }
                }

                if (chessPos[i][j][1] == 'Q')       //만약 백색 퀸이면
                {
                    if (i != 0)
                    {
                        for (int k = 0; k <= i; k++)
                        {
                            if (chessPos[i-k][j] == chessPiece[0])
                                wmv_cnt++;

                            if (chessPos[i-k][j][0] == BLACK || chessPos[i-k][j][0] == WHITE)
                            {
                                if (chessPos[i-k][j][0] == BLACK && chessPos[i-k][j][1] == 'K')
                                {
                                    wmv_cnt++;

                                    if (chessPos[i-k-1][j] == chessPiece[0])
                                        wmv_cnt++;
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
                                wmv_cnt++;

                            if (chessPos[i+k][j][0] == BLACK || chessPos[i+k][j][0] == WHITE)
                            {
                                if (chessPos[i+k][j][0] == BLACK && chessPos[i+k][j][1] == 'K')
                                {
                                    wmv_cnt++;
                                    if (chessPos[i+k+1][j] == chessPiece[0])
                                        wmv_cnt++;
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
                                wmv_cnt++;

                            if (chessPos[i][j-k][0] == BLACK || chessPos[i][j-k][0] == WHITE)
                            {
                                if (chessPos[i][j-k][0] == BLACK && chessPos[i][j-k][1] == 'K')
                                {
                                    wmv_cnt++;
                                    if (chessPos[i][j-k-1] == chessPiece[0])
                                        wmv_cnt++;
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
                                wmv_cnt++;

                            if (chessPos[i][j+k][0] == BLACK || chessPos[i][j+k][0] == WHITE)
                            {
                                if (chessPos[i][j+k][0] == BLACK && chessPos[i][j+k][1] == 'K')
                                {
                                    wmv_cnt++;
                                    if (chessPos[i][j+k+1] == chessPiece[0])
                                        wmv_cnt++;
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

                        wmv_cnt++;

                        if (chessPos[i-k][j-k][0] == BLACK)
                        {
                            if (chessPos[i-k][j-k][1] == 'K')
                            {
                                wmv_cnt++;

                                if (chessPos[i-k-1][j-k-1] == chessPiece[0])
                                    wmv_cnt++;
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

                        wmv_cnt++;

                        if (chessPos[i-k][j+k][0] == BLACK)
                        {
                            if (chessPos[i-k][j+k][1] == 'K')
                            {
                                wmv_cnt++;

                                if (chessPos[i-k-1][j+k+1] == chessPiece[0])
                                    wmv_cnt++;
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

                        wmv_cnt++;

                        if (chessPos[i+k][j-k][0] == BLACK)
                        {
                            if (chessPos[i+k][j-k][1] == 'K')
                            {
                                wmv_cnt++;
                                if (chessPos[i+k+1][j-k-1] == chessPiece[0])
                                    wmv_cnt++;
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

                        wmv_cnt++;

                        if (chessPos[i+k][j+k][0] == BLACK)
                        {
                            if (chessPos[i+k][j+k][1] == 'K')
                            {
                                wmv_cnt++;

                                if (chessPos[i+k+1][j+k+1] == chessPiece[0])
                                    wmv_cnt++;
                            }
                        break;
                        }
                    }
                }
            }

            if (chessPos[i][j][0] == BLACK)     //만약 흑색 말이면 (공격 범위 나타냄)
            {
                if (chessPos[i][j][1] == 'P')       //흑색 폰이면
                    if (i < 7)
                    {
                        if (chessPos[i+1][j-1][0] == WHITE || chessPos[i+1][j-1] == chessPiece[0])
                            check[i+1][j-1] = 1;

                        if (chessPos[i+1][j+1][0] == WHITE || chessPos[i+1][j+1] == chessPiece[0])
                            check[i+1][j+1] = 1;
                    }

                if (chessPos[i][j][1] == 'N')       //흑색 나이트면
                    BlackNight(i, j);

                if (chessPos[i][j][1] == 'R')       //흑색 룩이면
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

                if (chessPos[i][j][1] == 'B')       //흑색 비숍이면
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

                if (chessPos[i][j][1] == 'Q')       //흑색 퀸이면
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

                if (chessPos[i][j][1] == 'K')       //흑색 킹이면
                    BlackKing(i, j);
        }
    }
}


void BlackCheck()       //흑팀 체크 상황 판별 함수
{
    int i,j;

    for (i = 0; i < 8; i++)     //체크 변수 초기화
        for (j = 0; j < 8; j++)
            check[i][j] = 0;

    bmv_cnt = 0;     //흑색 움직임 횟수 변수 초기화

    for(i = 0; i < 8; i++)      //체스판 전체를 체크
        for(j = 0; j < 8; j++)
        {
            if (chessPos[i][j][0] == BLACK)     //만약 흑색 말이면
            {
                if (chessPos[i][j][1] != 'K')       //만약 킹이 아닌 다른 기물이면
                    check[i][j] = 1;        //체크 변수 1

                if (chessPos[i][j][1] == 'P')   //만약 흑색 폰이면 (스테일메이트 판별 함수)
                    if (i < 7)
                    {
                        if (chessPos[i+1][j-1][0] == WHITE || chessPos[i+1][j-1] == chessPiece[0])
                            bmv_cnt++;

                        if (chessPos[i+1][j+1][0] == WHITE || chessPos[i+1][j+1] == chessPiece[0])
                            bmv_cnt++;
                    }

                if (chessPos[i][j][1] == 'N')       //만약 흑색 나이트면
                    BlackNight(i, j);

                if (chessPos[i][j][1] == 'R')       //만약 흑색 룩이면
                {
                    if (i != 0)
                    {
                        for (int k = 0; k <= i; k++)
                        {
                            if (chessPos[i-k][j] == chessPiece[0])
                                bmv_cnt++;

                            if (chessPos[i-k][j][0] == BLACK || chessPos[i-k][j][0] == WHITE)
                            {
                                if (chessPos[i-k][j][0] == WHITE && chessPos[i-k][j][1] == 'K')
                                {
                                    bmv_cnt++;
                                    if (chessPos[i-k-1][j] == chessPiece[0])
                                        bmv_cnt++;
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
                                bmv_cnt++;

                            if (chessPos[i+k][j][0] == BLACK || chessPos[i+k][j][0] == WHITE)
                            {
                                if (chessPos[i+k][j][0] == WHITE && chessPos[i+k][j][1] == 'K')
                                {
                                    bmv_cnt++;

                                    if (chessPos[i+k+1][j] == chessPiece[0])
                                        bmv_cnt++;
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
                                bmv_cnt++;

                            if (chessPos[i][j-k][0] == BLACK || chessPos[i][j-k][0] == WHITE)
                            {
                                if (chessPos[i][j-k][0] == WHITE && chessPos[i][j-k][1] == 'K')
                                {
                                    bmv_cnt++;

                                    if (chessPos[i][j-k-1] == chessPiece[0])
                                        bmv_cnt++;
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
                                bmv_cnt++;

                            if (chessPos[i][j+k][0] == BLACK || chessPos[i][j+k][0] == WHITE)
                            {
                                if (chessPos[i][j+k][0] == WHITE && chessPos[i][j+k][1] == 'K')
                                {
                                    bmv_cnt++;

                                    if (chessPos[i][j+k+1] == chessPiece[0])
                                        bmv_cnt++;
                                }
                                break;
                            }
                        }
                    }
                }

                if (chessPos[i][j][1] == 'B')       //만약 흑색 비숍이면
                {
                    for (int k = 1; k < 8; k++)
                    {
                        if ((i-k < 0) || (j-k < 0))
                            break;

                        if (chessPos[i-k][j-k][0] == BLACK)
                            break;

                        bmv_cnt++;

                        if (chessPos[i-k][j-k][0] == WHITE)
                        {
                            if (chessPos[i-k][j-k][1] == 'K')
                            {
                                bmv_cnt++;

                                if (chessPos[i-k-1][j-k-1] == chessPiece[0])
                                    bmv_cnt++;
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

                        bmv_cnt++;

                        if (chessPos[i-k][j+k][0] == WHITE)
                        {
                            if (chessPos[i-k][j+k][1] == 'K')
                            {
                                bmv_cnt++;

                                if (chessPos[i-k-1][j+k+1] == chessPiece[0])
                                    bmv_cnt++;
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

                        bmv_cnt++;

                        if (chessPos[i+k][j-k][0] == WHITE)
                        {
                            if (chessPos[i+k][j-k][1] == 'K')
                            {
                                bmv_cnt++;

                                if (chessPos[i+k+1][j-k-1] == chessPiece[0])
                                    bmv_cnt++;
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
                                bmv_cnt++;

                                if (chessPos[i+k+1][j+k+1] == chessPiece[0])
                                    bmv_cnt++;
                            }
                        break;
                        }
                    }
                }

                if (chessPos[i][j][1] == 'Q')       //만약 흑색 퀸이면
                {
                    if (i != 0)
                    {
                        for (int k = 0; k <= i; k++)
                        {
                            if (chessPos[i-k][j] == chessPiece[0])
                                bmv_cnt++;

                            if (chessPos[i-k][j][0] == BLACK || chessPos[i-k][j][0] == WHITE)
                            {
                                if (chessPos[i-k][j][0] == WHITE && chessPos[i-k][j][1] == 'K')
                                {
                                    bmv_cnt++;

                                    if (chessPos[i-k-1][j] == chessPiece[0])
                                        bmv_cnt++;
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
                                bmv_cnt++;

                            if (chessPos[i+k][j][0] == BLACK || chessPos[i+k][j][0] == WHITE)
                            {
                                if (chessPos[i+k][j][0] == WHITE && chessPos[i+k][j][1] == 'K')
                                {
                                    bmv_cnt++;
                                    if (chessPos[i+k+1][j] == chessPiece[0])
                                        bmv_cnt++;
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
                                bmv_cnt++;

                            if (chessPos[i][j-k][0] == BLACK || chessPos[i][j-k][0] == WHITE)
                            {
                                if (chessPos[i][j-k][0] == WHITE && chessPos[i][j-k][1] == 'K')
                                {
                                    bmv_cnt++;

                                    if (chessPos[i][j-k-1] == chessPiece[0])
                                        bmv_cnt++;
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
                                bmv_cnt++;

                            if (chessPos[i][j+k][0] == BLACK || chessPos[i][j+k][0] == WHITE)
                            {
                                if (chessPos[i][j+k][0] == WHITE && chessPos[i][j+k][1] == 'K')
                                {
                                    bmv_cnt++;
                                    if (chessPos[i][j+k+1] == chessPiece[0])
                                        bmv_cnt++;
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

                        bmv_cnt++;

                        if (chessPos[i-k][j-k][0] == WHITE)
                        {
                            if (chessPos[i-k][j-k][1] == 'K')
                            {
                                bmv_cnt++;

                                if (chessPos[i-k-1][j-k-1] == chessPiece[0])
                                    bmv_cnt++;
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

                        bmv_cnt++;

                        if (chessPos[i-k][j+k][0] == WHITE)
                        {
                            if (chessPos[i-k][j+k][1] == 'K')
                            {
                                bmv_cnt++;

                                if (chessPos[i-k-1][j+k+1] == chessPiece[0])
                                    bmv_cnt++;
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

                        bmv_cnt++;

                        if (chessPos[i+k][j-k][0] == WHITE)
                        {
                            if (chessPos[i+k][j-k][1] == 'K')
                            {
                                bmv_cnt++;

                                if (chessPos[i+k+1][j-k-1] == chessPiece[0])
                                    bmv_cnt++;
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

                        bmv_cnt++;

                        if (chessPos[i+k][j+k][0] == WHITE)
                        {
                            if (chessPos[i+k][j+k][1] == 'K')
                            {
                                bmv_cnt++;

                                if (chessPos[i+k+1][j+k+1] == chessPiece[0])
                                    bmv_cnt++;
                            }
                        break;
                        }
                    }
                }
            }

            if (chessPos[i][j][0] == WHITE)     //만약 백색 말이면
            {
                if (chessPos[i][j][1] == 'P')       //만약 백색 폰이면
                    if (i > 0)
                    {
                        if (chessPos[i-1][j-1][0] == BLACK || chessPos[i-1][j-1] == chessPiece[0])
                            check[i-1][j-1] = 1;

                        if (chessPos[i-1][j+1][0] == BLACK || chessPos[i-1][j+1] == chessPiece[0])
                            check[i-1][j+1] = 1;
                    }

                if (chessPos[i][j][1] == 'N')       //만약 백색 나이트면
                    WhiteNight(i, j);

                if (chessPos[i][j][1] == 'R')       //만약 백색 룩이면
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

                if (chessPos[i][j][1] == 'B')       //만약 백색 비숍이면
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

                if (chessPos[i][j][1] == 'Q')       //만약 백색 퀸이면
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

                if (chessPos[i][j][1] == 'K')       //만약 백색 킹이면
                    WhiteKing(i, j);
        }
    }
}


void CheckCheck()       //체크 상황을 판별하는 함수
{
    wking_check = 0;        //체크메이트 상황을 판단하기 위한 변수
    bking_check = 0;        //체크메이트 상황을 판단하기 위한 변수

    if (wk_x == 0)      //만약 백색 킹이 가장 위에 있다면 (위치에 따른 체크메이트 판단 변수 조정)
    {
        if (wk_y == 0)      //만약 백색 킹이 가장 위에 왼쪽에 있다면 (왼쪽 상단 구석)
            wking_check = 5;        //체크메이트 변수 5로 설정

        if (wk_y > 0 && wk_y <8)    //만약 백색 킹이 가장 위 구석을 제외한 자리에 있다면
            wking_check = 3;

        if (wk_y == 7)      //만약 백색 킹이 가장 위에 오른쪽에 있다면 (오른쪽 상단 구석)
            wking_check = 5;
    }

    else if (wk_x == 7)     //만약 백색 킹이 가장 아래에 있다면
    {
        if (wk_y == 0)      //만약 백색 킹이 가장 아래에 왼쪽에 있다면 (왼쪽 하단 구석)
            wking_check = 5;

        if (wk_y > 0 && wk_y < 8)       //만약 백색 킹이 가장 아래 구석을 제외한 자리에 있다면
            wking_check = 3;

        if (wk_y == 7)      //만약 백색 킹이 가장 아래에 오른쪽에 있다면 (오른쪽 하단 구석)
            wking_check = 5;
    }

    else if (wk_y == 0)     //만약 백색 킹이 가장 왼쪽에 있다면
    {
        if (wk_x == 0)      //만약 백색 킹이 가장 왼쪽 위에 있다면 (왼쪽 상단 구석)
            wking_check = 5;

        if (wk_x > 0 && wk_x <8)        //만약 백색 킹이 구석을 제외한 가장 왼쪽에 있다면
            wking_check = 3;

        if (wk_x == 7)      //만약 백색 킹이 가장 왼쪽 아래에 있다면 (왼쪽 하단 구석)
            wking_check = 5;
    }

    else if (wk_y == 7)     //만약 백색 킹이 가장 오른쪽에 있다면
    {
        if (wk_x == 0)      //만약 백색 킹이 가장 오른쪽에 위에 있다면
            wking_check = 5;

        if (wk_x > 0 && wk_x < 8)       //만약 백색 킹이 구석을 제외한 가장 오른쪽에 있다면
            wking_check = 3;

        if (wk_x == 7)      //만약 백색 킹이 가장 오른쪽에 아래에 있다면
            wking_check = 5;
    }

    else if (bk_x == 0)     //흑색 킹의 경우 (백색 킹과 동일)
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

    if (check[wk_x][wk_y] == 1 && order%2 == 0)     //만약 백의 차례이고 박색 킹이 공격받고 있다면 (wking_check변수를 증가시켜 킹의 8개 모든 방향이 1이면 체크메이트로 판단)
    {
        if (wk_x+1 <= 7)     //만약 백색 킹의 뒤가
        {
            if (wk_y+1 <= 7)     //만약 백색 킹의 뒤에 오른쪽에
                if (check[wk_x+1][wk_y+1] == 1)     //체크 변수가 1이라면
                    wking_check++;      //체크 변수 켜짐

            if (wk_y-1 >= 0)    //만약 백색 킹의 뒤에 왼쪽에
                if (check[wk_x+1][wk_y-1] == 1)
                    wking_check++;

            if (check[wk_x+1][wk_y] == 1)       //만약 백색 킹의 뒤가
                    wking_check++;
        }

        if (wk_x-1 >= 0)        //만약 백색 킹의 앞이
        {
            if (wk_y+1 <= 7)        //만약 백색 킹의 앞에 오른쪽이
                if (check[wk_x-1][wk_y+1] == 1)
                    wking_check++;

            if (wk_y-1 >= 0)        //만약 백색 킹의 앞에 왼쪽이
                if (check[wk_x-1][wk_y-1] == 1)
                    wking_check++;

            if (check[wk_x-1][wk_y] == 1)       //만약 백색 킹의 앞이
                wking_check++;
        }
        if (wk_y+1 <= 7)        //만약 백색 킹의 오른쪽이
            if (check[wk_x][wk_y+1] == 1)
                wking_check++;

        if (wk_y-1 >= 0)        //만약 백색 킹의 왼쪽이
            if (check[wk_x][wk_y-1] == 1)
                wking_check++;

        if (wking_check == 8)       //만약 백색 킹의 모든 방향이 차단되었으면 (8곳이 체크가 1이면)
            WhiteCheckmate_Alert();     //백색 체크메이트 알림

        else
            WhiteCheck_Alert();     //아니면 백색 체크 알림 호출
    }

    if (check[wk_x][wk_y] == 0 && wmv_cnt == 0)     //만약 백색 킹이 공격받지 않고 나머지 기물들이 움직일 수 없는 상황이면 (스테일메이트)
    {
        if (wk_x+1 <=7)     //체크메이트와 동일한 메커니즘
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

        if (wking_check == 8)       //만약 킹이 아무 곳도 움직일 수 없다면
            Stalemate_Alert();      //스테일메이트 알림
    }

    if (check[bk_x][bk_y] == 1 && order%2 != 0)     //만약 흑색 킹이 공격 받고 있고 흑의 차례라면 (백색 킹과 체크메이트 알고리즘 동일)
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

    if (check[bk_x][bk_y] == 0 && bmv_cnt == 0)     //만약 흑색 킹이 공격받고 있지 않고 나머지 기물들이 움직일 수 없을 때
    {
        if (bk_x+1 <= 7)        //백색 킹의 스테일메이트와 알고리즘 동일
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

        if (bking_check == 8)       //만약 흑색 킹이 아무 곳도 움직일 수 없을 때
            Stalemate_Alert();      //스테일메이트 알림
    }
}
