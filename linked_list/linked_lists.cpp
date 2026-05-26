#include <iostream>
#include <cmath>
#include <string>
#include "linked_lists.h"
#include "../console_ui.h"

using namespace std;

// Persistent event streams (module-level state) 
static Node* singlyHead = nullptr;   // L1: raw event stream
static Node* singlyTail = nullptr;
static DNode* doublyHead = nullptr;   // L4: correction chain
static Node* stableSnap = nullptr;   // last stable state snapshot

static long timeCounter = 1;

// Helper: deep-copy singly list 
static Node* copyList(Node* src)
{
    if (!src)
    {
        return nullptr;
    }

    Node* newHead = new Node(src->zone, src->temp, src->smoke, src->timestamp);
    Node* cur = src->next;
    Node* tail = newHead;

    while (cur)
    {
        tail->next = new Node(cur->zone, cur->temp, cur->smoke, cur->timestamp);
        tail = tail->next;
        cur = cur->next;
    }

    return newHead;
}

static void freeList(Node* h)
{
    while (h)
    {
        Node* t = h;
        h = h->next;
        delete t;
    }
}

void LinkedLists::printSingly(Node* head)
{
    Node* temp = head;
    cout << "  " << BGREEN << "HEAD" << RESET;

    while (temp)
    {
        char buf[40];
        snprintf(buf, sizeof(buf), "Z%d T=%.0f S=%.0f", temp->zone, temp->temp, temp->smoke);
        cout << CYAN << " -> " << RESET << BWHITE << "[" << buf << "]" << RESET;
        temp = temp->next;
    }

    cout << CYAN << " -> " << RESET << DIM << "NULL" << endl << RESET;
}

void LinkedLists::printDoubly(DNode* head)
{
    DNode* temp = head;
    cout << "  " << BGREEN << "HEAD" << RESET;

    while (temp)
    {
        char buf[20];
        snprintf(buf, sizeof(buf), "Z%d T=%.0f", temp->zone, temp->temp);
        cout << CYAN << " <-> " << RESET << BWHITE << "[" << buf << "]" << RESET;
        temp = temp->next;
    }

    cout << CYAN << " <-> " << RESET << DIM << "NULL" << endl << RESET;
}

