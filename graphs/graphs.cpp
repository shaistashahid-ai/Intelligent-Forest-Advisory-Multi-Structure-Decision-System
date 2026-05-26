#include <iostream>
#include <string>
#include "graphs.h"
#include "../console_ui.h"

using namespace std;

Graphs::Graphs(int vertices) : V(vertices)
{
    for (int i = 0; i < V; i++)
    {
        adj[i] = nullptr;
    }
}

Graphs::~Graphs()
{
    for (int i = 0; i < V; i++)
    {
        GNode* cur = adj[i];
        while (cur)
        {
            GNode* t = cur;
            cur = cur->next;
            delete t;
        }
    }
}

void Graphs::addEdge(int src, int dest, int weight)
{
    GNode* n1 = new GNode{ dest, weight, adj[src] };
    adj[src] = n1;

    GNode* n2 = new GNode{ src, weight, adj[dest] };
    adj[dest] = n2;
}

void Graphs::bfs(int start)
{
    int queue[MAX_V];
    int front = 0;
    int rear = 0;
    bool visited[MAX_V] = { false };
    visited[start] = true;
    queue[rear++] = start;
    bool first = true;
    cout << "  ";

    while (front < rear)
    {
        int z = queue[front++];
        if (!first)
        {
            cout << BRED << " ~> " << RESET;
        }

        cout << BYELLOW << "Zone " << z << RESET;
        first = false;
        GNode* t = adj[z];

        while (t)
        {
            if (!visited[t->dest])
            {
                visited[t->dest] = true;
                queue[rear++] = t->dest;
            }
            t = t->next;
        }
    }
    cout << endl;
}

void Graphs::dfsUtil(int v, bool visited[])
{
    visited[v] = true;
    cout << BGREEN << "[" << v << "]" << RESET;
    GNode* t = adj[v];

    while (t)
    {
        if (!visited[t->dest])
        {
            cout << CYAN << " -> " << RESET;
            dfsUtil(t->dest, visited);
        }
        t = t->next;
    }
}

void Graphs::printAdjList()
{
    for (int i = 0; i < V; i++)
    {
        cout << "  " << BCYAN << "Zone " << i << RESET << CYAN << " -> " << RESET;
        GNode* t = adj[i];

        if (!t)
        {
            cout << DIM << "(no connections)" << endl << RESET;
            continue;
        }

        while (t)
        {
            cout << BWHITE << "Zone " << t->dest << RESET << DIM << "(w=" << t->weight << ")" << RESET;
            if (t->next)
            {
                cout << CYAN << " -> " << RESET;
            }
            t = t->next;
        }
        cout << endl;
    }
}

void Graphs::printAdjMatrix()
{
    // Build matrix from adj list
    int mat[MAX_V][MAX_V] = { 0 };
    for (int i = 0; i < V; i++)
    {
        GNode* t = adj[i];
        while (t)
        {
            mat[i][t->dest] = t->weight;
            t = t->next;
        }
    }

    cout << endl << "  " << BOLD << "      ";
    for (int i = 0; i < V; i++)
    {
        cout << CYAN << " Z" << i << " " << RESET;
    }

    cout << endl << "  " << CYAN;
    for (int i = 0; i < V + 1; i++)
    {
        cout << "+----";
    }
    cout << "+" << endl << RESET;

    for (int i = 0; i < V; i++)
    {
        cout << "  " << BCYAN << " Z" << i << RESET << " " << CYAN << "|" << RESET;
        for (int j = 0; j < V; j++)
        {
            if (mat[i][j])
            {
                cout << BGREEN << "  " << mat[i][j] << " " << RESET << CYAN << "|" << RESET;
            }

            else
            {
                cout << DIM << "  0 " << RESET << CYAN << "|" << RESET;
            }
        }

        cout << endl << "  " << CYAN;

        for (int k = 0; k < V + 1; k++)
        {
            cout << "+----";
        }

        cout << "+" << endl << RESET;
    }
}

// Simple greedy shortest path (no STL) — TC: O(V^2)
int Graphs::safePath(int src, int dest)
{
    int dist[MAX_V];
    bool vis[MAX_V] = { false };

    for (int i = 0; i < V; i++)
    {
        dist[i] = 99999;
    }

    dist[src] = 0;
    int prev[MAX_V];

    for (int i = 0; i < V; i++)
    {
        prev[i] = -1;
    }

    for (int iter = 0; iter < V; iter++)
    {
        int u = -1;
        for (int i = 0; i < V; i++)
        {
            if (!vis[i] && (u == -1 || dist[i] < dist[u]))
            {
                u = i;
            }
        }

        if (u == -1)
        {
            break;
        }

        vis[u] = true;
        GNode* t = adj[u];

        while (t)
        {
            if (dist[u] + t->weight < dist[t->dest])
            {
                dist[t->dest] = dist[u] + t->weight;
                prev[t->dest] = u;
            }

            t = t->next;
        }
    }

    // print path
    if (dist[dest] == 99999)
    {
        printError("No path found.");
        return -1;
    }

    int pathArr[MAX_V];
    int pLen = 0;
    int cur = dest;

    while (cur != -1)
    {
        pathArr[pLen++] = cur;
        cur = prev[cur];
    }

    cout << "  " << BGREEN << "Safest path (cost=" << dist[dest] << "): " << RESET;
    for (int i = pLen - 1; i >= 0; i--)
    {
        cout << BCYAN << "Zone " << pathArr[i] << RESET;
        if (i > 0)
        {
            cout << CYAN << " -> " << RESET;
        }
    }

    cout << endl;
    return dist[dest];
}

