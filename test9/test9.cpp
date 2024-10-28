#include <stdio.h>
#include <graphics.h>
#include "conio.h"
#include "EasyXPng.h"

#define WIDTH  640
#define HEIGHT 480

int cnt;
int i = 0;
int x = 0, y = 288, vx = 5;
bool direction = false;
int main()
{
	IMAGE im_bk;
	initgraph(WIDTH, HEIGHT);

	TCHAR filename[20];
	IMAGE left[4];
	IMAGE right[4];
	for (i = 0; i < 4; i++)
	{
		_stprintf_s(filename, _T("grd.png"), i);
		loadimage(&left[i], filename);
	}
	int j = 0;
	for (i = 4; i < 8; i++)
	{
		_stprintf_s(filename, _T("grd.png"), i);
		loadimage(&right[j], filename);
		j++;
	}
	i = 0;
	loadimage(&im_bk, _T("bk0.png"));

	BeginBatchDraw();
	while (1) {
		if (i > 3) i = 0;
		putimage(0, 0, &im_bk);
		x += vx;
		if (x > WIDTH - 68 || x < -18) {
			vx = -vx;
			direction = !direction;
		}
		if (direction) {
			putimagePng(x, y, &left[i]);
		}
		if (!direction) {
			putimagePng(x, y, &right[i]);
		}
		Sleep(65);

		i++;

		FlushBatchDraw();
	}

	_getch();
	return 0;
}