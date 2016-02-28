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
    this->showFullScreen();
    screenHeight = this->height();
    screenWidth = this->width();
    ui->imageLabel->setGeometry(0,0,screenWidth,screenHeight);
//    QDesktopWidget *m = QApplication::desktop();
//    qDebug() << "screen count is:" << m->screenCount();
//    qDebug() << "this screen is:" << m->screenNumber(this);

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

    t = new QTimer(this);

    t->setInterval(imageInterval*1000);
    connect(t,SIGNAL(timeout()),this,SLOT(showImage()));
    t->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettingsDialog()
{
    myOptionDialog = new OptionsDialog(this);
    connect(myOptionDialog,SIGNAL(DialogOK()),
            this,SLOT(DialogOK()));
    connect(myOptionDialog,SIGNAL(DialogCancel()),
            this,SLOT(DialogCancel()));
    myOptionDialog->show();
}

void MainWindow::loadSettings()
{
    photoUrlList.clear();
    QSettings settings("GaltApps", "PhotoViewer");
    imageInterval = settings.value("TimeOut", 10).toInt();
    imageCrop = static_cast<IMAGE>(settings.value("ImageFormat", Cropped).toUInt());
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
    QDesktopWidget *m = QApplication::desktop();
    int whichScreen = m->screenNumber(myOptionDialog);
    this->windowHandle()->setScreen(qApp->screens()[whichScreen]);
    myOptionDialog->close();
    disconnect(myOptionDialog,SIGNAL(DialogOK()),
            this,SLOT(DialogOK()));
    disconnect(myOptionDialog,SIGNAL(DialogCancel()),
            this,SLOT(DialogCancel()));
    myOptionDialog->deleteLater();
    qDebug() << topDir.absolutePath();
    FindImages();
//    saveSettings();
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
//                qDebug() << entry;
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
    DisplayImage(photoUrlList.at(qrand() % imageCount));
}

void MainWindow::DisplayImage( QString path )
{
    QImage image;
    QImageReader reader;

#ifdef QT_VERSION < 5.4
    reader.autoDetectImageFormat();
#elif
    reader.setAutoTransform(true);
#endif
    reader.setFileName(path);
    image = reader.read();

//    if (image.isNull()) {
//        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
//                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
//        setWindowFilePath(QString());
//        imageLabel->setPixmap(QPixmap());
//        imageLabel->adjustSize();
//        return false;
//    }

    qDebug() << "w:" << image.width() << "h" << image.height();
    if(image.height() > image.width()) {
        qDebug() << "Taller";
        image.scaledToWidth(screenWidth);
    } else {
        qDebug() << "Wider";
        image.scaledToHeight(screenHeight);
    }
    qDebug() << "w:" << image.width() << "h" << image.height();

    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
}


/*
 *     QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(1.0);
    ui->centralwidget->setGraphicsEffect(opacityEffect);
    QPropertyAnimation * anim = new QPropertyAnimation(this);
    anim->setTargetObject(opacityEffect);
    anim->setPropertyName("opacity");
    anim->setDuration(4000);
    anim->setStartValue(opacityEffect->opacity());
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->start(QAbstractAnimation::KeepWhenStopped);
    */



void MainWindow::on_imageLabel_customContextMenuRequested(const QPoint &pos)
{
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *me)
{
    myChooserDialog = new chooserDialog(this);
    connect(myChooserDialog,SIGNAL(OpenOptions()),
            this,SLOT(loadSettingsDialog()));

    connect(myChooserDialog,SIGNAL(ExitProgram()),
            this,SLOT(close()));
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
