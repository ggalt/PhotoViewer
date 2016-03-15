#include "myimagelabel.h"

myImageLabel::myImageLabel(QObject *parent) : QLabel(parent)
{

}

void myImageLabel::init(QImage *one = NULL, QImage *two = NULL)
{
    tick = 0;
    originalImage[0] = one;
    originalImage[1] = two;
}

void myImageLabel::nextImage(QImage *next)
{

}

void myImageLabel::animationStep(int val)
{

}

void myImageLabel::paintCompositeImage(void)
{

}
