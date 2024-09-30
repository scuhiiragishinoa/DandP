/*************************************************************
 * 代码使用AI工具优化注释和变量名及函数名提高可读性和易维护性*
 *************************************************************/

#include <stdio.h>
#include <math.h>  // 数学库，用于处理 y1, y0, yn 的所有具体计算
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

 /*------------------------
 游戏逻辑与图形界面作业
 使用outtextxy函数打印文本
 ---------------------------*/

 // 字符串转化
TCHAR* CharToTCHAR(const char* pChar)
{
    TCHAR* pTchar = NULL;
    int nLen = strlen(pChar) + 1;
#ifdef _UNICODE
    pTchar = new wchar_t[nLen];
    MultiByteToWideChar(CP_ACP, 0, pChar, nLen, pTchar, nLen);
#else
    pTchar = new char[nLen];
    wcsncp(pTchar, pChar, nLen * sizeof(char));
#endif
    return pTchar;
}

// 窗口尺寸
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 800;

// 块的尺寸
int blockWidth = 100, blockHeight = 10;
int blockPositions[7][3] = { 0 };  // blockPositions[i][0] = x, blockPositions[i][1] = y, blockPositions[i][2] 表示块的颜色: 0 - 灰色, 1 - 红色, 2 - 蓝色
double blockVelocity = -2;

// 小球初始化
double ballX, ballY;
int ballRadius = 10;
double ballVelocityX = 0, ballVelocityY = 0;
double gravity = 0.12;  // 重力加速度
double horizontalFriction = 0.3;  // 水平摩擦系数
int jumpStep = 3;  // 跳跃次数
int health = 3; // 血量
int score = 0; // 记录分数


void initializeGame() {
    // 初始化游戏状态
    for (int i = 0; i < 6; i++) {
        int blockX = rand() % 480 + 20;
        int blockY = 100 + i * 125;
        blockPositions[i][0] = blockX;
        blockPositions[i][1] = blockY;
        int temp = rand() % 100;
        if (temp < 10) blockPositions[i][2] = 1; // 红色
        else if (temp > 90) blockPositions[i][2] = 2; // 蓝色
        else blockPositions[i][2] = 0; // 灰色
    }
    // 小球初始位置设置
    ballX = blockPositions[5][0] + blockWidth / 2.0;
    ballY = blockPositions[5][1] - ballRadius;
}

void drawBackground() {
    // 绘制背景
    for (int i = 0; i < 20; i++) {
        line(30 * i, 0, 30 * i + 15, 30);
        line(30 * i + 15, 30, 30 * i + 30, 0);
    }

    // 绘制块
    for (int i = 0; i < 6; i++) {
        blockPositions[i][1] += blockVelocity;

        // 根据颜色设置填充色
        if (blockPositions[i][2] == 0)
            setfillcolor(RGB(128, 128, 128)); // 灰色
        else if (blockPositions[i][2] == 1)
            setfillcolor(RGB(255, 0, 0)); // 红色
        else if (blockPositions[i][2] == 2)
            setfillcolor(RGB(0, 255, 0)); // 绿色
        fillrectangle(blockPositions[i][0], blockPositions[i][1], blockPositions[i][0] + blockWidth, blockPositions[i][1] + blockHeight);

        // 刷新块的位置
        if (blockPositions[i][1] + blockHeight <= 0) {
            score++;
            blockPositions[i][0] = rand() % 480 + 20;
            blockPositions[i][1] = WINDOW_HEIGHT;
            int temp = rand() % 100;
            if (temp < 10) blockPositions[i][2] = 1;
            else if (temp > 90) blockPositions[i][2] = 2;
            else blockPositions[i][2] = 0;
        }
    }
    wchar_t r[20];
    _itow_s(score, r, 10);
    outtextxy(170, 30, CharToTCHAR("your score:"));//实时显示分数
    outtextxy(340, 30, r);
}

int checkCollision() {
    for (int i = 0; i <= 6; i++) {
        int blockX = blockPositions[i][0];
        int blockY = blockPositions[i][1];
        // 检查小球与块的碰撞
        if (ballY < blockY && ballY + ballRadius >= blockY - 5 && ballX >= blockX && ballX <= blockX + blockWidth)
            return blockPositions[i][2];
    }
    return -1; // 没有碰撞
}

