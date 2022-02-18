/**
 * @file indexedSet.h
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief A Balanced BST based data structure which keeps track of indices
 * @version 1.0
 * @date 2021-2-5
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __INDEXED_SET_H_
#define __INDEXED_SET_H_

template<typename T>
class indexedSet{
private:
  std::multiset<T> set_val; // Multiset to keep track of priorities in increasing order 
  std::vector<T> array_val; // Dynamic array 
public:
  indexedSet(); // Default Constructor
  indexedSet(int sz); // Constructor to initiate IS of a given size
  indexedSet(std::vector<T> v); // Constructor to initiate IS for given vector 
  T getMax(); // Returns the maximum value present in the IS
  T getVal(int index); // Returns the value at the given index
  void setVal(int index, T value); // Updates the given index to the given value
};

#endif