#include <iostream>
#include <string>
#include <sstream>
#include "scenarios.h"
#include "../monitoring/monitoring.h"
#include "../console_ui.h"

using namespace std;

// Anomaly linked list node (L3: Anomaly Event Stream)
// Time Complexity: O(1) insert at tail, O(n) traversal
struct AnomalyNode {
    int    zoneId;
    double temperature;
    double smoke;
    bool   wildlifeAlert;
    bool   humanAlert;
    string verdict;
    AnomalyNode* next;
    AnomalyNode(int z, double t, double s, bool w, bool h) : zoneId(z), temperature(t), smoke(s), wildlifeAlert(w), humanAlert(h), verdict(""), next(nullptr) { }
};

// Decision tree node
struct TNode {
    string label;
    TNode* yes;
    TNode* no;
    string result;
};

// Traverse tree and return verdict — TC: O(h)
static string classify(TNode* root, double temp, double smoke, bool wildlife, bool human)
{
    TNode* cur = root;
    cur = (temp > 45.0) ? cur->yes : cur->no;
    if (!cur->result.empty())
    {
        return cur->result;
    }

    cur = (smoke > 60.0) ? cur->yes : cur->no;
    if (!cur->result.empty())
    {
        return cur->result;
    }

    cur = (wildlife || human) ? cur->yes : cur->no;
    return cur->result;
}

