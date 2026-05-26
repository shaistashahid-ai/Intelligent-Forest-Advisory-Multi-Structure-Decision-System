#include <iostream>
#include <string>
#include "scenarios.h"
#include "../monitoring/monitoring.h"
#include "../console_ui.h"

using namespace std;

// Global sync uses adjacency matrix graph
const int S5_ZONES = 6;

// Each zone tracks its own reported state and HQ-verified state
struct ZoneState {
    int    id;
    double reportedTemp;
    double reportedSmoke;
    double verifiedTemp;
    double verifiedSmoke;
    bool   conflicting;   // computed, not hardcoded
    bool   synced;
};

// Compute whether two readings conflict — TC: O(1)
static bool isConflict(double a, double b, double threshold = 15.0) 
{
    double diff = a - b;
    if (diff < 0)
    {
        diff = -diff;
    }

    return diff > threshold;
}

void scenario5_global()
{
    printBanner("SCENARIO 5: GLOBAL EMERGENCY SYNCHRONIZATION");
    Monitoring mon("Scenario 5 Execution");

    // Adjacency matrix (G2) for zone connections
    int adj[S5_ZONES][S5_ZONES] = { 0 };
    adj[0][1] = 1; adj[1][0] = 1;
    adj[1][2] = 1; adj[2][1] = 1;
    adj[2][3] = 1; adj[3][2] = 1;
    adj[3][4] = 1; adj[4][3] = 1;
    adj[4][5] = 1; adj[5][4] = 1;
    adj[0][3] = 1; adj[3][0] = 1;   // cross-link HQ to Zone 3

    // Zone states: regional vs HQ readings (asynchronous)
    ZoneState zones[S5_ZONES] = {
        {0, 25.0, 10.0, 25.0, 10.0, false, false},  // HQ — always trusted
        {1, 62.0, 80.0, 30.0, 15.0, false, false},  // big drift — conflict
        {2, 45.0, 55.0, 47.0, 52.0, false, false},  // small drift — ok
        {3, 70.0, 90.0, 28.0, 12.0, false, false},  // big drift — conflict
        {4, 33.0, 22.0, 35.0, 20.0, false, false},  // small drift — ok
        {5, 55.0, 68.0, 29.0, 14.0, false, false},  // big drift — conflict
    };

    // Detect conflicts by computing drift 
    // Time Complexity: O(V)
    printSubHeader("Phase 1 — Conflict Detection  (Computed from Sensor Drift)");
    printInfo("Conflict = |regional - HQ| > 15  |  TC: O(V)");
    printTableHeader("Zone", "Regional T/S     HQ T/S          Status", 6, 44);

    int conflictCount = 0;
    for (int i = 0; i < S5_ZONES; i++) 
    {
        zones[i].conflicting = isConflict(zones[i].reportedTemp, zones[i].verifiedTemp) || isConflict(zones[i].reportedSmoke, zones[i].verifiedSmoke);
        if (zones[i].conflicting)
        {
            conflictCount++;
        }

        char buf[80];
        snprintf(buf, sizeof(buf), "T=%.0f S=%.0f   ->   T=%.0f S=%.0f   %s", zones[i].reportedTemp, zones[i].reportedSmoke,zones[i].verifiedTemp, zones[i].verifiedSmoke, zones[i].conflicting ? "CONFLICT" : "OK");

        const char* col = zones[i].conflicting ? BRED : (i == 0) ? BGREEN : BYELLOW;
        printTableRow("Z" + to_string(i), buf, 6, 44, col);
    }

    printTableFooter(6, 44);

    if (conflictCount > 0)
    {
        printWarn(to_string(conflictCount) + " zone(s) have conflicting state  -> BFS sync required.");
    }

    else
    {
        printOK("No conflicts detected.");
    }

    // Print adjacency matrix
    printSubHeader("G2: Zone Connection Matrix  (Adjacency Matrix)");
    printInfo("TC: O(V^2) space  |  Edges show sync propagation paths");

    cout << endl << "  " << BOLD << "     ";
    for (int i = 0; i < S5_ZONES; i++)
    {
        cout << CYAN << "  Z" << i << " " << RESET;
    }

    cout << endl << "  " << CYAN;
    for (int i = 0; i < S5_ZONES + 1; i++)
    {
        cout << "+----";
    }

    cout << "+" << endl << RESET;

    for (int i = 0; i < S5_ZONES; i++)
    {
        cout << "  " << BCYAN << "Z" << i << RESET << " " << CYAN << "|" << RESET;
        for (int j = 0; j < S5_ZONES; j++) 
        {
            if (adj[i][j])
            {
                cout << BGREEN << "  1 " << RESET << CYAN << "|" << RESET;
            }

            else
            {
                cout << DIM << "  0 " << RESET << CYAN << "|" << RESET;
            }
        }

        cout << endl << "  " << CYAN;

        for (int k = 0; k < S5_ZONES + 1; k++)
        {
            cout << "+----";
        }
        cout << "+" << endl << RESET;
    }

    // BFS global sync from Zone 0 (HQ)
    // Time Complexity: O(V + E)
    printSubHeader("Phase 2 — BFS Sync Broadcast from HQ (Zone 0)");
    printInfo("HQ overwrites conflicting zone states  |  TC: O(V + E)");

    int bfsQueue[S5_ZONES * 2];
    int front = 0, rear = 0;
    bool visited[S5_ZONES] = { false };

    bfsQueue[rear++] = 0;
    visited[0] = true;
    zones[0].synced = true;

    cout << endl;
    while (front < rear)
    {
        int z = bfsQueue[front++];

        // Overwrite conflicting zone with HQ-verified values
        if (zones[z].conflicting) 
        {
            zones[z].reportedTemp = zones[z].verifiedTemp;
            zones[z].reportedSmoke = zones[z].verifiedSmoke;
            zones[z].conflicting = false;
            cout << "  " << BRED << "[CORRECTED]" << RESET  << "  Zone " << z << "  state overwritten with HQ values" << "  T=" << zones[z].verifiedTemp << "  S=" << zones[z].verifiedSmoke << endl;
        }

        else
        {
            cout << "  " << BGREEN << "[CONFIRMED]" << RESET << "  Zone " << z << "  state consistent  -> accepted." << endl;
        }

        zones[z].synced = true;

        for (int n = 0; n < S5_ZONES; n++)
        {
            if (adj[z][n] && !visited[n])
            {
                visited[n] = true;
                bfsQueue[rear++] = n;
            }
        }
    }

    // Post-sync state
    printSubHeader("Phase 3 — Post-Sync Global State");
    printTableHeader("Zone", "Final T   Final S   Conflict   Synced", 6, 40);
    for (int i = 0; i < S5_ZONES; i++) 
    {
        char buf[60];
        snprintf(buf, sizeof(buf), "%-9.0f %-9.0f %-10s %s", zones[i].reportedTemp, zones[i].reportedSmoke, zones[i].conflicting ? "YES" : "NO", zones[i].synced ? "YES" : "NO");
        const char* col = zones[i].synced ? BGREEN : BRED;
        printTableRow("Z" + to_string(i), buf, 6, 40, col);
    }

    printTableFooter(6, 40);

    // Resource allocation
    printSubHeader("Phase 4 — Synchronised Response Strategy");
    printInfo("Resources allocated based on verified (post-sync) state.");
    for (int i = 0; i < S5_ZONES; i++)
    {
        if (zones[i].reportedTemp > 50 || zones[i].reportedSmoke > 60)
        {
            printFire("Zone " + to_string(i) + "  T=" + to_string((int)zones[i].reportedTemp) + "  S=" + to_string((int)zones[i].reportedSmoke) + "  -> Firefighting resources dispatched.");
        }

        else
        {
            printOK("Zone " + to_string(i) + "  -> Routine monitoring maintained.");
        }
    }

    printDivider();
    printOK("All " + to_string(S5_ZONES) + " zones synchronised. Global state consistent.");
    printOK("Scenario 5 complete.");
}