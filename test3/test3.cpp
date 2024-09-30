/*************************************************************
 * 代码使用AI工具优化注释和变量名及函数名提高可读性和易维护性*
 *************************************************************/

#include<stdio.h>
#include<math.h>   // 数学库，使用y1, y0, yn进行完整的数学计算
#include<graphics.h>
#include<conio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>

 /*------------------------
 游戏逻辑与图形界面作业
 使用outtextxy函数打印文本
 ---------------------------*/

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

// 窗口尺寸
int WINDOW_WIDTH = 600;
int WINDOW_HEIGHT = 800;

// 砖块设置
int BRICK_WIDTH = 70; //(70 + 5) * 8 = ;
int BRICK_HEIGHT = 30;
int BRICK_GAP = 5;
int brick_positions[8 * 5][3] = { 0 };  // 砖块显示位置
int total_bricks = 0;                 // 当前砖块数

// 小球设置
double ball_x, ball_y;
int ball_radius = 10;
double ball_velocity_x = 0.7, ball_velocity_y = -0.6; // 小球初速度
int missed_count = 0; // 记录遗漏的球数

// 板子设置
int paddle_x = WINDOW_WIDTH / 2 - 50, paddle_y = 700;
int paddle_width = 120, paddle_height = 10;
double paddle_velocity; // 板子移动速度
double friction = 0.3; // 摩擦力

// 附加物件设置  - 坏物体的坐标和颜色
int bonus_x, bonus_y;
int bonus_color;
int has_bonus = 0; // 是否已经生成奖励物
int bonus_radius = 15; // 奖励物半径
time_t bonus_start_time, bonus_end_time; // 计时器变量

void drawBricks() {
    // 绘制砖块
    for (int i = 0; i < 8 * 5; i++) {
        if (brick_positions[i][2] == 1) {
            int brick_right = brick_positions[i][0] + BRICK_WIDTH;
            int brick_bottom = brick_positions[i][1] + BRICK_HEIGHT;
            setfillcolor(RGB(0, 0, 200));
            fillrectangle(brick_positions[i][0], brick_positions[i][1], brick_right, brick_bottom);
        }
    }
    // 绘制遗漏球
    for (int i = 1; i <= 3 - missed_count; i++) {
        setfillcolor(RGB(255, 0, 0));
        fillcircle(20 * i, 180, 7);
    }
}

void initializeGame() {
    // 初始化砖块位置
    for (int i = 0; i < 8 * 5; i++) {
        int layer = i / 8;
        int row = i % 8;
        int x = row * (BRICK_WIDTH + BRICK_GAP);
        int y = layer * (BRICK_HEIGHT + BRICK_GAP);
        brick_positions[i][0] = x;
        brick_positions[i][1] = y;
        brick_positions[i][2] = 1; // 1表示砖块存在，0表示砖块消失
    }
    // 初始化小球位置
    ball_x = paddle_x + paddle_width / 2;
    ball_y = paddle_y - ball_radius;

    settextstyle(40, 0, _T("Consolas"));
    settextcolor(RGB(255, 0, 0));
    outtextxy(200, 300, CharToTCHAR("Start"));

    settextstyle(20, 0, _T("Consolas"));
    outtextxy(200, 400, CharToTCHAR("tap space to start:"));
    outtextxy(200, 450, CharToTCHAR("Press 'a''d'to move"));

    drawBricks();

    setfillcolor(RGB(255,0,0));
    fillcircle(ball_x, ball_y, ball_radius);

    setfillcolor(RGB(0, 189, 0));
    fillrectangle(paddle_x, paddle_y, paddle_x + paddle_width, paddle_y + paddle_height);

    char input_char = _getch();
    while (input_char != ' ') {
        if (input_char == 'a' || input_char == 'A') {
            paddle_x -= paddle_velocity;
        }
        else if (input_char == 'd' || input_char == 'D') {
            paddle_x += paddle_velocity;
        }

        ball_x = paddle_x + paddle_width / 2;

        cleardevice();

        settextstyle(20, 0, _T("Consolas"));
        outtextxy(200, 400, CharToTCHAR("tap space to start:"));
        outtextxy(200, 450, CharToTCHAR("Press 'a''d'to move"));

        drawBricks();

        setfillcolor(RGB(255, 0, 0));
        fillcircle(ball_x, ball_y, ball_radius);

        setfillcolor(RGB(0, 100, 0));
        fillrectangle(paddle_x, paddle_y, paddle_x + paddle_width, paddle_y + paddle_height);

        input_char = _getch();
    }
}

void checkBonus(double x_position, double y_position) {
    if (has_bonus)
        return;

    // 随机生成奖励物
    int rand_value = rand() % 100;
    if (rand_value <= 10) {  // 生成一个好物
        has_bonus = 1;
        bonus_x = x_position;
        bonus_y = y_position;
        bonus_color = 0;
    }
    else if (rand_value >= 90) {  // 生成一个坏物
        has_bonus = 1;
        bonus_x = x_position;
        bonus_y = y_position;
        bonus_color = 1;
    }
}

