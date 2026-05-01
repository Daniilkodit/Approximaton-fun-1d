#include "header.h"


void init_approximation (int n, int k, int method_id, double a, double b,
                        double *x, double *y,
                        double *x_cheb, double *y_cheb,
                        double *coeff_1, double *coeff_2,
                        double *d, double *d1, double *d2, double *d3, double *r_side, double err)
{
  double h = (b - a) / (n - 1);
  for (int i = 0; i < n; i++)
    {
      x[i] = a + i * h;
      y[i] = f (x[i], k);
    }
  for (int i = 0; i < n && n < 50; i++)
    {
      double t_i = cos (M_PI * (2 * i + 1) / (2 * n));
      double x_i = (a + b) / 2.0 + (b - a) / 2.0 * t_i;
      y_cheb[i] = f (x_i, k);
    }
  y [n / 2] += err;
  if (n < 50)
    y_cheb [n / 2] += err;

  d[0] = df (x[0], k);
  d[n - 1] = df (x[n-1], k);

  if (n < 50 && method_id != 1)
    create_aproximaniton_polynom (n, x_cheb, y_cheb, coeff_1, a, b);
  if (method_id != 0)
    create_aproximaniton_bit_polynom (n, x, y, coeff_2, d, d1, d2, d3, r_side);
}

double
f (double x, int k)
{
  switch (k)
    {
      case 0:
        return 1.0;
      case 1:
        return x;
      case 2:
        return x * x;
      case 3:
        return x * x * x;
      case 4:
        return x * x * x * x;
      case 5:
        return exp (x);
      case 6:
        return 1.0 / (25.0 * x * x + 1.0);

      default:
        return 0.0;
    }
}

double
df (double x, int k)
{
  switch (k)
    {
      case 0:
        return 0.0;
      case 1:
        return 1.0;
      case 2:
        return 2.0 * x;
      case 3:
        return 3.0 * x * x;
      case 4:
        return 4.0 * x * x * x;
      case 5:
        return exp (x);
      case 6:
        return -50.0 * x / ((25.0 * x * x + 1.0) * (25.0 * x * x + 1.0));

      default:
        return 0.0;
    }
}
