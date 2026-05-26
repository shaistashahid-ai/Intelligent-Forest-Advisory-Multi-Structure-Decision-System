#ifndef MONITORING_H
#define MONITORING_H
#include <string>
#include <ctime>

using namespace std;

class Monitoring {
    clock_t start;
    string opName;

public:
    Monitoring(string name);
    ~Monitoring();

    static void monitorMenu(int choice);
};

#endif
