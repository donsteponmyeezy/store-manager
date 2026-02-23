#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include "../customer.hpp"
#include <stdexcept>

class PriorityQueue
{
private:
    // Specified max size as given in doc provided on canvas
    static const int MAX_SIZE = 200;

    // Array to hold the heap
    Customer heapArray[MAX_SIZE];

    // # of elements in the heap
    int current_size;

    // Calculate array index for parents and children within the heap
    int parent(int index) { return (index - 1) / 2; }
    int left_child(int index) { return (2 * index) + 1; }
    int right_child(int index) { return (2 * index) + 2; }

    // parent is always <= children with pqTime as the key for comparison as described in readme
    void bubble_up(int index);
    void bubble_down(int index);

public:
    // Constructor
    PriorityQueue();

    // Queue Operations
    void insert(Customer new_customer);
    Customer removeMin(); // remove root and bubble down

    // Utility
    Customer peekMin() const; // return root wihtout removal
    bool isEmpty() const;
    bool isFull() const;
    int getSize() const;
};

#endif