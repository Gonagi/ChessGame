#include <stdio.h>
#include <windows.h>

void main()
{
        int time = 10;
        int min;
        int sec;

        while(1)
        {
                min = time / 60;
                sec = time % 60;
                printf("�����ð� = %2d:%2d\n", min, sec);
                Sleep(1000);
                time--;
                if(time == 0)
                {
                        printf("�ð��ʰ�");
                        break;
                }
        }
}
