#include "simulation.hpp"
#include <iostream>
#include <fstream>
#include <cmath>   // log, pow
#include <cstdlib> // using this and ctime for randomness to comply with your instructions
#include <ctime>   // otherwise would have used Mersenne Twister
#include <iomanip> // for formatting and setting precision

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

void Simulation::runAnalyticalModel()
{
    std::cout << "--- Analytical Model Results ---" << std::endl;

    // Prevent infinite growth of queue with a quick stability check
    if (M * mu <= lambda)
    {
        std::cout << "Error: The system is unstable (Arrival rate >= Max service rate)." << std::endl;
        return;
    }

    // Calculate Probability of zero people in the system indicated by P sub 0
    double sum = 0.0;
    // Sum from i = 0 to (M - 1)
    for (int i = 0; i < M; ++i)
    {
        sum += (1.0 / factorial(i)) * std::pow((lambda / mu), i);
    }
    // Apply formula for denominator
    double last_term = (1.0 / factorial(M)) * std::pow((lambda / mu), M) * ((M * mu) / (M * mu - lambda));
    double P0 = 1.0 / (sum + last_term);

    // Calculate Average number of people in the system indicated by L
    double numerator_L = lambda * mu * std::pow((lambda / mu), M);
    double denominator_L = factorial(M - 1) * std::pow((M * mu - lambda), 2);
    double L = (numerator_L / denominator_L) * P0 + (lambda / mu);

    // Calculate the average time a customer spends in the system indicated by W
    // Since W is equal to L / lambda by the given formula, we can apply it here to calculate W
    double W = L / lambda;

    // Calculate average number of customers in the queue represented by L sub q
    double Lq = L - (lambda / mu);

    // Calculate average time a customer spends waiting in the queue represented by W sub q
    // Given Wq = Lq / lambda from your formulas, apply it here to calculate W sub q
    double Wq = Lq / lambda;

    // Calculate system utilization factor represented by rho (percentage of time servers are busy)
    double rho = lambda / (M * mu);

    // Display values with 4 decimal places as shown in the example output for easy comparison
    std::cout << std::fixed << std::setprecision(4);

    std::cout << " Po = " << P0 << std::endl;
    std::cout << " L = " << L << std::endl;
    std::cout << " W = " << W << std::endl;
    std::cout << " Lq = " << Lq << std::endl;
    std::cout << " Wq = " << Wq << std::endl;
    std::cout << " rho = " << rho << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

void Simulation::runSimulation()
{
    // Place first arrival in queue
    Customer first_arrival;
    first_arrival.arrivalTime = current_time + getNextRandomInterval(lambda);
    first_arrival.pqTime = first_arrival.arrivalTime;
    first_arrival.departureTime = 0.0f; // Using 0.0f to indicate arrival
    pq.insert(first_arrival);

    server_available_cnt = M;
    events_processed = 0;
    last_departure_time = 0.0f;

    // NOTE: To avoid scheduling the next arrival based on the current time which includes
    // departures we must to track the last scheduled arrival time separately.
    // This way, we can ensure that arrivals follow the exponential distribution in all cases
    float last_scheduled_arrival_time = first_arrival.arrivalTime;

    while (!pq.isEmpty() && events_processed < total_events)
    {
        Customer current_event = pq.removeMin();
        current_time = current_event.pqTime;

        // Departure time == 0 means arrival event
        if (current_event.departureTime == 0.0f)
        {
            processArrival(current_event);
        }
        else
        {
            processDeparture(current_event);
        }

        events_processed++;

        // Refill the PQ with new arrivals if it gets too small and we haven't hit the event limit
        // If event limit hasn't been hit by the time we get close to the end of the PQ, add more arrivals
        if (pq.getSize() <= M + 1 && events_processed < total_events)
        {
            Customer next_arrival;

            next_arrival.arrivalTime = last_scheduled_arrival_time + getNextRandomInterval(lambda);
            next_arrival.pqTime = next_arrival.arrivalTime;
            next_arrival.departureTime = 0.0f;
            pq.insert(next_arrival);

            // Update the tracker each block for reasons listed above
            last_scheduled_arrival_time = next_arrival.arrivalTime;
        }
    }
}

void Simulation::processArrival(Customer &event)
{
    total_customers++;

    // Fully idle if all servers available (before first customer arrives)
    if (server_available_cnt == M)
    {
        total_idle_time += (current_time - last_departure_time);
    }

    if (server_available_cnt > 0)
    {
        server_available_cnt--;

        event.startOfServiceTime = current_time;
        float interval = getNextRandomInterval(mu);
        event.departureTime = current_time + interval;
        event.pqTime = event.departureTime; // Calculate again using departure time to ensure PQ is sorted corretly (pqTime)

        total_service_time += interval;

        pq.insert(event); // Customer departing, put them back in queue with updated departure time
    }
    else
    {
        fifo.enqueue(event);
    }
}

void Simulation::processDeparture(Customer &event)
{
    server_available_cnt++;

    // Check if anyone is waiting for a server; if so update their time and put them in queue
    if (!fifo.isEmpty())
    {
        Customer next_cust = fifo.dequeue();

        float wait_time = current_time - next_cust.arrivalTime;
        if (wait_time > 0)
        {
            customer_waited_cnt++;
            total_wait_time += wait_time;
        }

        next_cust.startOfServiceTime = current_time;
        float interval = getNextRandomInterval(mu);
        next_cust.departureTime = current_time + interval;
        next_cust.pqTime = next_cust.departureTime;

        total_service_time += interval;
        server_available_cnt--;

        pq.insert(next_cust);
    }

    // If all servers are now available, track idle time starting from this departure
    if (server_available_cnt == M)
    {
        last_departure_time = current_time;
    }
}

void Simulation::printResults()
{
    // Add idle time resulting from the simulation ending with servers idle to the total
    if (server_available_cnt == M)
    {
        total_idle_time += (current_time - last_departure_time);
    }

    // Calculate and print simulation measures from assignment
    float sim_Po = total_idle_time / current_time;
    float sim_W = (total_wait_time + total_service_time) / total_customers;
    float sim_Wq = total_wait_time / total_customers;
    float sim_rho = total_service_time / (M * current_time);
    float prob_wait = static_cast<float>(customer_waited_cnt) / total_customers;

    std::cout << "--- Simulation Results ---" << std::endl;
    std::cout << std::fixed << std::setprecision(4);
    std::cout << " Po = " << sim_Po << std::endl;
    std::cout << " W = " << sim_W << std::endl;
    std::cout << " Wq = " << sim_Wq << std::endl;
    std::cout << " rho = " << sim_rho << std::endl;
    std::cout << " Probability of waiting = " << prob_wait << std::endl;
    std::cout << "--------------------------------\n"
              << std::endl;
}