//#define	debug // 调式模式
//#define	block // 输入阻塞

// 游戏几何参数
enum Geometric
{
	WIDTH_WINDOW = 591, // 窗口宽度
	HEIGHT_WINDOW = 864, // 窗口高度
	WIDTH_PLAYER = 117, // 玩家飞机宽度
	HEIGHT_PLAYER = 120, // 玩家飞机高度
	WIDTH_BIG = 104, // 大型敌机宽度
	HEIGHT_BIG = 148, // 大型敌机高度
	WIDTH_SMALL = 52, // 小型敌机宽度
	HEIGHT_SMALL = 39 // 小型敌机高度
};

enum ENTITY_NUM
{
	BULLET_NUM = 15, // 玩家飞机子弹数量
	ENEMY_NUM = 10, // 敌机数量
};

enum ENEMY_TYPE
{
	BIG_PLANE,
	SMALL_PALNE
};

#ifdef block
enum Key
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	SPACE = 32,
	ESC = 27
};
#endif // block

// 定义玩家飞机移动的最大坐标
enum MoveArea
{	
	MIN_X = -(WIDTH_PLAYER / 3),
	MIN_Y = 0,
	MAX_X = WIDTH_WINDOW - (WIDTH_PLAYER * 2 / 3),
	MAX_Y = HEIGHT_WINDOW - HEIGHT_PLAYER
};

// 定义定时器的枚举标记
enum TIMER
{
	Create_Enemy,
	Create_Bullet
};

// 最大定时器数目
#define TIMER_MAX 10

// 刷新延时
#define DELAY 20

// 定义飞机
typedef struct _Plane
{
	int x; // 飞机的水平位置
	int y; // 飞机的垂直位置
	int width; // 飞机的宽度
	int height; // 飞机的高度
	int hp; // 飞机的生命值
	int type; // 飞机的类型
	bool live; // 是否存活
} Plane;

// 定义子弹
typedef struct _Bullet
{
	int x;
	int y;
	bool live;
} Bullet;