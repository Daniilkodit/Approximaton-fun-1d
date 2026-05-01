#include "header.h"
#include "window.h"

int
main (int argc, char **argv)
{
  QApplication app (argc, argv);

  QMainWindow *window = new QMainWindow;
  QMenuBar *tool_bar = new QMenuBar (window);
  Window *graph_area = new Window (window);
  QAction *action;

  if (graph_area->parse_command_line (argc, argv))
    {
      printf ("Usage: a b n k\n");
      QMessageBox::warning (0, "Wrong input arguments!",
                            "Wrong input arguments!");
      return -1;
    }

  action = tool_bar->addAction ("&Change function", graph_area, SLOT (change_func ()));
  action->setShortcut (QString ("0"));

  action = tool_bar->addAction ("&Redraw graph", graph_area, SLOT (redraw_graph ()));
  action->setShortcut (QString ("1"));

  action = tool_bar->addAction ("&Zoom in", graph_area, SLOT (zoom_in ()));
  action->setShortcut (QString ("2"));

  action = tool_bar->addAction ("&Zoom out", graph_area, SLOT (zoom_out ()));
  action->setShortcut (QString ("3"));

  action = tool_bar->addAction ("&n x 2", graph_area, SLOT (increase_n ()));
  action->setShortcut (QString ("4"));

  action = tool_bar->addAction ("&n / 2", graph_area, SLOT (decrease_n ()));
  action->setShortcut (QString ("5"));

  action = tool_bar->addAction ("&Model measurement error (plus f (x) / 10)", graph_area, SLOT (model_error_plus ()));
  action->setShortcut (QString ("6"));

  action = tool_bar->addAction ("&Model measurement error (minus f (x) / 10)", graph_area, SLOT (model_error_minus ()));
  action->setShortcut (QString ("7"));

  action = tool_bar->addAction ("E&xit", window, SLOT (close ()));
  action->setShortcut (QString ("Ctrl+X"));

  tool_bar->setMaximumHeight (30);

  window->setMenuBar (tool_bar);
  window->setCentralWidget (graph_area);
  window->setWindowTitle ("Graph");

  window->show ();
  app.exec ();

  delete window;
}
