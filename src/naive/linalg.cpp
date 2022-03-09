#include<bits/stdc++.h>
#include "linalg.h"

template<typename T>
std::vector<T> matrix_vector_mult(std::vector<std::vector<T> > mat, std::vector<T> v)
{
  std::vector<T> ans;
  if(mat.empty())
    return ans;
  int n = mat[0].size();
  if(n != (int)v.size() || n == 0)
  {
    errorHandler err("Matrix vector multiplication: bad dimensions");
  }
  for(int i = 0; i < (int)mat.size(); i++)
  {
    if(n != (int)mat[i].size())
    {
      errorHandler err("Matrix vector multiplication: not a matrix");
    }
    T r = 0;
    for(int j = 0; j < n; j++)
    {
      r += mat[i][j] * v[j]; 
    }
    ans.push_back(r);
  }
  return ans;
}

template<typename T>
std::vector<T> vector_scalar_mult(std::vector<T> v, T val)
{
  for(auto &it : v)
  {
    it *= val;
  }
  return v;
}

template<typename T>
std::vector<T> vector_addition(std::vector<T> a, std::vector<T> b)
{
  if(a.size() != b.size())
  {
    errorHandler err("Vector addition dimension mismatch");
  }
  for(int i = 0; i < (int)a.size(); i++)
  {
    a[i] += b[i];
  }
  return a;
}

template<typename T>
T l1_norm(std::vector<T> a)
{
  T r = 0;
  for(auto it : a)
  {
    r += std::abs(it);
  }
  return r;
}

template<typename T>
T l2_norm(std::vector<T> a)
{
  T r = 0;
  for(auto it : a)
  {
    r += (it * it);
  }
  r = sqrt(r);
  return r;
}

template<typename T>
T inf_norm(std::vector<T> a)
{
  T r = 0;
  for(auto it : a)
  {
    if(r < std::abs(it))
    {
      r = std::abs(it);
    }
  }
  return r;
}