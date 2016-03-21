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
    QGraphicsScene scene;
    QGraphicsPixmapItem foreGround[2];
    QGraphicsPixmapItem backGround[2];

    qDebug() << "this->size():" << this->size() << "this->rect():" << this->geometry();
    QImage composite(this->size(), QImage::Format_ARGB32);
    qDebug() << "composite image rectangle is:" << composite.rect();

    composite.fill(Qt::transparent);

//    if(backgroundEffect == Blur) {
//        // NOTE: Blur effect only uses the first QGraphcisPixmapItem
//        QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
//        if( effectStep < 127 ) { // blurring out
//            blur->setBlurRadius(effectStep * 2);
//            backGround[OLD].setPixmap(backgroundImage[OLD]);
//            backGround[OLD].setGraphicsEffect(blur);
//            scene.addItem(&backGround[OLD]);
//        } else {            // blurring back in
//            blur->setBlurRadius((MAX_STEP - effectStep) * 2);
//            backGround[OLD].setPixmap(backgroundImage[NEW]);
//            backGround[OLD].setGraphicsEffect(blur);
//            scene.addItem(&backGround[OLD]);
//        }
//    }
//    else if( backgroundEffect == Dissolve ) {
//        QGraphicsOpacityEffect *opacity[2];
//        opacity[OLD] = new QGraphicsOpacityEffect();
//        opacity[NEW] = new QGraphicsOpacityEffect();
//        qreal op = (1/MAX_STEP)*(qreal)effectStep;
//        opacity[OLD]->setOpacity((qreal)1.0 - op);
//        opacity[NEW]->setOpacity(op);
//        backGround[OLD].setPixmap(backgroundImage[OLD]);
//        backGround[OLD].setGraphicsEffect(opacity[OLD]);
//        backGround[NEW].setPixmap(backgroundImage[NEW]);
//        backGround[NEW].setGraphicsEffect(opacity[NEW]);
//        scene.addItem(&backGround[OLD]);
//        scene.addItem(&backGround[NEW]);
//    } else {
        scene.addPixmap(backgroundImage[NEW]);
//    }

    QPainter p(&composite);
    scene.render(&p, QRectF(), QRectF(0,0,this->width(),this->height()));
    scene.clear();

    // we need to capture which image to use for dimensioning the rendering rectangle
    // and in the instance of the dissolve one image might be taller while the other is
    // wider, so we used these two variables to capture the info
    int imageWidth, imageHeight;

//    if( foregroundEffect == Blur ) {
//        // NOTE: Blur effect only uses the first QGraphcisPixmapItem
//        QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
//        if( effectStep < 127 ) { // blurring out
//            blur->setBlurRadius(effectStep * 2);
//            foreGround[OLD].setPixmap(foregroundImage[OLD]);
//            foreGround[OLD].setGraphicsEffect(blur);
//            scene.addItem(&backGround[OLD]);
//            imageWidth = foregroundImage[OLD].width();
//            imageHeight = foregroundImage[OLD].height();
//        } else {            // blurring back in
//            blur->setBlurRadius((MAX_STEP - effectStep) * 2);
//            foreGround[OLD].setPixmap(foregroundImage[NEW]);
//            foreGround[OLD].setGraphicsEffect(blur);
//            scene.addItem(&backGround[OLD]);
//            imageWidth = foregroundImage[NEW].width();
//            imageHeight = foregroundImage[NEW].height();
//        }
//    }
//    else if( foregroundEffect == Dissolve ) {
//        QGraphicsOpacityEffect *opacity[2];
//        opacity[OLD] = new QGraphicsOpacityEffect();
//        opacity[NEW] = new QGraphicsOpacityEffect();
//        qreal op = (1/MAX_STEP)*(qreal)effectStep;
//        opacity[OLD]->setOpacity((qreal)1.0 - op);
//        opacity[NEW]->setOpacity(op);
//        foreGround[OLD].setPixmap(foregroundImage[OLD]);
//        foreGround[OLD].setGraphicsEffect(opacity[OLD]);
//        foreGround[NEW].setPixmap(foregroundImage[NEW]);
//        foreGround[NEW].setGraphicsEffect(opacity[NEW]);
//        scene.addItem(&foreGround[OLD]);
//        scene.addItem(&foreGround[NEW]);

//        // this little diversion tells us which image is the widest and
//        // which is the tallest so that we know how big of a rectangle
//        // we need to render the composite image
//        if(foregroundImage[OLD].width() > foregroundImage[NEW].width())
//            imageWidth = foregroundImage[OLD].width();
//        else
//            imageWidth = foregroundImage[NEW].width();

//        if(foregroundImage[OLD].height() > foregroundImage[NEW].height())
//            imageHeight = foregroundImage[OLD].height();
//        else
//            imageHeight = foregroundImage[NEW].height();
//    } else {
        scene.addPixmap(foregroundImage[NEW]);
        imageWidth = foregroundImage[NEW].width();
        imageHeight = foregroundImage[NEW].height();
//    }

    scene.render(&p, QRectF((this->width()-imageWidth)/2,
                            (this->height()-imageHeight)/2,
                            imageWidth, imageHeight),
                 QRectF(0,0,this->width(),this->height()));

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
