#include "bezier_calc.h"
#include <cmath>
#include <vector>
#include <QPointF>
#include "points.h"

BezierCalc::BezierCalc()
{
}

double
BezierCalc::calcBernsteinPolynom(const int index, const int degree,
                                 const double parameter)
{
  // https://de.wikipedia.org/wiki/Bernsteinpolynom

  const int binomialCoeff = calcBinomialCoeff(degree, index);

  const int degreeBinomCoeffRatio = degree - index;
  const double binomCoeffRatio =
    std::pow((1.0 - parameter), degreeBinomCoeffRatio);

  const double parameterRatio = std::pow(parameter, degree);

  const double bernsteinPolynom =
    binomialCoeff * binomCoeffRatio * parameterRatio;

  return bernsteinPolynom;
}

QPointF
BezierCalc::deCasteljau(Points& points, const double t, const int degree,
                        const int index)
{
  if (degree == 0) // P(0)_i sind immer die Stützpunkte selbst
    return points.getPoint(index);

  return (1.0 - t) * deCasteljau(points, t, degree - 1, index) +
         t * deCasteljau(points, t, degree - 1, index + 1);
}

double
BezierCalc::calcPointOnBezierCurve(Points& points, const double t)
{
}

int
BezierCalc::calcBinomialCoeff(int n, int k)
{
  // http://www.geeksforgeeks.org/space-and-time-efficient-binomial-coefficient/
  int res = 1;

  // Since C(n, k) = C(n, n-k)
  if (k > n - k)
    k = n - k;

  // Calculate value of [n * (n-1) *---* (n-k+1)] / [k * (k-1) *----* 1]
  for (int i = 0; i < k; ++i) {
    res *= (n - i);
    res /= (i + 1);
  }

  return res;
}
