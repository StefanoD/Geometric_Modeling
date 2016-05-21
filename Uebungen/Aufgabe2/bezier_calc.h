#ifndef BEZIERCALC_H
#define BEZIERCALC_H

#include "points.h"
class QPointF;

class BezierCalc
{
public:
    BezierCalc();

    static double calcBernsteinPolynom(const int index,
                                    const int degree,
                                    const double parameter);

    static QPointF deCasteljau(QList<QPointF> &points, const double t,
                               const int degree, const int index = 0);

    static double calcPointOnBezierCurve(Points &points, const double t);

private:

    // Returns value of Binomial Coefficient C(n, k)
    static int calcBinomialCoeff(int n, int k);
};

#endif // BEZIERCALC_H
