#include "airplane_game.h"

// ���汳��ͼƬ
IMAGE img_bg;
// �������ͼƬ
IMAGE img_player[4];
// �����ӵ�ͼƬ
IMAGE img_bullet[2];
// ����л�ͼƬ
IMAGE img_enemy[2][2];

// ������������߳̾��
HANDLE shoot_handle;
// �ύ��Ŀ
// ���ս����Ϣ
Plane player;
// ����ӵ���Ϣ
Bullet bullets[BULLET_NUM];
// �л���Ϣ
Plane enemy[ENEMY_NUM];

int score = 0;

int main()
{
	// ��ʼ����Ϸ
	initGame();

	// ˫�����ͼ ��������ͼƬ�Ⱥ����������˸
	BeginBatchDraw();
	while (1)
	{
		// ������Ϸ֡
		drawGame();
		// ˢ�»�����
		FlushBatchDraw();
		// �����ʹ����ɫ�ƶ����ƶ�����һ��ѭ��ͳһ���л���
		movePlayer(7);
		// �����л�
		createEnemy(500);
		// �ƶ��л�
		moveEnemy(1);
		// �ӵ��ƶ�
		moveBullets(10);
		// ����Ƿ񹥻����л�
		damageEnemy();
		// ����Ƿ���ײ���л�
		if (testCrash())
		{
			// ������Ϸ
			drawGame();
			break;
		}

		// ˢ����ʱ
		Sleep(DELAY);
	}
	EndBatchDraw();

	_getch();

	return 0;
}

// ��ʼ����Ϸ��Ϣ
void initGame()
{
	// ����һ�����ڣ�����̨����ʱ�Զ������ģ���ͼ�δ�����Ҫ�����ֶ�����
	// �������������� SHOWCONSOLE ����ͬʱչʾ����̨���
#ifdef debug
	initgraph(WIDTH_WINDOW, HEIGHT_WINDOW, SHOWCONSOLE);
#else
	initgraph(WIDTH_WINDOW, HEIGHT_WINDOW);
#endif // debug

	// ���ر���ͼƬ
	// ���ܻᱨ��error c2665: "loadimage": ����������û��һ������ת�����в�������
	// ��������������Ϊ�ַ������ԣ�unicode �л�Ϊ ���ֽ��ַ��� �����
	loadimage(&img_bg, "./images/background.jpg");

	// �������ͼƬ
	loadimage(&img_player[0], "./images/planeNormal_1.jpg");
	loadimage(&img_player[1], "./images/planeNormal_2.jpg");	
	loadimage(&img_player[2], "./images/planeExplode_1.jpg");
	loadimage(&img_player[3], "./images/planeExplode_2.jpg");

	// �����ӵ�ͼƬ
	loadimage(&img_bullet[0], "./images/bullet1.jpg");
	loadimage(&img_bullet[1], "./images/bullet2.jpg");

	// ���صл�ͼƬ
	loadimage(&img_enemy[0][0], "./images/enemy_1.jpg");
	loadimage(&img_enemy[0][1], "./images/enemy_2.jpg");
	loadimage(&img_enemy[1][0], "./images/enemyPlane1.jpg");
	loadimage(&img_enemy[1][1], "./images/enemyPlane2.jpg");

	// �������������
	srand((unsigned int)time(NULL));

	// �򿪱��������ļ�
	mciSendString(_T("open ./images/game_music.mp3 alias bgm"), NULL, 0, NULL);
	// ���ű�������
	mciSendString(_T("play bgm repeat"), NULL, 0, NULL);

	// ��ʼ����ҽ�ɫ��Ϣ
	player.x = (WIDTH_WINDOW / 2) - (WIDTH_PLAYER / 2);
	player.y = HEIGHT_WINDOW - HEIGHT_PLAYER;
	player.live = true;
	player.width = WIDTH_PLAYER;
	player.height = HEIGHT_PLAYER;

	// ��ʼ���ӵ���Ϣ
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullets[i].x = 0;
		bullets[i].y = 0;
		bullets[i].live = false;
	}

	// ��ʼ���л���Ϣ
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].live = false;
	}
}

// ������Ϸ����
void drawGame()
{
	// ���Ʊ���ͼ
	putimage(0, 0, &img_bg);

	// ������ҷɻ���ͨ�����������������������ӡ����ͼƬ��ȥ���ɻ��ı���ɫ
	// ���һ������ָ���������
	if (player.live)
	{
		putimage(
			player.x,
			player.y,
			&img_player[0],
			NOTSRCERASE
		);
		putimage(
			player.x,
			player.y,
			&img_player[1],
			SRCINVERT
		);
	}
	else
	{
		putimage(
			player.x,
			player.y,
			&img_player[2],
			NOTSRCERASE
		);
		putimage(
			player.x,
			player.y,
			&img_player[3],
			SRCINVERT
		);
	}
	// �����ӵ�
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullets[i].live)
		{
			putimage(
				bullets[i].x,
				bullets[i].y,
				&img_bullet[0],
				NOTSRCERASE
			);
			putimage(
				bullets[i].x,
				bullets[i].y,
				&img_bullet[1],
				SRCINVERT
			);
		}
	}
	// ���Ƶл�
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{	
			int type = enemy[i].type == BIG_PLANE ? 1 : 0;
			putimage(
				enemy[i].x,
				enemy[i].y,
				&img_enemy[type][0],
				NOTSRCERASE
			);				
			putimage(
				enemy[i].x,
				enemy[i].y,
				&img_enemy[type][1],
				SRCINVERT
			);
		}
	}
}

// ��ʱ��
bool timer(int ms, enum TIMER id)
{
	static DWORD t[TIMER_MAX];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	return false;
}