void restartBallPosition() { // 重置小球位置
    // 找到块中最下面的灰色块
    int maxY = blockPositions[0][1];
    int index = 0;
    for (int i = 0; i <= 5; i++) {
        if (blockPositions[i][1] > maxY && blockPositions[i][2] == 0) {
            maxY = blockPositions[i][1];
            index = i;
        }
    }

    ballX = blockPositions[index][0] + blockWidth / 2.0;
    ballY = blockPositions[index][1] - ballRadius;
}

void moveBall() {
    if (_kbhit()) {
        char keyPressed = _getch();
        if (keyPressed == 'a') ballVelocityX = -7;
        if (keyPressed == 'd') ballVelocityX = 7;
        if (jumpStep > 0 && keyPressed == 'w') {
            ballVelocityY = -7;
            jumpStep--;
        }
    }

    horizontalFriction = 0.3;
    int collisionType = checkCollision(); // 检查碰撞情况
    if (collisionType == -1) {
        ballVelocityY += gravity;
        ballY += ballVelocityY;
    }
    else if (collisionType == 0) {
        if (ballVelocityY < 0) {
            ballVelocityY = -ballVelocityY <= horizontalFriction ? 0 : ballVelocityY + horizontalFriction;
            ballY += ballVelocityY;
        }
        jumpStep = 3; // 重置跳跃次数
        ballY += blockVelocity;
    }
    else if (collisionType == 1) { // 碰到红色块
        health--;
        restartBallPosition();
    }
    else if (collisionType == 2) { // 碰到蓝色块
        horizontalFriction = 0.1; // 减小摩擦
        if (ballVelocityY < 0) {
            ballVelocityY = -ballVelocityY <= horizontalFriction ? 0 : ballVelocityY + horizontalFriction;
            ballY += ballVelocityY;
        }
        jumpStep = 3; // 重置跳跃次数
        ballY += blockVelocity;
    }

    // 处理小球的水平运动
    if (ballVelocityX < 0) {
        ballVelocityX = -ballVelocityX <= horizontalFriction ? 0 : ballVelocityX + horizontalFriction;
    }
    else if (ballVelocityX > 0) {
        ballVelocityX = ballVelocityX <= horizontalFriction ? 0 : ballVelocityX - horizontalFriction;
    }
    ballX += ballVelocityX;

    // 碰到边界处理
    if (ballY <= 30 || ballY > WINDOW_HEIGHT + 50) {
        health--;
        restartBallPosition();
    }

    setfillcolor(WHITE);
    fillcircle(ballX, ballY, ballRadius);

    // 显示血量
    for (int i = 1; i <= health; i++) {
        setfillcolor(RGB(255, 0, 0));
        fillcircle(30 + 15 * i, 40, 5);
    }
}

bool isGameOver() {
    if (health <= 0) {
        settextstyle(40, 0, _T("Consolas"));
        settextcolor(RGB(255, 0, 0));
        outtextxy(200, 300, CharToTCHAR("GAME OVER!"));
        settextstyle(20, 0, _T("Consolas"));
        outtextxy(200, 400, CharToTCHAR("Press any key to exit"));
        return true;
    }
    if (score >= 100) {
        settextstyle(40, 0, _T("Consolas"));
        settextcolor(RGB(255, 0, 0));
        outtextxy(200, 300, CharToTCHAR("YOU WIN!"));
        settextstyle(20, 0, _T("Consolas"));
        outtextxy(200, 400, CharToTCHAR("Press any key to exit"));
        return true;
    }
    return false;
}

int main() {
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, SHOWCONSOLE);
    setbkcolor(RGB(60, 0, 2));
    cleardevice();

    initializeGame();

    while (true) {
        outtextxy(230, 400, CharToTCHAR("tap space to start:"));
        outtextxy(120, 430, CharToTCHAR("Press 'a''d'to move,'w' to jump(only for 3 times)"));
        outtextxy(90, 460, CharToTCHAR("red blocks cause damage while green ones are flexible, you have 3 lives"));
        wchar_t t[20];
        wchar_t r[20];
        if (_kbhit()) {
            char input = _getch();
            if (input == ' ') {
                break;
            }
        }
    }

    while (true) {
        BeginBatchDraw();

        cleardevice();

        drawBackground();

        moveBall();

        EndBatchDraw();

        if (isGameOver()) {
            char ch = _getch();
            break;
        }

        Sleep(10);
    }
    closegraph();

    return 0;
}