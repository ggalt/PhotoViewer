#include "myimagelabel.h"

myImageLabel::myImageLabel(QWidget *parent) : QLabel(parent)
{

}

void myImageLabel::init(QImage *one, QImage *two)
{
    tick = 0;
    originalImage[NEW] = one;
    originalImage[OLD] = two;
    timeLine = new QTimeLine(2000,this);
    timeLine->setFrameRange(0,254);
    connect(timeLine, SIGNAL(frameChanged(int)),
            this, SLOT(animationStep(int)));
    createImages();
}

void myImageLabel::nextImage(QImage *next)
{
//    originalImage[(tick&1)] = originalImage[((tick+1)&1)];
//    originalImage[(tick&1)] = next;
    originalImage[OLD] = originalImage[NEW];
    originalImage[NEW] = next;

//    tick++;

    createImages();
}

void myImageLabel::animationStep(int val)
{
    // effectStep should be between 0 -> 254
    if( val > 254 )
        effectStep = 254;
    else if( val < 0 )
        effectStep = 0;
    else
        effectStep = val;
}

void myImageLabel::createImages(void)
{
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
    QGraphicsScene scene;
    QGraphicsPixmapItem foreGround[2];
    QGraphicsPixmapItem backGround[2];

    QImage composite(this->size(), QImage::Format_ARGB32);
    composite.fill(Qt::transparent);

    if(backgroundEffect == Blur) {
        // NOTE: Blur effect only uses the first QGraphcisPixmapItem
        QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
        if( effectStep < 127 ) { // blurring out
            blur->setBlurRadius(effectStep * 2);
            backGround[OLD].setPixmap(backgroundImage[OLD]);
            backGround[OLD].setGraphicsEffect(blur);
            scene.addItem(&backGround[OLD]);
        } else {            // blurring back in
            blur->setBlurRadius((254 - effectStep) * 2);
            backGround[OLD].setPixmap(backgroundImage[NEW]);
            backGround[OLD].setGraphicsEffect(blur);
            scene.addItem(&backGround[OLD]);
        }
    }
    else if( backgroundEffect == Dissolve ) {
        QGraphicsOpacityEffect *opacity[2];
        opacity[OLD] = new QGraphicsOpacityEffect();
        opacity[NEW] = new QGraphicsOpacityEffect();
        qreal op = (1/254)*(qreal)effectStep;
        opacity[OLD]->setOpacity((qreal)1.0 - op);
        opacity[NEW]->setOpacity(op);
        backGround[OLD].setPixmap(backgroundImage[OLD]);
        backGround[OLD].setGraphicsEffect(opacity[OLD]);
        backGround[NEW].setPixmap(backgroundImage[NEW],blur);
        backGround[NEW].setGraphicsEffect(opacity[NEW]);
        scene.addItem(&backGround[OLD]);
        scene.addItem(&backGround[NEW]);
    } else {
        scene.addPixmap(backgroundImage[NEW]);
    }

    if( foregroundEffect == Blur ) {

    }
    else if( foregroundEffect == Dissolve ) {

    } else {

    }

    QPainter p(&composite);
    scene.render(&p, QRectF(), QRectF(0,0,this->width(),this->height()));
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

