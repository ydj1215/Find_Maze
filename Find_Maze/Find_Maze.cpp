#define _CRT_SECURE_NO_WARNINGS
#include "LinkedStack.h" //�̷� Ž�� ����.cpp
#include "LinkedQueue.h"
#include "RandomMaze.h"
#define MAXCAP 100

// ���� ����
static string map[MAXCAP];
static string savedStackMap[MAXCAP];
static int MAZE_HEIGHT;
static int MAZE_WIDTH;
static Node* entry = nullptr;
static Node* exit_ = nullptr;

//ť�� �̿��� ��� ��� ����
static LinkedQueue* stackPath = nullptr; // ���� ��� ���� ť
static LinkedQueue* queuePath = nullptr; // ť ��� ���� ť

// �Լ� ����
bool isValidLoc(int r, int c);
void printMap();
void GetMap(int ran);
void StackMaze(int ran);
void QueueMaze(int ran);
void StackMapSave();
void PrintSavedStackMap();

void main()
{
	int a = 0, ran = 0;
	string str;
	do
	{
		printf("[1] ���� ����� �̿�, [2] ���� �̷�, [3] ����\n");
		printf("�Է� : ");
		cin >> str;
		system("cls"); // �ܼ� â �����

		try {
			a = stoi(str); // stoi() �� ���ڿ��� ������ ��ȯ�ϴ� �Լ�
		}
		catch (exception e)
		{
			printf("�ùٸ� ���ڸ� �Է����ּ���.\n");
			continue;
		}

		if (a == 1) ran = 1; // [1] ���� ����� �̿�
		else if (a == 2) // [2] ���� �̷�
		{
			ran = 2;
			MakeRandom();
		}
		else if (a == 3) break;
		do
		{
			printf("[1] ����&ť �̷�Ž��, [2] ���� �̷�Ž�� (���� �켱), [3] ť �̷�Ž�� (�ʺ� �켱), [4] �ڷΰ��� \n");
			printf("�Է� : ");
			cin >> str;
			system("cls");
			try {
				a = stoi(str);
			}
			catch (exception e)
			{
				printf("�ùٸ� ���ڸ� �Է����ּ���.\n");
			}
			switch (a)
			{
			case 1:
				StackMaze(ran);
				QueueMaze(ran);
				PrintSavedStackMap();
				printf("\n���� �̷� Ž�� ����\n");
				printf("\n���� ��� ���: "); stackPath->display();
				printf("\n");
				delete stackPath;
				delete queuePath;
				stackPath = nullptr;
				queuePath = nullptr;
				break;
			case 2:
				StackMaze(ran);
				delete stackPath;
				stackPath = nullptr;
				break;
			case 3:
				QueueMaze(ran);
				delete queuePath;
				queuePath = nullptr;
				break;
			case 4:
				break;
			default:
				cout << "�ùٸ� ���ڸ� �Է����ּ���." << endl;
			}
		} while (a != 4);
	} while (1);
}

// �Լ� ����
// ���� ��������� �̷� �޾ƿ��� �Լ�
void GetMap(int ran)
{
	FILE* getting;
	if (ran == 1)
		getting = fopen("input.txt", "r"); // input.txt�� ���� getting ������ �о���δ�.
	else
		getting = fopen("randomMaze.txt", "r");
	char temp[100];// �޾ƿ� ������ �ӽ÷� �޴� ����
	for (int i = 0; i < MAXCAP; i++)      // �ִ�뷮���� �ݺ��� ����
	{
		if (!fgets(temp, 100, getting))     // �޾ƿ� ���� �����̸�,
		{
			MAZE_HEIGHT = i;                // i = �̷��� ���α���
			break;                          // ���� Ż��.
		}
		else map[i] = temp;                // �޾ƿ� ���� ���뿡 �����Ѵٸ�, map�� i��° �ٿ� �����Ѵ�.
	}
	MAZE_WIDTH = map[0].size();         // ������ Ż���ϸ�, ù��° ��Ʈ���� ũ�Ⱑ �̷��� ���α��̰� �ȴ�. �̷δ� �簢�� �̹Ƿ� ���° ���ڿ��� �� �������� �����Ѵ�.
	fclose(getting);
}

// �ش� ��ġ ��ȿ�� Ȯ�� �Լ�
bool isValidLoc(int r, int c)
{
	if (r < 0 || c < 0 || r >= MAZE_HEIGHT || c >= MAZE_WIDTH) return false; //�迭 ���� ���� �̷θ� �����ϸ� false ��ȯ
	else return map[r][c] == '0' || map[r][c] == 'x'; // ���̳� �ⱸ�� true�� ��ȯ. ���̸� false ��ȯ
}

// ���ü� ��� �Լ�
void printMap()
{
	printf("���͸� �Է����ּ���.");
	if (cin.get() == '\n')
		system("cls");
	for (int i = 0; i < MAZE_HEIGHT; i++)
	{
		for (int j = 0; j < MAZE_WIDTH; j++)
		{
			if (map[i][j] == '1') printf("��"); // ��
			else if (map[i][j] == '0') printf("��"); // ��
			else if (map[i][j] == 'e' || (i == entry->getRow() && j == entry->getCol())) // �Ա�
			{
				printf("��");
				// �Ա� ��ġ ����
				if (map[i][j] == 'e')
				{
					entry->setRow(i);
					entry->setCol(j);
				}
			}
			else if (map[i][j] == 'x' || (i == exit_->getRow() && j == exit_->getCol())) {
				printf("��");
				if (map[i][j] == 'x')
				{
					exit_->setRow(i);
					exit_->setCol(j);
				}
			}// �ⱸ
			else if (map[i][j] == '.') printf("��"); // ������ ��θ� ǥ��
			else if (map[i][j] == '2') printf("��");
		}
		printf("\n");
	}
}

