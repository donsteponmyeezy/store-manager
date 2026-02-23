# Class Diagram
```mermaid
classDiagram
    class Customer {
        +float arrivalTime
        +float startOfServiceTime
        +float departureTime
        +float pqTime
        +int eventType
        +Customer* nextCust
    }

    class PriorityQueue {
        -int current_size
        +PriorityQueue()
        +isEmpty() bool
        +isFull() bool
        +getSize() int
        +peekMin() Customer
        +insert(new_customer: Customer) void
        +removeMin() Customer
        -bubble_up(index: int) void
        -bubble_down(index: int) void
    }

    class FifoQueue {
        -Customer* head
        -Customer* tail
        -int current_size
        +FifoQueue()
        +~FifoQueue()
        +enqueue(new_customer: Customer) void
        +dequeue() Customer
        +isEmpty() bool
        +getSize() int
    }

    class Simulation {
        -float lambda
        -float mu
        -int M
        -int total_events
        -PriorityQueue pq
        -FifoQueue fifo
        -int server_available_cnt
        -float current_time
        -int events_processed
        -float total_wait_time
        -float total_service_time
        -float total_idle_time
        -int customer_waited_cnt
        -int total_customers
        -float last_departure_time
        +Simulation()
        +loadParameters(filename: string) bool
        +runAnalyticalModel() void
        +runSimulation() void
        +printResults() void
        -processArrival(event: Customer&) void
        -processDeparture(event: Customer&) void
        -getNextRandomInterval(avg: float) float
        -factorial(n: int) long double
    }

    Simulation *-- PriorityQueue : contains
    Simulation *-- FifoQueue : contains
    PriorityQueue o-- Customer : manages
    FifoQueue o-- Customer : manages
    ```