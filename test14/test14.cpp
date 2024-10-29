#include <graphics.h>  
#include <conio.h>
#include <time.h>
#include "EasyXPng.h"
#include "Timer.h"
#include <vector>
#include <queue>
using namespace std;

#define  WIDTH 800  
#define  HEIGHT 600

enum PlayerStatus // 枚举类型，游戏角色所有的可能状态
{
	standleft, standright, runleft, runright, jumpleft, jumpright, die
};

class Land  // 地面类
{
public:
	IMAGE im_land;  // 地面图像
	float left_x, right_x, top_y; // 用来刻画一块地面的左、右、上坐标
	float land_width, land_height; // 一块地面图像的宽度、高度

	void initialize() // 初始化
	{
		loadimage(&im_land, _T("land.png")); // 导入地面图片
		land_width = im_land.getwidth(); // 获得地面图像的宽、高
		land_height = im_land.getheight();
		left_x = WIDTH / 2;  // land初始化在画面正中间，正好就在玩家角色脚底下
		right_x = left_x + land_width;
		top_y = HEIGHT / 2;
	}

	void draw()// 显示相关信息	
	{
		putimage(left_x, top_y, &im_land);  // 绘制一个地面
	}
};

class Scene // 游戏场景类
{
public:
	//IMAGE im_bk;  // 背景图像	
	vector<IMAGE> ims_bk;
	vector<Land> lands; // 多个地面
	//queue<Land> lands;
	int animID; //滚动播放的id
	int dx;
	int vx;
	//int count = 0;  //记录每一高度台阶的数量

	void bkmove() {
		dx -= 2;
		if (dx < -1200) {
			dx = 0;
			animID++;
		}
		if (animID > 8) animID = 0;

		//地面随机生成
		if (lands[0].right_x <= 0) {
			//元素右移
			for (int i = 0; i < lands.size() - 1; i++) {
				lands[i] = lands[i + 1];
			}
			Land land;
			land.initialize();
			land.left_x = 9 * land.land_width;
			land.right_x = land.left_x + land.land_width;

			land.top_y = HEIGHT / 2 + rand() % 2 * HEIGHT / 10;
			lands[lands.size() - 1] = land;
		}
	}

	void draw()// 显示相关信息	
	{
		//putimage(-100, -100, &im_bk);	// 显示背景	
		//背景慢速移动
		bkmove();
		putimage(dx, -100, &ims_bk[animID]);


		for (int i = 0; i < lands.size(); i++)
		{
			lands[i].draw();  // 绘制所有地面
		}
	}

	void initialize() // 初始化
	{
		ims_bk.clear();
		TCHAR filename[80];
		for (int i = 1; i <= 9; i++) // 把向右奔跑的八张图片对象添加到ims_runright中
		{
			_stprintf_s(filename, _T("landscape%d.png"), i);
			IMAGE im;
			loadimage(&im, filename);
			ims_bk.push_back(im);
		}
		lands.clear();// 先清空掉vector
		//while (!lands.empty()) lands.pop();
		for (int i = 0; i < 10; i++) // 产生一些随机地面
		{
			Land land;
			land.initialize();
			land.left_x = i * land.land_width;
			land.right_x = land.left_x + land.land_width;
			land.top_y = HEIGHT / 2 + rand() % 2 * HEIGHT / 10;
			lands.push_back(land);
		}

		animID = 1;
		dx = 0;
		vx = 10;
	}
};

class Bat1 {
public:
	IMAGE im_bat;
	float x;
	float y;
	bool exist;

	void initialize() {
		loadimage(&im_bat, _T("bat.png"));
		x = WIDTH;
		y = HEIGHT / 4;
		exist = false;
	}
	void getBat(int ph) {//随着land移动,概率出现
		if (!exist && rand() % 100 < 20) {
			exist = true;

			switch (rand() % 3)
			{
			case 0: y = ph + 10; break;
			case 1: y = ph - 40; break;
			case 2: y = ph - 170; break;
			default:y = HEIGHT / 2; break;

			}
		}
	}

