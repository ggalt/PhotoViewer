#include "photoglwidget.h"

//photoGLWidget::photoGLWidget()
//{
//}

void photoGLWidget::LoadImage(QImage image)
{
    dynamic_cast<QImage&>(currentImage[OLD])=dynamic_cast<QImage&>(currentImage[CURRENT]);
    dynamic_cast<QImage&>(currentImage[CURRENT])=image;
}

void photoGLWidget::LoadImage(void)
{
    QImageReader reader;
    QString path = "/home/ggalt/Pictures/Nikon/2015/Hawii and California/DSC_0719-1.JPG";


#if QT_VERSION < QT_VERSION_CHECK(5, 4, 0)
    reader.autoDetectImageFormat();
#else
    reader.setAutoTransform(true);
#endif
    reader.setFileName(path);
    QImage a(reader.read());
    LoadImage(a);
    qDebug() << currentImage[CURRENT].size();
}

void photoGLWidget::initializeGL(void)
{
    // Set up the rendering context, load shaders and other resources, etc.:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    winWidth = this->geometry().width();
    winHeight = this->geometry().height();
}

void photoGLWidget::resizeGL(int w, int h)
{
    winWidth = w;
    winHeight = h;
    aspectRatio = (float)winWidth / (float)winHeight;
    isWider = aspectRatio > 1 ? true : false;
    this->update();
}

void photoGLWidget::paintGL(void)
{
    // Draw the scene:
    QPainter p;
    p.begin(this);
    QRect r = QRect(0,0,winWidth,winHeight);
    qDebug() << r.size();
    QBrush b(Qt::black);
    p.fillRect(r,b);

    QRect scaledR;
    if( aspectRatio > currentImage[CURRENT].GetAspectRatio() ) { // window has a wider aspect ratio than image so just worry about height
        int imageWidth = winHeight*currentImage[CURRENT].GetAspectRatio();
        int imageHeight = winHeight;
        scaledR = QRect((winWidth - imageWidth) / 2,0,imageWidth,imageHeight);
    } else {
        int imageWidth = winWidth;
        int imageHeight = winWidth*currentImage[CURRENT].GetAspectRatio();
        scaledR = QRect(0, (winHeight - imageHeight)/2, imageWidth, imageHeight);
    }

    p.drawImage(scaledR,currentImage[CURRENT]);
    p.end();
}

