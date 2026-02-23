#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

// Arrivals and Departures
enum EventType
{
    ARRIVAL,
    DEPARTURE
};

class Customer
{
public:
    //  Simulation Data
    float arrivalTime;
    float startOfServiceTime;
    float departureTime; // Equivalent to time when service has been completed

    //  Queue Data
    float pqTime; // Key for sorting
    EventType type;
    Customer *nextCust;

    Customer(float time, EventType t)
    {
        pqTime = time;
        type = t;

        // Depending on the event, populate the appropriate time variable and set the other to 0 (ensure to be stored as a float)
        arrivalTime = (t == ARRIVAL) ? time : 0.0f;
        startOfServiceTime = 0.0f;
        departureTime = (t == DEPARTURE) ? time : 0.0f;

        nextCust = nullptr;
    }

    // Default constructor for making heaps
    Customer()
    {
        arrivalTime = 0.0f;
        startOfServiceTime = 0.0f;
        departureTime = 0.0f;
        pqTime = 0.0f;
        type = ARRIVAL;
        nextCust = nullptr;
    }
};

#endif