#ifndef PHOTOGLWIDGET_H
#define PHOTOGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class photoGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    photoGLWidget(QWidget *parent) : QOpenGLWidget(parent){}

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // PHOTOGLWIDGET_H
