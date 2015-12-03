/* File: vectorshpp.h
 * -----------------------------------
 *
 * This file exports a simple implementation
 * of Vector class based on the array.
 
 */

#ifndef VECTORSHPP_H
#define VECTORSHPP_H

#include <iostream>
#include <stdlib.h>

/* Class VectorSHPP<ValueType>
 * --------------------------------
 * This class implements a vector of a specified value type.
 */
template<typename ValueType>
class VectorSHPP{

    /* Public methods prototypes*/
public:

    /* Constructor: VectorSHPP
     * Usage: VectorSHPP<ValueType> vector;
     * -----------------------------------------------------
     * Initializes a new empty vector
     */
    VectorSHPP();

    /* Destructor: ~VectorSHPP
     * -----------------------------------------------------
     * Frees memory allocated for array in the heap.
     */
    virtual ~VectorSHPP();

    /* Method: add
     * Usage: vector.add(value);
     * -----------------------------------------------------
     * Add the specified value in the vector
     */
    void add(ValueType);

    /* Method: clear
     * Usage: vector.clear();
     * -----------------------------------------------------
     * Removes all elements of the vector.
     */
    void clear();

    /* Method: get
     * Usage: value = vector.get(index);
     * -----------------------------------------------------
     * Returns value corresponding to the index in this vector
     */
    ValueType get(int) const;

    /* Method: insert
     * Usage: vector.insert(index, value);
     * -----------------------------------------------------
     * Inserts element in this vector in the specified index
     */
    void insert(int, ValueType);

    /* Method: isEmpty
     * Usage: if (vector.isEmpty())
     * -----------------------------------------------------
     * Returns true if this vector is empty
     */
    bool isEmpty() const;

    /* Method: remove
     * Usage: vector.remuve(index);
     * -----------------------------------------------------
     * Removes value of the vector corresponding to the index
     */
    void remove(int);

    /* Method: set
     * Usage: vector.set(index, value);
     * -----------------------------------------------------
     * Replaces the value at the specified index
     */
    void set(int, ValueType);

    /* Method: size
     * Usage: int = vector.size();
     * -----------------------------------------------------
     * Returns the number of elements of this vector
     */
    int size() const;

    /* Operator: []
     * Usage: vec[index]
     * -----------------------------------------------------
     * Overloads [] to select elements from this vector.
     */
    ValueType & operator[](int);
	
	/* Copy constructor*/
    VectorSHPP(const VectorSHPP<ValueType> & src);

    /* Operator: =
     * vectorNew = vectorOld;
     * -----------------------------------------------------
     * Overloads assign operator
     */
    VectorSHPP<ValueType> & operator=(const VectorSHPP<ValueType> & src);

    /* Private methods prototypes and instase variables*/
private:

    /* Dynamic array for storing elements*/
    ValueType *array;

    /* Current size of the dynamic array*/
    int currentSize;

    /* Current number of the element in array*/
    int count;

    /* The initial size of the array*/
    static const int START_SIZE = 10;

    /* Method: extendArray
     * Usage: extendArray();
     * ------------------------------------------------
     * Increases dynamyc array in two times
     */
    void extendArray();
	
	/* Method: deepCoping;
     * Usage: deepCoping(VectorSHPP src);
     * ------------------------------------------------
     * Coping received  VectorSHPP to "this" VectorSHPP
     */
    void deepCoping(const VectorSHPP<ValueType> & src);

};

/* Implementation of all methods of VectorSHPP class*/

template<typename ValueType>
VectorSHPP<ValueType>::VectorSHPP(){
    array = new ValueType[START_SIZE];
    currentSize = START_SIZE;
    count = 0;
}

template<typename ValueType>
ValueType & VectorSHPP<ValueType>::operator[](int index){
    if(index < 0 || index >= count){
        std::cout << "Fatal error: index is not valid" << std::endl;
        exit(1);
    }

    return array[index];
}

template<typename ValueType>
void VectorSHPP<ValueType>::add(ValueType value){
    if (count == currentSize) extendArray();

    array[count] = value;
    count++;
}

template <typename ValueType>
void VectorSHPP<ValueType>::clear(){
    count = 0;
}

template <typename ValueType>
ValueType VectorSHPP<ValueType>::get(int index) const{
    if(index < 0 || index >= count){
        std::cout << "Fatal error: index is not valid" << std::endl;
        exit(1);
    }

    return array[index];
}

template <typename ValueType>
void VectorSHPP<ValueType>::insert(int index, ValueType value){
    if (count == currentSize) extendArray();
    if(index < 0 || index >= count){
        std::cout << "Fatal error: index is not valid" << std::endl;
        exit(1);
    }

    ValueType *tmpArray = new ValueType[count - index];
    int j = 0;
    for(int i = index; i < count; i++){
        tmpArray[j] = array[i];
        j++;
    }
    j = 0;
    array[index] = value;
    for (int i = index + 1; i <= count; i++){
        array[i] = tmpArray[j];
        j++;
    }
    count++;

}

template <typename ValueType>
bool VectorSHPP<ValueType>::isEmpty() const{
    return count == 0;
}

template <typename ValueType>
void VectorSHPP<ValueType>::remove(int index){
    if(index < 0 || index >= count){
        std::cout << "Fatal error: index is not valid" << std::endl;
        exit(1);
    }
    for(int i = index; i < count-1; i++){
        array[i] = array[i+1];
    }
    count--;
}


template <typename ValueType>
void VectorSHPP<ValueType>::set(int index, ValueType value){
    if(index < 0 || index >= count){
        std::cout << "Fatal error: index is not valid" << std::endl;
        exit(1);
    }
    array[index] = value;
}

template <typename ValueType>
int VectorSHPP<ValueType>::size() const{
    return count;
}

template <typename ValueType>
void VectorSHPP<ValueType>::extendArray(){
    ValueType *oldArray = array;
    currentSize *= 2;
    array = new ValueType[currentSize];

    for (int i = 0; i < count; i++){
        array[i] = oldArray[i];
    }
    delete[] oldArray;
}

template<typename ValueType>
void VectorSHPP<ValueType>::deepCoping(const VectorSHPP<ValueType> &src){
    this->array = new ValueType[src.currentSize];
    count = src.count;
    currentSize = src.currentSize;
    for (int i = 0; i < src.count; i++){
        array[i] = src.array[i];
    }
}

template<typename ValueType>
VectorSHPP<ValueType>::VectorSHPP(const VectorSHPP<ValueType> & src){
    deepCoping(src);
}

template<typename ValueType>
VectorSHPP<ValueType> & VectorSHPP<ValueType>::operator =(const VectorSHPP<ValueType> & src){
    delete[] array;
    deepCoping(src);
    return *this;
}

template<typename ValueType>
VectorSHPP<ValueType>::~VectorSHPP(){
    count = 0;
	delete [] array;
}
#endif // VECTORSHPP

