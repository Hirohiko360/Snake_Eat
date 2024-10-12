#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define WIDE 60
#define HIGH 20
#define WALL_RIGHT WIDE
#define WALL_BOTTOM HIGH
#define INITIAL_SNAKE_SIZE 5
#define INITIAL_SPEED 100

// 结构体声明
struct BODY {
	int X;
	int Y;
};

struct SNAKE {
	struct BODY body[WIDE * HIGH];
	int size;
}snake;

struct Food {
	int X;
	int Y;
}food;

// 分数和游戏参数对象
int score;
int speed;
int kx;
int ky;
int lastX;
int lastY;

// 函数声明
void initSnake();
void initFood();
void initUI();
void initWall();
void hideCursor();
void displayScore();
void playGame(time_t limit_time);
void moreSetting();

// 函数实现
void initSnake() {
	snake.size = INITIAL_SNAKE_SIZE; // 初始化蛇的长度
	for (int i = 0; i < snake.size; i++) {
		snake.body[i].X = WIDE / 2 - i; // 蛇初始位置
		snake.body[i].Y = HIGH / 2;
	}
}

void initFood() {
	food.X = rand() % WIDE; // 随机X坐标
	food.Y = rand() % HIGH; // 随机Y坐标
}

void initUI() {
	COORD corrd = { 0 };
	// 画蛇
	for (size_t i = 0; i < snake.size; i++) {
		corrd.X = snake.body[i].X;
		corrd.Y = snake.body[i].Y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corrd);
		putchar(i == 0 ? '@' : '*'); // 绘制蛇头或蛇身
	}

	// 清空上一个蛇尾的位置
	corrd.X = lastX;
	corrd.Y = lastY;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corrd);
	putchar(' ');

	// 食物
	corrd.X = food.X;
	corrd.Y = food.Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corrd);
	putchar('#');

	// 显示分数
	displayScore();

	// 让光标不影响游戏的位置
	corrd.X = 0;
	corrd.Y = HIGH + 3; // 让分数在蛇的下方显示
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corrd);
}

void displayScore() {
	COORD corrd = { 0 };
	corrd.Y = HIGH + 2; // 分数显示在界面下方
	corrd.X = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corrd);
	printf("分数: %d", score); // 输出当前分数
}

void hideCursor() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1; // 光标大小
	cursorInfo.bVisible = 0; // 隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void initWall() {
	for (size_t i = 0; i <= WALL_BOTTOM; i++) {
		for (size_t j = 0; j <= WALL_RIGHT; j++) {
			if (j == WALL_RIGHT) {
				printf("|"); // 右边框
			}
			else if (i == WALL_BOTTOM) {
				printf("_"); // 下边框
			}
			else {
				printf(" "); // 空白区域
			}
		}
		printf("\n");
	}
}

void playGame(time_t limit_time) {
	char key = 'd'; // 初始移动方向
	while (1) {
		initUI(); // 重新渲染

		// 时间限制
		if (time(NULL) >= limit_time) {
			printf("时间到,游戏结束");
			return;
		}

		// 接受用户输入
		if (_kbhit()) {
			key = _getch(); // 获取按键
		}

		// 更新蛇的移动方向
		switch (key) {
		case 'w': kx = 0; ky = -1; break; // 上
		case 's': kx = 0; ky = 1; break;  // 下
		case 'd': kx = 1; ky = 0; break;  // 右
		case 'a': kx = -1; ky = 0; break; // 左
		default: break;
		}

		// 检查蛇是否撞到自己
		for (size_t i = 1; i < snake.size; i++) {
			if (snake.body[0].X == snake.body[i].X && snake.body[0].Y == snake.body[i].Y) {
				printf("游戏结束！你撞上了自己！\n");
				return; // 结束游戏
			}
		}

		// 检查蛇头是否撞到食物
		if (snake.body[0].X == food.X && snake.body[0].Y == food.Y) {
			initFood(); // 重置坐标
			snake.size++; // 增加蛇的长度
			score += 10; // 增加分数
			speed = (speed > 10) ? speed - 10 : speed; // 提高速度
		}

		// 保存蛇尾的位置
		lastX = snake.body[snake.size - 1].X;
		lastY = snake.body[snake.size - 1].Y;

		// 蛇移动
		for (size_t i = snake.size - 1; i > 0; i--) {
			snake.body[i] = snake.body[i - 1]; // 前一节身体给后一节身体赋值
		}
		snake.body[0].X += kx; // 更新蛇头X坐标
		snake.body[0].Y += ky; // 更新蛇头Y坐标

		// 检查蛇头是否撞墙
		if (snake.body[0].X < 0 || snake.body[0].X >= WALL_RIGHT || snake.body[0].Y < 0 || snake.body[0].Y >= WALL_BOTTOM) {
			printf("游戏结束！你撞上了边框！\n");
			return; // 结束游戏
		}

		Sleep(speed); // 根据速度调整游戏循环的延迟
	}
}

void moreSetting() {
	int option, speed_in, size_in, time_in;

	printf("欢迎游玩yby的贪吃蛇游戏\n");
	do {
		printf("输入0开始游戏, 输入1查看游戏参数, 输入2退出\n");
		scanf("%d", &option);
		switch (option) {
		case 0:
			printf("输入0进入无尽模式, 输入1进入挑战模式\n");
			scanf("%d", &option);
			if (option != 0) {
				printf("请输入您要挑战的参数（速度 尾巴 时间）：\n");
				scanf("%d%d%d", &speed_in, &size_in, &time_in);
				system("cls");
				speed = speed_in;
				snake.size = size_in;
				time_t start_time = time(NULL);
				time_t limit_time = start_time + time_in;
				initFood(); // 初始化食物
				initWall(); // 初始化墙体
				playGame(limit_time);
				return;
			}
			else {
				system("cls");
				time_t limit_time = time(NULL) + 99999; // 设置一个较长的时间，保持游戏运行
				initFood(); // 初始化食物
				initWall(); // 初始化墙体
				playGame(limit_time);
				return;
			}
			break;
		case 1:
			printf("初始速度: %d, 初始尾巴: %d\n", speed, snake.size);
			break;
		case 2:
			exit(0);
		default:
			printf("无效输入，请重试。\n");
		}
	} while (1);
}

int main(void) {
	srand(time(NULL)); // 设置随机种子
	hideCursor(); // 隐藏光标
	initSnake(); // 初始化蛇
	moreSetting(); // 菜单

	system("pause"); // 等待用户按键
	return EXIT_SUCCESS; // 程序成功结束
}