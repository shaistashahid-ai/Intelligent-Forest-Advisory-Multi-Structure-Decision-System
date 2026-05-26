#include <iostream>
#include <string>
#include "queues.h"
#include "../console_ui.h"

using namespace std;

// ManualQueue
ManualQueue::ManualQueue()
{
    front = 0;
    rear = -1;
    count = 0;
}

void ManualQueue::push(int x)
{
    if (count >= 200)
    {
        printError("Queue Full!");
        return;
    }

    rear = (rear + 1) % 200;
    arr[rear] = x;
    count++;
}

void ManualQueue::pop()
{
    if (isEmpty())
    {
        return;
    }

    front = (front + 1) % 200; count--;
}

int ManualQueue::getFront()
{
    return isEmpty() ? -1 : arr[front];
}

bool ManualQueue::isEmpty()
{
    return count == 0;
}

// PriorityQueue
PriorityQueue::~PriorityQueue()
{
    while (head)
    {
        PTask* t = head;
        head = head->next;
        delete t;
    }
}

void PriorityQueue::enqueue(int p, string n, string t)
{
    PTask* node = new PTask(p, n, t);
    count++;
    if (!head || head->priority < p)
    {
        node->next = head;
        head = node;
        return;
    }

    PTask* cur = head;
    while (cur->next && cur->next->priority >= p)
    {
        cur = cur->next;
    }

    node->next = cur->next;
    cur->next = node;
}

PTask* PriorityQueue::dequeue()
{
    if (!head)
    {
        return nullptr;
    }

    PTask* t = head;
    head = head->next;
    count--;
    return t;
}

void PriorityQueue::printAll()
{
    PTask* cur = head;
    int pos = 1;

    while (cur)
    {
        const char* col = (cur->type == "CRITICAL") ? BRED : (cur->type == "HIGH") ? BYELLOW : DIM;
        string pad = cur->name;
        while ((int)pad.size() < 32)
        {
            pad += ' ';
        }

        cout << "  " << CYAN << "[" << pos++ << "]" << RESET << "  " << col << pad << RESET << "  P=" << col << cur->priority << RESET << "  [" << col << cur->type << RESET << "]" << endl;
        cur = cur->next;
    }
}

// Persistent queues 
static ManualQueue routineQ, survQ;
static PriorityQueue emergQ;
static PriorityQueue pausedQ;

static void drawQueue(int* arr, int n, const char* label)
{
    cout << "  " << BOLD << CYAN << label << ": " << RESET << BYELLOW << "FRONT [" << RESET;
    for (int i = 0; i < n; i++)
    {
        cout << BWHITE << arr[i] << RESET;
        if (i < n - 1)
        {
            cout << CYAN << "|" << RESET;
        }
    }

    cout << BYELLOW << "] REAR" << endl << RESET;
}

