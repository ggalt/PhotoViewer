#ifndef MYIMAGELABEL_H
#define MYIMAGELABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>

#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>
#include <QRectF>

#include <QTimeLine>

enum EFFECT{
    NoEffect = 0,
    Dissolve,
    Blur
};

#define NEW 0
#define OLD 1

class myImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myImageLabel(QWidget *parent = 0);
    void init(QImage *one = NULL, QImage *two = NULL);
    void nextImage(QImage *next);
    void setBlurValue(int b) { blurValue = b; }

signals:

public slots:
    void animationStep(int val);

private:
    void paintCompositeImage(void);
    void createImages(void);
    QImage applyEffectToImage(QPixmap src, QGraphicsEffect *effect, int extent = 0);

private:
    QImage *originalImage[2];
    QPixmap foregroundImage[2];
    QPixmap backgroundImage[2];

    EFFECT foregroundEffect;
    EFFECT backgroundEffect;
    int blurValue;          // base blur value of background
    QTimeLine *timeLine;
    int effectStep;
    int tick;       // read value with (tick & 1) to get the last bit value (0 or 1)
};

#endif // MYIMAGELABEL_H
