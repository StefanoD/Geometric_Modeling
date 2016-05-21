#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "bezier_calc.h"
#include <QPointF>
#include "points.h"
#include <iostream>

int
main(int argc, char* argv[])
{
  // Test BEGIN slide 5-22
  QList<QPointF>  points;
  points.append(QPointF(0.0, 0.0));
  points.append(QPointF(2, 4.5));
  points.append(QPointF(8.5, 6.5));
  points.append(QPointF(11.0, 2.0));

  QPointF result = BezierCalc::deCasteljau(points, 0.6, points.count() - 1);

  std::cout << "Result: (" << result.x() << ", " << result.y() << "), Exptected: (6.63, 4.54)" << std::endl;
  // Test END


  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
