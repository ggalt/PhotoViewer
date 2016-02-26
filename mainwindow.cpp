#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();
    if( photoUrlList.size() <= 0 ) {
        // need to get photos
        loadSettingsDialog();
    }
    this->showFullScreen();
    screenHeight = this->height();
    screenWidth = this->width();
    ui->imageLabel->setGeometry(0,0,screenWidth,screenHeight);
    QDesktopWidget *m = QApplication::desktop();
    qDebug() << "screen count is:" << m->screenCount();
    qDebug() << "this screen is:" << m->screenNumber(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettingsDialog()
{
    dialog = new OptionsDialog(this);
    connect(dialog,SIGNAL(DialogOK()),
            this,SLOT(DialogOK()));
    connect(dialog,SIGNAL(DialogCancel()),
            this,SLOT(DialogCancel()));
    dialog->show();
//    QFileDialog dialog;
//    dialog.setFileMode(QFileDialog::Directory);
//    dialog.setOption(QFileDialog::DontUseNativeDialog,false);
//    dialog.setOption(QFileDialog::DontResolveSymlinks);
//    dialog.setNameFilterDetailsVisible(true);
//    dialog.setViewMode(QFileDialog::Detail);

//    int res = dialog.exec();
//    QDir directory;

//    if (res) {
//        directory = dialog.selectedFiles().at(0);
//        qDebug() << directory.absolutePath();
//    }
}

void MainWindow::loadSettings()
{
    photoUrlList.clear();
    QSettings settings("GaltApps", "PhotoViewer");
    timeout = settings.value("TimeOut", 10000).toInt();
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
    settings.setValue("Timeout", timeout);
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
    timeout = dialog->getTimeOut();
    imageCrop = dialog->getImageCrop();
    topDir = dialog->getTopDir();
    QDesktopWidget *m = QApplication::desktop();
    int whichScreen = m->screenNumber(dialog);
    this->windowHandle()->setScreen(qApp->screens()[whichScreen]);
    dialog->close();
    disconnect(dialog,SIGNAL(DialogOK()),
            this,SLOT(DialogOK()));
    disconnect(dialog,SIGNAL(DialogCancel()),
            this,SLOT(DialogCancel()));
    dialog->deleteLater();
    qDebug() << topDir.absolutePath();
    FindImages();
//    saveSettings();
}

void MainWindow::DialogCancel()
{
    dialog->close();
    disconnect(dialog,SIGNAL(DialogOK()),
            this,SLOT(DialogOK()));
    disconnect(dialog,SIGNAL(DialogCancel()),
            this,SLOT(DialogCancel()));
    dialog->deleteLater();
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
    DisplayImage(photoUrlList.at(0));
}

void MainWindow::DisplayImage( QString path )
{
    QImage image;
    QImageReader reader;

    reader.setAutoTransform(true);
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

    if(image.height() > image.width()) {
        image.scaledToWidth(screenWidth);
    } else {
        image.scaledToHeight(screenHeight);
    }

    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_imageLabel_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = this->mapToGlobal(pos);
    qDebug() << "context menu request";

    QMenu myMenu;

    QAction *setupDialog;
    QAction *exitProgram;

    setupDialog = new QAction("Launch Setup &Dialog", this);
    connect(setupDialog, SIGNAL(triggered(bool)),this,SLOT(loadSettingsDialog()));

    exitProgram = new QAction("Close Program", this);
    connect(exitProgram, SIGNAL(triggered(bool)), this, SLOT(close()));
    myMenu.addAction(setupDialog);
    myMenu.addSeparator();
    myMenu.addAction(exitProgram);

    QAction* selectedItem = myMenu.exec(globalPos);
}
