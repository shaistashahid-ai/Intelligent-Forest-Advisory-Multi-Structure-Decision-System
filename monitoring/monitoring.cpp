#include <iostream>
#include <string>
#include <ctime>
#include "monitoring.h"
#include "../console_ui.h"

using namespace std;

Monitoring::Monitoring(string name) : opName(name)
{
    // TC: O(1)
    start = clock();
    printMon("START  " + opName);
}

Monitoring::~Monitoring() 
{
    clock_t end = clock();
    double ms = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
    if (ms > 5.0)
    {
        cout << "  " << BRED << BOLD << "[BOTTLENECK]" << RESET << "  " << BYELLOW << opName << RESET << "  " << BRED << ms << " ms" << RESET << "  > 5ms threshold!" << endl;
    }

    else
    {
        cout << "  " << SYM_MON << "  " << BCYAN << opName << RESET << "  completed in " << BGREEN << ms << " ms" << RESET << endl;
    }
}

void Monitoring::monitorMenu(int choice)
{
    switch (choice) 
    {
    case 1:
    {
        // 9.1 Monitor System Load
        printSubHeader("9.1 System Load  (All Modules)");
        printInfo("Load = Active Tasks / Processing Capacity");
        printBar("Array Processing", 35);
        printBar("Linked List Events", 52);
        printBar("Queue Scheduling", 71);
        printBar("Tree Decisions", 44);
        printBar("Graph Routing", 68);
        printBar("Hash Lookups", 29);
        printBar("Scenario Execution", 85);
        printWarn("Queue and Scenario modules approaching high load.");
        printOK("Overall system load: MODERATE");
        break;
    }

    case 2: 
    {
        // 9.2 Track Execution Time
        printSubHeader("9.2 Execution Time Tracking  TC: O(1)");
        printInfo("Formula: Latency = FinishTime - StartTime");
        struct OpTiming { const char* name; double ms; };
        OpTiming ops[] = {
            {"Array 1D traversal",      0.12},
            {"Array 2D interpolation",  0.08},
            {"LinkedList insertion",     0.04},
            {"BFS traversal (5 nodes)",  0.31},
            {"DFS traversal (5 nodes)",  0.28},
            {"Hash insert (no coll.)",  0.03},
            {"Hash insert (collision)",  0.09},
            {"Decision score calc",     0.05},
            {"Scenario 1 (full)",       3.80},
            {"Scenario 4 (overload)",   6.20},
        };
        printTableHeader("Operation", "Latency (ms)  Status", 28, 24);
        for (auto& op : ops) 
        {
            char buf[40];
            snprintf(buf, sizeof(buf), "%-13.2f %s", op.ms, op.ms > 5.0 ? "BOTTLENECK" : op.ms > 1.0 ? "SLOW" : "OK");
            printTableRow(op.name, buf, 28, 24, op.ms > 5.0 ? BRED : op.ms > 1.0 ? BYELLOW : BGREEN);
        }

        printTableFooter(28, 24);
        printOK("Execution timing logged  TC: O(1) per measurement");
        break;
    }

    case 3:
    {
        // 9.3 Detect Bottlenecks
        printSubHeader("9.3 Bottleneck Detection");
        printInfo("Bottleneck = module whose latency exceeds 5ms during scenario run.");
        struct BN { const char* module; double load; bool bottleneck; };
        BN modules[] = {
            {"Array Processing", 35.0, false},
            {"Queue Scheduler", 91.0, true },
            {"Graph Routing", 82.0, true },
            {"Hash Table", 28.0, false},
            {"Tree Decisions", 44.0, false},
        };
        printTableHeader("Module", "Load%   Bottleneck?  Action", 20, 30);
        for (auto& m : modules)
        {
            char buf[50];
            snprintf(buf, sizeof(buf), "%-7.0f %-12s %s", m.load, m.bottleneck ? "YES" : "NO", m.bottleneck ? "Redistribute tasks" : "Normal operation");
            printTableRow(m.module, buf, 20, 30, m.bottleneck ? BRED : BGREEN);
        }

        printTableFooter(20, 30);
        printWarn("2 bottlenecks detected: Queue + Graph. Run Scenario 4 to see resolution.");
        break;
    }

    case 4:
    {
        // 9.4 Optimize Performance
        printSubHeader("9.4 Performance Optimization");
        printInfo("Actions taken to resolve detected bottlenecks:");
        printOK("Queue — low-priority tasks paused, capacity freed from 91% -> 45%");
        printOK("Graph — blocked routes cached in hash table, re-routing O(1) lookup");
        printOK("Hash   — table size increased, collision probes reduced from 3 -> 1");
        printInfo("Before optimization:");
        printBar("Queue Scheduler", 91);
        printBar("Graph Routing", 82);
        cout << endl;
        printInfo("After optimization:");
        printBar("Queue Scheduler", 45);
        printBar("Graph Routing", 38);
        printOK("System optimized. All modules within normal load range.");
        break;
    }

    case 5:
    {
        // 9.5 View System Health
        printSubHeader("9.5 System Health Dashboard");
        cout << endl;
        cout << "  " << BOLD << BCYAN << "  IFAMDS SYSTEM HEALTH" << endl << RESET;
        printDivider();
        printKV("System Status:", "OPERATIONAL", BGREEN);
        printKV("Active Sensors:", "5 / 5 Online", BGREEN);
        printKV("Zones Monitored:", "4 (Zones 0-3)", BWHITE);
        printKV("Fire Events:", "2 active", BRED);
        printKV("Warnings:", "3 active", BYELLOW);
        printKV("Queue Load:", "45% (normal)", BGREEN);
        printKV("Graph Routes:", "6 edges active", BWHITE);
        printKV("Hash Collisions:", "2 resolved", BYELLOW);
        printKV("Uptime:", "Continuous", BGREEN);
        printDivider();
        printBar("Overall Health", 78);
        cout << endl;
        printOK("System healthy. No critical failures detected.");
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