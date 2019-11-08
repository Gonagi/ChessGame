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
#define mainX 24
#define mainY 10
#define WHITE 'W'
#define BLACK 'B'
#define NONE 'N'

void titleDraw();   //타이틀 화면 출력
void gotoxy(int, int);  //콘솔내 좌표 이동
void cursorDraw(int, int);  //커서 '>' 출력
void erase(int, int);   //해당 좌표 지우기
void chessDraw();   //체스판 출력
void chessRule();   //체스 규칙 설명내용 출력
void controlRule(); //조작 설명내용 출력
void textcolor(int);    //콘솔내 글씨 색깔 변경
void backcolor(int);    //콘솔내 바탕 색깔 변경
void pieceNowDraw();    //선택된 말 표시
void pieceDeadDraw();   //죽은 말 표시
void pieceMove(int, int);   //말 이동 함수
void gameInit();    //게임 초기화
void wrongMove_Alert(); //잘못된 움직임 경고
void wrongOrder_Alert();    //순서 경고
void orderDraw();   //순서 표시
void gameCheck();   //게임 내 체크 사항 확인
void sooDraw();     //몇 수인지 표시
void CursorView(char show); //커서 표시 함수
//void timerDraw();

int cursor_x = mainX, cursor_y = mainY;
int sel_x, sel_y;
char sel_color=NONE;
int pieceSel_check=0;
int order=0;    //짝수면 백, 홀수면 흑
int check_error=0;
int check_gameend=0;
int soo=1;
int check_rule=0;
int bk_live=0, wk_live=0;
int bq_live=0, wq_live=0;
int br_live=0, wr_live=0;
int bn_live=0, wn_live=0;
int bb_live=0, wb_live=0;
int bp_live=0, wp_live=0;

enum color {red=12, yellow=14, white=7, grey=8};
char *chessPos[8][8] = {{"BR","BN","BB","BQ","BK","BB","BN","BR"},
                        {"BP","BP","BP","BP","BP","BP","BP","BP"},
                        {"  ","  ","  ","  ","  ","  ","  ","  "},
                        {"  ","  ","  ","  ","  ","  ","  ","  "},
                        {"  ","  ","  ","  ","  ","  ","  ","  "},
                        {"  ","  ","  ","  ","  ","  ","  ","  "},
                        {"WP","WP","WP","WP","WP","WP","WP","WP"},
                        {"WR","WN","WB","WQ","WK","WB","WN","WR"}};

