#ifndef BEZIERCALC_H
#define BEZIERCALC_H

#include <QPointF>
#include <QList>

class BezierCalc
{
public:
  static QPointF deCasteljau(const QList<QPointF>& points, const double t,
                             const int degree, const int index = 0);

  static QList<QPointF> deCasteljauPolarForm(const QList<QPointF>& points,
                                             const double t);

  static QPointF deCasteljauPolarFormRecursiv(
    const QList<QPointF>& points, const double t, const int totalDegree,
    const int currentDegree, QList<QPointF>& diagonal, QList<QPointF>& lastRow,
    const int index = 0);

  static QList<QPointF> calcBezierCurvePolar(
    const QList<QPointF>& controllPoints, const double epsilon);

  static double getMaxForwardDistance(const QList<QPointF>& points);

  static void splitIntoHalf(const QList<QPointF>& source, QList<QPointF>& left,
                            QList<QPointF>& right);

  static bool boundingBoxesIntersects(QList<QPointF>& bezier1,
                                      QList<QPointF>& bezier2);

  static void getMinMax(QList<QPointF>& bezier, QPointF& min, QPointF& max);

  static void computeSelfIntersectionFreeSegments(
    const QList<QPointF>& bezierPoints, QList<QList<QPointF>>& resultSegments);

  static double getTotalAngle(const QList<QPointF>& controllPoints);

private:
  static void calcBezierCurvePolar(const QList<QPointF>& controllPoints,
                                   const int k, const double epsilon,
                                   QList<QPointF>& result);
};

#endif // BEZIERCALC_H
