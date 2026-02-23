#include "simulation.hpp"
#include <iostream>
#include <fstream>
#include <cmath>   // log, pow
#include <cstdlib> // using this and ctime for randomness to comply with your instructions
#include <ctime>   // otherwise would have used Mersenne Twister

// Constructor
Simulation::Simulation()
{
    lambda = 0.0f; // make sure any non-int is initialized as float (f after the number to avoid warnings)
    mu = 0.0f;
    M = 0;
    total_events = 0;

    server_available_cnt = 0;
    current_time = 0.0f;
    events_processed = 0;

    total_wait_time = 0.0f;
    total_service_time = 0.0f;
    total_idle_time = 0.0f;
    customer_waited_cnt = 0;
    total_customers = 0;

    last_departure_time = 0.0f;

    // Seed random generator for Poisson distribution
    std::srand(std::time(nullptr));
}

// Load input params from file, return false if failed to load
bool Simulation::loadParameters(const std::string &filename)
{
    std::ifstream input_file(filename);

    if (!input_file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    // Read lambda, mu, M, total events and then end file stream
    input_file >> lambda;
    input_file >> mu;
    input_file >> M;
    input_file >> total_events;

    input_file.close();

    // Initialize available servers to M value read from file
    server_available_cnt = M;

    return true;
}

// Utility Definitions

// For Poisson/Exponential Distribution (Makes customers arrive at random intervals based on lambda)
// Poisson for how many, exponential for how long until next arrival on average
float Simulation::getNextRandomInterval(float avg)
{
    // Generate a random decimal between 0 and 1, avoid using 0 exactly to prevent ln(0) errors
    float f = (float)std::rand() / RAND_MAX;
    while (f == 0.0f)
    {
        f = (float)std::rand() / RAND_MAX;
    }

    // Since natural log of any fraction between 0 and 1 is negative, this will give us a positive interval time
    float interval_time = -1.0f * (1.0f / avg) * std::log(f);

    return interval_time;
}

// Required to execute formulas given
long double Simulation::factorial(int n)
{
    if (n == 0 || n == 1)
        return 1.0;
    long double result = 1.0;
    for (int i = 2; i <= n; ++i)
    {
        result *= i;
    }
    return result;
}