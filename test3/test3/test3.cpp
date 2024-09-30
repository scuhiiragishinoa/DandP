#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include<iomanip>
#include<iostream>

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
    strcpy(pTchar, pChar);
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
    void bounce(double normalX, double normalY) {
        double length = sqrt(normalX * normalX + normalY * normalY);
        normalX /= length;
        normalY /= length;

        double dotProduct = vx * normalX + vy * normalY;

        vx -= 2 * dotProduct * normalX;
        vy -= 2 * dotProduct * normalY;
    }

    void move(double timeStep) {
        x += vx * timeStep;
        y += vy * timeStep;
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

class Rectmat {
public:
    double left;
    double top;
    double right;
    double bottom;
    double x, y;
    bool active; // to track if the rectangle is still active

    Rectmat() {
        active = true;
    }

    Rectmat(int x, int y) : x(x), y(y) {
        active = true;
    }

    double getPositionx() const {
        return x;
    }
    double getPositiony() const {
        return y;
    }

    void getRectmat(double left, double top, double right, double bottom, int x, int y) {
        this->left = left;
        this->top = top;
        this->right = right;
        this->bottom = bottom;
        this->x = x;
        this->y = y;
    }

    bool isColliding(Ball& ball) {
        double closestX = max(left, min(ball.getX(), right));
        double closestY = max(top, min(ball.getY(), bottom));

        double distanceX = ball.getX() - closestX;
        double distanceY = ball.getY() - closestY;

        double distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

        return distanceSquared < (ball.size * ball.size);
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
    double getX() {
        return x;
    }
    double getY() {
        return y;
    }
};

int main() {
    int len = 10;
    int wid = 5;
    int singleLen = 50;
    int singleWid = 20;
    int gap = 5;
    int testvx = 10 + rand() % 5;
    int testvy = -sqrt(100 - testvx);
    int remain = len * wid;
    bool isGameover = 0;
    bool isWin = 0;

    initgraph(600, 800);
    setbkcolor(RGB(50, 50, 50));

    Ball ball;
    ball.getBall(10, 300, 700, testvx, testvy);

    Rect rect1;
    rect1.getRect(280, 710, 320, 720, NULL, NULL);

    Rectmat rectArray[10][5];
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < wid; j++) {
            rectArray[i][j].getRectmat(gap * (i + 1) + i * singleLen, gap * (j + 1) + j * singleWid, gap * (i + 1) + (i + 1) * singleLen, gap * (j + 1) + (j + 1) * singleWid, i, j);
        }
    }

    // to start
    while (true) {
        outtextxy(230, 400, CharToTCHAR("tap space to start:"));
        if (_kbhit()) {
            char input = _getch();
            if (input == ' ') {
                break;
            }
        }
    }

    // gaming
    while (true) {
        // input
        if (_kbhit()) {
            char input = _getch();
            if (input == 'a' && rect1.left > 0) {
                rect1.left -= 10;
                rect1.right -= 10;
            }
            if (input == 'd' && rect1.right < 600) {
                rect1.left += 10;
                rect1.right += 10;
            }
        }

        // create rectangle matrix
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < wid; j++) {
                if (rectArray[i][j].active) {
                    setfillcolor(RGB(0, 128, 128));
                    fillrectangle(rectArray[i][j].left, rectArray[i][j].top, rectArray[i][j].right, rectArray[i][j].bottom);
                }
            }
        }

        // ball moving
        ball.x += ball.vx;
        ball.y += ball.vy;

        // collision
        // wall
        if (ball.x - ball.size + ball.vx < 0 || ball.x + ball.size + ball.vx > 600) {
            ball.vx = -ball.vx;
        }
        if (ball.y - ball.size + ball.vy < 0) {
            ball.vy = -ball.vy;
        }

        // paddle collision
        if (ball.x + ball.size >= rect1.left && ball.x - ball.size <= rect1.right && ball.y >= rect1.top && ball.y <= rect1.bottom) {
            ball.vx = -ball.vx;
        }
        if (ball.y + ball.size >= rect1.top && ball.x + ball.size >= rect1.left && ball.x - ball.size <= rect1.right) {
            ball.vy = -ball.vy;
        }

        // rectangle matrix collision
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < wid; j++) {
                if (rectArray[i][j].active && rectArray[i][j].isColliding(ball)) {
                    rectArray[i][j].active = false;
                    if (ball.x + ball.size >= rectArray[i][j].left && ball.x - ball.size <= rectArray[i][j].right && ball.y >= rectArray[i][j].top && ball.y <= rectArray[i][j].bottom) {
                        ball.vx = -ball.vx;
                    }
                    if ((ball.y + ball.size >= rectArray[i][j].top || ball.y + ball.size <= rectArray[i][j].bottom) && ball.x + ball.size >= rectArray[i][j].left && ball.x - ball.size <= rectArray[i][j].right) {
                        ball.vy = -ball.vy;
                    }
                }
            }
        }

        // dead condition
        if (ball.y + ball.size - ball.vy > 800) {
            isGameover = 1;
        }
        if (isGameover) {
            outtextxy(230, 400, CharToTCHAR("YOU DIED!"));
            Sleep(10000);
            _getch();
            return 0;
        }

        fillcircle(ball.x, ball.y, ball.size);
        fillrectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
        Sleep(50);
        cleardevice();
    }

    _getch();
    return 0;
}