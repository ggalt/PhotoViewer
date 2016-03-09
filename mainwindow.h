#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QWindow>

#include <QString>
#include <QStringList>

#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QDirIterator>

#include <QDebug>

#include <QMenu>
#include <QPoint>
#include <QAction>
#include <QSize>

#include <QImage>
#include <QImageReader>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include "optionsdialog.h"
#include "chooserdialog.h"

namespace Ui {
class MainWindow;
}

enum DISPLAYSTATE {
    NotShowing = 0,
    StartTransitionOne,
    TransitionOneEnded,
    StartTransitionTwo,
    TransitionTwoEnded,
    Displaying
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void TransitionOneOver(void);
    void TransitionTwoOver(void);

private slots:
    void DialogOK();
    void DialogCancel();
    void loadSettingsDialog();
    void showImage();
    void ToggleFullScreen(bool fullScreen);

private:
    void loadSettings();
    void saveSettings();
    void FindImages(void);
    void DisplayImage( QString path );
    QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent = 0);
    void FadeOut(QWidget *widget);
    void FadeIn(QWidget *widget);
    void BlurOut(QWidget *widget);
    void BlurIn(QWidget *widget);

    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;
    OptionsDialog *myOptionDialog;
    chooserDialog *myChooserDialog;

    QString m_sSettingsFile;
    int screenHeight;
    int screenWidth;

    QStringList photoUrlList;
    int imageInterval;
    IMAGE imageCrop;
    int percentage;
    QDir topDir;
    int whichScreen;
    int blurValue;

    QTimer *t;

    int imageItem;
    int imageCount;
    DISPLAYSTATE displayState;

    bool isFullScreen;
};

#endif // MAINWINDOW_H
