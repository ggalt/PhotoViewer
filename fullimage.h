#ifndef FULLIMAGE_H
#define FULLIMAGE_H

#include <QImage>


class fullImage : public QImage
{
public:
    fullImage(void);
    fullImage(const QImage &image);
    fullImage(const fullImage &image);

//    fullImage &operator=(const fullImage &image);

    float GetAspectRatio(void) { return aspectRatio; }
    bool GetIsWider(void) { return isWider; }
    QImage GetImage(void);

private:
    float aspectRatio;
    bool isWider;
};

#endif // FULLIMAGE_H
