#include <ctime>
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include "EasyxPng.h"

#define WIDTH 560
#define HEIGHT 800
#define	 MaxBulletNum 200


class Rocket  // define rocket
{
public:
	IMAGE im_rocket;             // rocket image
	float x, y;                  // coordinate
	float width, height;         // size

	void draw()                  // 
	{
		putimagePng(x - width / 2, y - height / 2, &im_rocket);
		// 
	}

	void update(float mx, float my) // 
	{
		x = mx;
		y = my;
	}
};

class Bullet {
public:
	IMAGE im_bullet;
	float x, y;
	float vx, vy;
	float radius;

	void draw() {
		putimagePng(x - radius, y - radius, &im_bullet);
	}

	void update() {
		x += vx;
		y += vy;
		if (x <= 0 || x >= WIDTH) vx = -vx;
		if (y <= 0 || y >= HEIGHT) vy = -vy;
	}
};

IMAGE im_bk, im_bullet, im_rocket;
Bullet bullet[MaxBulletNum];
Rocket rocket;

int bulletNum = 0;                             // 

void startup()                                 //  
{
	srand(time(0));
	loadimage(&im_bk, _T("resource\\background.png"));
	loadimage(&im_bullet, _T("resource\\bullet.png"));
	loadimage(&im_rocket, _T("resource\\rocket.png"));


	rocket.im_rocket = im_rocket;  
	rocket.width = im_rocket.getwidth(); // 
	rocket.height = im_rocket.getheight(); // 


	initgraph(WIDTH, HEIGHT);                  // 
	BeginBatchDraw();                          // 
}

void show()                             // 
{
	putimage(0, 0, &im_bk);

	rocket.draw();  // 
	FlushBatchDraw(); // 

	for (int i = 0; i < bulletNum; i++)
		bullet[i].draw();               // 

	FlushBatchDraw();                  // 
	Sleep(10);                         // 
}

void updateWithoutInput()              // 
{
	MOUSEMSG m;		                 // 
	while (MouseHit())               // 
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_MOUSEMOVE)  // 		
			rocket.update(m.x, m.y); // 
	}

	static int lastSecond = 0;         // 
	static int nowSecond = 0;          // 
	static clock_t start = clock();    // 
	clock_t now = clock();             // 
	// 
	nowSecond = (int(now - start) / CLOCKS_PER_SEC);

	if (nowSecond == lastSecond + 2)   // 
	{
		lastSecond = nowSecond;        // 
		// 
		if (bulletNum < MaxBulletNum)
		{
			bullet[bulletNum].x = WIDTH / 2; //
			bullet[bulletNum].y = 10;
			float angle = (rand() / double(RAND_MAX) - 0.5) * 0.9 * PI;
			float scalar = 2 * rand() / double(RAND_MAX) + 2;
			bullet[bulletNum].vx = scalar * sin(angle); //
			bullet[bulletNum].vy = scalar * cos(angle);
			bullet[bulletNum].im_bullet = im_bullet;  // 
			bullet[bulletNum].radius = im_bullet.getwidth() / 2; // 
		}
		bulletNum++; // create a new bullet
	}

	for (int i = 0; i < bulletNum; i++) // for all bullets
		bullet[i].update(); // update their positions and speed	
}

int main()                      // 
{
	startup();                  // 
	while (1)                   // 
	{
		show();                // 
		updateWithoutInput();  // 
	}
	return 0;
}