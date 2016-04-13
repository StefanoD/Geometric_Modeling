////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "quaternion.h"
#include "math.h"
#include <iostream>

Quaternion::Quaternion()
{
    for (int i=0; i<4; ++i) x[i]=0;
}

Quaternion::Quaternion(CVec4f homogenizedVector)
{
    x[0] = 0;
    x[1] = homogenizedVector(0);
    x[2] = homogenizedVector(1);
    x[3] = homogenizedVector(2);
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
