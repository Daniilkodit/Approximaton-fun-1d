#include "header.h"
#include "window.h"

#define DEFAULT_A -1
#define DEFAULT_B 1
#define DEFAULT_N 5
#define L2G(X,Y,A,B) (l2g ((X), (Y), (A), (B), min_y, max_y))

Window::Window (QWidget *parent)
  : QWidget (parent)
  , func_id (0)
  , method_id (0)
  , f_name ("f (x) = 1")
  , method_name_pol ("Chebyshev polynomial expansion")
  , method_name_bit ("Cubic spline interpolation")
  , method_name_err_bit ("Method error cubic spline")
  , method_name_err_pol ("Method error polynomial")
  , a (DEFAULT_A)
  , b (DEFAULT_B)
  , err (0)
  , n (DEFAULT_N)
  , s (0)
  , x       (std::make_unique<double[]> (n))
  , y       (std::make_unique<double[]> (n))
  , x_cheb  (std::make_unique<double[]> (n))
  , y_cheb  (std::make_unique<double[]> (n))
  , coeff_1 (std::make_unique<double[]> (n))
  , coeff_2 (std::make_unique<double[]> ((n - 1) * 4))
  , d       (std::make_unique<double[]> (n))
  , d1      (std::make_unique<double[]> (n))
  , d2      (std::make_unique<double[]> (n))
  , d3      (std::make_unique<double[]> (n))
  , r_side  (std::make_unique<double[]> (n))
{
  init_and_update ();
}

void Window::reallocate_arrays ()
{
  x       = std::make_unique<double[]> (n);
  y       = std::make_unique<double[]> (n);
  x_cheb  = std::make_unique<double[]> (n);
  y_cheb  = std::make_unique<double[]> (n);
  coeff_1 = std::make_unique<double[]> (n);
  coeff_2 = std::make_unique<double[]> ((n - 1) * 4);
  d       = std::make_unique<double[]> (n);
  d1      = std::make_unique<double[]> (n);
  d2      = std::make_unique<double[]> (n);
  d3      = std::make_unique<double[]> (n);
  r_side  = std::make_unique<double[]> (n);
}

void Window::init_and_update ()
{
  init_approximation (n, func_id, method_id, a, b,
                      x.get (), y.get (),
                      x_cheb.get (), y_cheb.get (),
                      coeff_1.get (), coeff_2.get (),
                      d.get (), d1.get (), d2.get (), d3.get (), r_side.get (), err);
  update ();
}

QSize Window::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize Window::sizeHint () const
{
  return QSize (1000, 1000);
}

int Window::parse_command_line (int argc, char *argv[])
{
  if (!(argc == 5
        && sscanf (argv[1], "%lf", &a)       == 1
        && sscanf (argv[2], "%lf", &b)       == 1
        && sscanf (argv[3], "%d",  &n)       == 1
        && sscanf (argv[4], "%d",  &func_id) == 1))
    {
      return -1;
    }

  if (n <= 1 || func_id < 0 || a >= b  || b - a <= 1.e-6)
    {
      return -2;
    }

  reallocate_arrays ();
  init_and_update ();
  return 0;
}

/// change current function for drawing
void Window::change_func ()
{
  func_id = (func_id + 1) % 7;

  switch (func_id)
    {
      case 0:
        f_name = "f (x) = 1";
        break;
      case 1:
        f_name = "f (x) = x";
        break;
      case 2:
        f_name = "f (x) = x^2";
        break;
      case 3:
        f_name = "f (x) = x^3";
        break;
      case 4:
        f_name = "f (x) = x^4";
        break;
      case 5:
        f_name = "f (x) = exp (x)";
        break;
      case 6:
        f_name = "f (x) = 1 / (25x^2 + 1)";
        break;
    }
  init_and_update ();
}

void Window::redraw_graph ()
{
  method_id = (method_id + 1) % 4;
  init_and_update ();
}

void Window::increase_n ()
{
  n *= 2;
  reallocate_arrays ();
  init_and_update ();
}

void Window::decrease_n ()
{
  n /= 2;
  if (n < 5)
    n = 5;
  reallocate_arrays ();
  init_and_update ();
}

void Window::zoom_in ()
{
  s++;
  update ();
}

void Window::zoom_out ()
{
  s--;
  update ();
}

void Window::model_error_plus ()
{
  err += 0.1;
  init_and_update ();
}

void Window::model_error_minus ()
{
  err -= 0.1;
  init_and_update ();
}

QPointF Window::l2g (double x_loc, double y_loc, double a, double b, double y_min, double y_max)
{
  double x_gl = (x_loc - a) / (b - a) * width ();
  double y_gl = (y_max - y_loc) / (y_max - y_min) * height ();
  return QPointF (x_gl, y_gl);
}

