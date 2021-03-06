////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#include "viewSystem.h"
#include <iostream>

viewSystem::viewSystem()
{
    RotMode = VIEW_MATRIX_MODE;
}

viewSystem::viewSystem(const float eye[4], const float dir[4], const float up[4], float f)
{
    F = f;

    EyePoint.setData(eye);
    ViewDir .setData(dir);
    ViewUp  .setData(up);

    RotMode = VIEW_MATRIX_MODE;
}

viewSystem::viewSystem(const CVec4f eye, const CVec4f dir, const CVec4f up, float f)
{
    F        = f;
    EyePoint = eye;
    ViewDir  = dir;
    ViewUp   = up;
    RotMode  = VIEW_MATRIX_MODE;
}

viewSystem::~viewSystem()
{
}

void viewSystem::setData(const float eye[4], const float dir[4], const float up[4], float f)
{
    viewSystem buf(eye,dir,up,f);
    *this = buf;
}

void viewSystem::setData(const CVec4f eye, const CVec4f dir, const CVec4f up, float f)
{
    viewSystem buf(eye,dir,up,f);
    *this = buf;
}

void viewSystem::setMode(MatrixMode mode)
{
    RotMode = mode;
}
void viewSystem::Zoom(float f)
{
    F+=f;
}

void viewSystem::RotateX(float angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
        {
            CMat4f Rot;
            float c = cos(angle);
            float s = sin(angle);

            Rot(1,1) = c; Rot(1,2) = -s;
            Rot(2,1) = s; Rot(2,2) =  c;

            Rotate(Rot);break;
        }
    case VIEW_QUATERNION_MODE:
        {
            // AUFGABE02
            CVec4f xAxis(1, 0, 0, 0);
            Quaternion Rot(cos(angle / 2), xAxis * sin(angle / 2));
            Rotate(Rot);
            break;
        }
    }
}

void viewSystem::RotateY(float angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
        {
            CMat4f Rot;
            float c = cos(angle);
            float s = sin(angle);

            Rot(0,0) =  c; Rot(0,2) = s;
            Rot(2,0) = -s; Rot(2,2) = c;

            Rotate(Rot);break;
        }
    case VIEW_QUATERNION_MODE:
        {
            // AUFGABE02
            // Skript S. 2-51
            CVec4f yAxis(0, 1, 0, 0);
            Quaternion quatRot(cos(angle / 2), yAxis * sin(angle / 2));
            Rotate(quatRot);
            break;
        }
    }
}

void viewSystem::RotateZ(float angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
        {
            CMat4f Rot;
            float c = cos(angle);
            float s = sin(angle);

            Rot(0,0) = c; Rot(0,1) = -s;
            Rot(1,0) = s; Rot(1,1) =  c;

            Rotate(Rot);
            break;
        }
    case VIEW_QUATERNION_MODE:
        {
            // AUFGABE02
            CVec4f zAxis(0, 0, 1, 0);
            Quaternion Rot(cos(angle / 2), zAxis * sin(angle / 2));
            Rotate(Rot);
            break;
        }
    }
}

void viewSystem::RotateDir(float angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
        ViewUp = RotationMatrix(ViewDir,angle)*ViewUp;
        break;
    case VIEW_QUATERNION_MODE:
        {
            // AUFGABE02
            Quaternion qDir(cos(angle / 2), ViewDir * sin(angle / 2));
            Quaternion qUp(0, ViewUp);

            qUp = qUp.rotate(qDir);

            ViewUp = qUp.toVector();
            break;
        }
    }
    ViewUp.normalize();
}

void viewSystem::RotateUp(float angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
        ViewDir = RotationMatrix(ViewUp,angle)*ViewDir;
        break;
    case VIEW_QUATERNION_MODE:
        {
            // AUFGABE02
            Quaternion qUp(cos(angle / 2), ViewUp * sin(angle / 2));
            Quaternion qDir(0, ViewDir);

            qDir = qDir.rotate(qUp);

            ViewDir = qDir.toVector();
            break;
        }
    }
    ViewDir.normalize();
}

