#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets/QtWidgets>
#include "header.h"

class Window : public QWidget
{
  Q_OBJECT
private:
  int func_id;
  int method_id;
  std::string f_name;
  std::string method_name_pol;
  std::string method_name_bit;
  std::string method_name_err_bit;
  std::string method_name_err_pol;
  double a;
  double b;
  double err;
  int n;
  int s;

  std::unique_ptr<double[]> x;
  std::unique_ptr<double[]> y;
  std::unique_ptr<double[]> x_cheb;
  std::unique_ptr<double[]> y_cheb;
  std::unique_ptr<double[]> coeff_1;
  std::unique_ptr<double[]> coeff_2;
  std::unique_ptr<double[]> d;
  std::unique_ptr<double[]> d1;
  std::unique_ptr<double[]> d2;
  std::unique_ptr<double[]> d3;
  std::unique_ptr<double[]> r_side;

  void reallocate_arrays ();
  void init_and_update ();

public:
  Window (QWidget *parent);

  QSize minimumSizeHint () const;
  QSize sizeHint () const;
  int parse_command_line (int argc, char *argv[]);
  QPointF l2g (double x_loc, double y_loc, double a, double b, double y_min, double y_max);

public slots:
  void change_func ();
  void redraw_graph ();
  void zoom_in ();
  void zoom_out ();
  void increase_n ();
  void decrease_n ();
  void model_error_plus ();
  void model_error_minus ();

protected:
  void paintEvent (QPaintEvent *event);
};
#endif
