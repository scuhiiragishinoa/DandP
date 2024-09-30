// bubble.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <winuser.h>

//字符串转化
TCHAR* CharToTCHAR(const char* pChar)
{
	TCHAR* pTchar = NULL;
	int nLen = strlen(pChar) + 1;
#ifdef _UNICODE
	pTchar = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, 0, pChar, nLen, pTchar, nLen);
#else
	pTchar = new char[nLen];
	strcpy(pTchar, pChar);
#endif
	return pTchar;
}

int ballX = 0, ballY = 0;
int MonsterX = 0, MonsterY = 0, MonsterRadius = 20;
int windowLength = 850, windowHeight = 850;
int ballRadius = 20;
int i = 0, j = 0;
int speed = 5, speedx = 5, speedy = 5;
bool Box[17][17][2] = {0};
bool IsMonster_verticalmv;
bool bombexist = 0;
int bombtiming = 200;
int bombx = 0, bomby = 0;
int bombRadius = 20;
bool IsBombing = 0;
int bombingValue = 0;
int level = 225;
int bonusRadius = 10;
bool IsGameover = 0;
bool IsGamewin = 0;
bool IsMonsterExist = 1;
double gametime = 1000;



void KeyControl()//键盘控制与位置修正
{
	char userkey = _getch();
	if (GetAsyncKeyState(VK_UP) && (((ballX - 25) % 100 == 0) || ((ballX - 20) % 100 == 0) || ((ballX - 30) % 100 == 0)))   // Up
	{
		if ((ballX - 20) % 100 == 0)ballX += 5;
		if ((ballX - 30) % 100 == 0)ballX -= 5;
		ballY -= speed;
		if (ballY <= ballRadius) ballY = ballRadius;
	}
	if (GetAsyncKeyState(VK_DOWN) && (((ballX - 25) % 100 == 0) || ((ballX - 20) % 100 == 0) || ((ballX - 30) % 100 == 0))) // Down
	{
		if ((ballX - 20) % 100 == 0)ballX += 5;
		if ((ballX - 30) % 100 == 0)ballX -= 5;
		ballY += speed;
		if (ballY >= windowLength - ballRadius) ballY = windowLength - ballRadius;
	}
	if (GetAsyncKeyState(VK_LEFT) && (((ballY - 25) % 100 == 0) || ((ballY - 20) % 100 == 0) || ((ballY - 30) % 100 == 0))) // Left
	{
		if ((ballY - 20) % 100 == 0)ballY += 5;
		if ((ballY - 30) % 100 == 0)ballY -= 5;
		ballX -= speed;
		if (ballX <= ballRadius) ballX = ballRadius;
	}
	if (GetAsyncKeyState(VK_RIGHT) && (((ballY - 25) % 100 == 0) || ((ballY - 20) % 100 == 0) || ((ballY - 30) % 100 == 0))) // Right
	{
		if ((ballY - 20) % 100 == 0)ballY += 5;
		if ((ballY - 30) % 100 == 0)ballY -= 5;
		ballX += speed;
		if (ballX >= windowLength - ballRadius) ballX = windowLength - ballRadius;
	}
	if (GetAsyncKeyState(VK_SPACE) && !IsBombing)
	{
		bombexist = 1;
		bombtiming = 200;
		bombx = ballX;
		bomby = ballY;
	}//bomb position memory
}
void Monster_move() {
	if (IsMonsterExist) {
		if (IsMonster_verticalmv) {
			MonsterY += speedy;
		}
		else {
			MonsterX += speedx;
		}
		if (MonsterX <= MonsterRadius || MonsterX == windowLength - MonsterRadius)speedx = -speedx;
		if (MonsterY <= MonsterRadius || MonsterY == windowLength - MonsterRadius)speedy = -speedy;
	}
	if (!IsMonsterExist) {
		MonsterX = -100;
		MonsterY = -100;
	}
}

void ballCollide() {
	for (i = 0; i < 17; i++) {
		for (j = 0; j < 17; j++) {
			if (Box[i][j][1]) {
				if (ballX + ballRadius == i * 50 + 5 && ballX - ballRadius < i * 50 + 50 && ballY == j * 50 + 25)ballX -= speed;
				if (ballX - ballRadius == i * 50 + 45 && ballX + ballRadius > i * 50 && ballY == j * 50 + 25)ballX += speed;
				if (ballY + ballRadius == j * 50 + 5 && ballY - ballRadius < j * 50 + 50 && ballX == i * 50 + 25)ballY -= speed;
				if (ballY - ballRadius == j * 50 + 45 && ballY + ballRadius > j * 50 && ballX == i * 50 + 25)ballY += speed;
			}
		}
	}
}

