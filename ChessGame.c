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
void orderDraw();   //���� ǥ��
void gameCheck();   //���� �� üũ ���� Ȯ��
void sooDraw();     //�� ������ ǥ��
void CursorView(char show); //Ŀ�� ǥ�� �Լ�
//void timerDraw();

int cursor_x = mainX, cursor_y = mainY;
int sel_x, sel_y;
char sel_color=NONE;
int pieceSel_check=0;
int order=0;    //¦���� ��, Ȧ���� ��
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
char *chessPiece[13]={"  ","BR","BN","BB","BQ","BK","BP","WR","WN","WB","WQ","WK","WP"};


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
			printf("� ������ �����ðڽ��ϱ�?");
			gotoxy(20,6);
			printf("���� ���� ��Ģ");
			gotoxy(20,7);
			printf("ü�� ��Ģ");
			textcolor(grey);
			gotoxy(0,16);
            printf("����ȭ������ ���� (Backspace)");
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
			printf("������ �����մϴ�...\n");
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
        CursorView(1);
	    gameCheck();
	    pieceDeadDraw();
	    orderDraw();
        sooDraw();
        CursorView(1);

	    //gotoxy(20,6);
	    //printf("Test %d %d %d %c",pieceSel_check,sel_x,sel_y,sel_color);  //������ Ȯ�ο� (���� ��)
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
            if(pieceSel_check==0&&chessPos[x_pos][y_pos]!=chessPiece[0])
        {
            if(chessPos[x_pos][y_pos][0]=='B')
                sel_color=BLACK;
            else
                sel_color=WHITE;

            if((order%2==0&&sel_color==BLACK)||(order%2!=0&&sel_color==WHITE))
            {
                wrongOrder_Alert();
                gotoxy(20,2);
                printf("���� �� �� : ����       ");
            }
            else{
            pieceNowDraw();
            gotoxy(1,10);
            printf("                        ");
            pieceSel_check=1;
            sel_x=x_pos, sel_y=y_pos;
            }
        }
        else if(pieceSel_check==0&&chessPos[x_pos][y_pos]==chessPiece[0])
        {
            gotoxy(1,10);
            printf("                        ");
            gotoxy(cursor_x,cursor_y);
            pieceSel_check=0;
            pieceNowDraw();
        }
        else if(pieceSel_check==1&&chessPos[x_pos][y_pos]!=chessPiece[0])
        {
            if (sel_color!=chessPos[x_pos][y_pos][0])
            {
                pieceMove(cursor_x,cursor_y);
            }
            else if(sel_color==chessPos[x_pos][y_pos][0]&&(sel_x!=x_pos||sel_y!=y_pos))
            {
                wrongMove_Alert();
                gotoxy(20,2);
                printf("���� �� �� : ����       ");
                pieceSel_check=0;
            }
            else if(sel_color==chessPos[x_pos][y_pos][0]&&sel_x==x_pos&&sel_y==y_pos)
            {
                gotoxy(1,10);
                printf("                        ");
                gotoxy(20,2);
                printf("���� �� �� : ����       ");
                gotoxy(cursor_x,cursor_y);
                pieceSel_check=0;
            }
        }
        else if(pieceSel_check==1&&chessPos[x_pos][y_pos]==chessPiece[0])
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
            if(j%2==0&&i%2==0)
                backcolor(112);
            else if(j%2!=0&&i%2==0)
                backcolor(128);
            else if(j%2==0&&i%2!=0)
                backcolor(128);
            else
                backcolor(112);

            if(chessPos[i][j]==chessPiece[1])
                printf("��");
            else if(chessPos[i][j]==chessPiece[2])
                printf("��");
            else if(chessPos[i][j]==chessPiece[3])
                printf("��");
            else if(chessPos[i][j]==chessPiece[4])
                printf("��");
            else if(chessPos[i][j]==chessPiece[5])
                printf("��");
            else if(chessPos[i][j]==chessPiece[6])
                printf("��");
            else if(chessPos[i][j]==chessPiece[7])
                printf("��");
            else if(chessPos[i][j]==chessPiece[8])
                printf("��");
            else if(chessPos[i][j]==chessPiece[9])
                printf("��");
            else if(chessPos[i][j]==chessPiece[10])
                printf("��");
            else if(chessPos[i][j]==chessPiece[11])
                printf("��");
            else if(chessPos[i][j]==chessPiece[12])
                printf("��");
            else if(chessPos[i][j]==chessPiece[0])
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
	printf("------------���â---------------------\n");
	printf("\n");
	printf("---------------------------------------\n");
	printf(" �� : ��, �� : ����Ʈ, �� : ���\n");
	printf(" �� : ��, �� : ŷ, �� : ��\n");
	textcolor(grey);
	gotoxy(0,16);
	printf("����ȭ������ ���ư��� (Backspace)\n");
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
	printf("----------------���� ���---------------\n");
	printf("1. ��� ���� �����ư��� �÷����մϴ�.\n");
	printf("2. ���� ���� ������ �����մϴ�.\n");
	printf("3.  \n");
	printf("4.  \n");
	printf("\n");
	textcolor(grey);
	gotoxy(0,16);
	printf("���ư��� (Backspace)");
	textcolor(white);
}

