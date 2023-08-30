#pragma once //LinkedStack.h
#include "Node.h"

class LinkedStack
{
    Node* top;
public:
    LinkedStack() { top = NULL; }

    ~LinkedStack() { while (!isEmpty()) delete pop(); }

    bool isEmpty() { return top == NULL; }

    void push(Node* p)
    {
        if (isEmpty()) top = p;
        else {
            p->setLink(top);
            top = p;
        }
    }

    Node* pop()
    {
        if (isEmpty()) return NULL;
        Node* p = top;
        top = top->getLink();
        return p;
    }

    Node* peek() { return top; } //만들어놨으나, 사용할 일 없을 수도

    void display() //만들어놨으나, 사용할 일 없을 수도
    {
        printf("[스택 내용]\n");
        for (Node* p = top; p != NULL; p = p->getLink())
            p->display();
        printf("\n");
    }
};
