//#define	debug // ��ʽģʽ
//#define	block // ��������

// ��Ϸ���β���
enum Geometric
{
	WIDTH_WINDOW = 591, // ���ڿ��
	HEIGHT_WINDOW = 864, // ���ڸ߶�
	WIDTH_PLAYER = 117, // ��ҷɻ����
	HEIGHT_PLAYER = 120, // ��ҷɻ��߶�
	WIDTH_BIG = 104, // ���͵л����
	HEIGHT_BIG = 148, // ���͵л��߶�
	WIDTH_SMALL = 52, // С�͵л����
	HEIGHT_SMALL = 39 // С�͵л��߶�
};

enum ENTITY_NUM
{
	BULLET_NUM = 15, // ��ҷɻ��ӵ�����
	ENEMY_NUM = 10, // �л�����
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

// ������ҷɻ��ƶ����������
enum MoveArea
{	
	MIN_X = -(WIDTH_PLAYER / 3),
	MIN_Y = 0,
	MAX_X = WIDTH_WINDOW - (WIDTH_PLAYER * 2 / 3),
	MAX_Y = HEIGHT_WINDOW - HEIGHT_PLAYER
};

// ���嶨ʱ����ö�ٱ��
enum TIMER
{
	Create_Enemy,
	Create_Bullet
};

// ���ʱ����Ŀ
#define TIMER_MAX 10

// ˢ����ʱ
#define DELAY 20

// ����ɻ�
typedef struct _Plane
{
	int x; // �ɻ���ˮƽλ��
	int y; // �ɻ��Ĵ�ֱλ��
	int width; // �ɻ��Ŀ��
	int height; // �ɻ��ĸ߶�
	int hp; // �ɻ�������ֵ
	int type; // �ɻ�������
	bool live; // �Ƿ���
} Plane;

// �����ӵ�
typedef struct _Bullet
{
	int x;
	int y;
	bool live;
} Bullet;