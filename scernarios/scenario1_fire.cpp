#include <iostream>
#include <string>
#include "scenarios.h"
#include "../monitoring/monitoring.h"
#include "../console_ui.h"

using namespace std;

// Manual Priority Queue (Sorted Linked List) 
// Time Complexity: O(n) insert, O(1) delete
struct Task {
    int priority;
    string description;
    Task* next;
    Task(int p, string d) : priority(p), description(d), next(nullptr) {}
};

class ManualPriorityQueue {
    Task* head;

public:
    ManualPriorityQueue() : head(nullptr) {}
    ~ManualPriorityQueue()
    {
        while (head) 
        {
            Task* t = head; 
            head = head->next; 
            delete t; 
        }
    }

    void enqueue(int priority, string desc) 
    {
        Task* n = new Task(priority, desc);
        if (!head || head->priority < priority)
        {
            n->next = head;
            head = n;
        }

        else
        {
            Task* cur = head;
            while (cur->next && cur->next->priority >= priority)
            {
                cur = cur->next;
            }

            n->next = cur->next; cur->next = n;
        }
    }

    void dequeue() 
    {
        if (!head)
        {
            return;
        }

        Task* t = head; 
        head = head->next;
        delete t;
    }

    Task* getTop()
    {
        return head;
    }

    bool isEmpty()
    {
        return head == nullptr;
    }
};

void scenario1_fire()
{
    printBanner("SCENARIO 1: CASCADING FIRE");
    Monitoring mon("Scenario 1 Execution");

    // Sensor readings 
    printSubHeader("Live Sensor Readings");
    printTableHeader("Zone", "Temp (C) | Smoke | Status", 8, 30);
    printTableRow("Zone 3", "60 C  | 85  | FIRE", 8, 30, BRED);
    printTableRow("Zone 4", "45 C  | 55  | WARNING", 8, 30, BYELLOW);
    printTableRow("Zone 6", "30 C  | 20  | MONITOR", 8, 30, BGREEN);
    printTableFooter(8, 30);

    // Priority dispatch
    printSubHeader("Emergency Dispatch  (Priority Queue)");
    printInfo("TC: O(n) insert | O(1) dequeue");

    ManualPriorityQueue dispatch;
    dispatch.enqueue(10, "Zone A (Catastrophic)");
    dispatch.enqueue(5, "Zone B (Moderate)");
    dispatch.enqueue(8, "Zone C (Severe)");

    printInfo("Dispatch order:");
    int step = 1;
    while (!dispatch.isEmpty())
    {
        Task* t = dispatch.getTop();
        const char* col = (t->priority >= 9) ? BRED : (t->priority >= 7) ? BYELLOW : BGREEN;
        cout << "  " << CYAN << "[" << step++ << "]" << RESET << "  " << col << t->description << RESET << "  Priority: " << BOLD << col << t->priority << RESET << endl;
        dispatch.dequeue();
    }

    // Resource allocation 
    printSubHeader("Water Resource Allocation");
    printBar("Zone 3", 90);
    printBar("Zone 4", 55);
    printBar("Zone 6", 20);

    printDivider();
    printFire("Scenario 1 complete. Zone 3 contained.");
}