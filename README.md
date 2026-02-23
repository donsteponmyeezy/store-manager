# Store Manager

## MMc Queuing System

**M — Markovian Arrivals**
Customers arrive randomly according to a Poisson distribution. Time between arrivals follows an exponential curve and the probability of the next arrival is not dependent on wait time of current arrival. λ (lambda) is used to represent the average arrival rate measured in customers per hour.

**M — Markovian Service Times**
Time to service is exponentially distributed, and is without "memory" like the arrivals. The remaining service time for a given customer is independent of how long that server has been busy. μ (mu) is used to represent the average service rate, measured in number of customers per hour served, per server. (Ex: 2 customers/h per server)

**c — Number of Servers**
How many parallel service channels exist. When c = 1, we're dealing with a single-server queue (M/M/1). If c = 3, three parallel service channels exist. Customers waiting in a single shared line get routed to the next available server.

