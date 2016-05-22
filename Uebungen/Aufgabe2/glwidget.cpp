
#include "glwidget.h"
#include <QtGui>
#include <GL/glut.h>

#include "mainwindow.h"

#include "bezier_calc.h"

GLWidget::GLWidget(QWidget* parent)
  : QGLWidget(parent)
{
  doIntersection = false;
  doSelfIntersection = false;
  epsilon_draw = (float)0.05;
  epsilon_intersection = (float)0.000005;

  // Hier Punkte hinzufügen (schönere Startpositionen!)
  points.addPoint(-1.00, -0.5);
  points.addPoint(-0.75, -0.5);
  points.addPoint(-0.50, -0.5);
  points.addPoint(-0.25, -0.5);
  points.addPoint(0.00, -0.5);

  points.addPoint(0.25, 0.5);
  points.addPoint(0.50, 0.5);
  points.addPoint(0.75, 0.5);
  points.addPoint(1.00, 0.5);
}

GLWidget::~GLWidget()
{
}

void
GLWidget::paintGL()
{
  // clear
  glClear(GL_COLOR_BUFFER_BIT);

  // Koordinatensystem
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_LINES);
  glVertex2f(-1.0, 0.0);
  glVertex2f(1.0, 0.0);
  glVertex2f(0.0, -1.0);
  glVertex2f(0.0, 1.0);
  glEnd();
  glColor3f(1.0, 0.0, 0.0);

  // Punkte
  glPointSize(7.0);
  glBegin(GL_POINTS);
  for (int i = 0; i < points.getCount(); i++) {
    glVertex2f(points.getPointX(i), points.getPointY(i));
  }

  glEnd();

  // Hüllpolygone zeichnen
  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < 5; i++) {
    glVertex2f(points.getPointX(i), points.getPointY(i));
  }
  glEnd();
  glBegin(GL_LINE_STRIP);
  for (int i = 5; i < points.getCount(); i++) {
    glVertex2f(points.getPointX(i), points.getPointY(i));
  }
  glEnd();

  // Kurve
  glColor3f(1.0, 1.0, 1.0);
  // AUFGABE: Hier Kurve zeichnen
  // dabei epsilon_draw benutzen

  const QList<QPointF>  controllPoints2 = getControllPoints2();
  const int degree = controllPoints2.count() - 1;
  calcBezierCurvePolar(controllPoints2, degree, epsilon_draw);

  QList<QPointF> curve1 =
    BezierCalc::calcBezierCurveSimple(getControllPoints1(), epsilon_draw);

  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_LINE_STRIP);

  for (QPointF point : curve1) {
    glVertex2f(point.x(), point.y());
  }
  glEnd();

  // Schnittpunkte zeichnen
  if (doIntersection) {
    glColor3f(0.0, 1.0, 0.0);
    // AUFGABE: Hier Schnitte zeichnen
    // dabei epsilon_intersection benutzen
  }
  if (doSelfIntersection) {
    glColor3f(1.0, 0.0, 1.0);
    // AUFGABE: Hier Selbstschnitte zeichnen
    // dabei epsilon_intersection benutzen
  }
}

void
GLWidget::initializeGL()
{
  resizeGL(width(), height());
}

void
GLWidget::resizeGL(int width, int height)
{
  aspectx = 1.0;
  aspecty = 1.0;
  if (width > height) {
    aspectx = float(width) / height;
  } else {
    aspecty = float(height) / width;
  }
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // gluOrtho2D(-aspectx,aspectx,-aspecty,aspecty);
  // gluOrtho2D sets up a two-dimensional orthographic viewing region.
  // This is equivalent to calling glOrtho with near = -1 and far = 1 .
  glOrtho(-aspectx, aspectx, -aspecty, aspecty, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

QPointF
GLWidget::transformPosition(QPoint p)
{
  return QPointF((2.0 * p.x() / width() - 1.0) * aspectx,
                 -(2.0 * p.y() / height() - 1.0) * aspecty);
}

QList<QPointF>
GLWidget::getControllPoints1()
{
  QList<QPointF> controllPoints1;

  for (int i = 0; i < 5; ++i) {
    controllPoints1.append(points.getPoint(i));
  }

  return controllPoints1;
}

QList<QPointF>
GLWidget::getControllPoints2()
{
  QList<QPointF> controllPoints2;

  for (int i = 5; i < points.getCount(); ++i) {
    controllPoints2.append(points.getPoint(i));
  }

  return controllPoints2;
}

void
GLWidget::plotBezier(const QList<QPointF>& _points)
{
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_LINE_STRIP);

  for (const QPointF& point : _points) {
    glVertex2f(point.x(), point.y());
  }

  glEnd();
}

void
GLWidget::mouseMoveEvent(QMouseEvent* event)
{
  if (event->buttons() & Qt::LeftButton) {
    QPoint pos = event->pos();
    QPointF posF = transformPosition(pos);
    points.setPointX(clickedPoint, posF.x());
    points.setPointY(clickedPoint, posF.y());
    update();
  }
}

void
GLWidget::mousePressEvent(QMouseEvent* event)
{
  if (event->buttons() & Qt::LeftButton) {
    QPoint pos = event->pos();
    QPointF posF = transformPosition(pos);
    clickedPoint = points.getClosestPoint(posF.x(), posF.y());
    points.setPointX(clickedPoint, posF.x());
    points.setPointY(clickedPoint, posF.y());
    update();
  }
}

void
GLWidget::mouseDoubleClickEvent(QMouseEvent*)
{
}

void
GLWidget::calcBezierCurvePolar(const QList<QPointF>& controllPoints,
                               const int degree, const double epsilon)
{
  const double t = 0.5;
  const QPointF maxDist = BezierCalc::getMaxForwardDistance(controllPoints);

  if (degree == 0 || (maxDist.x() < epsilon && maxDist.y() < epsilon)) {
    plotBezier(controllPoints);
  } else {
    const QList<QPointF> curvePoints = BezierCalc::deCasteljauPolarForm(controllPoints, t);
    QList<QPointF> leftHalf;
    QList<QPointF> rightHalf;

    BezierCalc::splitIntoHalf(curvePoints, leftHalf, rightHalf);

    calcBezierCurvePolar(leftHalf, degree - 1, epsilon);
    calcBezierCurvePolar(rightHalf, degree - 1, epsilon);
  }
}

void
GLWidget::setIntersection(int state)
{
  doIntersection = (state == Qt::Checked);
  update();
}

void
GLWidget::setSelfIntersection(int state)
{
  doSelfIntersection = (state == Qt::Checked);
  update();
}

void
GLWidget::setEpsilonDraw(double value)
{
  epsilon_draw = value;
}

void
GLWidget::setEpsilonIntersection(double value)
{
  epsilon_intersection = value;
}