void Window::paintEvent (QPaintEvent * /* event */)
{
  QPainter painter (this);
  clock_t start = clock();
  double x0, y1, y2, y3;
  double x0_prev, y1_prev, y2_prev, y3_prev;
  double max_y, min_y;
  double delta_y;
  double scale = pow (2.0, s);
  double mid   = (a + b) / 2.0;
  double a_s   = mid + (a - mid) / scale;
  double b_s   = mid + (b - mid) / scale;

  double *x_ptr       = x.get ();
  double *coeff_1_ptr = coeff_1.get ();
  double *coeff_2_ptr = coeff_2.get ();

  auto my_method = [&](double x0)
    {
      if (x0 < a || x0 > b)
        {
          y1 = 0;
          y2 =0;
          y3 =0;
          return;
        }
      if (method_id == 0)
        {
          y1 = f (x0, func_id);
          y2 = my_aproximation (x0, a, b, n, x_ptr, coeff_1_ptr);
          y3 = y1;
        }
      else if (method_id == 1)
        {
          y1 = f (x0, func_id);
          y2 = my_bit_aproximation (x0, a, b, n, x_ptr, coeff_2_ptr);
          y3 = y1;
        }
      else if (method_id == 2)
        {
          y1 = f (x0, func_id);
          y2 = my_aproximation (x0, a, b, n, x_ptr, coeff_1_ptr);
          y3 = my_bit_aproximation (x0, a, b, n, x_ptr, coeff_2_ptr);
        }
      else
        {
          y1 = f (x0, func_id) - my_bit_aproximation (x0, a, b, n, x_ptr, coeff_2_ptr);
          y2 = f (x0, func_id) - my_aproximation (x0, a, b, n, x_ptr, coeff_1_ptr);
          y3 = y1;
        }

      if (n > 50 && method_id != 1)
        {
          y2 = y1; // т.е исключили апроксимацию многочленами
        }
    };

  QPen pen_green (Qt::green, 2, Qt::SolidLine);
  QPen pen_red (Qt::red, 2, Qt::SolidLine);
  QPen pen_blue (Qt::blue, 2, Qt::SolidLine);
  QPen pen_black (Qt::black, 1, Qt::SolidLine);

  int W = width (), i;
  double hx = (b_s - a_s) / W;
  max_y = min_y = 0;
  for (i = 0; i <= W; i++)
    {
      x0 = a_s + i * hx;
      my_method (x0);

      double y_loc_min = std::min (y1, std::min (y2, y3));
      double y_loc_max = std::max (y1, std::max (y2, y3));
      if (y_loc_min < min_y)
        min_y = y_loc_min;
      if (y_loc_max > max_y)
        max_y = y_loc_max;
    }

  delta_y = 0.01 * (max_y - min_y);
  min_y -= delta_y;
  max_y += delta_y;

  // draw approximated line for graph
  x0 = a_s;
  my_method (x0);
  for (i = 1; i <= W; i++)
    {
      y1_prev = y1;
      y2_prev = y2;
      y3_prev = y3;
      x0_prev = x0;

      x0 = a_s + i * hx;
      my_method (x0);
      // local coords are converted to draw coords
      painter.setPen (pen_green);
      painter.drawLine (L2G(x0_prev, y1_prev, a_s, b_s), L2G(x0, y1, a_s, b_s));
      if ((method_id != 1 && n <= 50) || method_id == 1)
        {
          painter.setPen (pen_red);
          painter.drawLine (L2G(x0_prev, y2_prev, a_s, b_s), L2G(x0, y2, a_s, b_s));
        }
      if (method_id == 2)
        {
          painter.setPen (pen_blue);
          painter.drawLine (L2G(x0_prev, y3_prev, a_s, b_s), L2G(x0, y3, a_s, b_s));
        }
    }

  // draw axis
  painter.setPen (pen_black);
  painter.drawLine (L2G(a_s, 0, a_s, b_s), L2G(b_s, 0, a_s, b_s));
  painter.drawLine (L2G(0, min_y, a_s, b_s), L2G(0, max_y, a_s, b_s));

  QString qf_name         = QString::fromStdString (f_name);
  QString qmethod_pol     = QString::fromStdString (method_name_pol);
  QString qmethod_bit     = QString::fromStdString (method_name_bit);
  QString qmethod_err_bit = QString::fromStdString (method_name_err_bit);
  QString qmethod_err_pol = QString::fromStdString (method_name_err_pol);

  if (method_id == 0)
    {
      painter.setPen (Qt::green);
      painter.drawText (0, 20, qf_name);
      painter.setPen (Qt::red);
      painter.drawText (0, 40, qmethod_pol);
    }
  else if (method_id == 1)
    {
      painter.setPen (Qt::green);
      painter.drawText (0, 20, qf_name);
      painter.setPen (Qt::red);
      painter.drawText (0, 40, qmethod_bit);
    }
  else if (method_id == 2)
    {
      painter.setPen (Qt::green);
      painter.drawText (0, 20, qf_name);
      painter.setPen (Qt::red);
      painter.drawText (0, 40, qmethod_pol);
      painter.setPen (Qt::blue);
      painter.drawText (0, 60, qmethod_bit);
    }
  else
    {
      painter.setPen (Qt::green);
      painter.drawText (0, 20, qmethod_err_bit);
      painter.setPen (Qt::red);
      painter.drawText (0, 40, qmethod_err_pol);
      painter.setPen (Qt::black);
      painter.drawText (0, 60, qf_name);
    }
  painter.setPen (Qt::black);
  painter.drawText (0, 80, QString ("n = ") + QString::number (n)
                         + QString (", err = ") + QString::number (err)
                         + QString (", FMAX = ") + QString::number (fabs (max_y))
                         + QString (", FMIN = ") + QString::number (fabs (min_y))
                         + QString (", scale = ") + QString::number (scale));

  double t = double(clock() - start) / CLOCKS_PER_SEC;
  int task1 = 5, task2 = 35, p = static_cast<int> (err / 0.1);
  printf("a.out : Task1 = %d Task2 = %d : k = %d; m = %d; s = %d; n = %d; p = %d; max = %e; elapsed = %.2f\n",
             task1, task2, func_id, method_id, s, n, p, std::max (max_y, min_y), t);
}
