#ifndef QUEUES_H
#define QUEUES_H
#include <string>

using namespace std;

// Manual circular queue — TC: O(1) push/pop
class ManualQueue {
    int arr[200];
    int front;
    int rear;
    int count;

public:
    ManualQueue();

    void push(int x);
    void pop();
    int  getFront();
    bool isEmpty();
    int  size()
    {
        return count;
    }
};

// Priority task node (sorted linked list)
struct PTask {
    int priority;
    string name;
    string type;
    bool paused;
    PTask* next;
    PTask(int p, std::string n, std::string t) : priority(p), name(n), type(t), paused(false), next(nullptr) {}
};

// Manual priority queue
class PriorityQueue {
    PTask* head;
    int count;

public:
    PriorityQueue() : head(nullptr), count(0) {}
    ~PriorityQueue();

    void enqueue(int p, std::string name, std::string type);
    PTask* dequeue();

    bool isEmpty() 
    {
        return !head;
    }

    int size()   
    {
        return count;
    }

    PTask* peek()
    {
        return head;
    }

    void printAll();
};

class Queues {
public:
    static void demo();
    static void scheduleMenu(int choice);
};

#endif
