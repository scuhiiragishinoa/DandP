#include <stdio.h>
#include <graphics.h>  
#include <conio.h>
#include <time.h>
#include "EasyXPng.h"            

#define  WIDTH  400              
#define  HEIGHT 600              
#define	 MaxBulletNum 200        
#define  MaxUFONum 10
#define MAX_ENEMY  50
#define MAX_BULLET 100

bool gameover = false;
bool enemyActive[MAX_ENEMY] = { false };
bool bulletActive[MAX_BULLET] = { false };
int t;

DWORD t1, t2;
DWORD tt1, tt2;

class Bullet {
public:
    IMAGE im_bullet;
    float x, y;
    float radius;

    void draw() {
        putimagePng(x - radius, y - radius, &im_bullet);
    }
};

IMAGE im_bk, im_bullet;
Bullet bullet[MaxBulletNum];

int bulletNum = 0;

class MyPlane {
public:
    IMAGE im_myplane;
    float x, y;
    float width, height;

    void draw() {
        putimagePng(x, y, &im_myplane);
    }
};

class EnemyPlane {
public:
    IMAGE im_enemyplane;
    float x, y;
    float width, height;

    void draw() {
        putimagePng(x, y, &im_enemyplane);
    }
};

EnemyPlane enemyplanes[MAX_ENEMY];
IMAGE im_enemyplane;
IMAGE im_heart, im_boom;

int heart_width, heart_height;
int heart = 3;

void Heart() {
    for (int i = 0; i < heart; i++) {
        putimagePng(10 + (heart_width * i), 10, &im_heart);
    }
}

IMAGE im_myplane;
MyPlane myplane;

bool checkCollision(float x1, float y1, float r1, float x2, float y2, float r2) {
    float dx = x1 - x2;
    float dy = y1 - y2;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = r1 + r2;
    return distanceSquared <= (radiusSum * radiusSum);
}

void boom(float x3, float y3) {
    putimagePng(x3 - 80, y3 - 80, &im_boom);
    FlushBatchDraw();
    Sleep(100);
}

void IsPressKey() {
    if (_kbhit()) { // if any key is pressed
        char key = _getch(); // keyboard info

        // 移动控制
        if (GetAsyncKeyState(VK_UP) && (myplane.y > 0)) {
            myplane.y -= 8;
        }
        if (GetAsyncKeyState(VK_DOWN) && (myplane.y < HEIGHT - myplane.height)) {
            myplane.y += 8;
        }
        if (GetAsyncKeyState(VK_LEFT) && (myplane.x > 0)) {
            myplane.x -= 6;
        }
        if (GetAsyncKeyState(VK_RIGHT) && (myplane.x < WIDTH - myplane.width)) {
            myplane.x += 6;
        }
    }
}

void AddEnemy() {
    for (int i = 0; i < MAX_ENEMY; ++i) {
        if (!enemyActive[i]) {
            enemyplanes[i].x = rand() % (WIDTH - 50);
            enemyplanes[i].y = 0; // 从顶部开始
            enemyActive[i] = true;
            break;
        }
    }
}

void AddBullet() {
    for (int i = 0; i < MAX_BULLET; ++i) {
        if (!bulletActive[i]) {
            bullet[i].x = myplane.x + myplane.width / 2;
            bullet[i].y = myplane.y;
            bullet[i].im_bullet = im_bullet;
            bullet[i].radius = im_bullet.getwidth() / 2;
            bulletActive[i] = true;
            break;
        }
    }
}

void startup() {
    srand(time(0));
    loadimage(&im_bk, _T("background.png"));
    loadimage(&im_bullet, _T("bullet.png"));
    loadimage(&im_myplane, _T("myplane.png"));
    loadimage(&im_enemyplane, _T("enemyplane.png"));
    loadimage(&im_heart, _T("heart.png"));
    loadimage(&im_boom, _T("blowup.png"));

    myplane.im_myplane = im_myplane;
    myplane.width = im_myplane.getwidth();
    myplane.height = im_myplane.getheight();
    myplane.x = WIDTH / 2 - myplane.width / 2;
    myplane.y = HEIGHT - myplane.height - 10;
    heart_width = im_heart.getwidth();
    heart_height = im_heart.getheight();

    t1 = GetTickCount(); // initial time of plane
    tt1 = GetTickCount(); // initial time of bullet

    initgraph(WIDTH, HEIGHT);
    BeginBatchDraw();
}

void show() {
    putimage(0, 0, &im_bk);
    Heart();

    for (int i = 0; i < MAX_BULLET; i++) {
        if (bulletActive[i]) {
            bullet[i].draw();
            bullet[i].y--;
        }
    }

    for (int i = 0; i < MAX_ENEMY; i++) {
        if (enemyActive[i]) {
            enemyplanes[i].draw();
            enemyplanes[i].y++;
        }
    }

    myplane.draw();


    for (int i = 0; i < MAX_BULLET; i++) {
        if (bulletActive[i]) {
            for (int j = 0; j < MAX_ENEMY; j++) {
                if (enemyActive[j] && checkCollision(bullet[i].x, bullet[i].y, bullet[i].radius, enemyplanes[j].x, enemyplanes[j].y, im_enemyplane.getwidth() / 2)) {
                    bulletActive[i] = false;
                    enemyActive[j] = false;
                    boom(enemyplanes[j].x, enemyplanes[j].y);
                }
            }
        }
    }


    for (int j = 0; j < MAX_ENEMY; j++) {
        if (enemyActive[j] && checkCollision(myplane.x, myplane.y, myplane.width / 2, enemyplanes[j].x, enemyplanes[j].y, im_enemyplane.getwidth() / 2)) {

            heart--;
            enemyActive[j] = false;
            boom(enemyplanes[j].x, enemyplanes[j].y);
        }

        FlushBatchDraw();
        Sleep(10);
    }

    void updateWithoutInput() {
        t = rand() % (WIDTH - 50);
        t2 = GetTickCount();
        if (t2 - t1 >= 1000) {
            AddEnemy();
            t1 = t2;
        }

        tt2 = GetTickCount();
        if (tt2 - tt1 >= 500) {
            AddBullet();
            tt1 = tt2;
        }

        for (int i = 0; i < MAX_BULLET; i++) {
            if (bullet[i].y < 0) bulletActive[i] = false;
        }

        for (int i = 0; i < MAX_ENEMY; i++) {
            if (enemyplanes[i].y > HEIGHT) enemyActive[i] = false;
        }

        IsPressKey();
    }

    int main() {
        startup();
        while (1) {
            show();
            updateWithoutInput();
            if (heart <= 0) gameover = true;
            if (gameover) {
                settextstyle(40, 0, _T("黑体"));
                outtextxy(200, HEIGHT / 2, "Game over!");
                FlushBatchDraw();
                Sleep(500);
                break;
            }
        }
        closegraph();
        return 0;
    }
