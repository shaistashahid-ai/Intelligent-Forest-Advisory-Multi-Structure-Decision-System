#include <iostream>
#include "scenarios.h"
#include "../console_ui.h"

using namespace std;

void Scenarios::runIndividual()
{
    printBanner("SCENARIO SELECTION");

    cout << endl;
    cout << "  " << CYAN << "  [1]" << RESET << "  " << BRED << "Cascading Fire & Resource Conflict\n" << RESET;
    cout << "  " << CYAN << "  [2]" << RESET << "  " << BYELLOW << "Sensor Failure & Reconstruction\n" << RESET;
    cout << "  " << CYAN << "  [3]" << RESET << "  " << BMAGENTA << "Multi-Factor Anomaly Escalation\n" << RESET;
    cout << "  " << CYAN << "  [4]" << RESET << "  " << BBLUE << "System Overload & Load Redistribution\n" << RESET;
    cout << "  " << CYAN << "  [5]" << RESET << "  " << BGREEN << "Global Multi-Zone Emergency Sync\n" << RESET;
    cout << endl;
    cout << "  " << BOLD << BCYAN << "  Select Scenario: " << RESET;

    int choice;
    cin >> choice;
    switch (choice)
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

    default:
    {
        printWarn("Invalid scenario number.");
    }
    }
}

void Scenarios::runFullSystem() 
{
    printBanner("FULL SYSTEM SIMULATION");
    printInfo("Running all 5 scenarios in sequence...");

    scenario1_fire();
    scenario2_sensor();
    scenario3_anomaly();
    scenario4_overload();
    scenario5_global();

    printBanner("FULL SYSTEM RUN COMPLETE");
    printOK("All 5 scenarios executed successfully.");
}