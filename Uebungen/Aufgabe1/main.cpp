////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
//#include "stdafx.h"

#include "color.h"
#include "vec.h"
#include "mat.h"
#include "quader.h"
#include "viewSystem.h"

#include <iostream>
using namespace std;

// might be you have to switch to
// #include "glut.h" depending on your GLUT installation
//#include "glut.h"

// Include-File für die GLUT-Library
#if defined(HAVE_FREEGLUT)

#ifdef WIN32
#include "../include/GL/freeglut.h"
#else
#include <GL/freeglut.h>
#endif

#else
#include <GL/glut.h>
#endif



////////////////////////////////////////////////////////////
//
// system relevant global variables
//
const int g_iWidth  = 500; // window width  (choose an appropriate size)
const int g_iHeight = 500; // window height (choose an appropriate size)
//
/////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// private, global variables ... replace by your own ones
viewSystem view;
Quader Q1,Q2,Q3;
float relativeStep;
//
/////////////////////////////////////////////////////////////

void init ()
{	// function to initialize our own variables
    view.setData(CVec4f(0, 0, 0, 1), // EyePoint
                 CVec4f(0, 0,-1, 0), // ViewDir
                 CVec4f(0, 1, 0, 0), // ViewUp
                 299);
    view.setMode(VIEW_QUATERNION_MODE); // Hier für AUFGABE01 auf VIEW_MATRIX_MODE setzen.

    Q1.setData(CVec4f( -25, -25, -25, 1),CVec4f(  25,  25,  25, 1));
    Q2.setData(CVec4f(   0,   0,   0, 1),CVec4f( 100, 100, 100, 1));
    Q3.setData(CVec4f(-155, -55, -75, 1),CVec4f( -90,  50,  50, 1));

    relativeStep = 0.1;
}

void initGL ()
{	// function to initialize the view to ortho-projection
    glViewport (0, 0, g_iWidth, g_iHeight);	// Establish viewing area to cover entire window.

    glMatrixMode (GL_PROJECTION);			// Start modifying the projection matrix.
    glLoadIdentity ();						// Reset project matrix.
    glOrtho (-g_iWidth/2, g_iWidth/2, -g_iHeight/2, g_iHeight/2, 0, 1);	// Map abstract coords directly to window coords.

    // tell GL that we draw to the back buffer and
    // swap buffers when image is ready to avoid flickering
    glDrawBuffer (GL_BACK);

    // tell which color to use to clear image
    glClearColor (0,0,0,1);
}

void display (void) {

    glClear (GL_COLOR_BUFFER_BIT);
    //
    ///////

    Q1.draw(view,Color(1,1,1));
    Q2.draw(view,Color(1,1,0));
    Q3.draw(view,Color(0,1,0));

    glFlush ();
    glutSwapBuffers (); // swap front and back buffer
}

void keyboard (unsigned char key, int x, int y)
{
    float  angle=0.05;
    CVec4f Translation;

    CVec4f startPoint(1, 1, 1, 0);
    CVec4f endPoint(0, -1, 0, 0);

    switch (key) {
        case 'f':
            view.Zoom(-5);
            break;
        case 'F':
            view.Zoom(5);
            break;

            // Rotationen des Sichtsystems um die Achsen des Weltkoordinatensystems
        case 'X':
            view.RotateX(angle);
            break;
        case 'x':
            view.RotateX(-angle);
            break;
        case 'Y':
            view.RotateY(angle);
            break;
        case 'y':
            view.RotateY(-angle);
            break;
        case 'Z':
            view.RotateZ(angle);
            break;
        case 'z':
            view.RotateZ(-angle);
            break;

        // Rotationen des Sichtsystems um die Achsen des Sichtkoordinatensystem
        case 'A':
            view.RotateDir(angle);
            break;
        case 'a':
            view.RotateDir(-angle);
            break;
        case 'B':
            view.RotateUp(angle);
            break;
        case 'b':
            view.RotateUp(-angle);
            break;
        case 'C':
            view.RotateHor(angle);
            break;
        case 'c':
            view.RotateHor(-angle);
            break;

        // Verschiebung des Sichtsystems entlang der Achsen des Sichtkoordinatensystem
        case 'U':
            Translation(0) = 3;
            view.Translate(Translation);
            break;
        case 'u':
            Translation(0) = -3;
            view.Translate(Translation);
            break;
        case 'V':
            Translation(1) =  3;
            view.Translate(Translation);
            break;
        case 'v':
            Translation(1) = -3;
            view.Translate(Translation);
            break;
        case 'W':
            Translation(2) = 3;
            view.Translate(Translation);
            break;
        case 'w':
            Translation(2) = -3;
            view.Translate(Translation);
            break;
        case '1':
            if (relativeStep <= 1) {
                std::cout << "LERP with relative step: " << relativeStep << std::endl;

                relativeStep += 0.1;
                view.lerpUp(startPoint, endPoint, relativeStep);
            }
        break;
        case '2':
            if (relativeStep <= 1) {
                std::cout << "SLERP with relative step: " << relativeStep << std::endl;

                relativeStep += 0.1;
                view.slerpUp(startPoint, endPoint, relativeStep);
            }
            break;
        case '3':
            if (relativeStep <= 1) {
                std::cout << "NSLERP with relative step: " << relativeStep << std::endl;

                relativeStep += 0.1;
                view.nslerpUP(startPoint, endPoint, relativeStep);
            }
        break;
        // Reset
        case 'r':
            init();
            break;
        case 'R':
            init();
            break;

        // Quit
        case 'q':
        case 'Q':
            exit (0);
            break;


        default:
            break;
    }

    glutPostRedisplay ();
}


int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow ("Aufgabe 2");

    init  ();	// init my variables first
    initGL();	// init the GL (i.e. view settings, ...)

    // assign callbacks
    glutKeyboardFunc(keyboard);
    glutDisplayFunc (display);

    // start main loop
    glutMainLoop ();

    return 0;
}
