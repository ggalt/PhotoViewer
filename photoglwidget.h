#ifndef PHOTOGLWIDGET_H
#define PHOTOGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QImage>
#include <QImageReader>
#include <QBrush>
#include <QColor>
#include <QString>
#include <QTimeLine>
#include <QDebug>

#include "fullimage.h"


//enum EFFECT{
//    NoEffect = 0,
//    Dissolve,
//    Blur
//};

#define CURRENT 0
#define OLD 1

class photoGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    photoGLWidget(QWidget *parent=NULL) : QOpenGLWidget(parent){}

    void LoadImage(QImage image);
    void LoadImage(void);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
//    void paintEvent(QPaintEvent *p);

private:
    int blurValue;          // base blur value of background
    QTimeLine *timeLine;
    int effectStep;
    fullImage currentImage[2];
    QPixmap forgroundImage[2];
    QPixmap backgroundImage[2];

    int winWidth;
    int winHeight;
    bool isWider;
    float aspectRatio;
};

#endif // PHOTOGLWIDGET_H
