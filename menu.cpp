#define NOMINMAX

#include <iostream>
#include <limits>
#include "console_ui.h"
#include "menu.h"
#include "arrays/arrays.h"
#include "linked_lists/linked_lists.h"
#include "queues/queues.h"
#include "trees/trees.h"
#include "graphs/graphs.h"
#include "hash_tables/hash_tables.h"
#include "monitoring/monitoring.h"
#include "scenarios/scenarios.h"
#include "web_dashboard/web_dashboard.h"

using namespace std;

// Sub-menu helpers
static void subItem(int n, const char* label, const char* col = BWHITE)
{
    cout << "    " << CYAN << "[" << n << "]" << RESET << "  " << col << label << RESET << endl;
}

static int subPrompt()
{
    cout << endl << "    " << BOLD << BCYAN << "Select: " << RESET;
    int c;
    cin >> c;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(9999, '\n');
        return -1;
    }

    return c;
}

static void pressEnter()
{
    cout << endl << "    " << DIM << "Press ENTER to return to menu..." << RESET;
    cin.ignore(9999, '\n');
    cin.get();
}

// 1. Input Environmental Data 
static void menuInputData()
{
    printBanner("1. INPUT ENVIRONMENTAL DATA");
    subItem(1, "Add Sensor Reading  (Temperature, Smoke, Wind)", BWHITE);
    subItem(2, "Store Data in Dynamic Array", BWHITE);
    subItem(3, "Compare with Static Baseline", BWHITE);
    subItem(4, "Validate and Filter Noise", BWHITE);
    subItem(0, "Back", DIM);

    int c = subPrompt();
    Arrays::inputMenu(c);
}

// 2. View Forest Grid Status
static void menuViewForest()
{
    printBanner("2. VIEW FOREST GRID STATUS");
    subItem(1, "Display 1D Time Series Data", BWHITE);
    subItem(2, "Display 2D Forest Zone Matrix", BWHITE);
    subItem(3, "View Zone-wise Conditions", BWHITE);
    subItem(0, "Back", DIM);

    int c = subPrompt();
    Arrays::viewMenu(c);
}

// 3. Event Memory System 
static void menuEventMemory()
{
    printBanner("3. EVENT MEMORY SYSTEM");
    subItem(1, "Store Event  (Linked List)", BWHITE);
    subItem(2, "Traverse Event History Forward", BWHITE);
    subItem(3, "Traverse Event History Backward", BWHITE);
    subItem(4, "Circular Event Monitoring", BWHITE);
    subItem(5, "Restore Last Stable State", BYELLOW);
    subItem(6, "Verified Event Stream  (L2)", BGREEN);
    subItem(7, "Anomaly Event Stream  (L3)", BRED);
    subItem(0, "Back", DIM);

    int c = subPrompt();
    LinkedLists::eventMenu(c);
}

// 4. Fire Detection and Control
static void menuFireControl()
{
    printBanner("4. FIRE DETECTION AND CONTROL");
    subItem(1, "Detect Fire Risk  (Threshold Check)", BWHITE);
    subItem(2, "Trigger Emergency Alert", BRED);
    subItem(3, "Priority-Based Fire Response", BYELLOW);
    subItem(4, "Simulate Fire Spread  (Graph BFS)", BWHITE);
    subItem(5, "Allocate Firefighting Resources", BWHITE);
    subItem(0, "Back", DIM);

    int c = subPrompt();
    Graphs::fireMenu(c);
}

// 5. Task Scheduling System 
static void menuTaskScheduling()
{
    printBanner("5. TASK SCHEDULING SYSTEM");
    subItem(1, "Add Routine Task  (Queue)", BWHITE);
    subItem(2, "Add Surveillance Task", BWHITE);
    subItem(3, "Add Emergency Task  (Priority Queue)", BRED);
    subItem(4, "Process Tasks  (FIFO / Priority)", BWHITE);
    subItem(5, "Pause and Resume Tasks", BYELLOW);
    subItem(6, "Multi-Factor Decision Queue  (Q4)", BCYAN);
    subItem(0, "Back", DIM);

    int c = subPrompt();
    Queues::scheduleMenu(c);
}

// 6. Decision System 
static void menuDecision()
{
    printBanner("6. DECISION SYSTEM");
    subItem(1, "Compute Risk Score", BWHITE);
    subItem(2, "Zone-Level Decision Tree", BWHITE);
    subItem(3, "Regional Decision Processing", BWHITE);
    subItem(4, "Global Emergency Decision", BRED);
    subItem(5, "Execute Final Action", BMAGENTA);
    subItem(0, "Back", DIM);

    int c = subPrompt();
    Trees::decisionMenu(c);
}

// 7. Spatial Routing System 
static void menuSpatial()
{
    printBanner("7. SPATIAL ROUTING SYSTEM");
    subItem(1, "Load Graph  (Adjacency List)", BWHITE);
    subItem(2, "Load Graph  (Adjacency Matrix)", BWHITE);
    subItem(3, "BFS Traversal  (Fire Spread)", BWHITE);
    subItem(4, "DFS Traversal  (Deep Analysis)", BWHITE);
    subItem(5, "Compute Safe Path", BGREEN);
    subItem(6, "Update Blocked Routes", BYELLOW);
    subItem(0, "Back", DIM);

    int c = subPrompt();
    Graphs::routingMenu(c);
}

