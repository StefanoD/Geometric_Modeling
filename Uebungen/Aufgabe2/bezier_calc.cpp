#include "bezier_calc.h"
#include <cmath>
#include <vector>
#include <QLineF>
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
  QList<QPointF> diagonal;
  QList<QPointF> lastRow;

  deCasteljauPolarFormRecursiv(points, t, degree, degree, diagonal, lastRow);

  QList<QPointF> totalResult;
  totalResult.append(diagonal);

  // Die untere Zeile muss verdammt scheiße nochmal rückwärts geholt werden!!!
  for (auto it = lastRow.rbegin(); it != lastRow.rend(); ++it) {
    totalResult.append(*it);
  }

  return totalResult;
}

QPointF
BezierCalc::deCasteljauPolarFormRecursiv(const QList<QPointF>& points,
                                         const double t, const int totalDegree,
                                         const int currentDegree,
                                         QList<QPointF>& diagonal,
                                         QList<QPointF>& lastRow,
                                         const int index)
{
  QPointF result;

  // P(0)_i sind immer die Stützpunkte selbst
  if (currentDegree == 0) {
    result = points.at(index);
  } else {
    result = (1.0 - t) * deCasteljauPolarFormRecursiv(
                           points, t, totalDegree, currentDegree - 1, diagonal,
                           lastRow, index) +
             t * deCasteljauPolarFormRecursiv(points, t, totalDegree,
                                              currentDegree - 1, diagonal,
                                              lastRow, index + 1);
  }

  // Untere Zeile
  if ((totalDegree - currentDegree) == index) {
    lastRow.append(result);
  }
  // Obere Diagonale
  else if (index == 0) {
    diagonal.append(result);
  }

  return result;
}

QList<QPointF>
BezierCalc::calcBezierCurvePolar(const QList<QPointF>& controllPoints,
                                 const double epsilon)
{
  QList<QPointF> pointsOnCurve;

  calcBezierCurvePolar(controllPoints, 1000, epsilon, pointsOnCurve);

  return pointsOnCurve;
}

void
BezierCalc::splitIntoHalf(const QList<QPointF>& source, QList<QPointF>& left,
                          QList<QPointF>& right)
{
  const int half = source.size() / 2;
  const int end = source.size();

  for (int i = 0; i < end; ++i) {
    if (i < half) {
      left.append(source[i]);
    } else {
      right.append(source[i]);
    }
  }
}

bool
BezierCalc::boundingBoxesIntersects(QList<QPointF>& bezier1,
                                    QList<QPointF>& bezier2)
{
  QPointF bezier1Min;
  QPointF bezier1Max;

  QPointF bezier2Min;
  QPointF bezier2Max;

  getMinMax(bezier1, bezier1Min, bezier1Max);
  getMinMax(bezier2, bezier2Min, bezier2Max);

  if (bezier1Min.x() < bezier2Max.x() && bezier1Max.x() > bezier2Min.x() &&
      bezier1Min.y() < bezier2Max.y() && bezier1Max.y() > bezier2Min.y()) {
    return true;
  }
  return false;
}

void
BezierCalc::getMinMax(QList<QPointF>& bezier, QPointF& min, QPointF& max)
{
  max.rx() = -255;
  max.ry() = -255;

  min.rx() = 255;
  min.ry() = 255;

  for (auto& point : bezier) {
    if (point.x() < min.rx())
      min.rx() = point.x();
    if (point.y() < min.ry())
      min.ry() = point.y();

    if (point.x() > max.rx())
      max.rx() = point.x();
    if (point.y() > max.ry())
      max.ry() = point.y();
  }
}

void
BezierCalc::calcBezierCurvePolar(const QList<QPointF>& controllPoints,
                                 const int k, const double epsilon,
                                 QList<QPointF>& result)
{
  const double t = 0.1;
  const double maxDist = getMaxForwardDistance(controllPoints);

  if (k == 0 || maxDist < epsilon) {
    result.append(controllPoints);
  } else {
    const QList<QPointF> curvePoints = deCasteljauPolarForm(controllPoints, t);
    QList<QPointF> leftHalf;
    QList<QPointF> rightHalf;

    splitIntoHalf(curvePoints, leftHalf, rightHalf);

    calcBezierCurvePolar(leftHalf, k - 1, epsilon, result);
    calcBezierCurvePolar(rightHalf, k - 1, epsilon, result);
  }
}

double
BezierCalc::getTotalAngle(const QList<QPointF>& controllPoints)
{
  int end = controllPoints.size() - 1;

  // compute first derivation
  QList<QPointF> derivation;

  for (int i = 0; i < end; ++i) {
    derivation.append(controllPoints[i + 1] - controllPoints[i]);
  }

  double totalAngle = 0.0;
  end = derivation.size() - 1;

  for (int i = 0; i < end; ++i) {
    const QLineF line1 = QLineF(QPointF(0, 0), derivation[i]);
    const QLineF line2 = QLineF(QPointF(0, 0), derivation[i + 1]);

    double tmpAngle = line1.angleTo(line2);

    if (tmpAngle > 180) {
      tmpAngle = 360 - tmpAngle;
    }

    totalAngle += tmpAngle;
  }

  return totalAngle;
}

// private

double
BezierCalc::getMaxForwardDistance(const QList<QPointF>& points)
{
  double maxNorm = -2;

  for (int i = 0; i < points.size() - 2; ++i) {
    // Slide 5-40
    const QPointF temp = points[i + 2] - 2 * points[i + 1] + points[i];
    maxNorm =
      fmax(maxNorm, std::sqrt(temp.x() * temp.x() + temp.y() * temp.y()));
  }

  return maxNorm;
}

void
BezierCalc::computeSelfIntersectionFreeSegments(
  const QList<QPointF>& bezierPolygon, QList<QList<QPointF>>& resultSegments)
{
  const double t = 0.5;
  const double angle = getTotalAngle(bezierPolygon);

  if (angle < 180) {
    resultSegments.append(bezierPolygon);
  } else {
    const QList<QPointF> newBezierPolygon = deCasteljauPolarForm(bezierPolygon, t);
    QList<QPointF> left, right;
    splitIntoHalf(newBezierPolygon, left, right);
    computeSelfIntersectionFreeSegments(left, resultSegments);
    computeSelfIntersectionFreeSegments(right, resultSegments);
  }
}