void monsterCollide() {
	if (IsMonsterExist) {
		for (i = 0; i < 17; i++) {
			for (j = 0; j < 17; j++) {
				if (Box[i][j][1]) {
					if (MonsterX + MonsterRadius == i * 50 + 5 && MonsterX - MonsterRadius < i * 50 + 50 && MonsterY == j * 50 + 25)speedx = -speedx;
					if (MonsterX - MonsterRadius == i * 50 + 45 && MonsterX + MonsterRadius > i * 50 && MonsterY == j * 50 + 25)speedx = -speedx;
					if (MonsterY + MonsterRadius == j * 50 + 5 && MonsterY - MonsterRadius < j * 50 + 50 && MonsterX == i * 50 + 25)speedy = -speedy;
					if (MonsterY - MonsterRadius == j * 50 + 45 && MonsterY + MonsterRadius > j * 50 && MonsterX == i * 50 + 25)speedy = -speedy;
				}
			}
		}
	}
	
}

void eating() {
	if ((ballX - MonsterX) * (ballX - MonsterX) + (ballY - MonsterY) * (ballY - MonsterY) <= (MonsterRadius + ballRadius) * (MonsterRadius + ballRadius)) {
		IsGameover = 1;
	}
}

void bombLaying() {
	if (bombexist && !IsBombing) {
		bombtiming -= speed;
		if (bombtiming == 0) {
			IsBombing = 1;
		}
	}
}
void bombing() {
	if (IsBombing) {
		bombingValue += speed;

		for (i = 0; i < 17; i++) {
			for (j = 0; j < 17; j++) {
				if (Box[i][j][1]) {
					if (bombx + bombingValue == i * 50 + 5 && bombx - bombingValue < i * 50 + 50 && bomby == j * 50 + 25)Box[i][j][1] = false;
					if (bombx - bombingValue == i * 50 + 45 && bombx + bombingValue > i * 50 && bomby == j * 50 + 25)Box[i][j][1] = false;
					if (bomby + bombingValue == j * 50 + 5 && bomby - bombingValue < j * 50 + 50 && bombx == i * 50 + 25)Box[i][j][1] = false;
					if (bomby - bombingValue == j * 50 + 45 && bomby + bombingValue > j * 50 && bombx == i * 50 + 25)Box[i][j][1] = false;
				}
			}
		}
		if (((ballX - ballRadius < bombx + bombingValue && ballX + ballRadius > bombx - bombingValue) && (ballY - ballRadius < bomby + 25 && ballY + ballRadius > bomby - 25)) || ((ballY - ballRadius < bomby + bombingValue && ballY + ballRadius > bomby - bombingValue)) && (ballX - ballRadius < bombx + 25 && ballX + ballRadius > bombx - 25)) {
			IsGameover = 1;
		}
		if (((MonsterX - MonsterRadius < bombx + bombingValue && MonsterX + MonsterRadius > bombx - bombingValue) && (MonsterY - MonsterRadius < bomby + 25 && MonsterY + MonsterRadius > bomby - 25)) || ((MonsterY - MonsterRadius < bomby + bombingValue && MonsterY + MonsterY > bomby - bombingValue)) && (MonsterX - MonsterRadius < bombx + 25 && MonsterX + MonsterRadius > bombx - 25)) {
			IsMonsterExist = 0;
		}
		if (bombingValue == level) {
			bombx = -20;
			bomby = -20;
			IsBombing = 0;
			bombexist = 0;
			bombingValue = 0;
		}
	}
 }

void DrawMap()
{
	for (i = 0; i < 17; i++) {
		for (j = 0; j < 17; j++) {
			if (i % 2 != 0 && j % 2 != 0) {
				int left = (i) * 50;
				int top = (j) * 50;
				int right = left + 50;
				int bottom = top + 50;
				setfillcolor(RGB(190, 190, 190));
				setlinecolor(RGB(150, 150, 150));
				fillrectangle(left, top, right, bottom);
			}
			else if (Box[i][j][1]) {
				int left_b = (i) * 50 + 5;
				int top_b = (j) * 50 + 5;
				int right_b = left_b + 40;
				int bottom_b = top_b + 40;
				setlinecolor(RGB(50, 50, 50));
				setfillcolor(RGB(50, 50, 50));
				fillrectangle(left_b, top_b, right_b, bottom_b);
			}
		}
	}
}

