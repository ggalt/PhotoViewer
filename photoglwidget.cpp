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
    qDebug() << "Width:" << winWidth << "Height:" << winHeight;
    this->update();
}

void photoGLWidget::paintGL(void)
{
    // Draw the scene:
    QPainter p;
    p.begin(this);
    QRect r = QRect(0,0,winWidth,winHeight);
//    qDebug() << "R.SIZE =" << r.size();
//    qDebug() << "R.rec =" << r;
    QBrush b(Qt::black);
    p.fillRect(r,b);

//    qDebug() << "Image size =" << currentImage[CURRENT].size();

    QRect scaledR = r;

//    qDebug() << "Scaled R = r:" << scaledR;

    float imageAspectRatio = GetAspectRatio( currentImage[CURRENT] );

    if( aspectRatio > imageAspectRatio ) { // window has a wider aspect ratio than image so just worry about height
        qDebug() <<"Windows is wider than image";
        int imageWidth = winHeight * imageAspectRatio;
        int imageHeight = winHeight;

//        qDebug() << "image aspect ratio =" << imageAspectRatio;
//        qDebug() << "ImageWidth" << imageWidth;
//        qDebug() << "winWidth" << winWidth;
//        qDebug() << "ImageHeight" << imageHeight;
//        qDebug() << "winHeight" << winHeight;

        scaledR = QRect((winWidth - imageWidth) / 2, 0,
                        imageWidth,imageHeight);
    } else {
        qDebug() <<"Windows is narrower than image";
        int imageWidth = winWidth;
        int imageHeight = winWidth * imageAspectRatio;
//        qDebug() << "image aspect ratio =" << imageAspectRatio;
//        qDebug() << "ImageWidth" << imageWidth;
//        qDebug() << "winWidth" << winWidth;
//        qDebug() << "ImageHeight" << imageHeight;
//        qDebug() << "winHeight" << winHeight;
        scaledR = QRect(0, (winHeight - imageHeight)/2,
                        imageWidth, imageHeight);
    }
    p.drawImage(scaledR,currentImage[CURRENT].scaled(winWidth,winHeight,Qt::KeepAspectRatio));
    p.end();
}

float photoGLWidget::GetAspectRatio(QImage &i)
{
    return (float)i.width()/(float)i.width();
}

