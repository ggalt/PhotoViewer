#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QImage>
#include <QImageReader>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QWindow>
#include <QMenu>
#include <QPoint>
#include <QAction>

#include "optionsdialog.h"
#include "chooserdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void DialogOK();
    void DialogCancel();
    void loadSettingsDialog();
    void showImage();

    void on_imageLabel_customContextMenuRequested(const QPoint &pos);

private:
    void loadSettings();
    void saveSettings();
    void FindImages(void);
    void DisplayImage( QString path );

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

    QTimer *t;

    int imageItem;
    int imageCount;
};

#endif // MAINWINDOW_H