// ���ս���ƶ�����ȡ������Ϣ����������
void movePlayer(int speed)
{
	// ���ַ�ʽ
#ifdef block
	// 1. _getch() �������룬windowsר��
	// _kbhit()����Ƿ��м��̰��£�������£������棬���򷵻ؼ�
	if (_kbhit())
	{
		char key = _getch();
		switch (key)
		{
			case 'w':
			case 'W':
			case UP:
				player.y -= speed;
				break;
			case 's':
			case 'S':
			case DOWN:
				player.y += speed;
				break;
			case 'a':
			case 'A':
			case LEFT:
				player.x -= speed;
				break;
			case 'd':
			case 'D':
			case RIGHT:
				player.x += speed;
				break;
			case ESC:
				exit(0);
				break;
			default:
				break;
        }
	}
#else
	// 2. ʹ��windows������ȡ�������� GetAsyncKeyState ��������������ǳ�����
	// ���Ҫ�����ĸ����������ʹ�ô�д����������ͬʱ��⵽��Сд
	// ���ʹ��Сд����ô���벻�ᱻ��⵽
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))
	{
		if (player.y > MIN_Y)
		{
			player.y -= speed;
		}
	}	
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
	{
		if (player.y < MAX_Y)
		{
			player.y += speed;
		}
	}	
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
	{
		if (player.x > MIN_X)
		{
			player.x -= speed;
		}
	}	
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
	{
		if (player.x < MAX_X)
		{
			player.x += speed;
		}
	}
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		exit(0);
	}
#endif // block

	// �����ӵ�
	if (GetAsyncKeyState(VK_SPACE))
	{
		createBullets(200);
	}
}

// ����һ���ӵ�
void createBullets(int span)
{
	if (!timer(span, Create_Bullet))
	{
		return;
	}
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bullets[i].live)
		{
			bullets[i].x = player.x + (WIDTH_PLAYER / 2) - 15;
			bullets[i].y = player.y;
			bullets[i].live = true;
			//// ͨ�����̲߳�������
			//shoot_handle = CreateThread(NULL, 0, playMusic , NULL , 0, 0);

			break;
		}
	}
}

// �ƶ��ӵ�
void moveBullets(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullets[i].live)
		{
			bullets[i].y -= speed;
			if (bullets[i].y < 0)
			{
				bullets[i].live = false;
			}
		}
	}
}

// ����һ���л�
void createEnemy(int span)
{
	if (!timer(span, Create_Enemy))
	{
		return;
	}
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			int r = rand() % 10;
			if (r >= 0 && r <= 1) // 0~9
			{
				enemy[i].type = BIG_PLANE;
				enemy[i].hp = 3;
				enemy[i].x = rand() % (WIDTH_WINDOW - WIDTH_BIG);
				enemy[i].y = -HEIGHT_BIG;
				enemy[i].width = WIDTH_BIG;
				enemy[i].height = HEIGHT_BIG;
			}
			else
			{
				enemy[i].type = SMALL_PALNE;
				enemy[i].hp = 1;
				enemy[i].x = rand() % (WIDTH_WINDOW - WIDTH_SMALL);
				enemy[i].y = -HEIGHT_SMALL;
				enemy[i].width = WIDTH_SMALL;
				enemy[i].height = HEIGHT_SMALL;
			}
			break;
		}
	}
}

// �ƶ��л�
void moveEnemy(int speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			enemy[i].y += enemy[i].type == BIG_PLANE ? speed : speed * 2;
			if (enemy[i].y > HEIGHT_WINDOW)
			{
				enemy[i].live = false;
			}
		}
	}
}

// �������֣����̻߳ص�����
DWORD WINAPI playMusic(LPVOID lp)
{
	// �ر�������֣��ȹرգ������ٴδ򿪺Ͳ��ţ�
	mciSendString("close shoot", NULL, NULL, NULL);
	// �������ļ�
	mciSendString("open ./images/f_gun.mp3 alias shoot", NULL, NULL, NULL);
	// ��������
	mciSendString("play shoot", NULL, NULL, NULL);
	// �ر��߳� ��Ч
	//CloseHandle(shoot_handle);

	return 0;
}

// �ж��ӵ��������л�
void damageEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			continue;
		}
		for (int j = 0; j < BULLET_NUM; j++)
		{
			if (!bullets[j].live)
			{
				continue;
			}
			if (
				bullets[j].x > enemy[i].x && bullets[j].x < enemy[i].x + enemy[i].width
				&&
				bullets[j].y > enemy[i].x && bullets[j].y < enemy[i].y + enemy[i].height
				)
			{
				bullets[j].live = false;
				enemy[i].hp--;
				// �ر���������
				mciSendString("close damage", NULL, NULL, NULL);
				// ����������
				mciSendString("open ./images/gotEnemy.mp3 alias damage", NULL, NULL, NULL);
				// ��������
				mciSendString("play damage", NULL, NULL, NULL);
			}
		}
		if (enemy[i].hp <= 0)
		{
			score++;
			enemy[i].live = false;
			createEnemy(100);
		}
	}
}

// ����Ƿ���ײ���л�
bool testCrash()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (
			player.x > enemy[i].x && player.x < enemy[i].x + enemy[i].width
			&&
			player.y > enemy[i].x && player.y < enemy[i].y + enemy[i].height
			)
		{
			player.live = false;
			// ��ײ�������ļ�
			mciSendString("open ./images/explode.mp3 alias explode", NULL, 0, NULL);
			// ����ײ������
			mciSendString("play explode", NULL, 0, NULL);
			return true;
		}
	}
	return false;
}