int main()
{
    system("mode con cols=67 lines=18");
Main:
    CursorView(0);
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
Rule:
			system("cls");
			cursor_x=18,cursor_y=6;
			check_rule=0;
			gotoxy(18,4);
			printf("어떤 설명을 들으시겠습니까?");
			gotoxy(20,6);
			printf("게임 조작 규칙");
			gotoxy(20,7);
			printf("체스 규칙");
			textcolor(grey);
			gotoxy(0,16);
            printf("메인화면으로 가기 (Backspace)");
            textcolor(white);
			cursorDraw(cursor_x,cursor_y);
			while (1) {
				key = _getch();

				if(cursor_y==6&&key==ENTER)
                {
                    system("cls");
                    controlRule();
                }
                else if(cursor_y==7&&key==ENTER)
                {
                    system("cls");
                    chessRule();
                }
				switch (key)
				{
                case UP:
                    if (cursor_y == 6)
                        continue;
                    else if(check_rule==1)
                        continue;
                    cursor_y--;
                    erase(cursor_x, cursor_y + 1);
                    cursorDraw(cursor_x, cursor_y);
                    break;

                case DOWN:
                if (cursor_y == 7)
                    continue;
                else if(check_rule==1)
                        continue;
                else
                    cursor_y++;
                    erase(cursor_x, cursor_y - 1);
                    cursorDraw(cursor_x, cursor_y);
                break;
				case BACK:
				    if(check_rule==1)
                        goto Rule;
				    cursor_x=mainX,cursor_y=mainY+1;
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
			gotoxy(0,20);
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

	gameInit();
	gameCheck();
	while (1)
	{
	    CursorView(0);
	    gameCheck();
	    pieceDeadDraw();
	    orderDraw();
        sooDraw();
        CursorView(1);
        
	    //gotoxy(20,6);
	    //printf("Test %d %d %d %c",pieceSel_check,sel_x,sel_y,sel_color);  //변수값 확인용 (지울 것)
        gotoxy(cursor_x,cursor_y);

	    int y_pos=cursor_x/2;
        int x_pos=cursor_y;
		int key;
		key = _getch();

		if(check_error==1)
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
			cursor_x+=2;
			gotoxy(cursor_x, cursor_y);
			break;
		case BACK:
			cursor_x = mainX, cursor_y = mainY;
			gameInit();
			cursor_x=mainX, cursor_y=mainY;
			goto Main;
			break;
        case SPACE:
            if(pieceSel_check==0&&chessPos[x_pos][y_pos]!="  ")
        {
            if(chessPos[x_pos][y_pos][0]=='B')
                sel_color=BLACK;
            else
                sel_color=WHITE;

            if((order%2==0&&sel_color==BLACK)||(order%2!=0&&sel_color==WHITE))
            {
                wrongOrder_Alert();
                gotoxy(20,2);
                printf("현재 고른 말 : 없음       ");
            }
            else{
            pieceNowDraw();
            gotoxy(1,10);
            printf("                        ");
            pieceSel_check=1;
            sel_x=x_pos, sel_y=y_pos;
            }
        }
        else if(pieceSel_check==0&&chessPos[x_pos][y_pos]=="  ")
        {
            gotoxy(1,10);
            printf("                        ");
            gotoxy(cursor_x,cursor_y);
            pieceSel_check=0;
            pieceNowDraw();
        }
        else if(pieceSel_check==1&&chessPos[x_pos][y_pos]!="  ")
        {
            if (sel_color!=chessPos[x_pos][y_pos][0])
            {
                pieceMove(cursor_x,cursor_y);
            }
            else if(sel_color==chessPos[x_pos][y_pos][0]&&(sel_x!=x_pos||sel_y!=y_pos))
            {
                wrongMove_Alert();
                gotoxy(20,2);
                printf("현재 고른 말 : 없음       ");
                pieceSel_check=0;
            }
            else if(sel_color==chessPos[x_pos][y_pos][0]&&sel_x==x_pos&&sel_y==y_pos)
            {
                gotoxy(1,10);
                printf("                        ");
                gotoxy(20,2);
                printf("현재 고른 말 : 없음       ");
                gotoxy(cursor_x,cursor_y);
                pieceSel_check=0;
            }
        }
        else if(pieceSel_check==1&&chessPos[x_pos][y_pos]=="  ")
        {
            gotoxy(1,10);
            printf("                        ");
            gotoxy(cursor_x,cursor_y);
            pieceMove(cursor_x,cursor_y);
        }
		default:
		    continue;
		}
		//pieceNowDraw();
	}
	gotoxy(0,20);
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
            if(j%2==0&&i%2==0)
                backcolor(112);
            else if(j%2!=0&&i%2==0)
                backcolor(128);
            else if(j%2==0&&i%2!=0)
                backcolor(128);
            else
                backcolor(112);

            if(chessPos[i][j]=="BR")
                printf("■");
            else if(chessPos[i][j]=="BN")
                printf("◆");
            else if(chessPos[i][j]=="BB")
                printf("♠");
            else if(chessPos[i][j]=="BQ")
                printf("♥");
            else if(chessPos[i][j]=="BK")
                printf("★");
            else if(chessPos[i][j]=="BP")
                printf("●");
            else if(chessPos[i][j]=="WR")
                printf("□");
            else if(chessPos[i][j]=="WN")
                printf("◇");
            else if(chessPos[i][j]=="WB")
                printf("♤");
            else if(chessPos[i][j]=="WQ")
                printf("♡");
            else if(chessPos[i][j]=="WK")
                printf("☆");
            else if(chessPos[i][j]=="WP")
                printf("○");
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
        textcolor(white);
    }

	backcolor(7);
	textcolor(yellow);
	printf("\n");
	printf(" a b c d e f g h\n");
	textcolor(white);
	printf("------------경고창---------------------\n");
	printf("\n");
	printf("---------------------------------------\n");
	printf(" ■ : 룩, ◆ : 나이트, ♠ : 비숍\n");
	printf(" ♥ : 퀸, ★ : 킹, ● : 폰\n");
	textcolor(grey);
	gotoxy(0,16);
	printf("메인화면으로 돌아가기 (Backspace)\n");
	textcolor(white);

	for(i=0; i<14; i++)
    {
        gotoxy(49,i);
        printf("|");
    }
}

