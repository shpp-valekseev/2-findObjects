/* File: queueshpp.h
 * -----------------------------------------------------
 * This file exports a simple version of the Queue class
 * based on the linked list.
 */

#ifndef QUEUESHPP_H
#define QUEUESHPP_H

#include <iostream>

/* Class: QueueSHPP<ValueType>
 * ---------------------------------------------------
 * This clas implements queue of a specified ValueType
 * elements
 */
template <typename ValueType>
class QueueSHPP{

    /* Public methods prototypes*/
public:

    /* Constructor: QueueSHPP
     * Usage: QueueSHPP<ValueType> queue;
     * -----------------------------------------------
     * Initializes a new empty queue
     */
    QueueSHPP();

    /* Destructor: ~QueueSHPP
     * ----------------------------------------------
     * Frees all allocated memory for the queue elements
     */
    virtual ~QueueSHPP();

    /* Method: dequeue
     * Usage: value = queue.dequeue();
     * ---------------------------------------------
     * Removes the first elements of the queue and
     * returns it's value
     */
    ValueType dequeue();

    /* Method: enqueue
     * Usage: queue.enqueue(value);
     * -----------------------------------------------
     * Adds new element to the end of the queue`
     */
    void enqueue (ValueType);

    /* Method: clear
     * Usage: queue.clear();
     * ---------------------------------------------
     * Removes all elements of the queue
     */
    void clear();

    /* Method: size
     * Usage: int size = queue.size();
     * --------------------------------------------
     * Return current number of the elements of the
     * queue
     */
    int size() const;

    /* Method: isEmpty
     * Usage: if(queue.isEmpty)...
     * --------------------------------------------
     * Returns true if current number of the elements
     * is 0.
     */
    bool isEmpty() const;

    /* Method: peek
     * Usage: value = queue.peek();
     * ---------------------------------------------
     * Return value of the first element without removing it.
     */
    ValueType peek() const;

    /* Private methods prototypes and instase variables*/
private:

    /* Structure for saving elements of the queue*/
    struct Cell{
        ValueType value;
        Cell* link;
    };

    /* Links for the linked list*/
    Cell* top;
    Cell* down;

    /* Current number of the elements in the queue*/
    int count;

};


/* Implementation of all methods of QueueSHPP class*/
template <typename ValueType>
QueueSHPP<ValueType> :: QueueSHPP(){
    count = 0;
    top = down = NULL;
}

template<typename ValueType>
void QueueSHPP<ValueType> :: enqueue(ValueType newValue){
    Cell *newCell = new Cell;
    newCell->value = newValue;
    newCell->link = NULL;
    if(top == NULL) {
        top = down = newCell;
    } else {
        down->link = newCell;
        down  = newCell;
    }
    count++;
}

template<typename ValueType>
ValueType QueueSHPP<ValueType> :: dequeue(){
    if (count == 0){
      std:: cout << "Fatal error: queue is empty" << std:: endl;
        exit(1);
    }
        Cell *tmpCell = top;
        top = top->link;
        ValueType tmpValue = tmpCell->value;
        count--;
        delete tmpCell;
        return tmpValue;
}

template<typename ValueType>
void QueueSHPP<ValueType> :: clear(){
    for(int i = 0; i < count; i++){
        Cell *tmpCell = top;
        top = top->link;
        delete tmpCell;
    }
    count = 0;
}

template<typename ValueType>
int QueueSHPP<ValueType> :: size() const{
    return count;
}

template<typename ValueType>
bool QueueSHPP<ValueType> :: isEmpty() const{
    return count == 0;
}

template<typename ValueType>
ValueType QueueSHPP<ValueType> :: peek() const{
    return top->value;
}

template <typename ValueType>
QueueSHPP<ValueType> :: ~QueueSHPP(){
    for(int i = 0; i < count; i++){
        Cell *tmpCell = top;
        top = top->link;
        delete tmpCell;
    }
}
#endif // QUEUESHPP

