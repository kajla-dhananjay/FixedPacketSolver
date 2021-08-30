#include<bits/stdc++.h>

int distSelector(const std::vector<double> &dist)
{
  int n = dist.size();
 
  for(auto it : dist)
  {
    std::cout << it << ' ';
  }
  std::cout << std::endl;

  double z = rand();
  z /= RAND_MAX;

  std::cout << std::endl << "z = " << z << std::endl; 

  if(z <= dist[0])
  {
    return 0;
  }


  int low = 0;
  int high = n-1;
 
  int mid = 0;

  while(true)
  {
    mid = low + high;
    mid /= 2;
   
    if(high == low - 1)
    {
      if(dist[low] <= z && dist[high] >= z)
      {
        mid = low;
      }
      else
      {
        mid = high;
      }
      break;
    }

    if(dist[mid+1] < z)
    {
      low = mid+1;
    }
    else if(dist[mid] > z)
    {
      high = mid-1;
    }
    else
    {
      while(dist[mid] == z && dist[mid+1] == dist[mid] && mid >0)
      {
        mid--;
      }
      break;
    }
  }
  //std::cout << "Chosen value: " << mid << " | Chosen Interval: " << dist[mid] << " - " << dist[mid+1] << std::endl;
  return mid+1;
}

int distSelector(const std::vector<std::pair<double, int> > &dist)
{
  int n = dist.size();
 
  /*
  for(auto it : dist)
  {
    std::cout << it.second << ' ' << it.first << std::endl;
  }
  std::cout << std::endl;
  */

  double z = rand();
  z /= RAND_MAX;

  if(z <= dist[0].first)
  {
    return dist[0].second;
  }

  //std::cout << std::endl << "z = " << z << std::endl; 

  int low = 0;
  int high = n-1;
 
  int mid = 0;

  while(true)
  {
    mid = low + high;
    mid /= 2;
   
    if(high == low - 1)
    {
      if(dist[low].first <= z && dist[high].first >= z)
      {
        mid = low;
      }
      else
      {
        mid = high;
      }
      break;
    }

    if(dist[mid+1].first < z)
    {
      low = mid+1;
    }
    else if(dist[mid].first > z)
    {
      high = mid-1;
    }
    else
    {
      while(dist[mid].first == z && dist[mid+1].first == dist[mid].first && mid >0)
      {
        mid--;
      }
      break;
    }
  }
  //std::cout << "Chosen value: " << dist[mid+1].second << " | Chosen Interval: " << dist[mid].first << " - " << dist[mid+1].first << std::endl;
  return dist[mid+1].second;
}

int main()
{
  srand(time(0));
  std::vector<std::pair<double,int> > d({{0.1,0},{0.2,2},{0.35,4},{0.4,6},{0.4,8},{0.45,10},{0.8,12},{0.91,14},{0.95,16},{1,18}});
  std::vector<double> d1({0.1,0.2,0.35,0.4,0.4,0.45,0.8,0.91,0.95,1});
  int z = 5;
  while(z--)
  {
    std::cout << "Case #" << 5-z << ": \n" << distSelector(d) << std::endl;
  }
}
