#include <Windows.h>	// COORD

// ũ��� Ȧ��������
const int MAP_WIDTH = 11;
const int MAP_HEIGHT = 11;

typedef enum _Direction {
	DIRECTION_LEFT,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_DOWN
} Direction;

typedef enum _MapFlag {
	MAP_FLAG_WALL,
	MAP_FLAG_EMPTY,
	MAP_FLAG_VISITED,
} MapFlag;

// ��, ��, ��, �� �̶� ���� ����Ͽ� 2ĭ������ �����Ѵ�.
const int DIR[4][2] = { {0,-2},{0,2},{-2,0},{2,0} };

void shuffleArray(int array[], int size)
{
	int i, r, temp;

	for (i = 0; i < (size - 1); ++i)
	{
		r = i + (rand() % (size - i));
		temp = array[i];
		array[i] = array[r];
		array[r] = temp;
	}
}

int inRange(int y, int x)
{
	return (y < MAP_HEIGHT - 1 && y > 0) && (x < MAP_WIDTH - 1 && x > 0);
}

// (x,y)���� ���̿켱Ž���� �ϸ� ���� �����Ѵ�.
//
// �ʱ⿡�� ��� ������ �Ǿ��ִ�. �湮���� ���� ������ Ž���ϸ� �湮�� ������ 
// ǥ���Ѵ�. �湮 �Ŀ��� ���� �վ� ���� �����Ѵ�.
void generateMap(int y, int x, int map[MAP_HEIGHT][MAP_WIDTH])
{
	int i, nx, ny;
	int directions[4] = {
		DIRECTION_UP,
		DIRECTION_RIGHT,
		DIRECTION_DOWN,
		DIRECTION_LEFT
	};

	map[y][x] = MAP_FLAG_VISITED;

	shuffleArray(directions, 4);

	for (i = 0; i < 4; i++)
	{
		// ���� ��ġ�� ���Ѵ�.
		nx = x + DIR[directions[i]][0];
		ny = y + DIR[directions[i]][1];

		if (inRange(ny, nx) && map[ny][nx] == MAP_FLAG_WALL)
		{
			generateMap(ny, nx, map);
			// ���� �� �̵��� ���
			if (ny != y)
				map[(ny + y) / 2][x] = MAP_FLAG_EMPTY;
			// ���� �� �̵��� ���
			else
				map[y][(x + nx) / 2] = MAP_FLAG_EMPTY;
			map[ny][nx] = MAP_FLAG_EMPTY;
		}
	}
}

COORD getRandomStartingPoint()
{
	int x = 1 + rand() % (MAP_WIDTH - 1);
	int y = 1 + rand() % (MAP_HEIGHT - 1);
	if (x % 2 == 0)
		x--;
	if (y % 2 == 0)
		y--;
	COORD a;
	a.X = x;
	a.Y = y;
	return a;
};

void MakeRandom()
{
	int map[MAP_HEIGHT][MAP_WIDTH];
	COORD startPoint = getRandomStartingPoint();

	srand((unsigned int)time(NULL));
	memset(map, MAP_FLAG_WALL, sizeof(map));

	generateMap(startPoint.Y, startPoint.X, map);
	map[1][0] = 'e';
	map[MAP_HEIGHT-2][MAP_WIDTH-1] = 'x';
	map[startPoint.Y][startPoint.X] = 1;
	FILE* fp = fopen("randomMaze.txt", "w");
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0;j < MAP_WIDTH;j++)
		{
			if (map[i][j] == 0)
			{
				map[i][j] = 1;
				fprintf(fp, "%d", map[i][j]);
			}
			else if (map[i][j] == 1)
			{
				map[i][j] = 0;
				fprintf(fp, "%d", map[i][j]);
			}
			else
				fputc(map[i][j], fp);
			if (j == MAP_WIDTH - 1)
				fputc('\n',fp);
		}
	fclose(fp);
}