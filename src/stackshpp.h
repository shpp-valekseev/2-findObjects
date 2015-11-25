/* File: stackshpp.h
 * -----------------------------------
 *
 * This file exports a simple implementation
 * of Stack class based on the dynamic array.
 */

#ifndef STACKSHPP_H
#define STACKSHPP_H

#include <iostream>

/* Class StackSHPP<ValueType>
 * --------------------------------
 * This class implements a stack of a specified value type.
 */
template <typename ValueType>
class StackSHPP {

    /* Public methods prototypes*/
public:

    /* Constructor: StackSHPP
     * Usage: StackSHPP<ValueType> stack;
     * -----------------------------------------------------
     * Initializes a new empty stack
     */
    StackSHPP();

    /* Destructor: ~StackSHPP
     * -----------------------------------------------------
     * Frees memory allocated for array in the heap.
     */
    virtual ~StackSHPP();

    /* Method: push
     * Usage: stack.push(value);
     * -----------------------------------------------------
     * Pushes the specified value on the stack
     */
    void push(ValueType value);

    /* Method: pop
     * Usage: value = stack.pop();
     * ----------------------------------------------------
     * Removes top element of the stack and returns it's value
     */
    ValueType pop();

    /* Method: clear
     * Usage: stack.clear();
     * -----------------------------------------------------
     * Removes all elements of the stack.
     */
    void clear();

    /* Method: isEmpty
     * Usage: if (stack.isEmpty())...
     * -----------------------------------------------------
     * Returns true if stack is empty
     */
    bool isEmpty() const;

    /* Method: top
     * Usage: value = stack.top();
     * -----------------------------------------------------
     * Returns value of the top element of the stack
     * without removing it.
     */
    ValueType top() const;

    /* Method: size
     * Usage: int size = stack.size();
     * -----------------------------------------------------
     * Returns current number of the element in the stack
     */
    int size() const;

    /* Method: peek
     * Usage: ValueType value = stack.peek();
     * -----------------------------------------------------
     * Returns the value of top element from this stack, without removing it
     */
    ValueType peek() const;

    /* Private methods prototypes and instase variables*/
private:
    static const int START_SIZE = 10;

    /* Dynamic array for storing elements*/
    ValueType *array;

    /* Current size of the dynamic array*/
    int currentSize;

    /* Current number of the element in array*/
    int count;

    /* Method: extendArray
     * Usage: extendArray();
     * ------------------------------------------------
     * Increases dynamyc arra in two times
     */
    void extendArray();
};


/* Implementation of all methods of StackSHPP class*/

template <typename ValueType>
StackSHPP<ValueType>::StackSHPP(){
    array = new ValueType[START_SIZE];
    currentSize = START_SIZE;
    count = 0;
}

template <typename ValueType>
StackSHPP<ValueType>::~StackSHPP(){
    delete[] array;
}

template <typename ValueType>
void StackSHPP<ValueType>::push(ValueType value){
    if (count == currentSize){ //check for a free space for new element
        extendArray();
    }
    array[count] = value;
    count++;
}

template <typename ValueType>
ValueType StackSHPP<ValueType>::pop(){
    if (isEmpty()){
        std:: cout << "Error: Stack is empty!!!" << std::endl;
        exit(1);
    }
    count--;
    return array[count];

}

template <typename ValueType>
void StackSHPP<ValueType>::clear(){
    count = 0;
}


template <typename ValueType>
bool StackSHPP<ValueType>::isEmpty() const {
    return count == 0;
}

template <typename ValueType>
ValueType StackSHPP<ValueType>::top() const {
    if (isEmpty()){
        std:: cout << "Error: Stack is empty!!!" << std::endl;
        exit(1);
    }
    return array[count-1];
}


template <typename ValueType>
int StackSHPP<ValueType>::size() const{
    return count;
}

template <typename ValueType>
void StackSHPP<ValueType>::extendArray(){
    ValueType *oldArray = array;
    currentSize *= 2;
    array = new ValueType[currentSize];

    for (int i = 0; i < count; i++){
        array[i] = oldArray[i];
    }
    delete[] oldArray;
}

template <typename ValueType>
ValueType StackSHPP<ValueType>::peek() const{
    return array[count-1];
}


#endif // STACKSHPP

