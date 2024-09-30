#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <iostream>
#include <time.h>
#include <list>

using namespace std;
#pragma comment(lib,"winmm.lib")
#pragma warning(disable:4996);

// size of the screen
#define WIN_WIDTH	400
#define	WIN_HEIGHT	600

// define the maximum of enemy planes and bullets
#define MAX_ENEMY  50
#define MAX_BULLET 100

// define bullet
class Node {
public:
	int x;
	int y;
};

/*
1, We use a rectangle to represent an enemy plane (50*30), where the initial x coordinate is randomly generated, it is moving downwards.
2, Our plane is designed as 80*60, which is controlled by the keyboard
3, Currently, only our plane fire shots
*/

Node myPlane = { 200, 590 };

//Store the enemey planes and bullets using arrays
Node enemyPlanes[MAX_ENEMY];
Node bullets[MAX_BULLET];
bool enemyActive[MAX_ENEMY] = { false };   // active state of enemy plane
bool bulletActive[MAX_BULLET] = { false }; // active state of bullets

//	detection of keyboard signal
void IsPressKey()
{
	if (_kbhit())			//	if any key is pressed
	{
		char key;
		key = getch();		//	keyboard info

		switch (key)
		{
		case 72:			//	up
			myPlane.y -= 8;
			//			
			//			Sleep(1500);
			break;
		case 80:			//	down
			myPlane.y += 8;
			break;
		case 75:			//	left
			myPlane.x -= 6;
			break;
		case 77:			//	right
			myPlane.x += 6;
			break;
		default:
			break;
		}
	}
}


//	collision detection
void Shoot() {
	for (int i = 0; i < MAX_ENEMY; ++i)
	{
		if (!enemyActive[i]) continue; //skip the inactive enemy planes
		for (int j = 0; j < MAX_BULLET; ++j)
		{
			if (!bulletActive[j]) continue; //skip the inactive enemy bullets
			// collision detection
			if (bullets[j].x >= (enemyPlanes[i].x - 10) &&
				bullets[j].x <= (enemyPlanes[i].x + 50) &&
				bullets[j].y >= (enemyPlanes[i].y - 15) &&
				bullets[j].y <= (enemyPlanes[i].y + 30))
			{
				// both the enemy planes and bullets disappear after collision
				enemyActive[i] = false;
				bulletActive[j] = false;
				break;
			}
		}
	}
}

//	check if they move out of the screen
void CheckEnemyBullet() {

	for (int i = 0; i < MAX_ENEMY; ++i)
	{
		if (enemyActive[i] && enemyPlanes[i].y >= WIN_HEIGHT)
			enemyActive[i] = false;
	}

	for (int j = 0; j < MAX_BULLET; ++j)
	{
		if (bulletActive[j] && bullets[j].y <= -10)
			bulletActive[j] = false;
	}
}

// create new enemy planes
void AddEnemy() {
	for (int i = 0; i < MAX_ENEMY; ++i)
	{
		if (!enemyActive[i])
		{ //find inactive enemy planes
			enemyPlanes[i] = { rand() % (WIN_WIDTH - 50), 0 };
			enemyActive[i] = true;
			break;
		}
	}
}

// shoot new bullets
void AddBullet() {
	for (int i = 0; i < MAX_BULLET; ++i)
	{
		if (!bulletActive[i]) {
			bullets[i] = { myPlane.x + 16, myPlane.y - 20 };
			bulletActive[i] = true;
			break;
		}
	}
}

int main()
{
	// 
	srand((unsigned int)time(NULL));
	//	
	initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);
	SetCursorPos(-100, -100);

	DWORD t1, t2;			//	speed of enemy planes
	DWORD tt1, tt2;			//	speed of bullets

	t1 = GetTickCount();			//	initial time of plane
	tt1 = GetTickCount();			//	initial time of bullet

	while (1)
	{
		//	add one more enemy plane every 1000ms
		t2 = GetTickCount();
		if (t2 - t1 >= 1000) {
			AddEnemy();
			t1 = t2;
		}

		//	add one more bullet every 500ms
		tt2 = GetTickCount();
		if (tt2 - tt1 >= 500) {
			AddBullet();
			tt1 = tt2;
		}

		BeginBatchDraw();		  // Begin batch draw
		cleardevice();			//

		//
		rectangle(myPlane.x, myPlane.y, myPlane.x + 32, myPlane.y + 18);

		//show bullet
		for (int i = 0; i < MAX_BULLET; ++i)
		{
			if (bulletActive[i])
			{
				circle(bullets[i].x, bullets[i].y, 5);
				bullets[i].y--;
			}
		}

		//show enemy plane
		for (int i = 0; i < MAX_ENEMY; ++i)
		{
			if (enemyActive[i])
			{
				roundrect(enemyPlanes[i].x, enemyPlanes[i].y, enemyPlanes[i].x + 30, enemyPlanes[i].y + 20, 5, 5);
				enemyPlanes[i].y++;
			}
		}
		Sleep(5);
		Shoot();
		CheckEnemyBullet();
		EndBatchDraw();
		IsPressKey();
	}

	getchar();

	return 0;
}