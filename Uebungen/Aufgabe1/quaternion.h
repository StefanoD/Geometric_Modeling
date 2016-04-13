#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "vec.h"

class Quaternion {
private:
    float x[4];

public:
    // constructors/destructors
    Quaternion();
    Quaternion(CVec4f homogenizedVector);
    ~Quaternion();

    Quaternion operator * (const Quaternion &q2);

    float &operator ()(unsigned i);
    float operator () (unsigned i) const;
};


#endif