	void draw() {
		if (exist) putimagePng(x, y, &im_bat);
	}

	void move(int speed) {
		if (x < 0) {
			initialize();
		}
		if (exist) {
			x -= speed;
		}
	}
};

class Bat2 {
public:
	vector<IMAGE> ims_bat;
	float x;
	float y;
	bool exist;
	float w;
	float h;
	float vx = 15;

	int index;  //播放动画
	void initialize() {
		ims_bat.clear();
		TCHAR filename[80];
		for (int i = 0; i <= 4; i++) //
		{
			_stprintf_s(filename, _T("bat%d.png"), i);
			IMAGE im;
			loadimage(&im, filename);
			ims_bat.push_back(im);
		}
		w = 50;
		h = 50;
		x = WIDTH;
		y = HEIGHT / 2 - h;
		//vx = 15;
		exist = 0;
		index = 0;
	}

	void getBat(int ph) {//随着land移动,概率出现
		if (!exist && rand() % 100 > 80) {
			exist = true;
			//相对人的高度 3中 下  跳过，上
			switch (rand() % 3)
			{
			case 0: y = ph + 10; break;
			case 1: y = ph - h; break;
			case 2: y = ph - 170; break;
			default:y = HEIGHT / 2; break;

			}
		}
	}

	void move() {
		if (x < 0) initialize();
		x -= vx;
	}

	void draw() {
		if (!exist) return;

		move();
		putimagePng(x, y, &ims_bat[index]);
		index++;
		if (index > 4) index = 0;
	}


};

class Star {
public:
	IMAGE im_star;
	float x;
	float y;
	bool exist;

	void initialize() {
		loadimage(&im_star, _T("star.png"));
		x = WIDTH;
		y = HEIGHT / 4;
		exist = false;
	}
	void getStar(int land) {//随着land移动,概率出现  15%
		if (!exist && rand() % 100 >= 30 && rand() % 100 <= 45) {
			exist = true;
			//y也随机一个高度  要在地面之上
			int h = im_star.getheight();
			if (rand() % 2 == 1) y = land - h - 10;
			else y = land - 170;
		}
	}

	void draw() {
		if (!exist) return;
		//printf("draw");
		putimagePng(x, y, &im_star);
	}

	void move(int speed) {
		if (x < 0) {
			initialize();
		}
		if (exist) {
			x -= speed;
		}
	}
};

class Player  // 玩家控制的游戏角色类
{
public:
	IMAGE im_show;  // 当前时刻要显示的图像
	IMAGE im_standright; // 向右站立图像
	IMAGE im_standleft; // 向左站立图像
	IMAGE im_jumpright; // 向右方向跳跃图像
	IMAGE im_jumpleft; // 向左方向跳跃图像
	vector <IMAGE> ims_runright; // 向右奔跑的图像序列
	vector <IMAGE> ims_runleft; // 向左奔跑的图像序列
	int animId;  // 用于循环动画播放的id
	PlayerStatus playerStatus; // 当前的状态
	float x_left, y_bottom; // 这两个坐标，因为只要用这两个和地面碰撞就行了
	float vx, vy; // 速度
	float gravity; // 重力加速度
	float width, height; // 图片的宽度、高度

	void draw()// 显示相关信息	
	{
		putimagePng(x_left, y_bottom - height, &im_show);  // 游戏中显示角色
	}

