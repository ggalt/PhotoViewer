#include <QDebug>
#include "myimagelabel.h"

#define MAX_STEP    1000

myImageLabel::myImageLabel(QWidget *parent) : QLabel(parent)
{
    foregroundEffect = Dissolve;
    backgroundEffect = Blur;
}

void myImageLabel::init(QImage *one, QImage *two)
{
    tick = 0;
    originalImage[NEW] = one;
    originalImage[OLD] = two;
    timeLine = new QTimeLine(2000,this);
    timeLine->setFrameRange(0,MAX_STEP);
    connect(timeLine, SIGNAL(frameChanged(int)),
            this, SLOT(animationStep(int)));
    createImages();
    timeLine->start();
}

void myImageLabel::nextImage(QImage *next)
{
//    originalImage[(tick&1)] = originalImage[((tick+1)&1)];
//    originalImage[(tick&1)] = next;
    originalImage[OLD] = originalImage[NEW];
    originalImage[NEW] = next;

//    tick++;

    qDebug() << "myImageLabel->geometry()" << this->geometry();
    qDebug() << "old image size:" << originalImage[OLD]->size();
    createImages();
    timeLine->stop();
    timeLine->start();
}

void myImageLabel::animationStep(int val)
{
    // effectStep should be between 0 -> MAX_STEP
    if( val > MAX_STEP )
        effectStep = MAX_STEP;
    else if( val < 0 )
        effectStep = 0;
    else
        effectStep = val;

//    qDebug () << "Effect step =" << effectStep;
    paintCompositeImage();
}

void myImageLabel::createImages(void)
{
    qDebug () << "Creating images";
    foregroundImage[OLD] = QPixmap::fromImage(
                originalImage[OLD]->scaled(this->size(),Qt::KeepAspectRatio));
    foregroundImage[NEW] = QPixmap::fromImage(
                originalImage[NEW]->scaled(this->size(),Qt::KeepAspectRatio));

    // background images always contain a blur, so blur the base image so we only do it once
    QGraphicsBlurEffect *blur[2];
    blur[OLD] = new QGraphicsBlurEffect();
    blur[NEW] = new QGraphicsBlurEffect();
    blur[OLD]->setBlurRadius(blurValue);
    blur[NEW]->setBlurRadius(blurValue);
    backgroundImage[OLD] = QPixmap::fromImage(
                applyEffectToImage(
                    QPixmap::fromImage( originalImage[OLD]->scaled(
                        this->size(),Qt::KeepAspectRatioByExpanding)),blur[OLD]));
    backgroundImage[NEW] = QPixmap::fromImage(
                applyEffectToImage(
                    QPixmap::fromImage( originalImage[NEW]->scaled(
                        this->size(),Qt::KeepAspectRatioByExpanding)),blur[NEW]));
}

void myImageLabel::paintCompositeImage(void)
{
//    qDebug() << "paintCompositeImage";

    qDebug() << "this->size():" << this->size() << "this->rect():" << this->geometry();
    QImage composite(this->size(), QImage::Format_ARGB32);
    qDebug() << "composite image rectangle is:" << composite.rect();

    composite.fill(Qt::transparent);
    QPainter p(&composite);


    if(backgroundEffect == Blur) {
        // NOTE: Blur effect only uses the first QGraphcisPixmapItem
        QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
        if( effectStep < 127 ) { // blurring out
            blur->setBlurRadius(effectStep * 2);
            p.drawImage(0,0, applyEffectToImage(backgroundImage[OLD],blur));
        } else {            // blurring back in
            blur->setBlurRadius((MAX_STEP - effectStep) * 2);
            p.drawImage(0,0, applyEffectToImage(backgroundImage[NEW],blur));
        }
    }
    else if( backgroundEffect == Dissolve ) {
        QGraphicsOpacityEffect *opacity[2];
        opacity[OLD] = new QGraphicsOpacityEffect();
        opacity[NEW] = new QGraphicsOpacityEffect();
        qreal op = (1/MAX_STEP)*(qreal)effectStep;
        opacity[OLD]->setOpacity((qreal)1.0 - op);
        opacity[NEW]->setOpacity(op);
        p.drawImage(0,0, applyEffectToImage(backgroundImage[OLD],opacity[OLD]));
        p.drawImage(0,0, applyEffectToImage(backgroundImage[NEW],opacity[NEW]));
    } else {
        p.drawPixmap(0,0,backgroundImage[NEW]);
    }


    // we need to capture which image to use for dimensioning the rendering rectangle
    // and in the instance of the dissolve one image might be taller while the other is
    // wider, so we used these two variables to capture the info
    int imageWidth, imageHeight;

    if( foregroundEffect == Blur ) {
        // NOTE: Blur effect only uses the first QGraphcisPixmapItem
        QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
        if( effectStep < 127 ) { // blurring out
            blur->setBlurRadius(effectStep * 2);
            imageWidth = foregroundImage[OLD].width();
            imageHeight = foregroundImage[OLD].height();
            p.drawImage( (composite.width()-imageWidth)/2,
                         (composite.height()-imageHeight)/2,
                         applyEffectToImage(foregroundImage[OLD],blur));
        } else {            // blurring back in
            blur->setBlurRadius((MAX_STEP - effectStep) * 2);
            imageWidth = foregroundImage[NEW].width();
            imageHeight = foregroundImage[NEW].height();
            p.drawImage( (composite.width()-imageWidth)/2,
                         (composite.height()-imageHeight)/2,
                         applyEffectToImage(foregroundImage[NEW],blur));
        }
    }
    else if( foregroundEffect == Dissolve ) {
        QGraphicsOpacityEffect *opacity[2];
        opacity[OLD] = new QGraphicsOpacityEffect();
        opacity[NEW] = new QGraphicsOpacityEffect();
        qreal op = (1/MAX_STEP)*(qreal)effectStep;
        opacity[OLD]->setOpacity((qreal)1.0 - op);
        opacity[NEW]->setOpacity(op);

        p.drawImage((composite.width() - foregroundImage[OLD].width())/2,
                    (composite.height() - foregroundImage[OLD].height())/2,
                    applyEffectToImage(foregroundImage[OLD],opacity[OLD]));
        p.drawImage((composite.width() - foregroundImage[NEW].width())/2,
                    (composite.height() - foregroundImage[NEW].height())/2,
                    applyEffectToImage(foregroundImage[NEW],opacity[NEW]));
    } else {
        p.drawPixmap((composite.width() - foregroundImage[NEW].width())/2,
                  (composite.height() - foregroundImage[NEW].height())/2,
                  foregroundImage[NEW]);
    }

    this->setPixmap(QPixmap::fromImage(composite));
}

QImage myImageLabel::applyEffectToImage(QPixmap src, QGraphicsEffect *effect, int extent)
{
    if(src.isNull())
        return QImage();   //No need to do anything else!

    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(src);
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    QImage res(src.size()+QSize(extent*2, extent*2), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene.render(&ptr, QRectF(), QRectF( -extent, -extent, src.width()+extent*2, src.height()+extent*2 ) );
    return res;
}

void myImageLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    QSize s = event->size();
    qDebug() << "Resized to size:" << s;
    qDebug() << "But we are size:" << this->size();
}
