/**
 * @file distSelector.cpp
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief Selects from a given distribution in O(log(N)) time
 * @version 2.0
 * @date 2021-05-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include<bits/stdc++.h>
#include "distSelector.h"

template<typename T>
T distSelector(const std::vector<std::pair<double, T> > &dist)
{
  int i1 = dist.size();

  double d1 = rand(); // d1 represents the chosen random value
  d1 /= RAND_MAX; 

  if(d1 <= dist[0].first) // first entry is the selected entry
  {
    return dist[0].second;
  }

  int low = 0, high = i1-1, mid = 0; // Temp variables for binary search

  while(true) // Binary search for the interval of distribution in which d1 falls
  {
    mid = (low + high)/2;

    if(high == low - 1)
    {
      if(dist[low].first <= d1 && dist[high].first >= d1)
      {
        mid = low;
      }
      else
      {
        mid = high;
      }
      break;
    }

    if(dist[mid+1].first < d1)
    {
      low = mid+1;
    }
    else if(dist[mid].first > d1)
    {
      high = mid-1;
    }
    else
    {
      while(dist[mid].first == d1 && dist[mid+1].first == dist[mid].first && mid >0)
      {
        mid--;
      }
      break;
    }
  }
  return dist[mid+1].second;
}

template int distSelector<int>(const std::vector<std::pair<double, int> > &);
template bool distSelector<bool>(const std::vector<std::pair<double, bool> > &);
template double distSelector<double>(const std::vector<std::pair<double, double> > &);
template std::string distSelector<std::string>(const std::vector<std::pair<double, std::string> > &);