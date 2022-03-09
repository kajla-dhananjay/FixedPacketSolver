#include<bits/stdc++.h>
#include "end.h"
#include "declarations.h"
#include "linalg.h"

/**
 * @brief Completes exit formalities
 * 
 */

void end()
{
  std::vector<double> x = chan->getMu();
  double z = x[u];
  x[u] = 0;
  double z_star = 0;
  for(int i = 0; i < n; i++)
  {
    x[i] /= (z * D[i]);
    z_star += x[i];
  }
  z_star /= n;
  for(int i = 0; i < n; i++)
  {
    x[i] -= z_star;
    x[i] *= sb;
  }
  //outcontainer(x);
  std::vector<double> Lx = matrix_vector_mult(L, x);
  //outcontainer(Lx);
  std::vector<double> Lx_b = vector_addition(Lx, vector_scalar_mult(b, -1.0));
  //outcontainer(Lx_b);
  //outcontainer(Lx_b);
  std::cout << "L1 norm: how to change " << l1_norm(Lx_b) << std::endl;
  std::cout << "L2 norm: " << l2_norm(Lx_b) << std::endl;
  std::cout << "Infinity norm: " << inf_norm(Lx_b) << std::endl;
  std::cout << "L1 norm(normalized to b): " << l1_norm(Lx_b) / l1_norm(b) << std::endl; 
  std::cout << "L1 norm(normalized to n): " << l1_norm(Lx_b) / n << std::endl; 
  std::cout << "L2 norm(normalized to b): " << l2_norm(Lx_b) / l2_norm(b) << std::endl;
  std::cout << "L2 norm(normalized to n): " << l2_norm(Lx_b) / n << std::endl;
  std::cout << "Infinity norm(normalized to b): " << inf_norm(Lx_b) / inf_norm(b) << std::endl;
  std::cout << "Infinity norm(normalized to n): " << inf_norm(Lx_b) / n << std::endl;
  std::cout << "Averaged L1 norm (L1 norm divided by n): " << l1_norm(Lx_b) / n  << std::endl; 
  std::cout << "Averaged L1 norm (L1 norm divided by n) (normalized to b): " << l1_norm(Lx_b) / (n * l1_norm(b)) << std::endl; 
}