#include "bezier_calc.h"
#include <cmath>
#include <vector>
#include <QPointF>
#include <QList>
#include "points.h"


QPointF
BezierCalc::deCasteljau(const QList<QPointF>& points, const double t, const int degree,
                        const int index)
{
  if (degree == 0) // P(0)_i sind immer die Stützpunkte selbst
    return points.at(index);

  return (1.0 - t) * deCasteljau(points, t, degree - 1, index) +
         t * deCasteljau(points, t, degree - 1, index + 1);
}

QList<QPointF>
BezierCalc::calcBezierCurve(const QList<QPointF> &controllPoints2, const double epsilon)
{
  const int degree = controllPoints2.count() - 1;

  QList<QPointF> bezierPoints;

  for (float t = 0.0; t < 1.0; t += epsilon) {
    bezierPoints.append(BezierCalc::deCasteljau(controllPoints2, t, degree));
  }

  // Add last point. (b_0)^n is the last controll point for t = 1.

  bezierPoints.append(controllPoints2[degree]);

  return bezierPoints;
}
