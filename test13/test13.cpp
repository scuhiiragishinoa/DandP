#include <graphics.h>  
#include <conio.h>
#include "EasyXPng.h"
#define  WIDTH 800  
#define  HEIGHT 600

class Player
{
public:
	IMAGE im_show;
	float width, height;

	void draw()	
	{
		putimagePng(WIDTH / 2, HEIGHT / 2 - height, &im_show);
	}

	void initialize()
	{
		loadimage(&im_show, _T("standright.png"));
		width = im_show.getwidth();
		height = im_show.getheight();
	}
};

IMAGE im_land;
IMAGE im_bk;
Player player;

void startup()
{
	player.initialize();
	loadimage(&im_land, _T("land.png"));
	loadimage(&im_bk, _T("landscape1.png"));

	initgraph(WIDTH, HEIGHT);
	BeginBatchDraw();
}

void show()
{
	putimage(-100, -100, &im_bk);
	putimage(WIDTH / 2, HEIGHT / 2, &im_land);
	player.draw();
	FlushBatchDraw();
}

void updateWithoutInput()
{

}

void updateWithInput()
{
	if (kbhit())
	{

	}
}
	}

int main()
{
	startup();
	while (1)
	{
		show();
		updateWithoutInput();
		updateWithInput();
	}
	return 0;
}
