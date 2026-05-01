#include "header.h"

int
create_aproximaniton_polynom (int n, double * /*x_cheb*/, double *y_cheb,
                                double *coeff, double /*a*/, double /*b*/)
{
  if (n > 50)
    return 0;

  double g1 = 0., g2 = 0.;
  memset (coeff, 0, n * sizeof(double));

  for (int j = 0; j < n; j++)
    {
      double z =  2 * cos (M_PI * (2 * j + 1) / (2 * n));

      g1 = y_cheb[j];
      coeff[0] += g1;

      g2 = g1 * z / 2;
      coeff[1] += g2;

      for (int i = 2; i < n; i++)
        {
          double temp;

          temp = z * g2 - g1;
          g1 = g2;
          g2 = temp;
          coeff[i] += temp;
        }
    }

  coeff[0] /= n;
  for (int i = 1; i < n; i++)
    {
      coeff[i] = coeff[i] * 2 / n;
    }

  return 0;
}

double
my_aproximation (double x0, double a, double b,
                  int n, double* /*x*/, double *coeff)
{
  if (n > 50)
    return 0;

  double z = 2 * (2 * x0 - (b + a)) / (b - a);
  double t_1 = 1, t_2 = z / 2;
  double y = t_1 * coeff[0] + t_2 * coeff[1];

  for (int i = 2; i < n; i++)
    {
      double temp = z * t_2 - t_1;
      t_1 = t_2;
      t_2 = temp;

      y += t_2 * coeff[i];
    }

  return y;
}
