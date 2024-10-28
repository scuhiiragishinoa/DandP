#include <stdio.h>
#include <graphics.h>
#include "conio.h"
#include "EasyXPng.h"

#define WIDTH  640
#define HEIGHT 480

int cnt;
int x = 0, y = 0, vx = 5;
int i = 0;
int direction = 0;
int main()
{
	IMAGE im_bk;
	initgraph(WIDTH, HEIGHT);

	TCHAR filename[20];
	IMAGE left[8];
	IMAGE right[8];
	IMAGE up[8];
	IMAGE down[8];
	for (i = 0; i < 8; i++) {
		_stprintf_s(filename, _T("grd.png"), i);
		loadimage(&down[i], filename);
		_stprintf_s(filename, _T("gud.png"), i);
		loadimage(&up[i], filename);
		_stprintf_s(filename, _T("gld.png"), i);
		loadimage(&left[i], filename);
		_stprintf_s(filename, _T("grr.png"), i);
		loadimage(&right[i], filename);

	}
	i = 0;
	loadimage(&im_bk, _T("grass.png"));

	BeginBatchDraw();
	while (1) {
		putimage(0, 0, &im_bk);
		if (i > 7) i = 0;
		if (direction == 0) putimagePng(x, y, &up[i]);
		if (direction == 1) putimagePng(x, y, &down[i]);
		if (direction == 2) putimagePng(x, y, &left[i]);
		if (direction == 3) putimagePng(x, y, &right[i]);
		if (GetAsyncKeyState(VK_UP) && y > 0) {
			direction = 0;
			y -= 5;
		}
		if (GetAsyncKeyState(VK_DOWN) && y < 400) {
			direction = 1;
			y += 5;
		}
		if (GetAsyncKeyState(VK_LEFT) && x > 0) {
			direction = 2;
			x -= 5;
		}
		if (GetAsyncKeyState(VK_RIGHT) && x < 585) {
			direction = 3;
			x += 5;
		}
		i++;
		Sleep(65);


		FlushBatchDraw();
	}

	_getch();
	return 0;
}