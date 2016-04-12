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

    CMat4f matRotation = R * T;

    return matRotation;
}

CMat4f viewSystem::getWorldToViewTransformMatrix()
{
    return getViewToWorldTransformMatrix().getTransposed();
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
