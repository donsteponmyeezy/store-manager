#include "fifo_queue.hpp"

// Constructor and Destructor Definitions
FifoQueue::FifoQueue()
{
    head = nullptr;
    tail = nullptr;
    current_size = 0;
}

FifoQueue::~FifoQueue()
{
    while (!isEmpty())
    {
        dequeue();
    }
}

// Utility Definitions
bool FifoQueue::isEmpty() const { return current_size == 0; }
int FifoQueue::getSize() const { return current_size; }

// Primary Definitions
void FifoQueue::enqueue(Customer new_customer)
{
    // Allocate new dynamic node with copy constructor and create new tail for the next customer to be added
    Customer *new_node = new Customer(new_customer);
    new_node->nextCust = nullptr;

    if (isEmpty())
    {
        // If no people in queue, head and tail both point to this new node
        head = new_node;
        tail = new_node;
    }
    else
    {
        // Add the customer to the back of the line (tail pointer)
        tail->nextCust = new_node;
        tail = new_node;
    }
    current_size++;
}

Customer FifoQueue::dequeue()
{
    if (isEmpty())
    {
        throw std::underflow_error("FIFO Queue is empty!");
    }

    // Find front of the line (head pointer) and save it to return at end of function
    Customer *temp = head;
    Customer returning_customer = *temp; // Safely copy customer data out of dynamic memory node before deletion

    // Make head point to next customer in line
    head = head->nextCust;

    // Check for an empty queue by referencing head. If empty, set tail to nullptr too since no one is in line
    if (head == nullptr)
    {
        tail = nullptr;
    }

    // Avoid memory leak and decrease size of queue
    delete temp;
    current_size--;

    return returning_customer;
}