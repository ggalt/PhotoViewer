#ifndef CHOOSERDIALOG_H
#define CHOOSERDIALOG_H

#include <QDialog>

namespace Ui {
class chooserDialog;
}

class chooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit chooserDialog(QWidget *parent = 0);
    ~chooserDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void OpenOptions(void);
    void ExitProgram(void);

private:
    Ui::chooserDialog *ui;
};

#endif // CHOOSERDIALOG_H
