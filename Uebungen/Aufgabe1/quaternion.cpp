#include "quaternion.h"
#include "math.h"
#include <iostream>

Quaternion::Quaternion()
{
    for (int i=0; i<4; ++i) x[i]=0;
}

Quaternion::Quaternion(const float s, const CVec4f vector)
{
    x[0] = s;
    x[1] = vector(0);
    x[2] = vector(1);
    x[3] = vector(2);
}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::operator *(const Quaternion &q2)
{
    Quaternion q;
    // (s1, X1) * (s2, X2) = (s1*s2 - X1 dot X2, s1*X2 + s2*X1 + X1 x X2)

    // Imaginaerteil: s1*s2 - X1 dot X2
    q(0) = x[0] * q2(0) - (x[1] * q2(1) + x[2] * q2(2) + x[3] * q2(3));

    // Realteil: s1*X2 + s2*X1 + X1 x X2
    q(1) = x[0] * q2(1) + q2(0) + x[1] + x[2] * q2(3) - x[3] * q2(2);
    q(2) = x[0] * q2(2) + q2(0) + x[2] + x[3] * q2(1) - x[1] * q2(3);
    q(3) = x[0] * q2(3) + q2(0) + x[3] + x[1] * q2(2) - x[2] * q2(1);

    return q;
}

float &Quaternion::operator ()(unsigned i)
{
    return x[i];
}

float Quaternion::operator ()(unsigned i) const
{
    return x[i];
}


// Wenn Quaternion Betrag 1 hat, dann ist die Konjugierte gleich die Inverse
Quaternion Quaternion::getConjunction()
{
    Quaternion conjunction;

    // http://www.iti.fh-flensburg.de/lang/algorithmen/grundlagen/quat.htm
    conjunction(0) = x[0];
    conjunction(1) = -x[1];
    conjunction(2) = -x[2];
    conjunction(3) = -x[3];

    return conjunction;
}

void Quaternion::rotate(Quaternion &qAxis)
{
    // Skripte, S. 2-51: q * (0, p) * q'
    // Da die Axe vorher normiert wurde, ist die Konjugierte die Inverse.
    const Quaternion rotatedQuatVec = qAxis * (*this) * qAxis.getConjunction();

    x[0] = 0;
    x[1] = rotatedQuatVec(1);
    x[2] = rotatedQuatVec(2);
    x[3] = rotatedQuatVec(3);
}

CVec4f Quaternion::toVector()
{
    return CVec4f(x[1], x[2], x[3], 0);
}
