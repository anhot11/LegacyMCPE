#pragma once

#include <vector>

#include "Node.h"
#include "java/System.h"

class Node;

class BinaryHeap {
private:
    std::vector<Node*> heap;
    int sizeVar;

    // 4J Jev, add common ctor code.
    void _init();

public:
    BinaryHeap();
    ~BinaryHeap();

    Node* insert(Node* node);
    void clear();
    Node* peek();
    Node* pop();
    void remove(Node* node);
    void changeCost(Node* node, float newCost);
    int size();

private:
    void upHeap(int idx);
    void downHeap(int idx);

public:
    bool isEmpty();
};