// 8. Hash-Based Fast Access 
static void menuHash()
{
    printBanner("8. HASH-BASED FAST ACCESS SYSTEM");
    subItem(1, "Insert Data  (Hash Table)", BWHITE);
    subItem(2, "Retrieve Data  (O(1) Access)", BWHITE);
    subItem(3, "Handle Collisions", BYELLOW);
    subItem(4, "Update Cache", BWHITE);
    subItem(5, "View Index Table", BWHITE);
    subItem(0, "Back", DIM);

    int c = subPrompt();
    HashTables::hashMenu(c);
}

// 9. System Monitoring 
static void menuMonitoring()
{
    printBanner("9. SYSTEM MONITORING");
    subItem(1, "Monitor System Load", BWHITE);
    subItem(2, "Track Execution Time", BWHITE);
    subItem(3, "Detect Bottlenecks", BYELLOW);
    subItem(4, "Optimize Performance", BWHITE);
    subItem(5, "View System Health", BGREEN);
    subItem(0, "Back", DIM);

    int c = subPrompt();
    Monitoring::monitorMenu(c);
}

// 10. Scenario Simulation 
static void menuScenarios()
{
    printBanner("10. SCENARIO SIMULATION");
    subItem(1, "Cascading Fire Scenario", BRED);
    subItem(2, "Sensor Failure Scenario", BYELLOW);
    subItem(3, "Multi-Factor Anomaly Scenario", BMAGENTA);
    subItem(4, "System Overload Scenario", BBLUE);
    subItem(5, "Global Emergency Scenario", BCYAN);
    subItem(6, "Run Full System Simulation", BGREEN);
    subItem(0, "Back", DIM);

    int c = subPrompt();
    switch (c)
    {
    case 1:
    {
        scenario1_fire();
        break;
    }

    case 2:
    {
        scenario2_sensor();
        break;
    }

    case 3:
    {
        scenario3_anomaly();
        break;
    }

    case 4:
    {
        scenario4_overload();
        break;
    }

    case 5:
    {
        scenario5_global();
        break;
    }

    case 6:
    {
        Scenarios::runFullSystem();
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

// MAIN MENU 
void Menu::mainMenu()
{
    int choice;
    do {
        printBanner("IFAMDS  MAIN  MENU");
        cout << endl;
        cout << "  " << CYAN << "  [1]" << RESET << "  " << BWHITE << "Input Environmental Data" << endl << RESET;
        cout << "  " << CYAN << "  [2]" << RESET << "  " << BWHITE << "View Forest Grid Status" << endl << RESET;
        cout << "  " << CYAN << "  [3]" << RESET << "  " << BWHITE << "Event Memory System" << endl << RESET;
        cout << "  " << CYAN << "  [4]" << RESET << "  " << BWHITE << "Fire Detection and Control" << endl << RESET;
        cout << "  " << CYAN << "  [5]" << RESET << "  " << BWHITE << "Task Scheduling System" << endl << RESET;
        cout << "  " << CYAN << "  [6]" << RESET << "  " << BWHITE << "Decision System" << endl << RESET;
        cout << "  " << CYAN << "  [7]" << RESET << "  " << BWHITE << "Spatial Routing System" << endl << RESET;
        cout << "  " << CYAN << "  [8]" << RESET << "  " << BWHITE << "Hash-Based Fast Access System" << endl << RESET;
        cout << "  " << CYAN << "  [9]" << RESET << "  " << BYELLOW << "System Monitoring" << RESET << endl << RESET;
        cout << "  " << CYAN << " [10]" << RESET << "  " << BCYAN << "Scenario Simulation" << endl << RESET;
        cout << "  " << CYAN << " [11]" << RESET << "  " << BBLUE << "Generate Web Dashboard Report" << endl << RESET;
        cout << endl;
        cout << "  " << BOLD << BCYAN << "  Enter selection: " << RESET;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(9999, '\n');
            choice = -1;
        }

        switch (choice)
        {
        case 1:
        {
            menuInputData();
            break;
        }

        case 2:
        {
            menuViewForest();
            break;
        }

        case 3:
        {
            menuEventMemory();
            break;
        }

        case 4:
        {
            menuFireControl();
            break;
        }

        case 5:
        {
            menuTaskScheduling();
            break;
        }

        case 6:
        {
            menuDecision();
            break;
        }

        case 7:
        {
            menuSpatial();
            break;
        }

        case 8:
        {
            menuHash();
            break;
        }

        case 9:
        {
            menuMonitoring();
            break;
        }

        case 10:
        {
            menuScenarios();
            break;
        }

        case 11:
        {
            WebDashboard wb;
            wb.generateFullReport();
            printOK("Web report generated. Opening in browser...");
            break;
        }

        case 0:
        {
            printBanner("IFAMDS SESSION ENDED");
            printInfo("Thank you for using IFAMDS.");
            break;
        }

        default:
        {
            printWarn("Invalid option.");
        }
        }
    } while (choice != 0);
}