#include "photoglwidget.h"

//photoGLWidget::photoGLWidget()
//{
//}

void photoGLWidget::LoadImage(QImage *image)
{
//    currentImage = image;
}

void photoGLWidget::LoadImage(void)
{
    QImageReader reader;
    QString path = "C:/Users/ggalt66/Pictures/Hawaii and California/DSC_0719";


#if QT_VERSION < QT_VERSION_CHECK(5, 4, 0)
    reader.autoDetectImageFormat();
#else
    reader.setAutoTransform(true);
#endif
    reader.setFileName(path);
    currentImage = QImage(reader.read());
    qDebug() << currentImage.size();
}

void photoGLWidget::initializeGL(void)
{
    // Set up the rendering context, load shaders and other resources, etc.:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void photoGLWidget::resizeGL(int w, int h)
{
    this->update();
}

void photoGLWidget::paintGL(void)
{
    // Draw the scene:
    QPainter p;
    p.begin(this);
//    p.beginNativePainting();
//    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
//    f->glClear(GL_COLOR_BUFFER_BIT);
//    p.endNativePainting();
    QRect r = this->geometry();
    qDebug() << r.size();
    QBrush b(Qt::black);
//    p.fillRect(r,b);

    p.drawImage(r,currentImage.scaledToWidth(r.width()));
    p.end();
}

