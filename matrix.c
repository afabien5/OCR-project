#include <stdio.h>
#include <stdlib.h>
 
#include "matrix.h"


void transpose(double mat[], size_t lines, size_t cols, double res[])
{
  for(size_t i = 0; i < lines;  i++)
  {
    for(size_t j = 0; j < cols; j++)
    {
      res[i+j*lines] = mat[j +i*cols];
    }
  }
}

void add(double mat1[], double mat2[], size_t lines, size_t cols, double res[])
{
  for(size_t i = 0; i < lines;  i++)
  {
    for(size_t j = 0; j < cols; j++)
    {
      res[j+i*cols] = mat1[j +i*cols]+ mat2[j +i*cols];
    }
  }
}

void mul(double m1[], double m2[], size_t n, size_t m, size_t p, double res[]) 
{
  for(size_t i = 0; i < n;  i++)
  {
    for(size_t j = 0; j < p; j++)
    {
      double sum = 0;
      for(size_t k = 0; k < m; k++)
      {
        sum += m1[k +i*m] * m2[j +k*p];
      }
      res[j+i*p] = sum;
    }
  }
}

void print_matrix(double mat[], size_t lines, size_t cols)
{
  for(size_t i = 0; i < lines;  i++)
  {
    for(size_t j = 0; j < cols; j++)
    {
      printf("%4g",mat[j+i*cols]);  
    }
    printf("\n");
  }
}


 





