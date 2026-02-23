# Store Manager

## This Store Manager app uses a MMc Queuing System


**M — Markovian Arrivals**
Customers arrive randomly according to a Poisson distribution. Time between arrivals follows an exponential curve and the probability of the next arrival is not dependent on wait time of current arrival. λ (lambda) is used to represent the average arrival rate measured in customers per hour.

**M — Markovian Service Times**
Time to service is exponentially distributed, and is without "memory" like the arrivals. The remaining service time for a given customer is independent of how long that server has been busy. μ (mu) is used to represent the average service rate, measured in number of customers per hour served, per server. (Ex: 2 customers/h per server)

**c — Number of Servers**
How many parallel service channels exist. When c = 1, we're dealing with a single-server queue (M/M/1). If c = 3, three parallel service channels exist. Customers waiting in a single shared line get routed to the next available server.

---

## Customer Data Type (Event Node)

In this simulation, the `Customer` class serves two purposes. It acts as the **event payload** for our timeline and the **waiting customer** in our line. 

A single `Customer` object is used to represent either an arrival or a departure:
* **Event Type (`type`):** An enum with two values indicating the object as either an `ARRIVAL` or a `DEPARTURE`.
* **Priority Queue Time (`pqTime`):** The time this event occurs. This acts as the key for our Min-Heap Priority Queue, meaning customer arrival time can be compared against server completion time and trigger the simulation to process the next event.
* **Simulation Times:** Tracks `arrivalTime`, `startOfServiceTime`, and `departureTime`. As the event moves through our simulation these times are calculated and recorded and are ultimately used to calculate wait times, system utilization, and averages.
* **Linked List Pointer (`nextCust`):** A reference/pointer to the next node, allowing the customer to be placed in the queue when all servers (`c`) are currently busy. 