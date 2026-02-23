#ifndef FIFO_QUEUE_HPP
#define FIFO_QUEUE_HPP

#include "../customer.hpp"
#include <stdexcept>

class FifoQueue
{
private:
    Customer *head;   // next to be served
    Customer *tail;   // latest arrival and last to be served
    int current_size; // # of customers in the queue

public:
    FifoQueue();
    ~FifoQueue(); // destroy any unused nodes in queue to prevent memory leaks

    void enqueue(Customer new_customer); // add as customer as latest arrival and update tail pointer
    Customer dequeue();                  // remove customer at head and update head pointer to next customer in line

    // Utility Declarations
    bool isEmpty() const;
    int getSize() const;
};

#endif