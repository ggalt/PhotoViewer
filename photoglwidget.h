#ifndef PHOTOGLWIDGET_H
#define PHOTOGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QApplication>
#include <QDesktopWidget>

#include <QPaintEvent>
#include <QImage>
#include <QImageReader>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QBrush>
#include <QColor>
#include <QTimeLine>

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

#include <cstdlib>      // for rand and srand

#include "optionsdialog.h"
#include "chooserdialog.h"
#include "myimagelabel.h"

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




//enum EFFECT{
//    NoEffect = 0,
//    Dissolve,
//    Blur
//};

#define CURRENT 0
#define OLD 1

class photoGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    photoGLWidget(QWidget *parent=NULL) : QOpenGLWidget(parent){}

    void LoadImage(QImage image);
    void LoadImage(void);

signals:
    void TransitionOneOver(void);
    void TransitionTwoOver(void);

private slots:
    void DialogOK();
    void DialogCancel();
    void loadSettingsDialog(void);
    void showImage();
    void ToggleFullScreen(bool fullScreen);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
//    void paintEvent(QPaintEvent *p);

private:
    float GetAspectRatio(QImage &i);
    void setupImageLabels(void);
    void loadSettings(void);
    void saveSettings(void);
    void FindImages(void);
    void DisplayImage( QString path );
    QImage *GetImage(int imageNumber );
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

    int blurValue;          // base blur value of background
    QTimeLine *timeLine;
    int effectStep;
    QImage currentImage[2];
    QPixmap forgroundImage[2];
    QPixmap backgroundImage[2];

    float winWidth;
    float winHeight;
    bool isWider;
    float aspectRatio;
};

#endif // PHOTOGLWIDGET_H
