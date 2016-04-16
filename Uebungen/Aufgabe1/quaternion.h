#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "vec.h"

class Quaternion {
private:
    float x[4];

public:
    // constructors/destructors
    Quaternion();
    Quaternion(const float s, const CVec4f vector);
    ~Quaternion();

    Quaternion operator * (const Quaternion &q2);

    Quaternion getConjunction();

    Quaternion rotate(Quaternion &qRotation);
    CVec4f toVector();
    CVec4f toPoint();

    float &operator ()(unsigned i);
    float operator () (unsigned i) const;
};


#endif