void chessRule()
{
    check_rule=1;
    textcolor(white);
	printf("----------------게임 방법---------------\n");
	printf("1. 백과 흑이 번갈아가며 플레이합니다.\n");
	printf("2. 백이 먼저 게임을 시작합니다.\n");
	printf("3.  \n");
	printf("4.  \n");
	printf("\n");
	textcolor(grey);
	gotoxy(0,16);
	printf("돌아가기 (Backspace)");
	textcolor(white);
}

void controlRule()
{
    check_rule=1;
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
    gotoxy(0,16);
	printf("돌아가기 (Backspace)");
	textcolor(white);
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

void pieceNowDraw(int x, int y)
{
    int y_pos=x/2;
    int x_pos=y;
    gotoxy(20, 2);
    printf("                         ");
    gotoxy(20,2);
    if(chessPos[x_pos][y_pos]=="BR")
        printf("현재 고른 말 : 흑색 룩");
    else if(chessPos[x_pos][y_pos]=="BN")
        printf("현재 고른 말 : 흑색 나이트");
    else if(chessPos[x_pos][y_pos]=="BB")
        printf("현재 고른 말 : 흑색 비숍");
    else if(chessPos[x_pos][y_pos]=="BQ")
        printf("현재 고른 말 : 흑색 퀸");
    else if(chessPos[x_pos][y_pos]=="BK")
        printf("현재 고른 말 : 흑색 킹");
    else if(chessPos[x_pos][y_pos]=="BP")
        printf("현재 고른 말 : 흑색 폰");
    else if(chessPos[x_pos][y_pos]=="WR")
        printf("현재 고른 말 : 백색 룩");
    else if(chessPos[x_pos][y_pos]=="WN")
        printf("현재 고른 말 : 백색 나이트");
    else if(chessPos[x_pos][y_pos]=="WB")
        printf("현재 고른 말 : 백색 비숍");
    else if(chessPos[x_pos][y_pos]=="WQ")
        printf("현재 고른 말 : 백색 퀸");
    else if(chessPos[x_pos][y_pos]=="WK")
        printf("현재 고른 말 : 백색 킹");
    else if(chessPos[x_pos][y_pos]=="WP")
        printf("현재 고른 말 : 백색 폰");
    else
        printf("현재 고른 말 : 없음");

    gotoxy(cursor_x,cursor_y);
}

void pieceDeadDraw()
{
    int i;

    gotoxy(55,0);
    printf("죽은 말");
    gotoxy(50,1);
    printf("------백팀------");
    gotoxy(51,2);
    for(i=0; i<8-wp_live; i++)
        printf("●");
    gotoxy(51,3);
    for(i=0; i<2-wr_live; i++)
        printf("■");
    gotoxy(56,3);
    for(i=0; i<2-wn_live; i++)
        printf("◆");
    gotoxy(61,3);
    for(i=0; i<2-wb_live; i++)
        printf("♠");
    gotoxy(51,4);
    if(wq_live==0)
         printf("♥");
    gotoxy(54,4);
    if(wk_live==0)
        printf("★");

    gotoxy(50,7);
    printf("------흑팀------");
    gotoxy(51,8);
    for(i=0; i<8-bp_live; i++)
        printf("○");
    gotoxy(51,9);
    for(i=0; i<2-br_live; i++)
        printf("□");
    gotoxy(56,9);
    for(i=0; i<2-bn_live; i++)
        printf("◇");
    gotoxy(61,9);
    for(i=0; i<2-bb_live; i++)
        printf("♤");
    gotoxy(51,10);
    if(bq_live==0)
        printf("♡");
    gotoxy(54,10);
    if(bk_live==0)
        printf("☆");

}

void orderDraw()
{
    char *order_str;
    if(order%2==0)
        order_str="백";
    else
        order_str="흑";
    gotoxy(20,0);
    printf("현재 차례 : %s",order_str);
    gotoxy(cursor_x,cursor_y);
}

void sooDraw()
{
    gotoxy(20,4);
    printf("제 %d수",soo);
    gotoxy(cursor_x,cursor_y);
}
void gameInit()
{
    int i;
    system("cls");
	chessDraw();
	textcolor(white);
	cursor_x = 4, cursor_y = 4;
	pieceSel_check=0;
	gotoxy(20,2);
	printf("현재 고른 말 : 없음");
	chessPos[0][0]="BR";
	chessPos[0][1]="BN";
	chessPos[0][2]="BB";
	chessPos[0][3]="BQ";
	chessPos[0][4]="BK";
	chessPos[0][5]="BB";
	chessPos[0][6]="BN";
	chessPos[0][7]="BR";
	for(i=0; i<=7; i++)
    {
        chessPos[1][i]="BP";
        chessPos[2][i]="  ";
        chessPos[3][i]="  ";
        chessPos[4][i]="  ";
        chessPos[5][i]="  ";
        chessPos[6][i]="WP";
    }
    chessPos[7][0]="WR";
	chessPos[7][1]="WN";
	chessPos[7][2]="WB";
	chessPos[7][3]="WQ";
	chessPos[7][4]="WK";
	chessPos[7][5]="WB";
	chessPos[7][6]="WN";
	chessPos[7][7]="WR";

	gotoxy(cursor_x, cursor_y);
}

void gameCheck()
{
    bk_live=0, wk_live=0;
    bq_live=0, wq_live=0;
    br_live=0, wr_live=0;
    bn_live=0, wn_live=0;
    bb_live=0, wb_live=0;
    bp_live=0, wp_live=0;
    int i,j;

    for(i=0; i<8; i++)
        for(j=0; j<8; j++)
    {
        if(chessPos[i][j]=="WK")
            wk_live++;
        else if(chessPos[i][j]=="BK")
            bk_live++;
        else if(chessPos[i][j]=="WQ")
            wq_live++;
        else if(chessPos[i][j]=="BQ")
            bq_live++;
        else if(chessPos[i][j]=="WR")
            wr_live++;
        else if(chessPos[i][j]=="BR")
            br_live++;
        else if(chessPos[i][j]=="WN")
            wn_live++;
        else if(chessPos[i][j]=="BN")
            bn_live++;
        else if(chessPos[i][j]=="WB")
            wb_live++;
        else if(chessPos[i][j]=="BB")
            bb_live++;
        else if(chessPos[i][j]=="WP")
            wp_live++;
        else if(chessPos[i][j]=="BP")
            bp_live++;
    }

    if(bk_live==0&&wk_live==1)
    {
        gotoxy(1,10);
        printf("                        ");
        gotoxy(1,10);
        textcolor(red);
        printf("백팀 승리!");
        check_gameend=1;
        textcolor(white);
    }
    else if(bk_live==1&&wk_live==0)
    {
        gotoxy(1,10);
        printf("                        ");
        gotoxy(1,10);
        textcolor(red);
        printf("흑팀 승리!");
        check_gameend=1;
        textcolor(white);
    }
    else if(bk_live==0&&wk_live==0)
    {
        gotoxy(1,10);
        printf("                        ");
        gotoxy(1,10);
        textcolor(red);
        printf("치명적 오류 발생");
        check_error=1;
        textcolor(white);
    }
}

void pieceMove(int x,int y)
{
    chessPos[y][x/2]=chessPos[sel_x][sel_y];
    chessPos[sel_x][sel_y]="  ";
    system("cls");
    chessDraw();
    pieceSel_check=0;
    if(order%2!=0)
        soo++;
    order++;
    sel_color=NONE;
    gotoxy(20,2);
	printf("현재 고른 말 : 없음");
}

void wrongMove_Alert()
{
    gotoxy(1,10);
    printf("                        ");
    gotoxy(1,10);
    textcolor(red);
    printf("잘못된 이동입니다.");
    sel_color=NONE;
    textcolor(white);
    gotoxy(cursor_x,cursor_y);
}

void wrongOrder_Alert()
{
    gotoxy(1,10);
    printf("                        ");
    gotoxy(1,10);
    textcolor(red);
    printf("차례가 아닙니다.");
    sel_color=NONE;
    textcolor(white);
    gotoxy(cursor_x,cursor_y);
}

void CursorView(char show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole , &ConsoleCursor);
}
/*void timerDraw()
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
*/
