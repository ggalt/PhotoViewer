#ifndef PHOTOGLWIDGET_H
#define PHOTOGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QImage>
#include <QImageReader>
#include <QBrush>
#include <QColor>
#include <QString>
#include <QTimeLine>
#include <QDebug>

class photoGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    photoGLWidget(QWidget *parent=NULL) : QOpenGLWidget(parent){}

    void LoadImage(QImage *image);
    void LoadImage(void);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    int blurValue;          // base blur value of background
    QTimeLine *timeLine;
    int effectStep;
    QImage currentImage;
};

#endif // PHOTOGLWIDGET_H
