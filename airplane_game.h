#include <stdio.h> // ��׼�����������
#include <graphics.h> // ͼ�ο⣬���԰������ֿ�������ͼ�α�̣�easyx
#include <conio.h> // _gatch() _kbhit()
#include <time.h> // ��ȡʱ��
#include <mmsystem.h> // ��������ͷ�ļ�
#pragma comment(lib, "winmm.lib") //���߱�����������winmm.lib��̬���ļ�

#include "macro.h" // �ɻ���ս��Ҫʹ�õĺꡢö�ٺͽṹ�嶨��

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