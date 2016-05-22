#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "points.h"

class GLWidget : public QGLWidget
{
  Q_OBJECT
public:
  GLWidget(QWidget* parent = 0);
  ~GLWidget();

public slots:
  void setIntersection(int state);
  void setSelfIntersection(int state);
  void setEpsilonDraw(double value);
  void setEpsilonIntersection(double value);

protected:
  void paintGL();
  void initializeGL();
  void resizeGL(int width, int height);
  void mouseMoveEvent(QMouseEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseDoubleClickEvent(QMouseEvent* event);

private:
  QPointF transformPosition(QPoint p);
  QList<QPointF> getControllPoints1();
  QList<QPointF> getControllPoints2();
  void calcBezierCurvePolar(QList<QPointF>& controllPoints, const int degree,
                            const double epsilon);
  void intersectBezier(QList<QPointF> bezier1, QList<QPointF> bezier2);

  void plotBezier(QList<QPointF>& points);
  Points points;
  float aspectx, aspecty;
  float epsilon_draw, epsilon_intersection;
  int clickedPoint;
  bool doIntersection, doSelfIntersection;
};

#endif // GLWIDGET_H
