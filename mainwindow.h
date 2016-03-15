#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QWindow>
#include <QLabel>

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

#include <cstdlib>      // for rand and srand

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
    void loadSettingsDialog(void);
    void showImage();
    void ToggleFullScreen(bool fullScreen);

private:
    void setupImageLabels(void);
    void loadSettings(void);
    void saveSettings(void);
    void FindImages(void);
    void DisplayImage( QString path );
    QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent = 0);
    void FadeOut(QWidget *widget);
    void FadeIn(QWidget *widget);
    void BlurOut(QWidget *widget);
    void BlurIn(QWidget *widget);
    void Transition(void);

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

    QLabel *imageLabel[2];
    QLabel *blurLabel[2];

    int counter;
};

#endif // MAINWINDOW_H
