#include <QTime>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->imageLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    whichScreen = 1;
    isFullScreen = false;
    blurValue = 300;
    displayState = NotShowing;

    ui->imageLabel->setGeometry(0,0,screenWidth,screenHeight);

    // Seed the random generator with current time
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    loadSettings();
    if( photoUrlList.size() <= 0 ) {
        // need to get photos
        loadSettingsDialog();
    } else {
        imageCount = photoUrlList.count();
        imageItem = qrand() % imageCount;
    }

//    this->windowHandle()->setScreen(qApp->screens()[whichScreen]);
    screenHeight = this->height();
    screenWidth = this->width();

    t = new QTimer(this);

    t->setInterval(imageInterval*1000);
    connect(t,SIGNAL(timeout()),this,SLOT(showImage()));
    t->start();
    showImage();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::ToggleFullScreen(bool fullScreen)
{
    isFullScreen = fullScreen;
    if(isFullScreen) {
        this->showFullScreen();
    } else {
        this->showNormal();
    }
    screenHeight = this->height();
    screenWidth = this->width();
}

void MainWindow::loadSettingsDialog()
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

void MainWindow::loadSettings()
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

void MainWindow::saveSettings()
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


void MainWindow::DialogOK()
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

void MainWindow::DialogCancel()
{
    myOptionDialog->close();
    disconnect(myOptionDialog,SIGNAL(DialogOK()),
            this,SLOT(DialogOK()));
    disconnect(myOptionDialog,SIGNAL(DialogCancel()),
            this,SLOT(DialogCancel()));
    myOptionDialog->deleteLater();
}

void MainWindow::FindImages(void)
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

    DisplayImage(photoUrlList.at(imageItem % photoUrlList.count()));
}

void MainWindow::showImage()
{
    qDebug() << "Display State is:" << (int)displayState;
    switch(displayState) {
        case StartTransitionOne:
            FadeOut(ui->imageLabel);
//            BlurOut(ui->imageLabel);
        break;

        case TransitionOneEnded:
            imageItem = qrand();
            DisplayImage(photoUrlList.at(imageItem % imageCount));
        break;

        case StartTransitionTwo:
        break;

        case TransitionTwoEnded:
        break;

        case Displaying:
            displayState = StartTransitionOne;
            showImage();
        break;

        case NotShowing:
            if(!photoUrlList.isEmpty()) {
                displayState = StartTransitionOne;
                showImage();
            }
        default:
        break;

    }
}

void MainWindow::DisplayImage( QString path )
{
    QImage image;
    QImage blurImage;
    QImage finalImage;
    QImageReader reader;

#if QT_VERSION < QT_VERSION_CHECK(5, 4, 0)
    reader.autoDetectImageFormat();
#else
    reader.setAutoTransform(true);
#endif
    reader.setFileName(path);
    image = reader.read();

    ui->imageLabel->setGeometry(0,0,ui->centralWidget->width(),ui->centralWidget->height());
    ui->blurImageLabel->setGeometry(0,0,ui->centralWidget->width(),ui->centralWidget->height());

    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    qDebug() << "blur value is" << blurValue;
    blur->setBlurRadius(blurValue);

    if(image.height() > image.width()) {
        qDebug() << "Taller";
        finalImage = image.scaled(ui->imageLabel->width(),ui->imageLabel->height(),Qt::KeepAspectRatio);

    } else {
        qDebug() << "Wider";
        finalImage = image.scaled(ui->imageLabel->width(), ui->imageLabel->height(), Qt::KeepAspectRatio);
    }
    ui->imageLabel->setGeometry((ui->centralWidget->width() -  finalImage.width())/2,
                                (ui->centralWidget->height() - finalImage.height())/2,
                                finalImage.width(),finalImage.height());

    // make sure we fully cover the background with the blurred image
    if(image.width() < ui->blurImageLabel->width()) {
        blurImage = applyEffectToImage(image.scaled(ui->imageLabel->width(), ui->imageLabel->height(), Qt::KeepAspectRatioByExpanding),blur);
    } else {
        blurImage = applyEffectToImage(image,blur);
    }

    ui->blurImageLabel->setGeometry((ui->centralWidget->width()-blurImage.width())/2,
                                    (ui->centralWidget->height()-blurImage.height())/2,
                                    blurImage.width(),blurImage.height());
    ui->blurImageLabel->setPixmap(QPixmap::fromImage(blurImage));
    ui->imageLabel->setPixmap(QPixmap::fromImage(finalImage));
//    BlurIn(ui->imageLabel);
    FadeIn(ui->imageLabel);
    qDebug() << path;
    qDebug() << "Image #" << imageItem;
}

QImage MainWindow::applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent)
{
    if(src.isNull()) return QImage();   //No need to do anything else!
    if(!effect) return src;             //No need to do anything else!
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    QImage res(src.size()+QSize(extent*2, extent*2), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene.render(&ptr, QRectF(), QRectF( -extent, -extent, src.width()+extent*2, src.height()+extent*2 ) );
    return res;
}

void MainWindow::FadeOut(QWidget *widget)
{
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(1.0);
    widget->setGraphicsEffect(opacityEffect);
    QPropertyAnimation * anim = new QPropertyAnimation(this);
    anim->setTargetObject(opacityEffect);
    anim->setPropertyName("opacity");
    anim->setDuration(1000);
    anim->setStartValue(opacityEffect->opacity());
    anim->setEndValue(0.0);
    anim->setEasingCurve(QEasingCurve::Linear);
//    anim->start(QAbstractAnimation::KeepWhenStopped);
    displayState=TransitionOneEnded;
    connect(anim,SIGNAL(finished()),this,SLOT(showImage()));
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::FadeIn(QWidget *widget)
{
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.0);
    widget->setGraphicsEffect(opacityEffect);
    QPropertyAnimation * anim = new QPropertyAnimation(this);
    anim->setTargetObject(opacityEffect);
    anim->setPropertyName("opacity");
    anim->setDuration(1000);
    anim->setStartValue(opacityEffect->opacity());
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::Linear);
//    anim->start(QAbstractAnimation::KeepWhenStopped);
    displayState = StartTransitionOne;
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::BlurOut(QWidget *widget)
{
    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(0);
    widget->setGraphicsEffect(blur);
    QPropertyAnimation * anim = new QPropertyAnimation(this);
    anim->setTargetObject(blur);
    anim->setPropertyName("blur");
    anim->setDuration(1000);
    anim->setStartValue(blur->blurRadius());
    anim->setEndValue(1000);
    anim->setEasingCurve(QEasingCurve::Linear);
//    anim->start(QAbstractAnimation::KeepWhenStopped);
    displayState=TransitionOneEnded;
    connect(anim,SIGNAL(finished()),this,SLOT(showImage()));
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::BlurIn(QWidget *widget)
{
    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(1000);
    widget->setGraphicsEffect(blur);
    QPropertyAnimation * anim = new QPropertyAnimation(this);
    anim->setTargetObject(blur);
    anim->setPropertyName("blur");
    anim->setDuration(1000);
    anim->setStartValue(blur->blurRadius());
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::Linear);
//    anim->start(QAbstractAnimation::KeepWhenStopped);
    displayState = StartTransitionOne;
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *me)
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

void MainWindow::mouseMoveEvent(QMouseEvent *me)
{
    return QWidget::mouseMoveEvent(me);
}

void MainWindow::mousePressEvent(QMouseEvent *me)
{
    return QWidget::mousePressEvent(me);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *me)
{
    return QWidget::mouseReleaseEvent(me);
}