// ���� �̷� Ž���ϴ� �Լ�
void StackMaze(int ran) 
{
	stackPath = new LinkedQueue();
	entry = new Node(0, 0);
	exit_ = new Node(0, 0);
	GetMap(ran); // input.txt�κ��� �̷� �ҷ�����

	int stackSuccess = 0; // ���� �̷� Ž�� ���� ���� Ȯ�ο�
	printMap(); // �� ���, ���ÿ� �Ա� ��ġ ���
	LinkedStack locStack;
	locStack.push(entry); // ���ÿ� �Ա� ��ġ�� ����ִ´�.

	while (locStack.isEmpty() == false)
	{
		Node* here = locStack.pop(); // ���� ��ġ ��带 �ҷ��´�.

		int r = here->getRow();
		int c = here->getCol();
		stackPath->enqueue(new Node(r, c)); // ���� ��ġ�� ����
		if (map[r][c] == 'x') // ���� ��ġ�� �ⱸ�̸� ����
		{
			printMap();
			printf("\n���� �̷� Ž�� ����\n\n");
			printf("���� ��� ���: "); stackPath->display();
			printf("\n");
			stackSuccess = 1;
			break;
		}
		else
		{
			map[r][c] = '.'; // ���� ��ġ�� "������" ó��
			// ���� ��ġ���� �����¿� �� �� �� �ִ� ��� ��θ� ���ÿ� �߰�
			if (isValidLoc(r - 1, c)) locStack.push(new Node(r - 1, c)); // ��
			if (isValidLoc(r + 1, c)) locStack.push(new Node(r + 1, c)); // ��
			if (isValidLoc(r, c - 1)) locStack.push(new Node(r, c - 1)); // ��
			if (isValidLoc(r, c + 1)) locStack.push(new Node(r, c + 1)); // ��
		}
		printMap();
	}
	if (stackSuccess == 0)
		printf("\n���� �̷� Ž�� ����\n\n");
	// ���� & ť ���� ������ ��, ť�� ������ ���� ���� �ٽ� ������ֱ� ���� ����
	StackMapSave();
	savedStackMap[entry->getRow()][entry->getCol()] = 'e'; // entry ��尡 �����ǹǷ� �� ���� ����
	delete exit_;
	delete entry;
}

// ť �̷� Ž���ϴ� �Լ�
void QueueMaze(int ran) 
{
	queuePath = new LinkedQueue();
	entry = new Node(0, 0);
	exit_ = new Node(0, 0);
	GetMap(ran);
	int queueSuccess = 0; // ť �̷� Ž�� ���� ���� Ȯ�ο�

	printMap();
	LinkedQueue locQueue;
	locQueue.enqueue(entry);

	while (locQueue.isEmpty() == false)
	{
		Node* here = locQueue.dequeue(); // ���� ��ġ ��� �ҷ���

		int r = here->getRow();
		int c = here->getCol();
		queuePath->enqueue(new Node(r, c)); // ���� ��ġ ����
		if (map[r][c] == map[exit_->getRow()][exit_->getCol()]) // ���� ��ġ�� �ⱸ�̸� ���� 
		{
			printMap();
			printf("\nť �̷� Ž�� ����\n\n");
			printf("ť ��� ���: "); queuePath->display();
			printf("\n");
			queueSuccess = 1;
			break;
		}
		else {
			map[r][c] = '.'; // ���� ��ġ�� "������" ó��
			// ���� ��ġ���� �����¿� �� �� �� �ִ� ��� ��θ� ť�� �߰�
			if (isValidLoc(r - 1, c)) {
				locQueue.enqueue(new Node(r - 1, c));
				map[r - 1][c] = '2';
			}; // ��
			if (isValidLoc(r + 1, c))
			{
				locQueue.enqueue(new Node(r + 1, c));
				map[r + 1][c] = '2';
			}// ��
			if (isValidLoc(r, c - 1)) {
				locQueue.enqueue(new Node(r, c - 1));
				map[r][c - 1] = '2';
			}
			// ��
			if (isValidLoc(r, c + 1))
			{
				locQueue.enqueue(new Node(r, c + 1));
				map[r][c + 1] = '2';
			}// ��
		}
		printMap();
	}
	if (queueSuccess == 0)
		printf("\nť �̷� Ž�� ����\n\n");
	delete entry;
	delete exit_;
}

// ���� �� �����ϴ� �Լ�
void StackMapSave()
{
	for (int i = 0; i < MAXCAP; i++)
	{
		if (map[i] != "")
			savedStackMap[i] = map[i];
	}
}

void PrintSavedStackMap()
{
	for (int i = 0; i < MAZE_HEIGHT; i++)
	{
		for (int j = 0; j < MAZE_WIDTH; j++)
		{
			if (savedStackMap[i][j] == '1') printf("��"); // ��
			else if (savedStackMap[i][j] == '0') printf("��"); // ��
			else if (savedStackMap[i][j] == 'e') // �Ա�
			{
				printf("��");
			}
			else if (savedStackMap[i][j] == 'x') printf("��"); // �ⱸ
			else if (savedStackMap[i][j] == '.') printf("��"); // ������ ���
		}
		printf("\n");
	}
}