#include "priority_queue.hpp"
#include <iostream>

// Min-Heap implementation of priority queue with pqTime as key for sorting

// Constructor Definition
PriorityQueue::PriorityQueue()
{
    current_size = 0;
}

// Utility Definitions
bool PriorityQueue::isEmpty() const { return current_size == 0; }
bool PriorityQueue::isFull() const { return current_size == MAX_SIZE; }
int PriorityQueue::getSize() const { return current_size; }

// Peek Definition
Customer PriorityQueue::peekMin() const
{
    if (isEmpty())
    {
        throw std::runtime_error("Priority Queue is empty!");
    }
    return heapArray[0];
}

// Heap Operations Definitions
void PriorityQueue::insert(Customer new_customer)
{
    if (isFull())
    {
        throw std::overflow_error("Priority Queue is full; Max size of 200.");
    }

    // Place incoming customer node at the end of heap
    heapArray[current_size] = new_customer;

    // Bubble up based on pqTime
    bubble_up(current_size);

    // Increase size of heap
    current_size++;
}

Customer PriorityQueue::removeMin()
{
    if (isEmpty())
    {
        throw std::underflow_error("Priority Queue is empty!");
    }

    // Find and save root for return at end of fn
    Customer root = heapArray[0];

    // Move last element to root and decrease heap size
    heapArray[0] = heapArray[current_size - 1];
    current_size--;

    // Bubble down to restore satisfaction of Min-Heap requirements
    if (current_size > 0)
    {
        bubble_down(0);
    }

    return root;
}

// Bubbling / Sorting / Rotation Logic (Bubble Up)
void PriorityQueue::bubble_up(int index)
{
    // If the current index isn't the root and parent's time is greater than current element's time
    while (index > 0 && heapArray[parent(index)].pqTime > heapArray[index].pqTime)
    {
        // Perform Swap
        Customer temp = heapArray[parent(index)];
        heapArray[parent(index)] = heapArray[index];
        heapArray[index] = temp;

        // Move index to parents position and repeat while loop until heap property satisfied
        index = parent(index);
    }
}

// Bubbling / Sorting / Rotation Logic (Bubble Down)
void PriorityQueue::bubble_down(int index)
{
    while (true)
    {
        int left = left_child(index);
        int right = right_child(index);
        int smallest = index;

        // Check existence of left child, if it does exist and pqTime is smaller than current smallest, update smallest to this new value
        if (left < current_size && heapArray[left].pqTime < heapArray[smallest].pqTime)
        {
            smallest = left;
        }

        // Check existence of right child, if it does and pqTime is smaller than current smallest, update smallest to this value
        if (right < current_size && heapArray[right].pqTime < heapArray[smallest].pqTime)
        {
            smallest = right;
        }

        // If index == smallest then heap is satisfied and loop can break
        if (smallest == index)
        {
            break;
        }

        // If loop didn't break, a child had a smaller pqTime than the parent
        // In order to satisfy the heap property we swap the parent with smaller child
        Customer temp = heapArray[index];
        heapArray[index] = heapArray[smallest];
        heapArray[smallest] = temp;

        // Point index to child's position and repeat while loop until heap properly satisfied
        index = smallest;
    }
}