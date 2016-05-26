
#include "glwidget.h"
#include <QtGui>
#include <GL/glut.h>

#include <algorithm>

#include "mainwindow.h"

#include "bezier_calc.h"

GLWidget::GLWidget(QWidget* parent)
  : QGLWidget(parent)
{
  doCalcCn1 = true;
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

  QList<QPointF> controllPoints2 = getControllPoints2();

  // glColor3f(1.0, 1.0, 0.0);
  // glBegin(GL_LINE_STRIP);
  QList<QPointF> curve2 =
    BezierCalc::calcBezierCurvePolar(controllPoints2, epsilon_draw);

  // glEnd();

  // QList<QPointF> curve2 =
  // BezierCalc::calcBezierCurveSimple(getControllPoints2(), epsilon_draw);

  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_LINE_STRIP);

  for (QPointF point : curve2) {
    glVertex2f(point.x(), point.y());
  }
  glEnd();

  QList<QPointF> controllPoints1 = getControllPoints1();
  QList<QPointF> curve1 =
    BezierCalc::calcBezierCurvePolar(controllPoints1, epsilon_draw);

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
    intersectBezier(controllPoints1, controllPoints2);
  }
  if (doSelfIntersection) {
    glColor3f(1.0, 0.0, 1.0);
    // AUFGABE: Hier Selbstschnitte zeichnen
    // dabei epsilon_intersection benutzen
    selfIntersect(controllPoints1);
    selfIntersect(controllPoints2);
  }
  if (doCalcCn1) {
    calcCn1(controllPoints2);
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
GLWidget::plotBezier(QList<QPointF>& _points)
{
  for (const QPointF& point : _points) {
    glVertex2f(point.x(), point.y());
  }
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

  if (event->buttons() & Qt::RightButton) {
    const QPoint pos = event->pos();
    const QPointF posF = transformPosition(pos);
    if (doCalcCn1) {
      clickedPoint = points.getClosestPoint(posF.x(), posF.y());
      cn1Point = QPointF(posF.x(), posF.y());
      update();
    }
  }
}

void
GLWidget::mouseDoubleClickEvent(QMouseEvent*)
{
}

void
GLWidget::selfIntersect(QList<QPointF>& controllPoints)
{
  const double totalAngle = BezierCalc::getTotalAngle(controllPoints);
  const bool isSelfIntersection = totalAngle > 180;

  if (isSelfIntersection) {
    QList<QList<QPointF>> segmentsNoSelfIntersection;

    // Zerteile solange kein Selbstschnitt gemessen wird
    BezierCalc::computeSelfIntersectionFreeSegments(controllPoints,
                                                    segmentsNoSelfIntersection);

    // Teste ein Segment auf Schnitt mit jedem anderen Segment
    for (int i = 0; i < segmentsNoSelfIntersection.size() - 1; ++i) {
      for (int j = i + 1; j < segmentsNoSelfIntersection.size(); ++j) {
        intersectBezier(segmentsNoSelfIntersection[i],
                        segmentsNoSelfIntersection[j]);
      }
    }
  }
}

void
GLWidget::intersectBezier(QList<QPointF> bezier1, QList<QPointF> bezier2)
{
  if (BezierCalc::boundingBoxesIntersects(bezier1, bezier2)) {
    const double t = 0.5;

    int m = bezier1.count();
    int n = bezier2.count();

    const double maxDistB1 = BezierCalc::getMaxForwardDistance(bezier1);
    const double maxDistB2 = BezierCalc::getMaxForwardDistance(bezier2);

    if ((m * (m - 1) * maxDistB1) > epsilon_intersection) {

      const QList<QPointF> curvePoints =
        BezierCalc::deCasteljauPolarForm(bezier1, t);

      QList<QPointF> leftHalf, rightHalf;

      BezierCalc::splitIntoHalf(curvePoints, leftHalf, rightHalf);
      // BezierCalc::splitIntoHalf(bezier1, leftHalf, rightHalf);

      intersectBezier(leftHalf, bezier2);
      intersectBezier(rightHalf, bezier2);
    } else if (((n * (n - 1) * maxDistB2) > epsilon_intersection)) {

      const QList<QPointF> curvePoints =
        BezierCalc::deCasteljauPolarForm(bezier2, t);

      QList<QPointF> leftHalf, rightHalf;

      BezierCalc::splitIntoHalf(curvePoints, leftHalf, rightHalf);
      // BezierCalc::splitIntoHalf(bezier2, leftHalf, rightHalf);

      intersectBezier(bezier1, leftHalf);
      intersectBezier(bezier1, rightHalf);
    } else {
      const QLineF line1 = QLineF(bezier1[0], bezier1[bezier1.count() - 1]);
      const QLineF line2 = QLineF(bezier2[0], bezier2[bezier2.count() - 1]);

      QPointF intersection;
      const QLineF::IntersectType intersectioType =
        line1.intersect(line2, &intersection);

      if (intersectioType == QLineF::BoundedIntersection) {
        glPointSize(7.0);
        glBegin(GL_POINTS);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(intersection.x(), intersection.y());
        glEnd();
      }
    }
  }
}

void
GLWidget::calcCn1(const QList<QPointF> controllPoints)
{
  if (!cn1Point.isNull()) {
    glColor3f(1.0, 1.0, 0.0);
    QList<QPointF> cn1_points =
      BezierCalc::computeCn1(controllPoints, cn1Point);

    QList<QPointF> bezierCurve =
      BezierCalc::calcBezierCurvePolar(cn1_points, epsilon_draw);

    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);

    for (const QPointF point : bezierCurve) {
      glVertex2f(point.x(), point.y());
    }
    glEnd();

    // draw new control point
    glColor3f(0.5, 0.0, 0.0);
    glPointSize(7.0);
    glBegin(GL_POINTS);
    glVertex2f(cn1Point.x(), cn1Point.y());
    glEnd();

    // draw new control polygon
    glColor3f(0.0, 0.5, 0.5);

    // draw new control polygon
    glBegin(GL_LINE_STRIP);

    for (const QPointF point : cn1_points) {
      glVertex2f(point.x(), point.y());
    }
    glEnd();
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
  update();
}

void
GLWidget::setEpsilonIntersection(double value)
{
  epsilon_intersection = value;
  update();
}
