#pragma once //Node.h
#include <iostream>
#include <string>
using namespace std;

class Node {
	Node* link = nullptr; // 다음 노드를 가리키는 포인터 변수
	int row; // 현재 위치의 행 번호 (x 좌표)
	int col; // 현재 위치의 열 번호 (y 좌표)
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