#ifndef BEZIERCALC_H
#define BEZIERCALC_H

#include "points.h"
class QPointF;

class BezierCalc
{
public:
  static QPointF deCasteljau(QList<QPointF>& points, const double t,
                             const int degree, const int index = 0);
};

#endif // BEZIERCALC_H
