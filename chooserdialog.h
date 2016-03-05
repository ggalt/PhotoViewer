#ifndef CHOOSERDIALOG_H
#define CHOOSERDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class chooserDialog;
}

class chooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit chooserDialog(QWidget *parent = 0, bool isFullScreen=false);
    ~chooserDialog();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void SetFullScreen(bool val);

signals:
    void OpenOptions(void);
    void ExitProgram(void);
    void SwitchScreen(bool setFullScreen);

private:
    Ui::chooserDialog *ui;
    bool fullScreen;
};

#endif // CHOOSERDIALOG_H