	void initialize() // 初始化
	{
		ims_runleft.clear(); // 先清空掉vector
		ims_runright.clear();
		loadimage(&im_standright, _T("standright.png")); // 导入向右站立图片
		loadimage(&im_standleft, _T("standleft.png")); // 导入向左站立图片
		loadimage(&im_jumpright, _T("jumpright.png")); // 导入向右方向跳跃图像
		loadimage(&im_jumpleft, _T("jumpleft.png")); // 导入向左方向跳跃图像

		playerStatus = standright; // 初始为向右站立的游戏状态
		im_show = im_standright;  // 初始显示向右站立的图片
		width = im_standright.getwidth(); // 获得图像的宽、高，所有动画图片大小一样
		height = im_standright.getheight();

		TCHAR filename[80];
		for (int i = 0; i <= 7; i++) // 把向右奔跑的八张图片对象添加到ims_runright中
		{
			_stprintf_s(filename, _T("runright%d.png"), i);
			IMAGE im;
			loadimage(&im, filename);
			ims_runright.push_back(im);
		}
		for (int i = 0; i <= 7; i++) // 把向左奔跑的八张图片对象添加到ims_runright中
		{
			_stprintf_s(filename, _T("runleft%d.png"), i);
			IMAGE im;
			loadimage(&im, filename);
			ims_runleft.push_back(im);
		}

		animId = 0; // 动画id开始设为0

		updateXY(WIDTH / 2, HEIGHT / 2); // 开始将角色放在画面中间
		vx = 2; // 水平方向初速度
		vy = 0;  // 竖直方向速度初始为0
		gravity = 3;  // 设定重力加速度
	}

	void updateXY(float mx, float my) // 根据输入，更新玩家坐标
	{
		x_left = mx;
		y_bottom = my;
	}

	void runRight(Scene& scene) // 游戏角色向右奔跑
	{
		//当向有跑动时，land移动
		for (int i = 0; i < scene.lands.size(); i++) {
			scene.lands[i].left_x -= scene.vx;
			scene.lands[i].right_x -= scene.vx;
		}


		if (x_left < WIDTH / 2) x_left += vx; // 横坐标增加，向右移动
		else x_left += 0;
		if (isNotOnAllLands(scene.lands, vy))  // 移动后不在任何一块地面上了
		{
			im_show = im_jumpright;// 切换到向右起跳图片
			playerStatus = jumpright;// 切换到向右起跳状态
			return;
		}

		if (playerStatus == jumpleft || playerStatus == jumpright) // 如果是起跳状态
		{
			im_show = im_jumpright; // 改变造型为向右起跳造型
		}
		else
		{
			if (playerStatus != runright) // 如果之前角色状态不是向右奔跑
			{
				playerStatus = runright; // 切换为向右奔跑状态
				animId = 0; // 动画播放id初始化为0
			}
			else // 表示之前就是向右奔跑状态了
			{
				animId++; // 动画图片开始切换
				if (animId >= ims_runright.size()) // 循环切换
					animId = 0;
			}
			im_show = ims_runright[animId];	 // 设置要显示的对应图片	 
		}
	}

	void runLeft(Scene& scene) // 游戏角色向左奔跑
	{
		x_left -= 10; // 横坐标减少，向左移动		
		if (isNotOnAllLands(scene.lands, vy))  // 移动后不在任何一块地面上了
		{
			im_show = im_jumpleft; // 切换到向左起跳图片
			playerStatus = jumpleft; // 切换到向左起跳状态
			return;
		}

		if (playerStatus == jumpleft || playerStatus == jumpright) // 如果是起跳状态
		{
			im_show = im_jumpleft; // 改变造型为向左起跳造型
		}
		else
		{
			if (playerStatus != runleft) // 如果之前角色状态不是向左奔跑
			{
				playerStatus = runleft; // 切换为向左奔跑状态
				animId = 0; // 动画播放id初始化为0
			}
			else // 之前就是向左奔跑状态了
			{
				animId++; // 动画图片开始切换
				if (animId >= ims_runleft.size()) // 循环切换
					animId = 0;
			}
			im_show = ims_runleft[animId];	 // 设置要显示的对应图片	
		}
	}

	void standStill() // 游戏角色默认为向左或向右静止站立
	{
		if (playerStatus == runleft || playerStatus == standleft)
		{
			im_show = im_standleft;
		}
		else if (playerStatus == runright || playerStatus == standright)
		{
			im_show = im_standright;
		}
	}

