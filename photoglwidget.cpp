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

/*
void photoGLWidget::DialogOK()
{
    imageInterval = myOptionDialog->getTimeOut();
    imageCrop = myOptionDialog->getImageCrop();
    topDir = myOptionDialog->getTopDir();
    blurValue = myOptionDialog->getBlurValue();
    QDesktopWidget *m = QApplication::desktop();
    whichScreen = m->screenNumber(myOptionDialog);
    this->windowHandle()->setScreen(qApp->screens()[whichScreen]);
    qDebug() << "should be on screen:" << whichScreen;
    qDebug() << "total number of screens:" << m->screenCount();
    myOptionDialog->close();
    disconnect(myOptionDialog,SIGNAL(DialogOK()),
            this,SLOT(DialogOK()));
    disconnect(myOptionDialog,SIGNAL(DialogCancel()),
            this,SLOT(DialogCancel()));
    myOptionDialog->deleteLater();
    qDebug() << topDir.absolutePath();
    if(t != NULL)
        t->setInterval(imageInterval*1000);
    FindImages();
    saveSettings();
}

void photoGLWidget::DialogCancel()
{
    myOptionDialog->close();
    disconnect(myOptionDialog,SIGNAL(DialogOK()),
            this,SLOT(DialogOK()));
    disconnect(myOptionDialog,SIGNAL(DialogCancel()),
            this,SLOT(DialogCancel()));
    myOptionDialog->deleteLater();
}

void photoGLWidget::loadSettingsDialog(void)
{
    myOptionDialog = new OptionsDialog(this);
    connect(myOptionDialog,SIGNAL(DialogOK()),
            this,SLOT(DialogOK()));
    connect(myOptionDialog,SIGNAL(DialogCancel()),
            this,SLOT(DialogCancel()));
    myOptionDialog->setTimeOut(imageInterval);
    myOptionDialog->setImageCount(photoUrlList.count());
    myOptionDialog->setCurrentRandNum(imageItem);
    myOptionDialog->setBlurValue(blurValue);
    myOptionDialog->show();
}

void photoGLWidget::loadSettings(void)
{
    photoUrlList.clear();
    QSettings settings("GaltApps", "PhotoViewer");
    imageInterval = settings.value("Timeout", 10).toInt();
    imageCrop = static_cast<IMAGE>(settings.value("ImageFormat", Cropped).toUInt());
    whichScreen = settings.value("WhichScrren", 0).toInt();
    blurValue = settings.value("BlurValue", 100).toInt();
    qDebug() << imageInterval << imageCrop << whichScreen;

    int size = settings.beginReadArray("PhotoURL");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        photoUrlList.append(settings.value("URL").toString());
    }
    settings.endArray();
}

void photoGLWidget::saveSettings(void)
{
    QSettings settings("GaltApps", "PhotoViewer");
    settings.setValue("Timeout", imageInterval);
    settings.setValue("ImageFormat", static_cast<unsigned int>(imageCrop));
    settings.setValue("WhichScreen", whichScreen);
    settings.setValue("BlurValue", blurValue);
    settings.beginWriteArray("PhotoURL");
    for( int i = 0; i < photoUrlList.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("URL",photoUrlList.at(i));
    }
    settings.endArray();
}

void photoGLWidget::showImage()
{
    qDebug() << "Display State is:" << (int)displayState;
    imageLabel->nextImage(GetImage(qrand() % imageCount));
}

void photoGLWidget::ToggleFullScreen(bool fullScreen)
{
}

void photoGLWidget::setupImageLabels(void)
{
}

void photoGLWidget::FindImages(void)
{
    photoUrlList.clear();
    QDirIterator it(topDir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        if( it.fileInfo().isFile() ) {
            QString entry = it.fileInfo().absoluteFilePath();
            if( entry.contains(".JPG") || entry.contains(".jpg")) {
                photoUrlList.append(entry);
            }
        }
        it.next();
    }
    imageCount = photoUrlList.count();
    imageItem = qrand() % imageCount;

//    DisplayImage(photoUrlList.at(imageItem % photoUrlList.count()));
}

void photoGLWidget::DisplayImage( QString path )
{
}

QImage *photoGLWidget::GetImage(int imageNumber )
{
    QImage *tempImage;
    QImageReader reader;
    imageItem = imageNumber;
    QString path = photoUrlList.at(imageItem);

    printf("DisplayImage called for counter = %d, for image: %s\n", imageItem, path.toLatin1().constData());


#if QT_VERSION < QT_VERSION_CHECK(5, 4, 0)
    reader.autoDetectImageFormat();
#else
    reader.setAutoTransform(true);
#endif
    reader.setFileName(path);
    tempImage = new QImage(reader.read());
    return tempImage;
}

QImage photoGLWidget::applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent = 0)
{
}

void photoGLWidget::FadeOut(QWidget *widget)
{
}

void photoGLWidget::FadeIn(QWidget *widget)
{
}

void photoGLWidget::BlurOut(QWidget *widget)
{
}

void photoGLWidget::BlurIn(QWidget *widget)
{
}

void photoGLWidget::Transition(void)
{
}

void photoGLWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    myChooserDialog = new chooserDialog(this, isFullScreen);
    connect(myChooserDialog,SIGNAL(OpenOptions()),
            this,SLOT(loadSettingsDialog()));

    connect(myChooserDialog,SIGNAL(ExitProgram()),
            this,SLOT(close()));
    connect(myChooserDialog,SIGNAL(SwitchScreen(bool)),
            this,SLOT(ToggleFullScreen(bool)));
    myChooserDialog->show();

    return QWidget::mouseDoubleClickEvent(me);
}

void photoGLWidget::mouseMoveEvent(QMouseEvent *)
{
}

void photoGLWidget::mousePressEvent(QMouseEvent *)
{
}

void photoGLWidget::mouseReleaseEvent(QMouseEvent *)
{
}

  * /
