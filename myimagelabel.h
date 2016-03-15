#ifndef MYIMAGELABEL_H
#define MYIMAGELABEL_H

#include <QObject>
#include <QLabel>

#include <QPainter>
#include <QImage>
#include <QPixmap>

#include <QTimeLine>

class myImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myImageLabel(QObject *parent = 0);
    void init(QImage *one = NULL, QImage *two = NULL);
    void nextImage(QImage *next);

signals:

public slots:
    void animationStep(int val);

private:
    void paintCompositeImage(void);

private:
    QImage *originalImage[2];
    QImage *foregroundImage[2];
    QImage *backgroundImage[2];
    QImage *composite;
    QPainter *p;
    QTimeLine *timeLine;
    int tick;       // read value with (tick & 1) to get the last bit value (0 or 1)
};

#endif // MYIMAGELABEL_H
