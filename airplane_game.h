#include <stdio.h> // 标准输入输出函数
#include <graphics.h> // 图形库，可以帮助新手快速入门图形编程：easyx
#include <conio.h> // _gatch() _kbhit()
#include <time.h> // 获取时间
#include <mmsystem.h> // 播放音乐头文件
#pragma comment(lib, "winmm.lib") //告诉编译器，加载winmm.lib静态库文件

#include "macro.h" // 飞机大战需要使用的宏、枚举和结构体定义

void initGame();

void drawGame();

void movePlayer(int speed);

void createBullets(int span);

void moveBullets(int speed);

void createEnemy(int span);

void moveEnemy(int speed);

DWORD WINAPI playMusic(LPVOID lp);

void damageEnemy();

bool timer(int ms, enum TIMER id);

bool testCrash();