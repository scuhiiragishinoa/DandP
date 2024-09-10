#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

using namespace std;

TCHAR* CharToTCHAR(const char* pChar)
{
	TCHAR* pTchar = NULL;
	int nLen = strlen(pChar) + 1;
#ifdef _UNICODE
	pTchar = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, 0, pChar, nLen, pTchar, nLen);
#else
	pTchar = new char[nLen];
	wcsncp(pTChar, pChar, nLen * sizeof(char));
#endif
	return pTchar;
}

class Ball {
public:
	double size;
	double x;
	double y;
	double vx;
	double vy;
	const double g = 1;

	void getBall(double size, double x, double y, double vx, double vy) {
		this->size = size;
		this->x = x;
		this->y = y;
		this->vx = vx;
		this->vy = vy;
	}

	double getX(Ball ball) {
		return x;
	}
	double getY(Ball ball) {
		return y;
	}
	double getVx(Ball ball) {
		return vx;
	}
	double getVy(Ball ball) {
		return vy;
	}
};

class Rect {
public:
	double left;
	double top;
	double right;
	double bottom;
	double vx;
	double vy;

	void getRect(double left, double top, double right, double bottom, double vx, double vy) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
		this->vx = vx;
		this->vy = vy;
	}

	double getTop(Rect rectangle) {
		return top;
	}
	double getBottom(Rect rectangle) {
		return bottom;
	}
	double getLeft(Rect rectangle) {
		return left;
	}
	double getRight(Rect rectangle) {
		return right;
	}
	double getVx(Rect rectangle) {
		return vx;
	}
	double getVy(Rect rectangle) {
		return vy;
	}
};

int main()
{
	int time = 5;
	int access = -1;
	int status = 0;
	
	initgraph(2000, 1000);
	setbkcolor(RGB(50, 50, 50));
	setlinecolor(RGB(57, 197, 187));
	setfillcolor(RGB(0, 140, 0));

	Ball ball3;
	ball3.getBall(50, 250, 950, 0, 0);
	
	Rect rect1;
	Rect rect2;
	rect1.getRect(400, 800, 430, 1000, 20, 0);
	rect2.getRect(400, 0, 430, 400, 20, 0);

	while (true) {
		if (_kbhit()) {
			char input = _getch();
			if (input == ' ') {
				ball3.vy = -20;
			}
		}
		//ball3.jump(ball3);
		
		ball3.vy += 1;
		ball3.y += ball3.vy;
		if (ball3.y >= 1000 - ball3.size) {
			ball3.vy = 0;
			ball3.y = 1000 - ball3.size;
		}

		rect1.left -= rect1.vx;
		rect2.left = rect1.left;
		rect1.right -= rect1.vx;
		rect2.right = rect1.right;

		if (rect1.left <= 0) {
			rect1.left = 2000;
			rect2.left = 2000;
			rect1.right = 2030;
			rect2.right = 2030;

			rect1.top = 800 - rand() % 400;
			rect2.bottom = rect1.top - 400;
			rect1.vx = 15 + rand() % 22;
			rect2.vx = rect1.vx;
			status = 0;
		}
		if (ball3.x >= rect1.left - ball3.size && ball3.x <= rect1.right + ball3.size && (rect1.top <= ball3.y + ball3.size || rect2.bottom >= ball3.y - ball3.size)) {
			access = -1;
		}

		cleardevice();
		fillcircle(ball3.x, ball3.y, ball3.size);
		setfillcolor(RGB(57, 197, 187));
		fillrectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
		fillrectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
		wchar_t s[20];
		_itow_s(access, s, 10);
		outtextxy(170, 190, s);
		Sleep(17);

		if (ball3.x >= rect1.right + ball3.size && status == 0) {
			access++;
			status = 1;
		}

		if (access == 10 && status == 1) {
			outtextxy(170, 210, CharToTCHAR("well done! wait to continue."));
			Sleep(3000);
			status = 0;
		}
		
	}
	
	closegraph();
	return 0;
}