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
#include <QOpenGLBuffer>
#include <QOpenGLWidget>
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

    void on_imageLabel_customContextMenuRequested(const QPoint &pos);

private:
    void loadSettings();
    void saveSettings();
    void FindImages(void);
    void DisplayImage( QString path );

private:
    Ui::MainWindow *ui;
    OptionsDialog *dialog;

    QOpenGLBuffer *glBuf;
    QOpenGLWidget *glWidget;

    QString m_sSettingsFile;
    int screenHeight;
    int screenWidth;

    QStringList photoUrlList;
    int timeout;
    IMAGE imageCrop;
    int percentage;
    QDir topDir;
};

#endif // MAINWINDOW_H