	void beginJump() // 按下w或向上方向键后，游戏角色跳跃的处理
	{
		if (playerStatus != jumpleft && playerStatus != jumpright) // 已经在空中的话，不要起跳
		{
			if (playerStatus == runleft || playerStatus == standleft)  // 起跳前是向左跑或向左站立状态
			{
				im_show = im_jumpleft; // 切换到向左起跳图片
				playerStatus = jumpleft; // 切换到向左起跳状态
			}
			else if (playerStatus == runright || playerStatus == standright)// 起跳前是向右跑或向右站立状态
			{
				im_show = im_jumpright;// 切换到向右起跳图片
				playerStatus = jumpright;// 切换到向右起跳状态
			}
			vy = -30; // 给角色一个向上的初速度
		}
	}

	// 判断游戏角色是否正站在这块地面上，如果是的话返回1，否则返回0
	int isOnLand(Land& land, float ySpeed)
	{
		float x_right = x_left + width;
		// 判断是否站在砖块上，还需要考虑player的y方向速度情况，速度过快有可能直接穿透地面
		if (ySpeed <= 0) // y轴方向速度小于0，表示正在向上运动，不需要考虑速度的影响
			ySpeed = 0;
		if (land.left_x - x_left <= width * 0.6 && x_right - land.right_x <= width * 0.6 && abs(y_bottom - land.top_y) <= 5 + ySpeed)
			return 1;
		else
			return 0;
	}

	int isNotOnAllLands(vector<Land>& lands, float speed) // 判断玩家是否不在所有的地面上
	{
		for (int i = 0; i < lands.size(); i++)
		{
			if (isOnLand(lands[i], speed))
				return 0; // 在任何一块地面上，返回0
		}
		return 1; // 不在所有地面上，返回1
	}

	void updateYcoordinate(Scene& scene) // x坐标是按键盘控制的，而y坐标是每帧自动更新的
	{
		if (isNotOnAllLands(scene.lands, vy) || playerStatus == jumpleft || playerStatus == jumpright) // 当前是在空中跳跃状态
		{
			vy += gravity; // y方向速度受重力影响变化
			y_bottom += vy;  // y方向位置受速度影响变化
			for (int i = 0; i < scene.lands.size(); i++)   // 对所有地面遍历
			{
				if (isOnLand(scene.lands[i], vy)) // 当火柴人正好站在一个地面上时
				{
					y_bottom = scene.lands[i].top_y; // 保证正好落在地面上
					if (playerStatus == jumpleft) // 向左跳，落地后切换到向左站立方向
						playerStatus = standleft;
					if (playerStatus == jumpright) // 向右跳，落地后切换到向右站立方向
						playerStatus = standright;
					break; // 跳出循环，不需要再对其他地面判断了
				}
			}
		}
	}

};

// 一些全局变量
Player player;  // 定义玩家控制的游戏角色对象
Scene scene;  // 定义场景全局对象
Bat1 bat1;
Bat2 bat2;
Star star;
Timer timer;  // 用于精确延时

int score = 0;
int life = 6;
IMAGE im_life;

int isHit(int x, int y, int w, int h) {
	if (x >= player.x_left + 20 - w && x <= player.x_left + player.width - 20 &&
		y >= player.y_bottom - h - player.height && y <= player.y_bottom) {
		return 1;
	}
	return 0;
}

void showScore() {
	//分数显示
	settextstyle(30, 0, _T("黑体"));
	settextcolor(RED);
	setbkmode(TRANSPARENT);   //将文字底纹设置为透明
	outtextxy(10, 80, "得分:");
	char str[20];
	sprintf_s(str, "%d", score);
	outtextxy(90, 80, str);

}

void showLife() {
	loadimage(&im_life, _T("life.png"));
	for (int i = 0; i < life; i++) {
		putimagePng(i * 35, 30, &im_life);
	}
}

void startup()  // 初始化
{
	srand(time(0)); // 初始化随机数种子
	scene.initialize();  // 场景初始化
	player.initialize(); // 玩家角色初始化
	bat1.initialize();
	star.initialize();
	bat2.initialize();

	initgraph(WIDTH, HEIGHT); // 新开一个画面
	BeginBatchDraw(); // 开始批量绘制
}

