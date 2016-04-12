#include "mainwindow.h"
#include "photoglwidget.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //    MainWindow w;
    //    w.show();
    photoGLWidget *widget = new photoGLWidget();
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
//    format.setVersion(3, 2);
//    format.setProfile(QSurfaceFormat::CoreProfile);
    widget->setFormat(format); // must be called before the widget or its parent window gets shown
    widget->LoadImageW();
    widget->show();
    return a.exec();
}