void Queues::scheduleMenu(int choice)
{
    switch (choice)
    {
    case 1:
    {
        // 5.1 Add Routine Task
        printSubHeader("5.1 Add Routine Task  (Q1: FIFO Queue)  TC: O(1)");
        int tasks[] = { 10, 20, 30, 40 };
        for (int v : tasks)
        {
            routineQ.push(v);
        }

        int arr[] = { 10,20,30,40 };
        drawQueue(arr, 4, "Q1-Routine");
        printOK("4 routine tasks enqueued.  Size=" + to_string(routineQ.size()));
        break;
    }

    case 2:
    {
        // 5.2 Add Surveillance Task
        printSubHeader("5.2 Add Surveillance Task  (Q2: Continuous)  TC: O(1)");
        int smoke[] = { 10,12,15,18,25,38,55 };
        for (int v : smoke)
        {
            survQ.push(v);
        }

        drawQueue(smoke, 7, "Q2-Surveillance");
        printInfo("Smoke readings from dry zone streamed in sequence.");
        printOK("7 surveillance readings enqueued.  Size=" + to_string(survQ.size()));
        break;
    }

    case 3:
    {
        // 5.3 Add Emergency Task (Priority Queue)
        printSubHeader("5.3 Add Emergency Task  (Q3: Priority Queue)  TC: O(n) insert");
        printInfo("Tasks sorted by priority (highest first):");
        emergQ.enqueue(10, "FIRE Zone 3 — Temp 62C", "CRITICAL");
        emergQ.enqueue(10, "FIRE Zone 2 — Temp 71C", "CRITICAL");
        emergQ.enqueue(8, "Smoke threshold Zone 5", "CRITICAL");
        emergQ.enqueue(7, "Evacuation Zone 4", "HIGH");
        emergQ.enqueue(5, "Wildlife alert Zone 7", "HIGH");
        emergQ.enqueue(3, "Routine temp Zone 9", "LOW");
        emergQ.printAll();
        printOK("Emergency queue built.  Size=" + to_string(emergQ.size()));
        break;
    }

    case 4:
    {
        // 5.4 Process Tasks (FIFO + Priority)
        printSubHeader("5.4 Process Tasks");

        printSubHeader("  Q1 Routine — FIFO dequeue  TC: O(1)");
        int step = 1;
        while (!routineQ.isEmpty())
        {
            int v = routineQ.getFront(); routineQ.pop();
            cout << "  " << BGREEN << "[FIFO " << step++ << "]" << RESET << "  Processed task " << BWHITE << v << RESET << endl;
        }

        if (step == 1)
        {
            printWarn("Routine queue empty. Add tasks first (option 1).");
        }

        printSubHeader("  Q3 Emergency — Priority dequeue  TC: O(1)");
        step = 1;
        while (!emergQ.isEmpty())
        {
            PTask* t = emergQ.dequeue();
            const char* col = (t->type == "CRITICAL") ? BRED : BYELLOW;
            cout << "  " << col << "[PRIORITY " << step++ << "]" << RESET << "  " << col << t->name << RESET << "  P=" << t->priority << endl;
            delete t;
        }

        if (step == 1)
        {
            printWarn("Emergency queue empty. Add tasks first (option 3).");
        }

        break;
    }

    case 5:
    {
        // 5.5 Pause and Resume Tasks
        printSubHeader("5.5 Pause and Resume Tasks  TC: O(n)");
        if (emergQ.isEmpty())
        {
            emergQ.enqueue(9, "FIRE Zone 3", "CRITICAL");
            emergQ.enqueue(6, "Sensor sync Zone 4", "HIGH");
            emergQ.enqueue(3, "Routine log Zone 9", "LOW");
            emergQ.enqueue(2, "Archive history", "LOW");
        }

        printInfo("Pausing all LOW-priority tasks:");
        PriorityQueue tempQ;
        while (!emergQ.isEmpty())
        {
            PTask* t = emergQ.dequeue();
            if (t->priority <= 3)
            {
                printWarn("PAUSED: " + t->name);
                pausedQ.enqueue(t->priority, t->name, t->type);
                delete t;
            }

            else
            {
                tempQ.enqueue(t->priority, t->name, t->type);
                delete t;
            }
        }

        // Move back to emergQ
        while (!tempQ.isEmpty())
        {
            PTask* t = tempQ.dequeue();
            emergQ.enqueue(t->priority, t->name, t->type);
            delete t;
        }

        printOK("Active queue (HIGH/CRITICAL only):");
        emergQ.printAll();
        printInfo("Resuming paused tasks after high-priority cleared:");
        while (!pausedQ.isEmpty())
        {
            PTask* t = pausedQ.dequeue();
            emergQ.enqueue(t->priority, t->name, t->type);
            cout << "  " << BBLUE << "[RESUMED]" << RESET << "  " << t->name << endl;
            delete t;
        }

        printOK("All tasks resumed. Final queue:");
        emergQ.printAll();
        break;
    }

    case 6:
    {
        // 5.6 Multi-Factor Decision Queue — Q4  TC: O(n) insert, O(1) dequeue
        printSubHeader("5.6 Multi-Factor Decision Queue  (Q4)  TC: O(n) insert");
        printInfo("Q4 combines Temp + Smoke + Wind before final dispatch decision.");
        printInfo("Formula: CombinedScore = 0.4*Temp + 0.3*Smoke + 0.3*Wind  (normalised 0-1)");

        struct MultiTask
        {
            int    zone;
            double temp;
            double smoke;
            double wind;
            double score;
            string verdict;
        };

        MultiTask tasks[] = {
            {3, 0.90, 0.85, 0.80, 0.0, ""},
            {4, 0.60, 0.55, 0.50, 0.0, ""},
            {1, 0.25, 0.10, 0.20, 0.0, ""},
            {2, 0.80, 0.90, 0.70, 0.0, ""},
            {5, 0.40, 0.30, 0.60, 0.0, ""},
        };

        // Compute combined scores and assign verdicts
        for (int i = 0; i < 5; i++)
        {
            tasks[i].score = 0.4 * tasks[i].temp + 0.3 * tasks[i].smoke + 0.3 * tasks[i].wind;
            tasks[i].verdict = (tasks[i].score > 0.6) ? "EVACUATE" :
                (tasks[i].score > 0.3) ? "MONITOR" : "NORMAL";
        }

        // Build Q4 as priority queue sorted by combined score
        PriorityQueue q4;
        for (int i = 0; i < 5; i++)
        {
            string label = "Zone " + to_string(tasks[i].zone) + " [" + tasks[i].verdict + "]";
            string type = (tasks[i].score > 0.6) ? "CRITICAL" :
                (tasks[i].score > 0.3) ? "HIGH" : "LOW";
            q4.enqueue((int)(tasks[i].score * 10), label, type);
        }

        printTableHeader("Zone", "Temp   Smoke  Wind   Score   Verdict", 6, 40);
        for (int i = 0; i < 5; i++)
        {
            char buf[60];
            snprintf(buf, sizeof(buf), "%-6.2f %-6.2f %-6.2f %-7.2f %s",
                tasks[i].temp, tasks[i].smoke, tasks[i].wind,
                tasks[i].score, tasks[i].verdict.c_str());
            const char* col = (tasks[i].score > 0.6) ? BRED :
                (tasks[i].score > 0.3) ? BYELLOW : BGREEN;
            printTableRow("Z" + to_string(tasks[i].zone), buf, 6, 40, col);
        }

        printTableFooter(6, 40);
        printOK("Q4 built with " + to_string(q4.size()) + " multi-factor tasks.  TC: O(n) insert");
        printInfo("Dispatching from Q4 (highest combined score first):");

        int step = 1;
        while (!q4.isEmpty())
        {
            PTask* t = q4.dequeue();
            const char* col = (t->type == "CRITICAL") ? BRED :
                (t->type == "HIGH") ? BYELLOW : BGREEN;
            cout << "  " << col << "[Q4-" << step++ << "]" << RESET << "  " << col << t->name << RESET << "  P=" << t->priority << endl;
            delete t;
        }

        printOK("Q4 Multi-Factor dispatch complete.  TC: O(1) dequeue");
        break;
    }

    case 0:
    {
        break;
    }

    default:
    {
        printWarn("Invalid option.");
    }
    }
}

void Queues::demo()
{
    scheduleMenu(1);
    scheduleMenu(2);
    scheduleMenu(3);
    scheduleMenu(4);
}