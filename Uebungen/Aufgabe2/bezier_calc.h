#ifndef BEZIERCALC_H
#define BEZIERCALC_H

#include "points.h"
class QPointF;
template <typename T> class QList;

class BezierCalc
{
public:
  static QPointF deCasteljau(const QList<QPointF> &points, const double t,
                             const int degree, const int index = 0);

  static QList<QPointF> calcBezierCurve(const QList<QPointF> &controllPoints2,
                                        const double epsilon);
};

#endif // BEZIERCALC_H
