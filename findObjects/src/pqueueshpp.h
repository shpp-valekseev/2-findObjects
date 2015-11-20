/* File: pqueueshpp.h
 * -----------------------------------------------------
 * This file exports a simple version of the PriorityQueue class
 * based on the vectors.
 */

#ifndef PQUEUESHPP_H
#define PQUEUESHPP_H
#include "vectorshpp.h"

#include <iostream>
#include <stdlib.h>


/* Class: PQueueSHPP<ValueType> pqueue;
 * ---------------------------------------------------
 * This class implements priority queue of a specified ValueType
 * elements
 */
template<typename ValueType>
class PQueueSHPP{

    /* Public methods prototypes*/
public:

    /* Constructor: PQueueSHPP
     * Usage: PQueueSHPP<ValueType> pqueue;
     * -----------------------------------------------
     * Initializes a new empty pqueue
     */
    PQueueSHPP();

    /* Destructor: ~PQueueSHPP
     * ----------------------------------------------
     * Frees all allocated memory for the priority queue elements
     */
    virtual ~PQueueSHPP();

    /* Method: enqueue
     * Usage: pqueue.enqueue(value, priority);
     * -----------------------------------------------
     * Adds new value in the priority queue with the appropriate priority
     */
    void enqueue(ValueType, double);

    /* Method: dequeue
     * Usage: value = pqueue.dequeue();
     * -----------------------------------------------
     * Removes and returns the value with the highest priority
     */
    ValueType dequeue();

    /* Method: peek
     * Usage: value = pqueue.peek();
     * ---------------------------------------------
     * Returns the value of the highest priority, without removing it
     */
    ValueType peek() const;

    /* Method: changePriority
     * Usage: pqueue.changePriority(value, priority);
     * ---------------------------------------------
     * Changes the priority of the specified value
     */
    void changePriority(ValueType, double);

    /* Method: peekPriority
     * Usage: double priority = pqueue.peekPriority();
     * ---------------------------------------------
     * Returns the priority of the first element in the queue, without removing it.
     */
    double peekPriority() const;

    /* Method: clear
     * Usage: pqueue.clear();
     * ---------------------------------------------
     * Removes all elements of the pqueue
     */
    void clear();

    /* Method: isEmpty
     * Usage: if(pqueue.isEmpty)...
     * --------------------------------------------
     * Returns true if vector is empty
     */
    bool isEmpty() const;

    /* Method: size
     * Usage: int size = pqueue.size();
     * --------------------------------------------
     * Return current number of the elements of the pqueue
     */
    int size() const;

    /* Private methods prototypes and instase variables*/
private:

    /* Vector in which stored priority*/
    VectorSHPP<double> priorities;

    /* Vector in which stored value*/
    VectorSHPP<ValueType> values;

    /* Method: findIndex
     * Usage: int index = findIndex(value);
     * --------------------------------------------
     * Returns the index values in the vector values
     */
    int findIndex(ValueType);


};


/* Implementation of all methods of PQueueSHPP class*/
template<typename ValueType>
PQueueSHPP<ValueType>::PQueueSHPP(){}

template<typename ValueType>
PQueueSHPP<ValueType>::~PQueueSHPP(){}

template<typename ValueType>
void PQueueSHPP<ValueType>::enqueue(ValueType value, double priority){
    bool trigger = true;
    for(int i = 0; i < priorities.size(); i++){
        if(priority < priorities[i]){
            priorities.insert(i, priority);
            values.insert(i, value);
            trigger = false;
            break;
        }
    }

    if(trigger){
        priorities.add(priority);
        values.add(value);
    }
}

template<typename ValueType>
ValueType PQueueSHPP<ValueType>::dequeue(){
    ValueType tmpValue = values[0];
    values.remove(0);
    priorities.remove(0);
    return tmpValue;
}

template<typename ValueType>
ValueType PQueueSHPP<ValueType>::peek() const{
    return values[0];
}

template<typename ValueType>
void PQueueSHPP<ValueType>::changePriority(ValueType value, double newPriority){
    int index = findIndex(value);
    if(index != -1){
        values.remove(index);
        priorities.remove(index);
        enqueue(value, newPriority);
    } else {
        std::cout << "Fatal error: value is not found!" << std::endl;
        exit(1);
    }
}

template<typename ValueType>
double PQueueSHPP<ValueType>::peekPriority() const{
    return priorities[0];
}

template<typename ValueType>
void PQueueSHPP<ValueType>::clear(){
    priorities.clear();
    values.clear();
}

template<typename ValueType>
bool PQueueSHPP<ValueType>::isEmpty() const{
    return values.isEmpty();
}

template<typename ValueType>
int PQueueSHPP<ValueType>::size() const{
    return values.size();
}


template<typename ValueType>
int PQueueSHPP<ValueType>::findIndex(ValueType value){
    for(int i = 0; i < values.size(); i++){
        if(value == values[i]) return i;
    }
    return -1;
}

#endif // PQUEUESHPP

