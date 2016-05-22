#include "bezier_calc.h"
#include <cmath>
#include <vector>
#include <QPointF>
#include <QList>
#include "points.h"

QPointF
BezierCalc::deCasteljau(const QList<QPointF>& points, const double t,
                        const int degree, const int index)
{
  if (degree == 0) // P(0)_i sind immer die Stützpunkte selbst
    return points.at(index);

  return (1.0 - t) * deCasteljau(points, t, degree - 1, index) +
         t * deCasteljau(points, t, degree - 1, index + 1);
}

QList<QPointF>
BezierCalc::deCasteljauPolarForm(const QList<QPointF>& points, const double t)
{
  const int degree = points.count() - 1;
  QList<QPointF> totalResult;

  deCasteljauPolarFormRecursiv(points, t, degree, degree, totalResult);

  return totalResult;
}

QPointF
BezierCalc::deCasteljauPolarFormRecursiv(const QList<QPointF>& points,
                                         const double t, const int totalDegree,
                                         const int currentDegree,
                                         QList<QPointF>& totalResult,
                                         const int index)
{
  QPointF result;

  // P(0)_i sind immer die Stützpunkte selbst
  if (currentDegree == 0) {
    result = points.at(index);
  } else {
    result = (1.0 - t) * deCasteljauPolarFormRecursiv(points, t, totalDegree,
                                                      currentDegree - 1,
                                                      totalResult, index) +
             t * deCasteljauPolarFormRecursiv(points, t, totalDegree,
                                              currentDegree - 1, totalResult,
                                              index + 1);
  }

  // Obere Diagonale
  if (index == 0) {
    totalResult.append(result);
  }
  // Untere Zeile
  else if ((totalDegree - currentDegree) == index) {
    totalResult.append(result);
  }

  return result;
}

QList<QPointF>
BezierCalc::calcBezierCurveSimple(const QList<QPointF>& controllPoints,
                                  const double epsilon)
{
  const int degree = controllPoints.count() - 1;

  QList<QPointF> bezierPoints;

  for (float t = 0.0; t < 1.0; t += epsilon) {
    bezierPoints.append(BezierCalc::deCasteljau(controllPoints, t, degree));
  }

  // Add last point. (b_0)^n is the last controll point for t = 1.

  bezierPoints.append(controllPoints[degree]);

  return bezierPoints;
}

QList<QPointF>
BezierCalc::calcBezierCurvePolar(const QList<QPointF>& controllPoints,
                                 const double epsilon)
{
  QList<QPointF> pointsOnCurve;

  calcBezierCurvePolar(controllPoints, controllPoints.count() - 1, epsilon,
                       pointsOnCurve);

  return pointsOnCurve;
}

void
BezierCalc::splitIntoHalf(const QList<QPointF>& source, QList<QPointF>& left,
              QList<QPointF>& right)
{
  const int half = source.size() / 2;

  for (int i = 0; i < source.size(); ++i) {
    if (i < half) {
      left.append(source[i]);
    } else {
      right.append(source[i]);
    }
  }
}

void
BezierCalc::calcBezierCurvePolar(const QList<QPointF>& controllPoints,
                                 const int degree, const double epsilon,
                                 QList<QPointF>& result)
{
  const double t = 0.5;
  const QPointF maxDist = getMaxForwardDistance(controllPoints);

  if (degree == 0 || (maxDist.x() < epsilon && maxDist.y() < epsilon)) {
    result.append(controllPoints);
  } else {
    const QList<QPointF> curvePoints = deCasteljauPolarForm(controllPoints, t);
    QList<QPointF> leftHalf;
    QList<QPointF> rightHalf;

    splitIntoHalf(curvePoints, leftHalf, rightHalf);

    calcBezierCurvePolar(leftHalf, degree - 1, epsilon, result);
    calcBezierCurvePolar(rightHalf, degree - 1, epsilon, result);
  }
}

// private

QPointF
BezierCalc::getMaxForwardDistance(const QList<QPointF> points)
{
  double maxX = 0.0;
  double maxY = 0.0;

  for (int i = 0; i < points.size() - 2; ++i) {
    // Slide 5-40
    const QPointF temp = points[i + 2] - 2 * points[i + 1] + points[i];

    maxX = fmax(std::abs(temp.x()), maxX);
    maxY = fmax(std::abs(temp.y()), maxY);
  }

  return QPointF(maxX, maxY);
}
