#ifndef SCENARIOS_H
#define SCENARIOS_H

// Function declarations for each scenario file
void scenario1_fire();
void scenario2_sensor();
void scenario3_anomaly();
void scenario4_overload();
void scenario5_global();

class Scenarios {
public:
    static void runIndividual();
    static void runFullSystem();
};

#endif