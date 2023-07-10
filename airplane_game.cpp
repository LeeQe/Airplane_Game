#include "airplane_game.h"

// 保存背景图片
IMAGE img_bg;
// 保存玩家图片
IMAGE img_player[4];
// 保存子弹图片
IMAGE img_bullet[2];
// 保存敌机图片
IMAGE img_enemy[2][2];

// 保存射击音乐线程句柄
HANDLE shoot_handle;
// 提交项目
// 玩家战机信息
Plane player;
// 玩家子弹信息
Bullet bullets[BULLET_NUM];
// 敌机信息
Plane enemy[ENEMY_NUM];

int score = 0;

int main()
{
	// 初始化游戏
	initGame();

	// 双缓冲绘图 避免两张图片先后输出导致闪烁
	BeginBatchDraw();
	while (1)
	{
		// 绘制游戏帧
		drawGame();
		// 刷新缓冲区
		FlushBatchDraw();
		// 监听和处理角色移动。移动后下一次循环统一进行绘制
		movePlayer(7);
		// 产生敌机
		createEnemy(500);
		// 移动敌机
		moveEnemy(1);
		// 子弹移动
		moveBullets(10);
		// 检测是否攻击到敌机
		damageEnemy();
		// 检测是否碰撞到敌机
		if (testCrash())
		{
			// 结束游戏
			drawGame();
			break;
		}

		// 刷新延时
		Sleep(DELAY);
	}
	EndBatchDraw();

	_getch();

	return 0;
}

// 初始化游戏信息
void initGame()
{
	// 创建一个窗口，控制台窗口时自动创建的，而图形窗口需要我们手动创建
	// 第三个参数传入 SHOWCONSOLE 可以同时展示控制台输出
#ifdef debug
	initgraph(WIDTH_WINDOW, HEIGHT_WINDOW, SHOWCONSOLE);
#else
	initgraph(WIDTH_WINDOW, HEIGHT_WINDOW);
#endif // debug

	// 加载背景图片
	// 可能会报错：error c2665: "loadimage": 两个重载中没有一个可以转换所有参数类型
	// 参数类型有误，因为字符集不对，unicode 切换为 多字节字符集 后可用
	loadimage(&img_bg, "./images/background.jpg");

	// 加载玩家图片
	loadimage(&img_player[0], "./images/planeNormal_1.jpg");
	loadimage(&img_player[1], "./images/planeNormal_2.jpg");	
	loadimage(&img_player[2], "./images/planeExplode_1.jpg");
	loadimage(&img_player[3], "./images/planeExplode_2.jpg");

	// 加载子弹图片
	loadimage(&img_bullet[0], "./images/bullet1.jpg");
	loadimage(&img_bullet[1], "./images/bullet2.jpg");

	// 加载敌机图片
	loadimage(&img_enemy[0][0], "./images/enemy_1.jpg");
	loadimage(&img_enemy[0][1], "./images/enemy_2.jpg");
	loadimage(&img_enemy[1][0], "./images/enemyPlane1.jpg");
	loadimage(&img_enemy[1][1], "./images/enemyPlane2.jpg");

	// 设置随机数种子
	srand((unsigned int)time(NULL));

	// 打开背景音乐文件
	mciSendString(_T("open ./images/game_music.mp3 alias bgm"), NULL, 0, NULL);
	// 播放背景音乐
	mciSendString(_T("play bgm repeat"), NULL, 0, NULL);

	// 初始化玩家角色信息
	player.x = (WIDTH_WINDOW / 2) - (WIDTH_PLAYER / 2);
	player.y = HEIGHT_WINDOW - HEIGHT_PLAYER;
	player.live = true;
	player.width = WIDTH_PLAYER;
	player.height = HEIGHT_PLAYER;

	// 初始化子弹信息
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullets[i].x = 0;
		bullets[i].y = 0;
		bullets[i].live = false;
	}

	// 初始化敌机信息
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].live = false;
	}
}

// 绘制游戏画面
void drawGame()
{
	// 绘制背景图
	putimage(0, 0, &img_bg);

	// 绘制玩家飞机。通过二进制掩码操作，连续打印两个图片，去除飞机的背景色
	// 最后一个参数指定掩码操作
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
	// 绘制子弹
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
	// 绘制敌机
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

// 定时器
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

// 玩家战机移动，获取键盘消息，上下左右
void movePlayer(int speed)
{
	// 两种方式
#ifdef block
	// 1. _getch() 阻塞输入，windows专有
	// _kbhit()检测是否有键盘按下，如果按下，返回真，否则返回假
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
	// 2. 使用windows函数获取键盘输入 GetAsyncKeyState 非阻塞函数，会非常流畅
	// 如果要检测字母按键，必须使用大写，这样可以同时检测到大小写
	// 如果使用小写，那么输入不会被检测到
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

	// 发射子弹
	if (GetAsyncKeyState(VK_SPACE))
	{
		createBullets(200);
	}
}

// 产生一个子弹
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
			//// 通过多线程播放音乐
			//shoot_handle = CreateThread(NULL, 0, playMusic , NULL , 0, 0);

			break;
		}
	}
}

// 移动子弹
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

// 产生一个敌机
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

// 移动敌机
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

// 播放音乐，多线程回调函数
DWORD WINAPI playMusic(LPVOID lp)
{
	// 关闭射击音乐（先关闭，才能再次打开和播放）
	mciSendString("close shoot", NULL, NULL, NULL);
	// 打开音乐文件
	mciSendString("open ./images/f_gun.mp3 alias shoot", NULL, NULL, NULL);
	// 播放音乐
	mciSendString("play shoot", NULL, NULL, NULL);
	// 关闭线程 无效
	//CloseHandle(shoot_handle);

	return 0;
}

// 判定子弹攻击到敌机
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
				// 关闭命中音乐
				mciSendString("close damage", NULL, NULL, NULL);
				// 打开命中音乐
				mciSendString("open ./images/gotEnemy.mp3 alias damage", NULL, NULL, NULL);
				// 播放音乐
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

// 检测是否碰撞到敌机
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
			// 打开撞击音乐文件
			mciSendString("open ./images/explode.mp3 alias explode", NULL, 0, NULL);
			// 播放撞击音乐
			mciSendString("play explode", NULL, 0, NULL);
			return true;
		}
	}
	return false;
}