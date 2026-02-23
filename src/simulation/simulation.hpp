#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <string>
#include "../customer.hpp"
#include "../priority_queue/priority_queue.hpp"
#include "../fifo_queue/fifo_queue.hpp"

// Simulation is based off of equations provided; P sub 0, L, W, L sub q, W sub q, and the system utilization factor rho
// Full implementation details provided in README

class Simulation
{
private:
    // Input Parameters
    float lambda;     // Arrival rate
    float mu;         // Service rate
    int M;            // # of servers
    int total_events; // # of events to simulate

    // Instances of FIFO Queue and Min-Heap for Simulation
    PriorityQueue pq;
    FifoQueue fifo;

    // Track number of servers available at given time
    int server_available_cnt;
    float current_time;
    int events_processed;

    // Variables for Holding Simulation Results
    float total_wait_time;
    float total_service_time;
    float total_idle_time;
    int customer_waited_cnt;
    int total_customers;

    // Track the time of last departure to calculate server idle time (P sub 0)
    float last_departure_time;

    // Helper Declarations
    float getNextRandomInterval(float avg);
    long double factorial(int n); // Needed for the analytical math formulas

    // Processing Arrivals and Departures
    void processArrival(Customer &event);
    void processDeparture(Customer &event);

public:
    Simulation();

    // Load input parameters from file, return false if failed to load
    bool loadParameters(const std::string &filename);

    // Use provided forumulas to calculate analytical results that estimate the results of longer simulations
    void runAnalyticalModel();

    // Run the sim of the application to process events until total_events have been processed
    void runSimulation();

    // Print the results of the simulation and analytical model to the console in a readable format
    void printResults();
};

#endif