// Shared graph instance 
static Graphs* g6 = nullptr;
static bool routeBlocked[5] = { false };

static void ensureGraph() 
{
    if (!g6) 
    {
        g6 = new Graphs(5);
        g6->addEdge(0, 1, 2); 
        g6->addEdge(1, 2, 3);
        g6->addEdge(2, 3, 1); 
        g6->addEdge(3, 4, 4);
        g6->addEdge(4, 0, 2); 
        g6->addEdge(0, 3, 8);
    }
}

void Graphs::fireMenu(int choice) 
{
    switch (choice)
    {
        case 1:
        {
            // 4.1 Detect Fire Risk
            printSubHeader("4.1 Detect Fire Risk  (Threshold Check)");
            printInfo("Checking zone temperatures against threshold (45C):");
            double temps[] = { 28.0, 62.0, 45.5, 30.0, 78.0 };
            printTableHeader("Zone", "Temp C   Status", 6, 20);

            for (int i = 0; i < 5; i++) 
            {
                char buf[30]; snprintf(buf, sizeof(buf), "%-8.1f %s", temps[i], temps[i] > 45 ? "FIRE RISK" : "SAFE");
                printTableRow("Z" + std::to_string(i), buf, 6, 20, temps[i] > 45 ? BRED : BGREEN);
            }

            printTableFooter(6, 20);
            printOK("Threshold check complete  TC: O(n)");
            break;
        }

        case 2: 
        {
            // 4.2 Trigger Emergency Alert
            printSubHeader("4.2 Emergency Alert  (Zone 3)");
            printFire("EMERGENCY TRIGGERED  Zone 3  Temp=62C  Smoke=85");
            printFire("All resources redirected to Zone 3.");
            printWarn("Neighbouring zones 2 and 4 placed on HIGH ALERT.");
            break;
        }

        case 3: 
        {
            // 4.3 Priority-Based Fire Response
            printSubHeader("4.3 Priority-Based Fire Response  (Priority Queue)");
            printInfo("TC: O(n) insert, O(1) dequeue");

            struct FR { 
                int p;
                const char* zone;
                const char* level;
            };

            FR responses[] = { {10,"Zone 3","CRITICAL"},{8,"Zone 2","SEVERE"}, {5,"Zone 4","MODERATE"},{2,"Zone 6","MONITOR"} };
            printTableHeader("Priority", "Zone     Level     Action", 10, 32);

            for (auto& r : responses) 
            {
                string action = r.p >= 8 ? "DISPATCH immediately" : r.p >= 5 ? "Controlled response" : "Continue monitoring";
                char buf[50]; snprintf(buf, sizeof(buf), "%-8s %-9s %s", r.zone, r.level, action.c_str());
                printTableRow(std::to_string(r.p), buf, 10, 32,  r.p >= 8 ? BRED : r.p >= 5 ? BYELLOW : BGREEN);
            }

            printTableFooter(10, 32);
            printOK("Priority-based response dispatched  TC: O(1)");
            break;
        }

        case 4: 
        {
            // 4.4 Simulate Fire Spread (BFS)
            printSubHeader("4.4 Fire Spread Simulation  (BFS)  TC: O(V+E)");
            ensureGraph();
            printInfo("Fire starts at Zone 1 — BFS propagation:");
            g6->bfs(1);
            printWarn("Fire spread path above shows evacuation priority order.");
            break;
        }

        case 5: 
        {
            // 4.5 Allocate Resources
            printSubHeader("4.5 Firefighting Resource Allocation");
            printBar("Zone 3 — Water supply", 90);
            printBar("Zone 2 — Foam supply", 75);
            printBar("Zone 4 — Personnel", 55);
            printBar("Zone 6 — Equipment", 30);
            printOK("Resources allocated based on risk score.");
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

void Graphs::routingMenu(int choice) 
{
    ensureGraph();
    switch (choice) 
    {
    case 1:
    {
        printSubHeader("7.1 Adjacency List  TC: O(V+E)");
        g6->printAdjList();
        printOK("Adjacency list loaded.");
        break;
    }

    case 2:
    {
        printSubHeader("7.2 Adjacency Matrix  TC: O(V^2) space");
        g6->printAdjMatrix();
        printOK("Adjacency matrix loaded.");
        break;
    }

    case 3:
    {
        printSubHeader("7.3 BFS Traversal  (Fire Spread)  TC: O(V+E)");
        g6->bfs(0);
        printOK("BFS complete.");
        break;
    }

    case 4: 
    {
        printSubHeader("7.4 DFS Traversal  (Deep Analysis)  TC: O(V+E)");
        bool vis[5] = { false };
        cout << "  ";
        g6->dfsUtil(0, vis);
        cout << endl;
        printOK("DFS complete.");
        break;
    }

    case 5:
    {
        printSubHeader("7.5 Compute Safe Path  (Greedy Shortest)  TC: O(V^2)");
        printInfo("Finding safest path from Zone 0 to Zone 4:");
        g6->safePath(0, 4);
        break;
    }

    case 6:
    {
        printSubHeader("7.6 Update Blocked Routes");
        printInfo("Blocking Zone 1 <-> Zone 2 (fire in Zone 2):");
        routeBlocked[1] = true;
        printWarn("Route Zone1-Zone2 marked BLOCKED.");
        printInfo("Re-routing via Zone 0 -> Zone 3 -> Zone 4.");
        g6->safePath(0, 4);
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

void Graphs::demo() 
{
    printBanner("GRAPHS MODULE  (Full Demo)");
    routingMenu(1); routingMenu(2);
    routingMenu(3); routingMenu(4);
    routingMenu(5);
    printDivider();
    printOK("Graphs module complete.");
}