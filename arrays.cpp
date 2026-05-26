#include <iostream>
#include <iomanip>
#include <cmath>
#include "arrays.h"
#include "../console_ui.h"

using namespace std;

// A1: Static baseline values
static const int NUM_SENSORS = 5;
static const int SENSOR_IDS[5] = {101, 102, 103, 104, 105};
static const double BASE_TEMP = 25.0;
static const double BASE_SMOKE = 0.0;
static const double BASE_HUMIDITY = 60.0;
static const double TEMP_THRESHOLD = 45.0;
static const double SMOKE_THRESHOLD = 70.0;
static const double HUMID_THRESHOLD = 20.0;

// A2: Dynamic sensor stream (live readings) 
static double* liveTemp = nullptr;
static double* liveSmoke = nullptr;
static double* liveHumid = nullptr;
static int liveCount = 0;

// A3/A4: 2D zone grid 4x4 
static double zoneGrid[4][4] = {
    {25.0, 62.0, 30.0, 28.0},
    {27.0, 45.0, 80.0, 32.0},
    {26.0, 29.0, 33.0, 70.0},
    {24.0, 31.0, 28.0, 55.0}
};

// Spatial interpolation for missing cell — TC: O(1)
static double interpolate(int r, int c, int rows, int cols) 
{
    double sum = 0; 
    int count = 0;

    if (r > 0)      
    {
        sum += zoneGrid[r-1][c]; 
        count++;
    }

    if (r < rows-1)  
    {
        sum += zoneGrid[r+1][c]; 
        count++;
    }

    if (c > 0)      
    {
        sum += zoneGrid[r][c-1]; 
        count++;
    }

    if (c < cols-1) 
    {
        sum += zoneGrid[r][c+1]; 
        count++;
    }

    return count ? sum / count : 25.0;
}

