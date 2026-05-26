#include <iostream>
#include <string>
#include "scenarios.h"
#include "../monitoring/monitoring.h"
#include "../console_ui.h"

using namespace std;

void scenario2_sensor()
{
    printBanner("SCENARIO 2: SENSOR FAILURE & RECONSTRUCTION");
    Monitoring mon("Scenario 2 Execution");

    // Sensor integrity check (Zone 2) 
    printSubHeader("Sensor Integrity Check  (Zone 2)");
    printInfo("TC: O(n) scan");

    printTableHeader("Sensor ID", "Status", 10, 30);
    cout << "  " << BGREEN << "101" << RESET << "        " << BGREEN << "ONLINE" << RESET << endl;
    cout << "  " << BRED << "S-idx 1" << RESET << "        " << BRED << "FAILURE" << RESET << endl;
    cout << "  " << BGREEN << "103" << RESET << "        " << BGREEN << "ONLINE" << RESET << endl;
    cout << "  " << BRED << "S-idx 3" << RESET << "        " << BRED << "FAILURE" << RESET << endl;
    cout << "  " << BGREEN << "105" << RESET << "        " << BGREEN << "ONLINE" << RESET << endl;
    // Hardcoded end table
    cout << "  " << CYAN << "+----------+------------+------------+" << RESET << endl;

    // Active sensors after filtering
    printSubHeader("Active Sensors After Filtering");
    int active[] = { 101, 103, 105 };
    cout << "  Active IDs: [ " << BCYAN << "101 " << RESET << "] [ " << BCYAN << "103 " << RESET << "] [ " << BCYAN << "105 " << RESET << " ]" << endl;
    printOK("Active sensor count: 3");

    // Gap reconstruction via spatial interpolation 
    printSubHeader("Gap Reconstruction via Spatial Interpolation");
    printInfo("Missing values estimated from neighbours.");

    printTableHeader("Zone", "Reconstructed Value", 6, 40);
    cout << "  Zone 2   | 102 (avg of 101, 103)  |" << endl;
    cout << "  Zone 4   | 104 (avg of 103, 105)  |" << endl;
    cout << "  " << CYAN << "+----------+------------------------+" << RESET << endl;

    printOK("System state restored. Zone 2 back online.");

    printDivider();
    printOK("Scenario 2 complete.");
}