void DrawBonus() {
	for (i = 0; i < 17; i++) {
		for (j = 0; j < 17; j++) {
			if (Box[i][j][2]) {
				setfillcolor(YELLOW);
				fillcircle(i * 50 + 25, j * 50 + 25, bonusRadius);
			}
		}
	}
}

void CreateBox() {// generate the bricks
	for (i = 0; i < 17; i++) {
		for (j = 0; j < 17; j++) {
			if ((i + j) % 2 != 0 && rand() % 3 == 0) {
				Box[i][j][1] = true;
				if (rand() % 100 == 0) {
					Box[i][j][2] = 1;//bonus
				}
			}
			else Box[i][j][1] = false;
		}
	}
}
void eatBonus() {
	for (i = 0; i < 17; i++) {
		for (j = 0; j < 17; j++) {
			if (Box[i][j][2] && ballX == i * 50 + 25 && ballY == j * 50 + 25) {
				IsGamewin = 1;//效果是赢得游戏，可修改并添加相应方法
			}
		}
	}
}

void timefail() {
	if (gametime == 0) {
		IsGameover = 1;
	}
}

void win() {
	if (!IsMonsterExist) {
		IsGamewin = 1;
		for (i = 0; i < 17; i++) {
			for (j = 0; j < 17; j++) {
				if (Box[i][j][1]) {
					IsGamewin = 0;
					break;
				}
			}
		}
	}
}

int main()
{
	initgraph(windowLength, windowHeight);	// Create screen
	ShowCursor(TRUE);
	setbkcolor(RGB(0, 120, 0));
	cleardevice();

	srand((unsigned int)time(NULL));

	ballX = rand() % 9 * 100 + ballRadius + 5; //  random position
	ballY = rand() % 9 * 100 + ballRadius + 5;

	CreateBox();
	MonsterX = rand() % 9 * 100 + MonsterRadius + 5;
	MonsterY = rand() % 9 * 100 + MonsterRadius + 5;
	IsMonster_verticalmv = rand() % 2 - 1;

	BeginBatchDraw();
	while (true) {
		DrawBonus();
		DrawMap();  //  create map
		gametime -= 0.2;

		if (_kbhit())
			KeyControl();

		ballCollide();
		setfillcolor(RED);
		fillcircle(ballX, ballY, ballRadius);

		Monster_move();
		monsterCollide();
		setfillcolor(BLUE);
		fillcircle(MonsterX, MonsterY, MonsterRadius);

		eating();
		bombLaying();
		bombing();
		setfillcolor(BLACK);
		fillcircle(bombx, bomby, bombRadius);

		setfillcolor(WHITE);
		setlinecolor(WHITE);
		fillrectangle(bombx - bombingValue, bomby + 25, bombx + bombingValue, bomby - 25);
		fillrectangle(bombx - 25, bomby - bombingValue, bombx + 25, bomby + bombingValue);

		eatBonus();

		settextstyle(40, 0, _T("Consolas"));
		settextcolor(RGB(255, 0, 0));
		wchar_t t[20];
		_itow_s(gametime, t, 10);
		outtextxy(10, 10, t);
		timefail();
		win();
		if (IsGameover) {
			settextstyle(40, 0, _T("Consolas"));
			settextcolor(RGB(255, 0, 0));
			outtextxy(400, 400, CharToTCHAR("GameOver"));
			outtextxy(300, 500, CharToTCHAR("press any key to continue"));
			FlushBatchDraw(); 
			Sleep(300);
			break;
		}
		if (IsGamewin) {
			settextstyle(40, 0, _T("Consolas"));
			settextcolor(RGB(255, 0, 0));
			outtextxy(400, 400, CharToTCHAR("You Win!"));
			outtextxy(300, 500, CharToTCHAR("press any key to continue"));
			FlushBatchDraw();
			Sleep(300);
			break;
		}
		FlushBatchDraw();
		Sleep(100);
		cleardevice();
	}
	
	_getch();
	closegraph();
	return 0;
}

