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

// �ṹ������
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

// ��������Ϸ��������
int score;
int speed;
int kx;
int ky;
int lastX;
int lastY;

// ��������
void initSnake();
void initFood();
void initUI();
void initWall();
void hideCursor();
void displayScore();
void playGame(time_t limit_time);
void moreSetting();

// ����ʵ��
void initSnake() {
	snake.size = INITIAL_SNAKE_SIZE; // ��ʼ���ߵĳ���
	for (int i = 0; i < snake.size; i++) {
		snake.body[i].X = WIDE / 2 - i; // �߳�ʼλ��
		snake.body[i].Y = HIGH / 2;
	}
}

void initFood() {
	food.X = rand() % WIDE; // ���X����
	food.Y = rand() % HIGH; // ���Y����
}

void initUI() {
	COORD corrd = { 0 };
	// ����
	for (size_t i = 0; i < snake.size; i++) {
		corrd.X = snake.body[i].X;
		corrd.Y = snake.body[i].Y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corrd);
		putchar(i == 0 ? '@' : '*'); // ������ͷ������
	}

	// �����һ����β��λ��
	corrd.X = lastX;
	corrd.Y = lastY;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corrd);
	putchar(' ');

	// ʳ��
	corrd.X = food.X;
	corrd.Y = food.Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corrd);
	putchar('#');

	// ��ʾ����
	displayScore();

	// �ù�겻Ӱ����Ϸ��λ��
	corrd.X = 0;
	corrd.Y = HIGH + 3; // �÷������ߵ��·���ʾ
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corrd);
}

void displayScore() {
	COORD corrd = { 0 };
	corrd.Y = HIGH + 2; // ������ʾ�ڽ����·�
	corrd.X = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corrd);
	printf("����: %d", score); // �����ǰ����
}

void hideCursor() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1; // ����С
	cursorInfo.bVisible = 0; // ���ع��
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void initWall() {
	for (size_t i = 0; i <= WALL_BOTTOM; i++) {
		for (size_t j = 0; j <= WALL_RIGHT; j++) {
			if (j == WALL_RIGHT) {
				printf("|"); // �ұ߿�
			}
			else if (i == WALL_BOTTOM) {
				printf("_"); // �±߿�
			}
			else {
				printf(" "); // �հ�����
			}
		}
		printf("\n");
	}
}

void playGame(time_t limit_time) {
	char key = 'd'; // ��ʼ�ƶ�����
	while (1) {
		initUI(); // ������Ⱦ

		// ʱ������
		if (time(NULL) >= limit_time) {
			printf("ʱ�䵽,��Ϸ����");
			return;
		}

		// �����û�����
		if (_kbhit()) {
			key = _getch(); // ��ȡ����
		}

		// �����ߵ��ƶ�����
		switch (key) {
		case 'w': kx = 0; ky = -1; break; // ��
		case 's': kx = 0; ky = 1; break;  // ��
		case 'd': kx = 1; ky = 0; break;  // ��
		case 'a': kx = -1; ky = 0; break; // ��
		default: break;
		}

		// ������Ƿ�ײ���Լ�
		for (size_t i = 1; i < snake.size; i++) {
			if (snake.body[0].X == snake.body[i].X && snake.body[0].Y == snake.body[i].Y) {
				printf("��Ϸ��������ײ�����Լ���\n");
				return; // ������Ϸ
			}
		}

		// �����ͷ�Ƿ�ײ��ʳ��
		if (snake.body[0].X == food.X && snake.body[0].Y == food.Y) {
			initFood(); // ��������
			snake.size++; // �����ߵĳ���
			score += 10; // ���ӷ���
			speed = (speed > 10) ? speed - 10 : speed; // ����ٶ�
		}

		// ������β��λ��
		lastX = snake.body[snake.size - 1].X;
		lastY = snake.body[snake.size - 1].Y;

		// ���ƶ�
		for (size_t i = snake.size - 1; i > 0; i--) {
			snake.body[i] = snake.body[i - 1]; // ǰһ���������һ�����帳ֵ
		}
		snake.body[0].X += kx; // ������ͷX����
		snake.body[0].Y += ky; // ������ͷY����

		// �����ͷ�Ƿ�ײǽ
		if (snake.body[0].X < 0 || snake.body[0].X >= WALL_RIGHT || snake.body[0].Y < 0 || snake.body[0].Y >= WALL_BOTTOM) {
			printf("��Ϸ��������ײ���˱߿�\n");
			return; // ������Ϸ
		}

		Sleep(speed); // �����ٶȵ�����Ϸѭ�����ӳ�
	}
}

void moreSetting() {
	int option, speed_in, size_in, time_in;

	printf("��ӭ����yby��̰������Ϸ\n");
	do {
		printf("����0��ʼ��Ϸ, ����1�鿴��Ϸ����, ����2�˳�\n");
		scanf("%d", &option);
		switch (option) {
		case 0:
			printf("����0�����޾�ģʽ, ����1������սģʽ\n");
			scanf("%d", &option);
			if (option != 0) {
				printf("��������Ҫ��ս�Ĳ������ٶ� β�� ʱ�䣩��\n");
				scanf("%d%d%d", &speed_in, &size_in, &time_in);
				system("cls");
				speed = speed_in;
				snake.size = size_in;
				time_t start_time = time(NULL);
				time_t limit_time = start_time + time_in;
				initFood(); // ��ʼ��ʳ��
				initWall(); // ��ʼ��ǽ��
				playGame(limit_time);
				return;
			}
			else {
				system("cls");
				time_t limit_time = time(NULL) + 99999; // ����һ���ϳ���ʱ�䣬������Ϸ����
				initFood(); // ��ʼ��ʳ��
				initWall(); // ��ʼ��ǽ��
				playGame(limit_time);
				return;
			}
			break;
		case 1:
			printf("��ʼ�ٶ�: %d, ��ʼβ��: %d\n", speed, snake.size);
			break;
		case 2:
			exit(0);
		default:
			printf("��Ч���룬�����ԡ�\n");
		}
	} while (1);
}

int main(void) {
	srand(time(NULL)); // �����������
	hideCursor(); // ���ع��
	initSnake(); // ��ʼ����
	moreSetting(); // �˵�

	system("pause"); // �ȴ��û�����
	return EXIT_SUCCESS; // ����ɹ�����
}