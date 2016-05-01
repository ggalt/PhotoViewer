#include "fullimage.h"

fullImage::fullImage(void) : QImage()
{

}

fullImage::fullImage(const QImage &image) : QImage(image)
{
    aspectRatio = (float)image.width() / (float)image.height();
    isWider = aspectRatio > 1 ? true : false;
}

fullImage::fullImage(const fullImage &image) : QImage((QImage)image)
{
    aspectRatio = (float)image.width() / (float)image.height();
    isWider = aspectRatio > 1 ? true : false;
}

//fullImage &fullImage::operator=(const fullImage &image)
//{
//    (QImage)this = QImage((QImage)image);
//    aspectRatio = (float)image.width() / (float)image.height();
//    isWider = aspectRatio > 1 ? true : false;
//}
