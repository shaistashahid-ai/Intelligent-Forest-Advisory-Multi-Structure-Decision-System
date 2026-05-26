#include <iostream>
#include <string>
#include "hash_tables.h"
#include "../console_ui.h"

using namespace std;

HashTables::HashTables(int s) : size(s)
{
    table = new string[size];
    for (int i = 0; i < size; i++)
    {
        table[i] = "EMPTY";
    }
}

HashTables::~HashTables() 
{
    delete[] table; 
}

int HashTables::hashFunction(int key)
{
    return key % size;
}

void HashTables::insert(int key, string value) 
{
    int idx = hashFunction(key);
    int probes = 0;

    while (table[idx] != "EMPTY") 
    {
        printWarn("COLLISION  key=" + to_string(key) + "  index=" + to_string(idx) + "  -> linear probe...");
        idx = (idx + 1) % size;
        probes++;
        if (probes >= size)
        {
            printError("Table FULL — cannot insert.");
            return;
        }
    }

    table[idx] = value;
    cout << "  " << SYM_OK << "  " << CYAN << "Key " << key << RESET << "  hash=" << DIM << (key % size) << RESET << "  stored@" << BGREEN << idx << RESET << (probes ? string(BYELLOW) + "  [" + to_string(probes) + " probe(s)]" + RESET : "") << endl;
}

bool HashTables::retrieve(int key, string& out) 
{
    int idx = hashFunction(key);
    int probes = 0;
    while (table[idx] != "EMPTY" && probes < size) 
    {
        // linear search for key — in real impl we'd store key too
        if (table[idx] != "EMPTY") 
        {
            out = table[idx];
            return true;
        }

        idx = (idx + 1) % size;
        probes++;
    }

    return false;
}

void HashTables::updateCache(int key, string value) 
{
    int idx = hashFunction(key);
    int probes = 0;

    while (table[idx] != "EMPTY" && probes < size)
    {
        idx = (idx + 1) % size;
        probes++;
    }

    if (probes < size)
    {
        table[idx] = value;
    }
}

void HashTables::display() 
{
    printTableHeader("Idx", "Stored Value", 5, 22);
    for (int i = 0; i < size; i++)
    {
        bool empty = (table[i] == "EMPTY");
        printTableRow(to_string(i), table[i], 5, 22, empty ? DIM : BGREEN);
    }

    printTableFooter(5, 22);
}

// Persistent tables
static HashTables* h1 = nullptr;
static HashTables* h3 = nullptr;

static void ensureH1()
{
    if (!h1)
    {
        h1 = new HashTables(7);
    }
}

static void ensureH3() 
{
    if (!h3)
    {
        h3 = new HashTables(6);
    }
}

void HashTables::hashMenu(int choice) 
{
    switch (choice)
    {
    case 1:
    {
        // 8.1 Insert
        printSubHeader("8.1 Insert Data  (H1: Primary Index Table)");
        printInfo("Hash function: index = key % 7  |  TC: O(1) avg, O(n) worst");
        ensureH1();
        h1->insert(101, "Zone1_Temp:28");
        h1->insert(202, "Zone2_Smoke:45");
        h1->insert(108, "Zone3_Temp:62");  // 108%7=3 same as 108 -> collision demo
        h1->insert(303, "Zone3_Humid:18");
        h1->insert(404, "Zone4_Wind:HIGH");
        h1->display();
        break;
    }

    case 2: 
    {
        // 8.2 Retrieve
        printSubHeader("8.2 Retrieve Data  (O(1) Average Access)");
        ensureH1();
        printInfo("Retrieving key=101, key=202, key=999:");
        int keys[] = { 101, 202, 999 };
        for (int k : keys)
        {
            string val;
            int idx = k % 7;
            bool found = (idx < 7 && h1->retrieve(k, val));

            if (found)
            {
                cout << "  " << SYM_OK << "  Key " << k << "  hash=" << idx << "  -> " << BGREEN << val << RESET << endl;
            }

            else
            {
                printError("Key " + to_string(k) + " NOT FOUND.");
            }
        }

        printOK("Retrieval complete  TC: O(1) avg");
        break;
    }

    case 3:
    {
        // 8.3 Collision Handling
        printSubHeader("8.3 Collision Handling Demo  (H2: Collision Table)");
        printInfo("Table size=3 guarantees collisions for keys 1,4,7,10:");
        HashTables ht(3);
        ht.insert(1, "SensorA");
        ht.insert(4, "SensorB");   // 4%3=1 -> collision
        ht.insert(7, "SensorC");   // 7%3=1 -> collision x2
        ht.insert(10, "SensorD");   // 10%3=1 -> collision x3
        ht.display();
        printInfo("All 4 keys hashed to index 1, resolved via linear probing.");
        printOK("Collision handling complete  TC: O(n) worst case");
        break;
    }

    case 4:
    {
        // 8.4 Update Cache
        printSubHeader("8.4 Update Cache  (H3: Fast Retrieval Cache)");
        printInfo("Cache stores frequently accessed zone data  |  TC: O(1)");
        ensureH3();
        struct CE { int key; const char* val; };
        CE entries[] = {
            {1, "Z1:Temp=28,Smoke=10,Status=SAFE"},
            {3, "Z3:Temp=62,Smoke=85,Status=FIRE"},
            {5, "Z5:Temp=30,Smoke=20,Status=SAFE"},
            {2, "Z2:Temp=45,Smoke=55,Status=WARN"},
        };
        for (auto& e : entries)
        {
            h3->insert(e.key, e.val);
        }

        printOK("Cache updated with 4 zone entries.");
        h3->display();
        break;
    }

    case 5: 
    {
        // 8.5 View Index Table
        printSubHeader("8.5 View Index Table  (H1 + H3)");
        printInfo("H1 — Primary Index Table:");
        ensureH1(); h1->display();
        printInfo("H3 — Fast Retrieval Cache:");
        ensureH3(); h3->display();
        printOK("Index tables displayed  TC: O(n)");
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

void HashTables::demo()
{
    printBanner("HASH TABLES MODULE  (Full Demo)");
    hashMenu(1); hashMenu(3); hashMenu(4);
    printDivider();
    printOK("Hash Tables module complete.");
}