/**
 * @file indexedSet.cpp
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief A Balanced BST based data structure which keeps track of indices
 * @version 1.0
 * @date 2021-2-5
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<bits/stdc++.h>
#include "indexedSet.h"
#include "errorHandler.h"

template class indexedSet<int>;
template class indexedSet<double>;

/**
 * @brief Construct a new indexed Set object
 */
template<typename T>
indexedSet<T>::indexedSet()
{

}

/**
 * @brief Constructs a new indexed Set object
 * 
 * @param sz Size of the new Set
 * 
 * All elements are initialized to null type of T
 */
template<typename T>
indexedSet<T>::indexedSet(int sz)
{
    array_val.resize(sz);
    for(auto it : array_val)
    {
        set_val.insert(it);
    }
}

/**
 * @brief Constructs a new indexed Set object
 * 
 * @param sz Size of the new queue
 * @param v Initial queue
 */

template<typename T>
indexedSet<T>::indexedSet(std::vector<T> v)
{
    array_val = v;
    set_val.clear();
    for(auto it : array_val)
    {
        set_val.insert(it);
    }
}


/**
 * @brief Get the maximum object in the set
 * 
 * @return T Returns the maximum object in the set
 */
template<typename T>
T indexedSet<T>::getMax()
{
    if(set_val.empty())
    {
        errorHandler error("Query for maximum value from an empty indexedSet");
    }
    return *(set_val.rbegin());
}

/**
 * @brief Get the object at index in the queue
 * 
 * @param index Index of the required object
 * @return T Object at index position
 */

template<typename T>
T indexedSet<T>::getVal(int index)
{
    if(index < 0 || index >= (int)array_val.size())
    {
        errorHandler error("Query for getting value of indexedSet element at bad index");
    }
    return array_val[index];
}

/**
 * @brief Sets the object at given index
 * 
 * @param index Index at which the object is to be set
 * @param value Value of the object to be set
 * @return true Value set successfully
 * @return false Value not set successfully
 */


template<typename T>
void indexedSet<T>::setVal(int index, T value)
{
    if(index < 0 || index >= (int)array_val.size())
    {
        errorHandler error("Query for setting value of indexedSet element at bad index");
    }
    set_val.erase(set_val.find(array_val[index]));
    array_val[index] = value;
    set_val.insert(array_val[index]);
}

/**
 * @brief Gets the first index containing the maximum value
 * 
 * @return The first index containing the maximum value in the set
 */

template<typename T>
int indexedSet<T>::getMaxIndex()
{
    if(array_val.empty())
    {
        errorHandler err("No max index in empty indexedSet");
    }
    T maxv = array_val[0];
    int maxi = 0;
    for(int i = 0; i < (int)array_val.size(); i++)
    {
        if(array_val[i] < maxv)
        {
            maxv = array_val[i];
            maxi = i;
        }
    }
    return maxi;
}