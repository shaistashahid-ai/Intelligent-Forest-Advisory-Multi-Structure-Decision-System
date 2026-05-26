#include <iostream>
#include <string>
#include "scenarios.h"
#include "../hash_tables/hash_tables.h"
#include "../monitoring/monitoring.h"
#include "../console_ui.h"

using namespace std;

// Manual Priority Queue for task scheduling 
// Time Complexity: O(n) insert (sorted), O(1) delete
struct Task {
    int priority;
    string name;
    string type;   // "CRITICAL" | "HIGH" | "LOW"
    Task* next;
    Task(int p, string n, string t) : priority(p), name(n), type(t), next(nullptr) { }
};

class OverloadQueue {
    Task* head;
    int count;

public:
    OverloadQueue() : head(nullptr), count(0) {}
    ~OverloadQueue() 
    {
        while (head) 
        {
            Task* t = head; 
            head = head->next; 
            delete t; 
        }
    }

    int size() const 
    {
        return count;
    }

    // Insert in descending priority order — TC: O(n)
    void enqueue(int p, string name, string type)
    {
        Task* n = new Task(p, name, type);
        count++;

        if (!head || head->priority < p)
        {
            n->next = head;
            head = n; 
            return; 
        }

        Task* cur = head;
        while (cur->next && cur->next->priority >= p)
        {
            cur = cur->next;
        }

        n->next = cur->next;
        cur->next = n;
    }

    // Dequeue highest priority — TC: O(1)
    Task* dequeue()
    {
        if (!head)
        {
            return nullptr;
        }

        Task* t = head; 
        head = head->next; 
        count--;
        return t;
    }

    bool isEmpty() 
    {
        return head == nullptr;
    }

    Task* peek()
    {
        return head;
    }

    void printAll() 
    {
        Task* cur = head;
        int pos = 1;
        while (cur)
        {
            const char* col = (cur->type == "CRITICAL") ? BRED :  (cur->type == "HIGH") ? BYELLOW : DIM;
            string pad = cur->name;
            while ((int)pad.size() < 30)
            {
                pad += ' ';
            }

            cout << "  " << CYAN << "[" << pos++ << "]" << RESET << "  " << col << pad << RESET << "  P=" << col << cur->priority << RESET << "  [" << col << cur->type << RESET << "]" << endl;
            cur = cur->next;
        }
    }
};

void scenario4_overload()
{
    printBanner("SCENARIO 4: SYSTEM OVERLOAD & LOAD REDISTRIBUTION");
    Monitoring mon("Scenario 4 Execution");

    // Phase 1: Show incoming burst 
    printSubHeader("Phase 1 — Incoming Task Burst  (18 simultaneous events)");
    printInfo("System receives 18 events at once — processing slows.");

    OverloadQueue incoming;
    // Enqueue 18 tasks of mixed priority — TC: O(n^2) worst
    incoming.enqueue(10, "FIRE ALERT Zone 3", "CRITICAL");
    incoming.enqueue(10, "FIRE ALERT Zone 2", "CRITICAL");
    incoming.enqueue(9, "Smoke threshold Zone 5", "CRITICAL");
    incoming.enqueue(8, "BFS re-route blocked path", "HIGH");
    incoming.enqueue(8, "Evacuation queue flush", "HIGH");
    incoming.enqueue(7, "Sensor sync Zone 4", "HIGH");
    incoming.enqueue(7, "Decision tree recalc", "HIGH");
    incoming.enqueue(6, "Hash cache invalidation", "HIGH");
    incoming.enqueue(5, "Wildlife log Zone 7", "LOW");
    incoming.enqueue(5, "Humidity reading Zone 1", "LOW");
    incoming.enqueue(4, "Routine temp Zone 9", "LOW");
    incoming.enqueue(4, "Routine temp Zone 6", "LOW");
    incoming.enqueue(3, "Wind speed log Zone 8", "LOW");
    incoming.enqueue(3, "Stability check Zone 10", "LOW");
    incoming.enqueue(2, "Archive old sensor log", "LOW");
    incoming.enqueue(2, "Update hash index table", "LOW");
    incoming.enqueue(1, "Generate report summary", "LOW");
    incoming.enqueue(1, "Write event history file", "LOW");

    printInfo("Full incoming queue (" + to_string(incoming.size()) + " tasks):");
    incoming.printAll();

    // Phase 2: Load measurement 
    printSubHeader("Phase 2 — System Load Analysis");
    printBar("Queue Module", 96);
    printBar("Graph Routing", 82);
    printBar("Tree Decisions", 61);
    printBar("Hash Lookup", 44);
    printBar("Array Access", 28);
    printWarn("Queue and Graph modules OVERLOADED — normal processing halted.");

    // Phase 3: Task Triage (split into CRITICAL and LOW queues)
    printSubHeader("Phase 3 — Task Triage  (Priority Separation)");
    printInfo("Separating critical from low-priority tasks...");

    OverloadQueue criticalQ, lowQ;
    while (!incoming.isEmpty()) 
    {
        Task* t = incoming.dequeue();
        if (t->priority >= 7)
        {
            criticalQ.enqueue(t->priority, t->name, t->type);
        }

        else
        {
            lowQ.enqueue(t->priority, t->name, t->type);
        }

        delete t;
    }

    printOK("CRITICAL queue  (" + to_string(criticalQ.size()) + " tasks) — processing NOW:");
    criticalQ.printAll();

    printWarn("LOW queue  (" + to_string(lowQ.size()) + " tasks) — PAUSED until load drops.");

    // Phase 4: Process critical tasks
    printSubHeader("Phase 4 — Processing Critical Tasks");
    int step = 1;
    while (!criticalQ.isEmpty())
    {
        Task* t = criticalQ.dequeue();
        const char* col = (t->type == "CRITICAL") ? BRED : BYELLOW;
        cout << "  " << BGREEN << "[EXEC " << step++ << "]" << RESET << "  " << col << t->name << RESET << endl;
        delete t;
    }
    printOK("All critical tasks dispatched.");

    // Phase 5: H2: Hash Table Load Redistribution
    printSubHeader("Phase 5 — H2: Hash Table Load Redistribution");
    printInfo("Requests rehashed to balance server load  |  TC: O(1) avg");
    HashTables serverCache(5);
    serverCache.insert(10, "Srv_A_Fire");
    serverCache.insert(13, "Srv_B_Smoke");
    serverCache.insert(3, "Srv_C_Sensor");
    serverCache.insert(8, "Srv_D_Graph");
    serverCache.insert(18, "Srv_E_Tree");   // collision with index 3
    serverCache.display();

    // Phase 6: Resume low-priority tasks 
    printSubHeader("Phase 6 — Resuming Low-Priority Tasks");
    printBar("Queue Module", 35);
    printBar("Graph Routing", 28);
    printOK("Load normalised. Resuming " + to_string(lowQ.size()) + " paused tasks:");

    step = 1;
    while (!lowQ.isEmpty()) 
    {
        Task* t = lowQ.dequeue();
        cout << "  " << BBLUE << "[RESUME " << step++ << "]" << RESET
            << "  " << t->name << endl;
        delete t;
    }

    printOK("All paused tasks resumed and processed.");

    printDivider();
    printOK("System load redistributed. Normal operation restored.");
    printOK("Scenario 4 complete.");
}