void controlRule()
{
    check_rule=1;
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
    gotoxy(0,16);
	printf("���ư��� (Backspace)");
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
    if(chessPos[x_pos][y_pos]==chessPiece[1])
        printf("���� �� �� : ��� ��");
    else if(chessPos[x_pos][y_pos]==chessPiece[2])
        printf("���� �� �� : ��� ����Ʈ");
    else if(chessPos[x_pos][y_pos]==chessPiece[3])
        printf("���� �� �� : ��� ���");
    else if(chessPos[x_pos][y_pos]==chessPiece[4])
        printf("���� �� �� : ��� ��");
    else if(chessPos[x_pos][y_pos]==chessPiece[5])
        printf("���� �� �� : ��� ŷ");
    else if(chessPos[x_pos][y_pos]==chessPiece[6])
        printf("���� �� �� : ��� ��");
    else if(chessPos[x_pos][y_pos]==chessPiece[7])
        printf("���� �� �� : ��� ��");
    else if(chessPos[x_pos][y_pos]==chessPiece[8])
        printf("���� �� �� : ��� ����Ʈ");
    else if(chessPos[x_pos][y_pos]==chessPiece[9])
        printf("���� �� �� : ��� ���");
    else if(chessPos[x_pos][y_pos]==chessPiece[10])
        printf("���� �� �� : ��� ��");
    else if(chessPos[x_pos][y_pos]==chessPiece[11])
        printf("���� �� �� : ��� ŷ");
    else if(chessPos[x_pos][y_pos]==chessPiece[12])
        printf("���� �� �� : ��� ��");
    else
        printf("���� �� �� : ����");

    gotoxy(cursor_x,cursor_y);
}

void pieceDeadDraw()
{
    int i;

    gotoxy(55,0);
    printf("���� ��");
    gotoxy(50,1);
    printf("------����------");
    gotoxy(51,2);
    for(i=0; i<8-wp_live; i++)
        printf("��");
    gotoxy(51,3);
    for(i=0; i<2-wr_live; i++)
        printf("��");
    gotoxy(56,3);
    for(i=0; i<2-wn_live; i++)
        printf("��");
    gotoxy(61,3);
    for(i=0; i<2-wb_live; i++)
        printf("��");
    gotoxy(51,4);
    if(wq_live==0)
         printf("��");
    gotoxy(54,4);
    if(wk_live==0)
        printf("��");

    gotoxy(50,7);
    printf("------����------");
    gotoxy(51,8);
    for(i=0; i<8-bp_live; i++)
        printf("��");
    gotoxy(51,9);
    for(i=0; i<2-br_live; i++)
        printf("��");
    gotoxy(56,9);
    for(i=0; i<2-bn_live; i++)
        printf("��");
    gotoxy(61,9);
    for(i=0; i<2-bb_live; i++)
        printf("��");
    gotoxy(51,10);
    if(bq_live==0)
        printf("��");
    gotoxy(54,10);
    if(bk_live==0)
        printf("��");

}

void orderDraw()
{
    char *order_str;
    if(order%2==0)
        order_str="��";
    else
        order_str="��";
    gotoxy(20,0);
    printf("���� ���� : %s",order_str);
    gotoxy(cursor_x,cursor_y);
}

void sooDraw()
{
    gotoxy(20,4);
    printf("�� %d��",soo);
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
	printf("���� �� �� : ����");
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
        printf("���� �¸�!");
        check_gameend=1;
        textcolor(white);
    }
    else if(bk_live==1&&wk_live==0)
    {
        gotoxy(1,10);
        printf("                        ");
        gotoxy(1,10);
        textcolor(red);
        printf("���� �¸�!");
        check_gameend=1;
        textcolor(white);
    }
    else if(bk_live==0&&wk_live==0)
    {
        gotoxy(1,10);
        printf("                        ");
        gotoxy(1,10);
        textcolor(red);
        printf("ġ���� ���� �߻�");
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
	printf("���� �� �� : ����");
}

void wrongMove_Alert()
{
    gotoxy(1,10);
    printf("                        ");
    gotoxy(1,10);
    textcolor(red);
    printf("�߸��� �̵��Դϴ�.");
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
    printf("���ʰ� �ƴմϴ�.");
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
        printf("�����ð� = %2d:%2d",min,sec);
        gotoxy(cursor_x,cursor_y);
        Sleep(1000);
        time--;

        if(time<=0)
        {
        system("cls");
        gotoxy(7,4);
        printf("�ð��ʰ�");
        break;
        }
    }
}
*/
