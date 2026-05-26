#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H
#include <string>

using namespace std;

struct Node {
    int data;
    double temp;
    double smoke;
    int zone;
    long timestamp;
    Node* next;

    Node(int v) : data(v), temp(0), smoke(0), zone(0), timestamp(0), next(nullptr) {}
    Node(int z, double t, double s, long ts) : data(z), temp(t), smoke(s), zone(z), timestamp(ts), next(nullptr) {}
};

struct DNode {
    int data;
    double temp;
    int zone;
    DNode* prev;
    DNode* next;
    DNode(int v) : data(v), temp(0), zone(0), prev(nullptr), next(nullptr) {}
    DNode(int z, double t) : data(z), temp(t), zone(z), prev(nullptr), next(nullptr) {}
};

class LinkedLists {
public:
    static void demo();
    static void eventMenu(int choice);
    static void printSingly(Node* head);
    static void printDoubly(DNode* head);
};

#endif
