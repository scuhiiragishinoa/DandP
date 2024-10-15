#include <ctime>
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include "EasyxPng.h"

#define WIDTH 720
#define HEIGHT 720

IMAGE im_bk, im_g[7];
int i = 0;

void startup()                                 //  
{
	srand(time(0));
	loadimage(&im_bk, _T("resource\\background.png"));
	loadimage(&im_g[0], _T("resource\\girl0.png"));
	loadimage(&im_g[1], _T("resource\\girl1.png"));
	loadimage(&im_g[2], _T("resource\\girl2.png"));
	loadimage(&im_g[3], _T("resource\\girl3.png"));
	loadimage(&im_g[4], _T("resource\\girl4.png"));
	loadimage(&im_g[5], _T("resource\\girl5.png"));
	loadimage(&im_g[6], _T("resource\\girl6.png"));

	initgraph(WIDTH, HEIGHT);                  // 
	BeginBatchDraw();                          // 
}

void show()                             // 
{
	putimage(0, 0, &im_bk);
	putimagePng(WIDTH / 2 - 300, HEIGHT / 2 - 460, &im_g[i]);
	FlushBatchDraw(); // 
	FlushBatchDraw();                  // 
	Sleep(60);                         // 
}

int main()                      // 
{
	startup();                  // 
	while (1)                   // 
	{
		if (i != 6)
		{
			show();
			i++;
		}
		else
		{
			i = 0;
			continue;
		}
	}
	return 0;
}