void show()  // 显示
{
	scene.draw();   // 显示场景相关信息
	player.draw();  // 显示玩家相关信息
	bat1.draw();
	star.draw();
	bat2.draw();

	showLife();
	showScore();

	FlushBatchDraw(); // 批量绘制
	timer.Sleep(50); // 暂停若干毫秒
}

void updateWithoutInput() // 和输入无关的更新
{
	player.updateYcoordinate(scene); // 游戏角色y坐标是每帧自动更新的


	if (scene.lands[0].right_x <= 10) {
		//printf("%f %f %d\n", bat1.x, bat1.y, bat1.exist);
		//printf("%f %f %d\n", star.x, star.y, star.exist);
		//生成的高度应该有限制，相对与land
		int land = int(scene.lands[scene.lands.size() - 1].top_y);
		bat1.getBat(player.y_bottom);
		star.getStar(land);
		bat2.getBat(player.y_bottom);
	}

	//蝙蝠和人物的碰撞
	if (isHit(bat1.x, bat1.y, bat1.im_bat.getwidth() - 20, bat1.im_bat.getheight())) {
		printf("碰到蝙蝠1\n");
		bat1.initialize();
		//掉一个血
		life--;
	}

	if (isHit(bat2.x, bat2.y, bat2.w, bat2.h)) {
		printf("碰到蝙蝠2\n");
		bat2.initialize();
		//掉一个血
		life--;
	}

	//star与人物的碰撞
	if (isHit(star.x, star.y, star.im_star.getwidth(), star.im_star.getwidth())) {
		printf("碰到星星\n");
		star.initialize();//star恢复初始化；
		score++;//分数加一
	}

}

void updateWithInput() // 和输入有关的更新
{
	player.standStill(); // 游戏角色默认为向左或向右静止站立

	if (_kbhit()) // 当按键时，切换角色显示图片，更改位置
	{
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))  // 按下D键或右方向键
		{
			player.runRight(scene);
			//
			bat1.move(scene.vx);
			star.move(scene.vx);
		}
		else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))// 按下A键或左方向键
		{
			player.runLeft(scene);
		}
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))     // 按下W键或上方向键
		{
			player.beginJump();
		}
	}
}

void gameLost() {
	settextstyle(50, 0, _T("黑体"));
	settextcolor(RED);
	setbkmode(TRANSPARENT);   //将文字底纹设置为透明
	outtextxy(WIDTH / 2 - textwidth("游戏结束") / 2, 250, "游戏结束!");
	settextstyle(30, 0, _T("黑体"));
	outtextxy(WIDTH / 2 - textwidth("点击空格键退出") / 2, 350, "点击空格退出");
	while (_getch() != ' ') {};

}
void gameNext() {
	settextstyle(50, 0, _T("黑体"));
	settextcolor(RED);
	setbkmode(TRANSPARENT);   //将文字底纹设置为透明
	outtextxy(WIDTH / 2 - textwidth("第一关通过") / 2, 250, "第一关通过");
	settextstyle(30, 0, _T("黑体"));
	outtextxy(WIDTH / 2 - textwidth("点击空格键开始第二关") / 2, 350, "点击空格开始第二关");
	while (_getch() != ' ') {};

}
int main() // 主函数
{
	startup();  	// 初始化
	while (1)       // 一直循环
	{
		show();  // 显示
		updateWithoutInput(); // 与输入无关的更新
		updateWithInput();    // 与输入有关的更新

		EndBatchDraw();
		if (life <= 0 || player.y_bottom >= HEIGHT) {
			gameLost();
			//_getch();
			break;
		}

		if (score >= 10 && scene.vx == 10) {
			gameNext();
			//Life++  
			life += 3;
			scene.vx = 15;
			bat2.vx = 20;
		}
	}
	closegraph();
	return 0;
}


