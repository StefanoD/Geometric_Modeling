#include "bezier_calc.h"
#include <cmath>
#include <vector>
#include <QPointF>
#include "points.h"


QPointF
BezierCalc::deCasteljau(QList<QPointF>& points, const double t, const int degree,
                        const int index)
{
  if (degree == 0) // P(0)_i sind immer die Stützpunkte selbst
    return points.at(index);

  return (1.0 - t) * deCasteljau(points, t, degree - 1, index) +
         t * deCasteljau(points, t, degree - 1, index + 1);
}