void viewSystem::RotateHor(float angle)
{
    ViewHor = ViewDir.cross(ViewUp);
    ViewHor.normalize();

    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
        {
            CMat4f Rot = RotationMatrix(ViewHor,angle);
            ViewUp     = Rot*ViewUp;
            ViewDir    = Rot*ViewDir;
        }
    case VIEW_QUATERNION_MODE:
        {
            // AUFGABE02
            // Skript S. 2-51
            Quaternion qHor(cos(angle / 2), ViewHor * sin(angle / 2));
            Quaternion qDir(0, ViewDir);
            Quaternion qUp(0, ViewUp);

            qDir = qDir.rotate(qHor);
            qUp = qUp.rotate(qHor);

            ViewDir = qDir.toVector();
            ViewUp = qUp.toVector();
            break;
        }
    }
    ViewUp .normalize();
    ViewDir.normalize();
}

void viewSystem::Rotate(CMat4f mat)
{
    EyePoint = mat * EyePoint;
    ViewDir  = mat * ViewDir;
    ViewUp   = mat * ViewUp;

    ViewDir.normalize();
    ViewUp .normalize();
}


void viewSystem::Rotate(Quaternion &quatRot)
{
    Quaternion quatEyePoint(0, EyePoint);
    Quaternion quatViewDir (0, ViewDir);
    Quaternion quatViewUp  (0, ViewUp);

    quatEyePoint = quatEyePoint.rotate(quatRot);
    quatViewDir  = quatViewDir.rotate(quatRot);
    quatViewUp   = quatViewUp.rotate(quatRot);

    EyePoint = quatEyePoint.toPoint();
    ViewDir  = quatViewDir.toVector();
    ViewUp   = quatViewUp.toVector();

    ViewDir.normalize();
    ViewUp .normalize();
}

void viewSystem::Rotate(CVec4f axis, float angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
        Rotate(RotationMatrix(axis,angle));
        break;
    case VIEW_QUATERNION_MODE:
        // AUFGABE02
        break;
    }
}

void viewSystem::Translate(CVec4f vec)
{
    EyePoint += vec;
}

void viewSystem::slerpUp(CVec4f startPoint, CVec4f endPoint, const float relativeStep)
{
    const double angle = acos(startPoint * endPoint) * (1.0 / (startPoint.norm() * endPoint.norm()));

    const double leftTerm = sin(1 - relativeStep) * angle / sin(angle);
    const double rightTerm = sin(relativeStep) * angle / sin(angle);

    ViewUp(0) = leftTerm * startPoint(0) + rightTerm * endPoint(0);
    ViewUp(1) = leftTerm * startPoint(1) + rightTerm * endPoint(1);
    ViewUp(2) = leftTerm * startPoint(2) + rightTerm * endPoint(2);

    ViewUp.normalize();
}

void viewSystem::nslerpUP(CVec4f startPoint, CVec4f endPoint, const float relativeStep)
{
    // Normalized SLERP
    startPoint.normalize();
    endPoint.normalize();
    slerpUp(startPoint, endPoint, relativeStep);
}

void viewSystem::lerpUp(CVec4f startPoint, CVec4f endPoint, const float relativeStep)
{
    // Liniear Interpolation
    // https://en.wikipedia.org/wiki/Linear_interpolation#Programming_language_support
    //(1-t)*v0 + t*v1

    ViewUp(0) = (1 - relativeStep) * startPoint(0) + relativeStep * endPoint(0);
    ViewUp(1) = (1 - relativeStep) * startPoint(1) + relativeStep * endPoint(1);
    ViewUp(2) = (1 - relativeStep) * startPoint(2) + relativeStep * endPoint(2);

    ViewUp.normalize();
}

CVec4f viewSystem::Project(CVec4f Point)
{	// central projection of Point
    CMat4f M;
    M(2,2) = 0.0;
    M(3,2) = -(1/F);
    Point  = M*(Point*(1/(1-Point(2)/F)));	// re-normalization
    return Point;
}

