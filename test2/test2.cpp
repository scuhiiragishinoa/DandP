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

	double getX() {
		return x;
	}
	double getY() {
		return y;
	}
	double getVx() {
		return vx;
	}
	double getVy() {
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

	double getTop() {
		return top;
	}
	double getBottom() {
		return bottom;
	}
	double getLeft() {
		return left;
	}
	double getRight() {
		return right;
	}
	double getVx() {
		return vx;
	}
	double getVy() {
		return vy;
	}
};

int main() {
	double g = 1;
	double v = 900;
	int status = 0;
	int hit = 0;
	int remain = 10;
	bool isGameover = 0;

	initgraph(600, 800);
	setbkcolor(RGB(50, 50, 50));

	Ball ball;
	Rect rect1;
	rect1.getRect(580, 500, 600, 600, 0, 20);

	while (true) {
		int angle = -45 + rand() % 90;
		wchar_t s[20];
		_itow_s(angle, s, 10);
		outtextxy(170, 180, s);

		wchar_t t[20];
		_itow_s(hit, t, 10);
		outtextxy(170, 220, CharToTCHAR("the times you have hit it:"));
		outtextxy(340, 220, t);

		wchar_t r[20];
		_itow_s(remain, r, 10);
		outtextxy(170, 260, CharToTCHAR("the shells  remained:"));
		outtextxy(340, 260, r);



		rect1.top += rect1.vy;
		rect1.bottom += rect1.vy;
		
		if (rect1.top <= 0 || rect1.bottom >= 800) {
			rect1.vy = -rect1.vy;
		}

		if (_kbhit()) {
			char input = _getch();
			if (input == ' ' && !isGameover && status == 0) {
				status = 1;
				remain--;
				ball.getBall(15, 0, 450, NULL, NULL);
				ball.vx =(1000 - ball.getY())/80 + (rand() % 20);
				ball.vy = -sqrt(v - ball.vx * ball.vx);
			}
		}

		ball.x += ball.vx; 
		ball.y += ball.vy;
		ball.vy += g;
		if (ball.x < 600 && ball.y < 800 && status == 1) {
			status = 1;
		}
		if (ball.x >= rect1.left - ball.size && ball.y <= rect1.bottom + ball.size && ball.y >= rect1.top - ball.size && ball.y <= rect1.right - ball.size && status == 1) {
			hit++;
			status = 0;
			if (remain == 0) {
				isGameover = 1;
				wchar_t g[20];
				_itow_s(hit, g, 10);
				outtextxy(170, 500, CharToTCHAR("Gameover. Your score is:"));
				outtextxy(340, 500, g);
				Sleep(10000);
			}
		}

		if ((ball.x >= 600 || ball.y >= 800) && status == 1) {
			status = 0;
			if (remain == 0) {
				isGameover = 1;
				wchar_t g[20];
				_itow_s(hit, g, 10);
				outtextxy(170, 500, CharToTCHAR("Gameover. Your score is:"));
				outtextxy(340, 500, g);
				Sleep(10000000);
			}
		}
		circle(ball.x, ball.y, ball.size);
		fillrectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
		Sleep(40);



		cleardevice();
	}
	_getch();
	return 0;
}