void drawBonus() {
    if (bonus_start_time + 10 == time(NULL)) {
        paddle_width = 120;  // 还原为默认宽度
    }
    if (!has_bonus)
        return;

    // 奖励物下移
    bonus_y += 1;
    if (bonus_color == 0)
        setfillcolor(RGB(236, 189, 81)); // 设置为好物颜色
    if (bonus_color == 1)
        setfillcolor(RGB(255, 0, 0)); // 设置为坏物颜色

    fillcircle(bonus_x, bonus_y, bonus_radius);

    // 检测碰撞
    if (bonus_y < paddle_y && bonus_y + bonus_radius > paddle_y && bonus_x + bonus_radius >= paddle_x && bonus_x - bonus_radius <= paddle_x + paddle_width) {
        if (bonus_color == 0) {
            paddle_width = 180; // 加宽板子
            bonus_start_time = time(NULL);
            has_bonus = 0;
        }
        else if (bonus_color == 1) { // 处理坏物
            missed_count--;
            has_bonus = 0;
        }
    }

    // 超出窗口下边界
    if (bonus_y >= WINDOW_HEIGHT)
        has_bonus = 0;
}

void moveBall() {
    // 移动小球
    if (ball_y < paddle_y && ball_y + ball_radius > paddle_y && ball_x + ball_radius >= paddle_x && ball_x - ball_radius <= paddle_x + paddle_width) {
        ball_velocity_y = -ball_velocity_y; // 碰到底边反弹
    }
    else if ((ball_x + ball_radius >= paddle_x && ball_x < paddle_x && ball_y >= paddle_y && ball_y <= paddle_y + paddle_height) ||
        (ball_x > paddle_x + paddle_width && ball_x - ball_radius < paddle_x + paddle_width && ball_y >= paddle_y && ball_y <= paddle_y + paddle_height)) {
        ball_velocity_x = -ball_velocity_x; // 碰到左边或右边
    }

    // 检测砖块碰撞并消除
    for (int i = 0; i < 8 * 5; i++) {
        if (brick_positions[i][2] == 1) {
            int brick_x = brick_positions[i][0];
            int brick_y = brick_positions[i][1];
            if ((ball_y > brick_y + BRICK_HEIGHT && ball_y - ball_radius <= brick_y + BRICK_HEIGHT && ball_x >= brick_x && ball_x <= brick_x + BRICK_WIDTH) ||
                (ball_y < brick_y && ball_y + ball_radius >= brick_y && ball_x >= brick_x && ball_x <= brick_x + BRICK_WIDTH)) {
                ball_velocity_y = -ball_velocity_y; // 反弹
                total_bricks++;
                brick_positions[i][2] = 0; // 移除砖块
                checkBonus(ball_x, ball_y); // 检查掉落物品
                break;
            }
            else if ((ball_x + ball_radius >= brick_x && ball_x - ball_radius < brick_x && ball_y >= brick_y && ball_y <= brick_y + BRICK_HEIGHT) ||
                (ball_x + ball_radius > brick_x + BRICK_WIDTH && ball_x - ball_radius < brick_x + BRICK_WIDTH && ball_y >= brick_y && ball_y <= brick_y + BRICK_HEIGHT)) {
                ball_velocity_x = -ball_velocity_x; // 反弹
                total_bricks++;
                brick_positions[i][2] = 0; // 移除砖块
                checkBonus(ball_x, ball_y); // 检查掉落物品
            }
        }
    }

    // 碰到上边界反弹///////////////////////////////////修正：增加一条命
    if (ball_y - ball_radius <= 0) {
        ball_velocity_y = -ball_velocity_y;
        missed_count--;
        bonus_color = 0;
    }
    // 检查小球是否掉落
    if (ball_y + ball_radius > WINDOW_HEIGHT) {
        ball_x = paddle_x + paddle_width / 2; // 重置小球位置
        ball_y = paddle_y - ball_radius;
        ball_velocity_x = 0.6;
        ball_velocity_y = -0.7;
        missed_count++; // 计数增加
    }

    // 碰到左右边界反弹
    if (ball_x - ball_radius <= 0 || ball_x + ball_radius >= WINDOW_WIDTH)
        ball_velocity_x = -ball_velocity_x;

    ball_x += ball_velocity_x;
    ball_y += ball_velocity_y;

    setfillcolor(RGB(255, 0, 0));
    fillcircle(ball_x, ball_y, ball_radius);
}

void movePaddle() {
    // 控制板子移动
    if (_kbhit()) {
        char input_char = _getch();
        if (input_char == 'a') // 向左移动
            paddle_velocity = -6;
        if (input_char == 'd') // 向右移动
            paddle_velocity = 6;
    }
    // 摩擦力影响移动速度
    if (paddle_velocity > 0)
        paddle_velocity = paddle_velocity <= friction ? 0 : paddle_velocity - friction;
    else if (paddle_velocity < 0)
        paddle_velocity = -paddle_velocity <= friction ? 0 : paddle_velocity + friction;

    paddle_x += paddle_velocity;
    setfillcolor(RGB(236, 189, 81));
    fillrectangle(paddle_x, paddle_y, paddle_x + paddle_width, paddle_y + paddle_height);
}

bool checkGameOver() {
    if (missed_count >= 3) {
        // 游戏结束界面
        settextstyle(40, 0, _T("Consolas"));
        settextcolor(RGB(255, 0, 0));
        outtextxy(200, 300, CharToTCHAR("GAME OVER!"));

        settextstyle(20, 0, _T("Consolas"));
        outtextxy(200, 400, CharToTCHAR("Press any key to exit"));
        return true;
    }
    if (total_bricks >= 8 * 5) {
        // 游戏胜利界面
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
    setbkcolor(0x6bae91);
    cleardevice();

    initializeGame();

    while (1) {
        BeginBatchDraw();
        cleardevice();

        drawBricks();
        movePaddle();
        moveBall();
        drawBonus();

        EndBatchDraw();

        if (checkGameOver()) {
            char ch = _getch();
            break;
        }

        Sleep(1);
    }
    closegraph();

    return 0;
}