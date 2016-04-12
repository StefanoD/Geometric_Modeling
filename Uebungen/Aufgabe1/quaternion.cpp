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
    for (int i=0; i<4; i++) x[i]=0;
}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::operator *(const Quaternion &quat)
{

}
