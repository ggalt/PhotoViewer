#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QTreeView>
#include <QFileSystemModel>
#include <QDir>
#include <QDebug>
#include <QStandardPaths>

enum IMAGE {
    Cropped = 0,
    Full,
    Fit,
    Sized,
    Actual
};


namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

    QDir getTopDir(void) {return topDir;}
    IMAGE getImageCrop(void) {return imageCrop;}
    int getTimeOut(void) {return timeout;}
    void setTimeOut(int interval);
    void setImageCount( int count );
    void setCurrentRandNum( int num );
    void setBlurValue( int val );
    int getBlurValue(void) { return blurVal; }

signals:
    void DialogOK();
    void DialogCancel();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::OptionsDialog *ui;
    QFileSystemModel *model;
    QDir topDir;
    int timeout;
    IMAGE imageCrop;

    int imageCount;
    int currentRand;
    int blurVal;
};

#endif // OPTIONSDIALOG_H
