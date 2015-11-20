/* File: mapshpp.h
 * -----------------------------------------------------
 * This file exports a simple version of the Map class
 * based on the vector.
 */

#ifndef MAPSHPP
#define MAPSHPP

#include <iostream>
#include <stdlib.h>
#include <vectorshpp.h>


/* Class: MapSHPP<ValueType> map;
 * ---------------------------------------------------
 * This class implements map of a specified ValueType
 * elements
 */
template<typename KeyType, typename ValueType>
class MapSHPP{

    /* Public methods prototypes*/
public:

    /* Constructor: MapSHPP
     * Usage: MapSHPP<ValueType> map;
     * -----------------------------------------------
     * Initializes a new empty map
     */
    MapSHPP();

    /* Destructor: ~MapSHPP
     * ----------------------------------------------
     * Frees all allocated memory for the map elements
     */
    virtual ~MapSHPP();

    /* Method: put
     * Usage: map.put(key, value);
     * -----------------------------------------------
     * Put the value in accordance with the key.
     * If this key already exists then its value is replaced by a new
     */
    void put(KeyType, ValueType);

    /* Method: get
     * Usage: value = map.get(key);
     * -----------------------------------------------
     * Returns the value of the corresponding key
     */
    ValueType get(KeyType);

    /* Method: isEmpty
     * Usage: if (map.isEmpty());
     * -----------------------------------------------
     * Returns true if map is empty
     */
    bool isEmpty()const;

    /* Method: clear
     * Usage: map.clear();
     * ---------------------------------------------
     * Removes all elements of the map
     */
    void clear();

    /* Method: size
     * Usage: int size = map.size();
     * --------------------------------------------
     * Return current number of the elements of the
     * map
     */
    int size() const;

    /* Method: remove
     * Usage: map.remuve(key);
     * -----------------------------------------------------
     * Removes value of the map corresponding to the key
     */
    void remove(KeyType);

    /* Method: getKey
     * Usage: KeyType key = map.getKey(int);
     * -----------------------------------------------------
     * Return key of the map corresponding to the index
     */
    KeyType getKey(int) const;

    /* Method: containsKey
     * Usage: if (map.containsKey(key))
     * -----------------------------------------------------
     * Return true if map contains input key
     */
    bool containsKey(KeyType key) const;


    /* Private methods prototypes and instase variables*/
private:

    /* Structure for saving elements of the map*/
    struct node {
        KeyType NodeKey;
        ValueType NodeValue;
    };

    /* Vector in which stored value*/
    VectorSHPP<node> vector;

    /* Method: findKey
     * Usage: int index = findKey(key);
     * --------------------------------------------
     * Returns index in the vector corresponding to key
     */
    int findKey(KeyType);
};


/* Implementation of all methods of MapSHPP class*/
template<typename KeyType, typename ValueType>
MapSHPP<KeyType, ValueType>::MapSHPP(){

}

template<typename KeyType, typename ValueType>
MapSHPP<KeyType, ValueType>::~MapSHPP(){

}

template<typename KeyType, typename ValueType>
void MapSHPP<KeyType, ValueType>::put(KeyType key, ValueType value){
    int index = findKey(key);
    if(index == -1){
        node tmpNode;
        tmpNode.NodeKey = key;
        tmpNode.NodeValue = value;
        vector.add(tmpNode);

    } else {
        vector[index].NodeValue = value;

    }
}

template<typename KeyType, typename ValueType>
int MapSHPP<KeyType, ValueType>::findKey(KeyType key){
    for(int i = 0; i < vector.size(); i++){
        node tmpNode = vector.get(i);
        if(tmpNode.NodeKey == key){
            return i;
        }
    }
    return -1;
}

template<typename KeyType, typename ValueType>
ValueType MapSHPP<KeyType, ValueType>::get(KeyType key){

    int index = findKey(key);
    if(index == -1){
        std::cout << "Error, key not found"<< std::endl;
        exit(1);
    }

    return vector[index].NodeValue;

}

template<typename KeyType, typename ValueType>
int MapSHPP<KeyType, ValueType>::size() const{
    return vector.size();
}

template<typename KeyType, typename ValueType>
void MapSHPP<KeyType, ValueType>::remove(KeyType key){
    int index = findKey(key);
    if(index == -1){
        std::cout << "Error, key not found"<< std::endl;
        exit(1);
    }

    vector.remove(index);
}

template<typename KeyType, typename ValueType>
bool MapSHPP<KeyType, ValueType>::isEmpty() const{
    return vector.size() == 0;
}

template<typename KeyType, typename ValueType>
void MapSHPP<KeyType, ValueType>::clear(){
    vector.clear();
}

template<typename KeyType, typename ValueType>
KeyType MapSHPP<KeyType, ValueType>::getKey(int index) const{
    return vector[index].NodeKey;
}

template<typename KeyType, typename ValueType>
bool MapSHPP<KeyType, ValueType>::containsKey(KeyType key) const{
    bool res = true;
    if(findKey(key) == -1) res = false;
    return res;
}

#endif // MAPSHPP

