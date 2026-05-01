#ifndef HEADER
#define HEADER

#include <cstdio>
#include <cmath>
#include <string>
#include <memory>
#include <ctime>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
//#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QtWidgets>
#include <QPainter>

double f (double x, int k);
double df (double x, int k);
void init_approximation (int n, int k, int method_id, double a, double b,
                         double *x, double *y,
                         double *x_cheb, double *y_cheb,
                         double *coeff_1, double *coeff_2,
                         double *d, double *d1, double *d2, double *d3, double *r_side, double err);

int gauss_tridiagonal (int n, double *d, double *d1,
                        double *d2, double *d3, double *b);

int create_aproximaniton_bit_polynom (int n, double *x, double *y, double *coeff,
                                        double *d, double *d1, double *d2, double *d3, double *b);
int create_aproximaniton_polynom (int n, double */*x_cheb*/, double *y_cheb,
                                  double *coeff, double /*a*/, double /*b*/);

double my_bit_aproximation (double x0, double a, double b,
                              int n, double *x, double *coeff);
double my_aproximation (double x0, double a, double b,
                          int n, double* /*x*/, double *coeff);

#endif
