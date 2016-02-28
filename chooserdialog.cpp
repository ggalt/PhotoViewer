#include "chooserdialog.h"
#include "ui_chooserdialog.h"

chooserDialog::chooserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooserDialog)
{
    ui->setupUi(this);
}

chooserDialog::~chooserDialog()
{
    delete ui;
}

void chooserDialog::on_pushButton_clicked()
{
    emit OpenOptions();
    deleteLater();
    close();
}

void chooserDialog::on_pushButton_2_clicked()
{
    emit ExitProgram();
    deleteLater();
    close();
}
