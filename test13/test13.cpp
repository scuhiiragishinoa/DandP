
#include <stdio.h>
#include <graphics.h>
#include "conio.h"
#include "EasyXPng.h"

#define WIDTH  640
#define HEIGHT 400

const int rows = 10, cols = 10;

int rowNum, colNum;


int mapIndex[rows * cols] =
{ 2, 2, 2, 2, 2, 0, 1, 0, 1, 0,
3, 3, 2, 2, 0, 0, 0, 1, 1, 0,
3, 0, 0, 0, 0, 0, 0, 0, 1, 2,
2, 2, 0, 0, 0, 0, 0, 2, 2, 2,
2, 2, 0, 0, 0, 0, 2, 2, 2, 2,
2, 2, 0, 0, 0, 2, 2, 0, 0, 2,
2, 0, 0, 2, 2, 2, 0, 0, 1, 0,
0, 0, 2, 0, 0, 0, 1, 1, 1, 1,
0, 2, 0, 3, 3, 3, 3, 3, 3, 1,
2, 0, 3, 3, 3, 3, 3, 3, 3, 3 };

int sceneIndex[rows * cols] =
{ 0, 2, 2, 0, 2, 0, 1, 0, 1, 1,
0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
0, 0, 1, 0, 1, 0, 0, 0, 2, 0,
2, 2, 0, 0, 1, 0, 0, 0, 0, 2,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 1, 0, 0, 0, 0, 0, 1, 0,
0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


int main()
{
	IMAGE im_bk, map[4], scene[2];
	initgraph(WIDTH, HEIGHT);

	int i, x, y;

	int xstart, ystart;
	xstart = 32 * (rows - 1);
	ystart = 0;

	loadimage(&im_bk, _T("sources\\Map\\bg.bmp"));
	loadimage(&map[0], _T("sources\\Map\\map0.png"));
	loadimage(&map[1], _T("sources\\Map\\map1.png"));
	loadimage(&map[2], _T("sources\\Map\\map2.png"));
	loadimage(&map[3], _T("sources\\Map\\map3.png"));

	loadimage(&scene[0], _T("sources\\Map\\scene1.png"));
	loadimage(&scene[1], _T("sources\\Map\\scene2.png"));


	putimage(0, 0, &im_bk);

	for (i = 0; i < rows * cols; i++)
	{

		rowNum = i / cols;
		colNum = i % cols;
		x = xstart + colNum * 32 + rowNum * (-32);
		y = ystart + rowNum * 16 + colNum * 16;

		putimagePng(x, y, &map[mapIndex[i]]);

		if (sceneIndex[i] == 1)
			putimagePng(x + 7, y - 44, &scene[0]);
		else if (sceneIndex[i] == 2)
			putimagePng(x + 7, y - 30, &scene[1]);

	}

	_getch();
	return 0;
}

