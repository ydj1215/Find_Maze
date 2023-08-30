#pragma once //Node.h
#include <iostream>
#include <string>
using namespace std;

class Node {
	Node* link = nullptr; // ���� ��带 ����Ű�� ������ ����
	int row; // ���� ��ġ�� �� ��ȣ (x ��ǥ)
	int col; // ���� ��ġ�� �� ��ȣ (y ��ǥ)
public:
	Node(int r, int c) : row(r), col(c) {}
	int getRow() { return row; }
	int getCol() { return col; }
	void setRow(int r) { this->row = r; }
	void setCol(int c) { this->col = c; }
	Node* getLink() { return link; }
	void setLink(Node* next) { link = next; }
	void display() { printf("[%d,%d],", row, col); }
};