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

    Quaternion getInverse();
    Quaternion getConjunction();

    void rotate(Quaternion &q2);

    float &operator ()(unsigned i);
    float operator () (unsigned i) const;
};


#endif