CMat4f viewSystem::getViewToWorldTransformMatrix()
{
    // AUFGABE01

    // Skript S. 2-24
    const float arrTranslation[4][4] = {
                                        {1, 0, 0, -EyePoint(0)},
                                        {0, 1, 0, -EyePoint(1)},
                                        {0, 0, 1, -EyePoint(2)},
                                        {0, 0, 0, 1}
                                        };
    const CMat4f matTranslation(arrTranslation);

    // Die einzelnen Zeilen der Rotationsmatrix berechnen
    // Skript S. 2-40
    CVec4f row3 = ViewDir;
    row3.normalize();

    CVec4f row1 = ViewUp.cross(ViewDir);
    row1.normalize();

    const CVec4f row2 = row3.cross(row1);

    const float arrRotation[4][4] = {
                            { row1(0), row1(1), row1(2), 0 },
                            { row2(0), row2(1), row2(2), 0 },
                            { row3(0), row3(1), row3(2), 0 },
                            { 0,       0,       0,       1 } };

    CMat4f T(arrTranslation);
    CMat4f R(arrRotation);

    return R * T;
}

CMat4f viewSystem::getWorldToViewTransformMatrix()
{
    // AUFGABE01
    //return getViewToWorldTransformMatrix().getTransposed();
    return getInverse(getViewToWorldTransformMatrix());
}

CMat4f viewSystem::RotationMatrix(CVec4f axis, float angle)
{
    CMat4f Rot;

    switch (RotMode) {
    case VIEW_FORMULA_MODE:
        {
            float c = cos(angle);
            float s = sin(angle);

            Rot(0,0) = axis(0)*axis(0)*(1-c) +         c;
            Rot(0,1) = axis(0)*axis(1)*(1-c) - axis(2)*s;
            Rot(0,2) = axis(0)*axis(2)*(1-c) + axis(1)*s;
            Rot(1,0) = axis(1)*axis(0)*(1-c) + axis(2)*s;
            Rot(1,1) = axis(1)*axis(1)*(1-c) +         c;
            Rot(1,2) = axis(1)*axis(2)*(1-c) - axis(0)*s;
            Rot(2,0) = axis(2)*axis(0)*(1-c) - axis(1)*s;
            Rot(2,1) = axis(2)*axis(1)*(1-c) + axis(0)*s;
            Rot(2,2) = axis(2)*axis(2)*(1-c) +         c;

            break;
        }
    case VIEW_MATRIX_MODE:
        {
            float d = sqrt(axis(0)*axis(0)+axis(1)*axis(1));
            float a = axis(0)/d;
            float b = axis(1)/d;

            CMat4f Rot1;
            if (d) {
                Rot1(0,0) = a; Rot1(1,0) = -b;
                Rot1(0,1) = b; Rot1(1,1) =  a;
            }

            CMat4f Rot2;
            Rot2(0,0) =  axis(2); Rot2(2,0) =  d;
            Rot2(0,2) = -d;       Rot2(2,2) =  axis(2);

            CMat4f Rot3;
            Rot3(0,0) =  cos(angle); Rot3(1,0) =  sin(angle);
            Rot3(0,1) = -sin(angle); Rot3(1,1) =  cos(angle);

            CMat4f Rot4(Rot2);
            Rot4(2,0) = Rot2(0,2); Rot4(0,2) = Rot2(2,0);

            CMat4f Rot5(Rot1);
            Rot5(1,0) = Rot1(0,1); Rot5(0,1) = Rot1(1,0);

            Rot = Rot5*Rot4*Rot3*Rot2*Rot1;
            break;
        }
    case VIEW_QUATERNION_MODE:
        std::cerr << "Error in rotation mode: For quaternions no rotation matrix required.\n";
        break;
    }

    return Rot;
}

CMat4f viewSystem::getInverse(CMat4f M)
// Berechnet die Inverse einer 4x4 Matrix M der Art
//             | A T |
//             | 0 1 |
// mit einer orthonormalen 3x3 Matrix A.
// Die Inverse ist dann gegeben durch
//             | A^T -A^t*T |
//             |  0     1   |
{
    float Mat[4][4];
    CVec4f a;
    for (int i=0; i<4; i++) for (int j=0; j<4; j++) Mat[i][j]=M(i,j);
    for (int i=0; i<3; i++) { Mat[i][3]=0; a(i)=M(i,3); }
    for (int i=0; i<3; i++) for (int j=0; j<i; j++) { float x=Mat[i][j]; Mat[i][j]=Mat[j][i]; Mat[j][i]=x; }
    CMat4f tmp(Mat);
    a = tmp*a;
    for (int i=0; i<3; i++) tmp(i,3)=-a(i);

    return tmp;
}
