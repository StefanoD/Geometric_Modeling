#include "bezier_calc.h"
#include <cmath>
#include <vector>
#include <QLineF>
#include "points.h"


void
BezierCalc::deCasteljau(QList<QPointF>& resultCurve,
                        const QList<QPointF>& controllPoints, const int degree,
                        const float t)
{
  QList<QPointF> tmpPoints;

  if (degree > 0) {
    // add first point
    resultCurve.append(controllPoints.first());

    for (int i = 0; i < degree - 1; ++i) {
      // lerp
      tmpPoints.append((1.0 - t) * controllPoints[i] +
                        t * controllPoints[i + 1]);
    }
    deCasteljau(resultCurve, tmpPoints, degree - 1, t);

    // add last point
    resultCurve.append(controllPoints.last());
  }
}

QList<QPointF>
BezierCalc::computeCn1(const QList<QPointF>& controllPoints,
                       const QPointF& newPoint)
{
  const double t = 2;

  QList<QPointF> consecutivePoints;
  deCasteljau(consecutivePoints, controllPoints, controllPoints.size(), t);

  QList<QPointF> resultPoints;

  for (int i = consecutivePoints.size() - 1; i > controllPoints.size(); --i) {
    resultPoints.append(consecutivePoints[i]);
  }

  resultPoints.append(newPoint);

  return resultPoints;
}

QList<QPointF>
BezierCalc::calcBezierCurvePolar(const QList<QPointF>& controllPoints,
                                 const double epsilon)
{
  QList<QPointF> pointsOnCurve;

  calcBezierCurvePolar(controllPoints, 10, epsilon, pointsOnCurve);

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
  const double t = 0.5;
  const double maxDist = getMaxForwardDistance(controllPoints);

  if (k == 0 || maxDist < epsilon) {
    result.append(controllPoints);
  } else {
    QList<QPointF> curvePoints;
    deCasteljau(curvePoints, controllPoints, controllPoints.size(), t);
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
    QList<QPointF> newBezierPolygon;
    deCasteljau(newBezierPolygon, bezierPolygon, bezierPolygon.size(), t);

    QList<QPointF> left, right;

    splitIntoHalf(newBezierPolygon, left, right);

    computeSelfIntersectionFreeSegments(left, resultSegments);
    computeSelfIntersectionFreeSegments(right, resultSegments);
  }
}