// Menu 1: Input Environmental Data
void Arrays::inputMenu(int choice) 
{
    switch (choice)
    {
        case 1: {
            // 1.1 Add Sensor Reading
            printSubHeader("1.1 Add Sensor Reading");
            printInfo("Enter zone (0-3), temperature, smoke, humidity:");

            int z; 
            double t;
            double s;
            double h;

            cout << "    Zone: "; 
            cin >> z;
            if (z < 0 || z > 3)
            {
                printError("Invalid zone (0-3)."); 
                break;
            }

            cout << "    Temp (C): ";
            cin >> t;

            cout << "    Smoke:    "; 
            cin >> s;

            cout << "    Humidity: "; 
            cin >> h;

            zoneGrid[z][0] = t;
            printOK("Reading accepted for Zone " + std::to_string(z));

            if (t > TEMP_THRESHOLD)
            {
                printFire("Temperature " + std::to_string((int)t) + "C exceeds threshold!");
            }

            if (s > SMOKE_THRESHOLD)
            {
                printWarn("Smoke level " + std::to_string((int)s) + " exceeds safe limit!");
            }

            if (h < HUMID_THRESHOLD)
            {
                printWarn("Humidity " + std::to_string((int)h) + "% critically low!");
            }

            break;
        }

        case 2: 
        {
            // 1.2 Store Data in Dynamic Array
            printSubHeader("1.2 Store Data in Dynamic Array  (A2)");
            // Allocate / re-allocate dynamic stream — TC: O(n)
            if (liveTemp)
            {
                delete[] liveTemp;
            }

            if (liveSmoke)
            {
                delete[] liveSmoke;
            }

            if (liveHumid)
            {
                delete[] liveHumid;
            }

            liveCount = 5;
            liveTemp  = new double[liveCount] {28.0, 33.0, 46.0, 55.0, 62.0};
            liveSmoke = new double[liveCount] {5.0,  12.0, 40.0, 68.0, 85.0};
            liveHumid = new double[liveCount] {58.0, 50.0, 35.0, 22.0, 15.0};

            printTableHeader("Time Step", "Temp C    Smoke    Humidity", 10, 30);
            for (int i = 0; i < liveCount; i++)
            {
                char buf[60];
           
                snprintf(buf, sizeof(buf), "%-9.1f %-8.1f %.1f%%", liveTemp[i], liveSmoke[i], liveHumid[i]);
                const char* col = (liveTemp[i] > TEMP_THRESHOLD) ? BRED : BWHITE;
                printTableRow("T+" + std::to_string(i+1), buf, 10, 30, col);
            }

            printTableFooter(10, 30);
            printOK("Dynamic array A2 allocated  TC: O(n)  size=" + std::to_string(liveCount));
            break;
        }

        case 3:
        {
            // 1.3 Compare with Static Baseline
            printSubHeader("1.3 Compare Live Readings vs Static Baseline  (A1 vs A2)");
            if (!liveTemp)
            {
                printWarn("Run option 2 first to load live data."); 
                break;
            }

            printInfo("Baseline: Temp=" + std::to_string((int)BASE_TEMP) + "C  Smoke=" + std::to_string((int)BASE_SMOKE) + "  Humid=" + std::to_string((int)BASE_HUMIDITY) + "%");
            printTableHeader("Step", "Delta-Temp  Delta-Smoke  Delta-Humid  Status", 6, 46);
            for (int i = 0; i < liveCount; i++)
            {
                double dt = liveTemp[i]  - BASE_TEMP;
                double ds = liveSmoke[i] - BASE_SMOKE;
                double dh = BASE_HUMIDITY - liveHumid[i];
                char buf[80];
                snprintf(buf, sizeof(buf), "+%-10.1f +%-12.1f -%-12.1f %s", dt, ds, dh, (dt > 20 || ds > 60) ? "ANOMALY" : "NORMAL");
                const char* col = (dt > 20 || ds > 60) ? BRED : BGREEN;
                printTableRow("T+" + std::to_string(i+1), buf, 6, 46, col);
            }

            printTableFooter(6, 46);
            printOK("Baseline comparison complete  TC: O(n)");
            break;
        }

        case 4: 
        {
            // 1.4 Validate and Filter Noise
            printSubHeader("1.4 Validate and Filter Noise");
            printInfo("Noise rule: |val_i - val_{i-1}| >= delta=15 => noise");
            double raw[] = {25.0, 27.0, 90.0, 29.0, 31.0, -5.0, 33.0};
            int n = 7;
            printTableHeader("Index", "Value     Status", 6, 22);
            for (int i = 0; i < n; i++) 
            {
                bool invalid = (raw[i] < 0 || raw[i] > 80);
                bool noise   = (i > 0 && !invalid && fabs(raw[i] - raw[i-1]) >= 15.0);
                string status = invalid ? "INVALID (out-of-range)" : noise   ? "NOISE   (spike)" : "VALID";
                const char* col = (invalid || noise) ? BRED : BGREEN;
                char buf[30]; snprintf(buf, sizeof(buf), "%-9.1f %s", raw[i], status.c_str());
                printTableRow(std::to_string(i), buf, 6, 22, col);
            }

            printTableFooter(6, 22);
            printOK("Noise filtering complete  TC: O(n)");
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

// Menu 2: View Forest Grid
void Arrays::viewMenu(int choice)
{
    switch (choice)
    {
    case 1: 
    {
        // 2.1 1D time-series
        printSubHeader("2.1 1D Time-Series Data  (Temperature Trend)");
        if (!liveTemp) 
        {
            double tmp[] = {25,27,30,33,46,55,62};
            printInfo("No live data — showing sample stream:");
            std::cout << "  " << CYAN << "Time -> " << RESET;
            for (int i = 0; i < 7; i++) 
            {
                const char* c = (tmp[i] > TEMP_THRESHOLD) ? BRED : BGREEN;
                cout << c << "[" << (int)tmp[i] << "]" << RESET;
                if (i < 6)
                {
                    cout << CYAN << "->" << RESET;
                }
            }
            cout << endl;
        } 

        else
        {
            cout << "  " << CYAN << "Time -> " << RESET;
            for (int i = 0; i < liveCount; i++)
            {
                const char* c = (liveTemp[i] > TEMP_THRESHOLD) ? BRED : BGREEN;
                cout << c << "[" << (int)liveTemp[i] << "]" << RESET;

                if (i < liveCount - 1)
                {
                    cout << CYAN << "->" << RESET;
                }
            }
            cout << endl;
        }

        printOK("Trend shows " + string(liveTemp && liveTemp[liveCount-1] > TEMP_THRESHOLD ? "RISING temperature — fire risk detected." : "stable readings."));
        break;
    }

    case 2: 
    {
        // 2.2 2D Forest Zone Matrix
        printSubHeader("2.2 2D Forest Zone Matrix  (Temperature Grid)");
        printInfo("Values > 45C = DANGER  |  TC: O(n*m) traversal");
        cout << endl << "  " << BOLD << "       Z0      Z1      Z2      Z3" << endl << RESET;
        cout << "  " << CYAN << "   +-------+-------+-------+-------+" << endl << RESET;
        for (int i = 0; i < 4; i++)
        {
            cout << "  " << BCYAN << "R" << i << RESET << " " << CYAN << "|" << RESET;
            for (int j = 0; j < 4; j++) 
            {
                double v = zoneGrid[i][j];
                const char* col = (v > TEMP_THRESHOLD) ? BRED : BGREEN;
                char buf[8]; snprintf(buf, sizeof(buf), "%5.1f", v);
                cout << col << buf << RESET << " " << CYAN << "|" << RESET;
            }

            cout << endl;
            if (i < 3)
            {
                cout << "  " << CYAN << "   +-------+-------+-------+-------+" << endl << RESET;
            }
        }
        cout << "  " << CYAN << "   +-------+-------+-------+-------+" << endl << RESET;
        cout << "  " << BGREEN << "  <=45C Safe  " << RESET << BRED << ">45C DANGER" << endl << RESET;
        break;
    }

    case 3:
    {
        // 2.3 Zone-wise Conditions
        printSubHeader("2.3 Zone-wise Conditions  (Boundary Detection)");
        printInfo("Boundary: adjacent zones differ by > 20C  |  TC: O(n*m)");
        printTableHeader("Zone", "Avg Temp   Status    Boundary", 6, 32);
        for (int i = 0; i < 4; i++) 
        {
            double sum = 0;
            for (int j = 0; j < 4; j++)
            {
                sum += zoneGrid[i][j];
            }

            double avg = sum / 4.0;
            bool boundary = false;
            if (i > 0)
            {
                double prevSum = 0;
                for (int j = 0; j < 4; j++)
                {
                    prevSum += zoneGrid[i - 1][j];
                }

                if (fabs(avg - prevSum / 4.0) > 20.0)
                {
                    boundary = true;
                }
            }

            char buf[40];
            snprintf(buf, sizeof(buf), "%-10.1f %-9s %s", avg, avg > TEMP_THRESHOLD ? "DANGER" : "SAFE", boundary ? "YES" : "NO");
            const char* col = avg > TEMP_THRESHOLD ? BRED : BGREEN;
            printTableRow("R" + std::to_string(i), buf, 6, 32, col);
        }
        printTableFooter(6, 32);

        // Spatial interpolation demo
        printSubHeader("Spatial Interpolation  (Missing Zone Estimate)");
        printInfo("Estimating missing Zone [1][1] from its 4 neighbours:");
        double est = interpolate(1, 1, 4, 4);
        char ibuf[40]; 
        snprintf(ibuf, sizeof(ibuf), "%.1f C  (avg of neighbours)", est);
        printKV("Interpolated value:", ibuf, BYELLOW);
        printOK("Interpolation complete  TC: O(1)");
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

// Legacy full demo (still callable)
void Arrays::demo() 
{
    printBanner("ARRAYS MODULE  (Full Demo)");
    inputMenu(2);  // load dynamic array
    viewMenu(2);   // show grid
    inputMenu(3);  // baseline compare
    inputMenu(4);  // noise filter
    printDivider();
    printOK("Arrays module complete.");
}
