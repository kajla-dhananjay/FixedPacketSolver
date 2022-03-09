#ifndef _LINALG_H
#define _LINALG_H
#include <bits/stdc++.h>
#include "errorHandler.h"

template<typename T> std::vector<T> matrix_vector_mult(std::vector<std::vector<T> > mat, std::vector<T> v);
template<typename T> std::vector<T> vector_scalar_mult(std::vector<T> v, T val);
template<typename T> std::vector<T> vector_addition(std::vector<T> a, std::vector<T> b);
template<typename T> T l1_norm(std::vector<T> a);
template<typename T> T l2_norm(std::vector<T> a);
template<typename T> T inf_norm(std::vector<T> a);

#endif