void scenario3_anomaly()
{
    printBanner("SCENARIO 3: MULTI-FACTOR ANOMALY ESCALATION");
    Monitoring mon("Scenario 3 Execution");

    // Build decision tree (T7: Fire Classification Tree)
    TNode leaf_fire = { "FIRE CONFIRMED", nullptr, nullptr, "FIRE CONFIRMED" };
    TNode leaf_smoke = { "SMOKE HAZARD", nullptr, nullptr, "SMOKE HAZARD" };
    TNode leaf_activity = { "HUMAN/WILDLIFE ALERT", nullptr, nullptr, "HUMAN/WILDLIFE ALERT" };
    TNode leaf_normal = { "NORMAL", nullptr, nullptr, "NORMAL" };
    TNode lvl3_smoke = { "Wildlife/Human?", &leaf_activity, &leaf_normal, "" };
    TNode lvl3_nosmoke = { "Wildlife/Human?", &leaf_activity, &leaf_normal, "" };
    TNode lvl2_hot = { "Smoke > 60?", &leaf_fire, &lvl3_smoke, "" };
    TNode lvl2_cool = { "Smoke > 60?", &leaf_smoke, &lvl3_nosmoke, "" };
    TNode root_ = { "Temp > 45C?", &lvl2_hot,      &lvl2_cool, "" };

    printSubHeader("T7: Decision Tree Structure");
    cout << "  " << BCYAN << "[Temp > 45C?]" << RESET << endl;
    cout << "  " << CYAN << "   YES --> " << RESET << BYELLOW << "[Smoke > 60?]" << RESET << CYAN << "  YES --> " << RESET << BRED << "[FIRE CONFIRMED]\n" << RESET;
    cout << "  " << CYAN << "              " << RESET << CYAN << "  NO  --> " << RESET << BYELLOW << "[Wildlife/Human?]" << RESET << CYAN << " YES --> " << RESET << BMAGENTA << "[HUMAN/WILDLIFE ALERT]\n" << RESET;
    cout << "  " << CYAN << "   NO  --> " << RESET << BYELLOW << "[Smoke > 60?]" << RESET << CYAN << "  YES --> " << RESET << BYELLOW << "[SMOKE HAZARD]\n" << RESET;
    cout << "  " << CYAN << "              " << RESET  << CYAN << "              NO  --> " << RESET << BGREEN << "[NORMAL]\n" << RESET;

    // Live sensor readings → build L3 anomaly linked list 
    printSubHeader("L3: Anomaly Event Stream  (Singly Linked List)");
    printInfo("Each node classified by decision tree  |  TC: O(n * h)");

    struct SensorReading 
    {
        int zone; 
        double temp; 
        double smoke; 
        bool wildlife;
        bool human;
    };

    SensorReading readings[] = {
        {3, 62.0, 85.0, false, false },
        {7, 28.0, 15.0, true,  false },
        {5, 48.0, 45.0, false, false },
        {11, 26.0, 10.0, false, true  },
        {2, 71.0, 91.0, false, false },
        {9, 30.0, 20.0, false, false },
        {4, 55.0, 72.0, true,  true  },
    };

    int numReadings = 7;

    // Build linked list — TC: O(n)
    AnomalyNode* head = nullptr;
    AnomalyNode* tail = nullptr;
    for (int i = 0; i < numReadings; i++)
    {
        AnomalyNode* node = new AnomalyNode( readings[i].zone, readings[i].temp, readings[i].smoke, readings[i].wildlife, readings[i].human);
        node->verdict = classify(&root_, readings[i].temp, readings[i].smoke, readings[i].wildlife, readings[i].human);
        if (!head) 
        {
            head = node;
            tail = node;
        }

        else 
        {
            tail->next = node;
            tail = node;
        }
    }

    // Print stream table
    printTableHeader("Zone", "Temp   Smoke  Wild   Human  Verdict", 6, 40);
    AnomalyNode* cur = head;
    int fires = 0, warnings = 0;
    while (cur)
    {
        char buf[100];
        ostringstream oss;
        oss << fixed << setprecision(1)  << cur->temperature << "C " << cur->smoke << "   " << (cur->wildlifeAlert ? "YES" : "NO") << "   " << (cur->humanAlert ? "YES" : "NO") << "   " << cur->verdict;

        string s = oss.str();

        const char* col = BWHITE;
        if (cur->verdict == "FIRE CONFIRMED")
        {
            col = BRED;    
            fires++; 
        }

        else if (cur->verdict == "SMOKE HAZARD")
        {
            col = BYELLOW;
            warnings++;
        }

        else if (cur->verdict == "HUMAN/WILDLIFE ALERT")
        {
            col = BMAGENTA;
            warnings++;
        }

        else 
        {
            col = BGREEN;
        }

        printTableRow("Z" + to_string(cur->zoneId), s, 6, 40, col);
        cur = cur->next;
    }
    printTableFooter(6, 40);

    // Traversal: escalation decisions 
    printSubHeader("Escalation Decisions  (Linked List Traversal)");
    cur = head;
    while (cur) 
    {
        if (cur->verdict == "FIRE CONFIRMED")
        {
            printFire("Zone " + to_string(cur->zoneId) + "  Temp=" + to_string((int)cur->temperature) + "C  Smoke=" + to_string((int)cur->smoke) + "  -> DISPATCH firefighters immediately.");
        }

        else if (cur->verdict == "SMOKE HAZARD")
        {
            printWarn("Zone " + to_string(cur->zoneId) + "  Temp=" + to_string((int)cur->temperature) + "C  Smoke=" + to_string((int)cur->smoke) + "  -> Evacuation advisory issued.");
        }

        else if (cur->verdict == "HUMAN/WILDLIFE ALERT")
        {
            cout << "  " << BMAGENTA << "[ALERT]" << RESET << "  Zone " << cur->zoneId << "  Wildlife=" << (cur->wildlifeAlert ? "YES" : "NO") << "  Human=" << (cur->humanAlert ? "YES" : "NO") << "  -> Security team notified." << endl;
        }

        else
        {
            printOK("Zone " + to_string(cur->zoneId) + "  NORMAL  -> Routine monitoring.");
        }

        cur = cur->next;
    }

    // Summary
    printSubHeader("Escalation Summary");
    printTableHeader("Verdict", "Zones", 26, 6);
    printTableRow("FIRE CONFIRMED", to_string(fires), 26, 6, BRED);
    printTableRow("Smoke / activity alerts", to_string(warnings), 26, 6, BYELLOW);
    printTableRow("NORMAL", to_string(numReadings - fires - warnings), 26, 6, BGREEN);
    printTableFooter(26, 6);

    if (fires > 0)
    {
        printFire(std::to_string(fires) + " fire zone(s) confirmed  -> Global escalation active.");
    }

    else
    {
        printOK("No fires confirmed. Elevated monitoring active.");
    }

    // Cleanup — TC: O(n)
    cur = head;
    while (cur) 
    {
        AnomalyNode* tmp = cur; 
        cur = cur->next;
        delete tmp;
    }

    printDivider();
    printOK("Scenario 3 complete.");
}