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

int main() {
	int g = 1;
	initgraph(600, 800);
	setbkcolor(RGB(50, 50, 50));
	cleardevice();
	Ball ball1;
	ball1.getBall(15, 0, 600, 15, -30);
	while (ball1.x < 600 && ball1.y < 800) {
		circle(ball1.x, ball1.y, ball1.size);
		ball1.x += ball1.vx;
		ball1.y += ball1.vy;
		ball1.vy += g;
		Sleep(40);
	}

	_getch();
	return 0;
}