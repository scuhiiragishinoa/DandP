﻿/*************************************************************
 * 代码使用AI工具优化注释和变量名及函数名提高可读性和易维护性*
 *************************************************************/


#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "EasyXPng.h"

#define SCREEN_WIDTH 560
#define SCREEN_HEIGHT 800
#define MAX_BULLET_COUNT 200
#define MAX_UFO_COUNT 10

bool isGameOver = false;

// This program modifies the size of the images provided

class Bullet
{
public:
    IMAGE image;
    float x, y;
    float velocityX, velocityY;
    float radius;

    void draw()
    {
        putimagePng(x - radius, y - radius, &image);
    }

    void update()
    {
        x += velocityX;
        y += velocityY;
        if (x <= 0 || x >= SCREEN_WIDTH)
            velocityX = -velocityX;
        if (y <= 0 || y >= SCREEN_HEIGHT)
            velocityY = -velocityY;
    }
};

IMAGE backgroundImg, bulletImg;
Bullet bullets[MAX_BULLET_COUNT];
int bulletCount = 0;

class Rocket
{
public:
    IMAGE image;
    float x, y;
    float width, height;

    void draw()
    {
        putimagePng(x - width / 2, y - height / 2, &image);
    }

    void update(float mouseX, float mouseY)
    {
        x = mouseX;
        y = mouseY;
    }
};

class UFO
{
public:
    IMAGE image;
    float x, y;
    float velocityX, velocityY;

    void draw()
    {
        putimagePng(x - (image.getwidth() / 2), y - (image.getheight() / 2), &image);
    }

    void trackAndMove(float targetX, float targetY)
    {
        // Update UFO's velocity to move towards the rocket
        float dx = targetX - x;
        float dy = targetY - y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance > 0)
        {
            velocityX = (dx / distance) * 1.5f; // Adjust speed
            velocityY = (dy / distance) * 1.5f;
        }

        x += velocityX;
        y += velocityY;

        if (x <= 0 || x >= SCREEN_WIDTH)
            velocityX = -velocityX;
        if (y <= 0 || y >= SCREEN_HEIGHT)
            velocityY = -velocityY;
    }
};

IMAGE heartImg, explosionImg;
int heartWidth, heartHeight;
int heartCount = 3;
UFO ufos[MAX_UFO_COUNT];
int ufoCount = 1; // Initially only one UFO
int timeSinceLastUFO = 0; // Timer, in seconds
int survivalTime = 0; // Survival time timer

void drawHearts()
{
    for (int i = 0; i < heartCount; i++)
    {
        putimagePng(10 + (heartWidth * i), 10, &heartImg);
    }
}

IMAGE rocketImg;
Rocket rocket;

bool checkCollision(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = r1 + r2;
    return distanceSquared <= (radiusSum * radiusSum);
}

void drawExplosion(float x, float y)
{
    putimagePng(x - 80, y - 80, &explosionImg);
    FlushBatchDraw();
    Sleep(100);
}

void initialize()
{
    srand(time(0));
    loadimage(&backgroundImg, _T("resource\\background.png"));
    loadimage(&bulletImg, _T("resource\\bullet.png"));
    loadimage(&rocketImg, _T("resource\\rocket.png"));
    loadimage(&heartImg, _T("resource\\heart.png"));
    loadimage(&explosionImg, _T("resource\\blowup.png"));

    for (int i = 0; i < MAX_UFO_COUNT; i++)
    {
        loadimage(&ufos[i].image, _T("resource\\ufo.png"));
    }

    rocket.image = rocketImg;
    rocket.width = rocketImg.getwidth();
    rocket.height = rocketImg.getheight();

    heartWidth = heartImg.getwidth();
    heartHeight = heartImg.getheight();

    initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
    BeginBatchDraw();
}

void render()
{
    putimage(0, 0, &backgroundImg);
    drawHearts();
    for (int i = 0; i < bulletCount; i++)
        bullets[i].draw();

    for (int i = 0; i < ufoCount; i++)
    {
        ufos[i].draw();
    }

    rocket.draw();

    // Display survival time
    char timerText[50];
    sprintf_s(timerText, "Survival Time: %d seconds", survivalTime);
    outtextxy(450, 10, timerText); // Display survival time in the top-right corner

    FlushBatchDraw();
    Sleep(10);
}

void updateGameLogic()
{
    static int lastSecond = 0;
    static int currentSecond = 0;
    static clock_t startTime = clock();
    clock_t currentTime = clock();

    currentSecond = (int(currentTime - startTime) / CLOCKS_PER_SEC);

    if (currentSecond == lastSecond + 2)
    {
        lastSecond = currentSecond;

        // Increment survival time
        survivalTime++;

        if (bulletCount < MAX_BULLET_COUNT)
        {
            bullets[bulletCount].x = SCREEN_WIDTH / 2;
            bullets[bulletCount].y = 10;
            float angle = (rand() / double(RAND_MAX) - 0.5) * 0.9 * PI;
            float speed = 2 * rand() / double(RAND_MAX) + 2;
            bullets[bulletCount].velocityX = speed * sin(angle);
            bullets[bulletCount].velocityY = speed * cos(angle);
            bullets[bulletCount].image = bulletImg;
            bullets[bulletCount].radius = bulletImg.getwidth() / 2;
            bulletCount++;
        }

        timeSinceLastUFO++; // Increment UFO timer
        if (timeSinceLastUFO >= 5 && ufoCount < MAX_UFO_COUNT) // Generate a UFO every 5 seconds
        {
            ufos[ufoCount].x = rand() % SCREEN_WIDTH;
            ufos[ufoCount].y = rand() % (SCREEN_HEIGHT / 2);
            ufoCount++;
            timeSinceLastUFO = 0; // Reset timer
        }
    }

    for (int i = 0; i < bulletCount; i++)
    {
        bullets[i].update();
        if (checkCollision(rocket.x, rocket.y, rocket.width / 2, bullets[i].x, bullets[i].y, bullets[i].radius) && survivalTime > 0)
        {
            heartCount--;
            drawExplosion(bullets[i].x, bullets[i].y);
            bullets[i] = bullets[bulletCount - 1];
            bulletCount--;
            i--;
        }
    }

    for (int i = 0; i < ufoCount; i++)
    {
        ufos[i].trackAndMove(rocket.x, rocket.y); // Pass rocket position to make UFO follow
        if (checkCollision(rocket.x, rocket.y, rocket.width / 2, ufos[i].x, ufos[i].y, ufos[i].image.getwidth() / 2) && survivalTime > 0)
        {
            heartCount--;
            drawExplosion(ufos[i].x, ufos[i].y);
            ufos[i] = ufos[ufoCount - 1];
            ufoCount--;
            i--;
        }
    }

    MOUSEMSG mouseMsg;
    while (MouseHit())
    {
        mouseMsg = GetMouseMsg();
        if (mouseMsg.uMsg == WM_MOUSEMOVE)
            rocket.update(mouseMsg.x, mouseMsg.y);
    }
}

int main()
{
    initialize();
    while (1)
    {
        render();
        updateGameLogic();
        if (heartCount <= 0) isGameOver = true;
        if (isGameOver)
        {
            settextstyle(40, 0, _T("Arial"));
            outtextxy(200, SCREEN_HEIGHT / 2, "GAME OVER!");
            FlushBatchDraw();
            Sleep(500);
            break;
        }
    }
    return 0;
}