void LinkedLists::eventMenu(int choice)
{
    switch (choice)
    {
    case 1:
    {
        // 3.1 Store Event
        printSubHeader("3.1 Store Event in Linked List  (L1: Raw Event Stream)");
        printInfo("New sensor event added to tail  |  TC: O(1) insert");
        // Add 3 sample events
        double temps[] = { 28.0, 46.0, 62.0 };
        double smokes[] = { 10.0, 45.0, 82.0 };
        int    zones[] = { 3, 4, 3 };

        for (int i = 0; i < 3; i++)
        {
            Node* n = new Node(zones[i], temps[i], smokes[i], timeCounter++);
            if (!singlyHead)
            {
                singlyHead = n;
                singlyTail = n;
            }

            else
            {
                singlyTail->next = n;
                singlyTail = n;
            }

            cout << "  " << SYM_OK << "  Event stored: " << BCYAN << "Zone " << zones[i] << RESET << "  Temp=" << BYELLOW << temps[i] << RESET << "  Smoke=" << BRED << smokes[i] << RESET << "  T=" << DIM << timeCounter - 1 << RESET << endl;
        }

        freeList(stableSnap);
        stableSnap = copyList(singlyHead);
        printOK("Snapshot saved as stable state.");
        printSingly(singlyHead);
        break;
    }

    case 2:
    {
        // 3.2 Forward traversal
        printSubHeader("3.2 Forward Traversal  (L1 Singly)  TC: O(n)");
        if (!singlyHead)
        {
            printWarn("No events stored. Run option 1 first.");
            break;
        }

        printInfo("Traversing HEAD -> NULL:");
        Node* cur = singlyHead; int step = 1;
        while (cur)
        {
            const char* col = (cur->temp > 45) ? BRED : BGREEN;
            cout << "  " << CYAN << "[" << step++ << "]" << RESET << "  Zone " << cur->zone << "  Temp=" << col << cur->temp << RESET << "  Smoke=" << col << cur->smoke << RESET << "  Time=" << DIM << cur->timestamp << RESET << endl;
            cur = cur->next;
        }

        printOK("Forward traversal complete.");
        break;
    }

    case 3:
    {
        // 3.3 Backward traversal (doubly)
        printSubHeader("3.3 Backward Traversal  (L4 Doubly)  TC: O(n)");
        // Build doubly from current singly data
        while (doublyHead)
        {
            DNode* t = doublyHead;
            doublyHead = doublyHead->next;
            delete t;
        }

        Node* cur = singlyHead;
        if (!cur)
        {
            printWarn("No events. Run option 1 first.");
            break;
        }

        while (cur)
        {
            DNode* n = new DNode(cur->zone, cur->temp);
            if (!doublyHead)
            {
                doublyHead = n;
            }

            else
            {
                DNode* tail = doublyHead;
                while (tail->next)
                {
                    tail = tail->next;
                }

                tail->next = n; n->prev = tail;
            }

            cur = cur->next;
        }

        printInfo("Traversing NULL <- TAIL -> HEAD (backward):");
        DNode* tail = doublyHead;
        while (tail && tail->next)
        {
            tail = tail->next;
        }

        int step = 1;
        while (tail)
        {
            const char* col = (tail->temp > 45) ? BRED : BGREEN;
            cout << "  " << CYAN << "[" << step++ << "]" << RESET << "  (backward) Zone " << tail->zone << "  Temp=" << col << tail->temp << RESET << endl;
            tail = tail->prev;
        }

        printOK("Backward traversal complete  (bidirectional correction chain ready).");
        break;
    }

    case 4:
    {
        // 3.4 Circular monitoring loop
        printSubHeader("3.4 Circular Monitoring Loop  (L7)  TC: O(n) per cycle");
        printInfo("Last node points back to HEAD — infinite monitoring loop.");

        // Build 4-node circular from zone data
        int zones[] = { 1, 2, 3, 4 };
        Node* nodes[4];

        // Safety check during allocation
        for (int i = 0; i < 4; i++)
        {
            nodes[i] = new Node(zones[i], 25.0 + i * 5, i * 10.0, i + 1);
            if (!nodes[i])
            {
                cout << BRED << "[ERR]  Memory allocation failed!" << RESET << endl;
                return;
            }
        }

        nodes[3]->next = nodes[0]; // circular link

        cout << "  " << BGREEN << "[HEAD]" << RESET;
        for (int i = 0; i < 4; i++)
        {
            cout << CYAN << " -> " << RESET << BWHITE << "[Z" << nodes[i]->zone << "]" << RESET;
        }

        cout << CYAN << " -> " << RESET << BYELLOW << "[HEAD]" << RESET << BMAGENTA << " (loop)" << endl << RESET;
        cout << flush;

        // Simulate 2 monitoring cycles
        printInfo("Simulating 2 monitoring cycles:");
        Node* start = nodes[0];

        // Verify start pointer
        if (!start)
        {
            cout << BRED << "[ERR]  Start pointer is null!" << RESET << endl;
            return;
        }

        for (int cycle = 1; cycle <= 2; cycle++)
        {
            cout << "  " << BCYAN << "Cycle " << cycle << ": " << RESET;
            Node* p = start;

            for (int i = 0; i < 4; i++)
            {
                if (p)
                {
                    cout << BWHITE << "Z" << p->zone << RESET;
                    if (i < 3)
                    {
                        cout << CYAN << " -> " << RESET;
                    }

                    p = p->next;
                }

                else
                {
                    cout << BRED << "[ERR]  Cycle broken: Null pointer encountered!" << RESET << endl;
                    break;
                }
            }

            cout << CYAN << " -> " << RESET << BYELLOW << "(back to Z1)" << endl << RESET;
            cout << std::flush;
        }

        // cleanup (break circular before delete)
        nodes[3]->next = nullptr;
        for (int i = 0; i < 4; i++)
        {
            delete nodes[i];
        }

        printWarn("Circular monitoring active — loop never stops until emergency cleared.");
        break;
    }

    case 5:
    {
        // 3.5 Restore last stable state
        printSubHeader("3.5 Restore Last Stable State  TC: O(n)");
        if (!stableSnap)
        {
            printWarn("No stable snapshot available. Store events first.");
            break;
        }

        printInfo("Current state:");
        printSingly(singlyHead);

        // Corrupt tail node to demonstrate restore is meaningful
        if (singlyTail)
        {
            singlyTail->temp = 99.0;
            singlyTail->smoke = 99.0;
            cout << "  " << BYELLOW << "[SIM]" << RESET << "  Injecting corrupt value into tail node (Temp=99, Smoke=99)..." << endl;
        }

        printInfo("Corrupted state:");
        printSingly(singlyHead);

        // Restore
        freeList(singlyHead);
        singlyHead = copyList(stableSnap);
        singlyTail = singlyHead;
        if (singlyTail)
        {
            while (singlyTail->next)
            {
                singlyTail = singlyTail->next;
            }
        }

        printOK("State restored to last stable snapshot:");
        printSingly(singlyHead);
        break;
    }

    case 6:
    {
        // 3.6 Verified Event Stream — L2: Noise-filtered stream  TC: O(n)
        printSubHeader("3.6 Verified Event Stream  (L2: Noise-Filtered)  TC: O(n)");
        printInfo("L2 rule: |val_i - val_{i-1}| >= delta=15 => noise (rejected)");
        printInfo("         val < 0 or val > 80 => out-of-range (rejected)");

        double raw[] = { 28.0, 30.0, 92.0, 31.0, 62.0, -3.0, 33.0, 65.0 };
        int    zones[] = { 3,    3,    3,    3,    4,    4,    4,    3 };
        int    n = 8;
        const double DELTA = 15.0;

        Node* l2Head = nullptr;
        Node* l2Tail = nullptr;
        double prevVal = raw[0];
        int accepted = 0;
        int rejected = 0;

        printTableHeader("Index", "Value     Status", 6, 24);
        for (int i = 0; i < n; i++)
        {
            bool outOfRange = (raw[i] < 0 || raw[i] > 80);
            bool isNoise = (!outOfRange && i > 0 && fabs(raw[i] - prevVal) >= DELTA);
            string status;
            const char* col;

            if (outOfRange)
            {
                status = "REJECTED (out-of-range)";
                col = BRED;
                rejected++;
            }

            else if (isNoise)
            {
                status = "REJECTED (noise spike)";
                col = BYELLOW;
                rejected++;
            }

            else
            {
                status = "ACCEPTED -> stored in L2";
                col = BGREEN;
                Node* node = new Node(zones[i], raw[i], 0.0, i + 1);
                if (!l2Head)
                {
                    l2Head = node;
                    l2Tail = node;
                }

                else
                {
                    l2Tail->next = node;
                    l2Tail = node;
                }

                prevVal = raw[i];
                accepted++;
            }

            char buf[40];
            snprintf(buf, sizeof(buf), "%-9.1f %s", raw[i], status.c_str());
            printTableRow(to_string(i), buf, 6, 24, col);
        }

        printTableFooter(6, 24);
        printOK("L2 Verified Stream built  |  Accepted=" + to_string(accepted) + "  Rejected=" + to_string(rejected) + "  TC: O(n)");

        cout << "  " << BOLD << "L2 contents:" << RESET << endl;
        printSingly(l2Head);

        // cleanup
        freeList(l2Head);
        break;
    }

    case 7:
    {
        // 3.7 Anomaly Event Stream — L3: Above-threshold events  TC: O(n)
        printSubHeader("3.7 Anomaly Event Stream  (L3: Anomaly Detection)  TC: O(n)");
        printInfo("L3 rule: |value - baseline| > theta=20 => anomaly node stored");
        printInfo("Baseline: Temp=25C  Smoke=0  |  Theta=20");

        const double THETA = 20.0;
        const double BASE_T = 25.0;
        const double BASE_S = 0.0;

        struct RawEvt
        {
            int zone;
            double temp;
            double smoke;
        };

        RawEvt events[] = {
            {1, 27.0,  5.0},
            {2, 62.0, 85.0},
            {3, 28.0, 15.0},
            {4, 71.0, 91.0},
            {5, 30.0, 10.0},
            {6, 55.0, 72.0},
            {7, 26.0,  8.0},
        };

        int m = 7;
        Node* l3Head = nullptr;
        Node* l3Tail = nullptr;
        int anomalyCount = 0;

        printTableHeader("Zone", "Temp   Smoke  DeltaT  DeltaS  Verdict", 6, 42);
        for (int i = 0; i < m; i++)
        {
            double dT = fabs(events[i].temp - BASE_T);
            double dS = fabs(events[i].smoke - BASE_S);
            bool isAnomaly = (dT > THETA || dS > THETA);
            const char* col = isAnomaly ? BRED : BGREEN;
            string verdict = isAnomaly ? "ANOMALY -> stored in L3" : "NORMAL  -> skipped";

            char buf[60];
            snprintf(buf, sizeof(buf), "%-6.1f %-6.1f %-7.1f %-7.1f %s",
                events[i].temp, events[i].smoke, dT, dS, verdict.c_str());
            printTableRow("Z" + to_string(events[i].zone), buf, 6, 42, col);

            if (isAnomaly)
            {
                Node* node = new Node(events[i].zone, events[i].temp, events[i].smoke, i + 1);
                if (!l3Head)
                {
                    l3Head = node;
                    l3Tail = node;
                }

                else
                {
                    l3Tail->next = node;
                    l3Tail = node;
                }

                anomalyCount++;
            }
        }

        printTableFooter(6, 42);
        printOK("L3 Anomaly Stream built  |  Anomalies=" + to_string(anomalyCount) + "  TC: O(n)");

        cout << "  " << BOLD << "L3 contents (anomaly nodes only):" << RESET << endl;
        printSingly(l3Head);

        // cleanup
        freeList(l3Head);
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

void LinkedLists::demo()
{
    eventMenu(1);
    eventMenu(2);
    eventMenu(3);
    eventMenu(4);
}