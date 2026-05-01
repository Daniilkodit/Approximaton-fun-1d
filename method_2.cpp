#include "header.h"

int
create_aproximaniton_bit_polynom (int n, double *x, double *y, double *coeff,
                                    double *d, double *d1, double *d2, double *d3, double *b)
{
  d2[0] = 1;
  b [0] = d[0];
  for (int i = 1; i < n - 1; i++)
    {
      d1[i] = (x[i + 1] - x[i]);
      d2[i] = 2 * (x[i + 1] - x[i - 1]);
      d3[i] = (x[i] - x[i - 1]);
      b [i] = 3 * (y[i] - y[i - 1]) * (x[i + 1] - x[i]) / (x[i] - x[i - 1])
          + 3 * (y[i + 1] - y[i]) * (x[i] - x[i - 1]) / (x[i + 1] - x[i]);
    }
  d2[n - 1] = 1;
  b [n - 1] = d[n - 1];

  gauss_tridiagonal (n, d, d1, d2, d3, b);
  for (int i =0 ; i < n - 1; i++)
    {
        double h_i = (x[i + 1] - x[i]);
      coeff[i * 4 + 0] = y[i];
      coeff[i * 4 + 1] = d[i];
      coeff[i * 4 + 2] = (3 * ((y[i + 1] - y[i]) / h_i) - 2 * d[i] - d[i + 1]) / h_i;
      coeff[i * 4 + 3] = d[i] + d[i + 1] - 2 * (y[i + 1] - y[i]) / h_i;
      coeff[i * 4 + 3] /=  h_i * h_i;
    }

  return 0;
}

double
my_bit_aproximation (double x0, double a, double b,
                      int n, double *x, double *coeff)
{
  double y = 0.;
  double h = (b - a) / (n - 1);
  int i = static_cast<int> ((x0 - a) / h);

  if (i >= n - 1)
    i = n - 2;

  y = coeff[i * 4 + 3];
  for (int j = 2; j >=0; j--)
    {
      y = y * (x0 - x[i]) + coeff[i * 4 + j];
    }

  return y;
}

int
gauss_tridiagonal (int n, double *d, double *d1,
                    double *d2, double *d3, double *b)
{
  double eps = 1.e-15;

  for (int i = 1; i < n; i++)
    {
      if (fabs (d2[i - 1]) < eps)
        {
          return -1;
        }

      double m = d1[i] / d2[i - 1];
      d2[i] = d2[i] - m * d3[i - 1];
      b[i] = b[i] - m * b[i - 1];
    }

  d[n - 1] = b[n - 1] / d2[n - 1];
  for (int i = n - 2; i >= 0; i--)
    {
      if (fabs (d2[i])  < eps)
        {
          return -1;
        }

      d[i] = (b[i] - d3[i] * d[i + 1]) / d2[i];
    }
  return 0;
}
