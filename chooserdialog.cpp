#include "chooserdialog.h"
#include "ui_chooserdialog.h"

chooserDialog::chooserDialog(QWidget *parent, bool isFullScreen) :
    QDialog(parent),
    ui(new Ui::chooserDialog)
{
    ui->setupUi(this);
    SetFullScreen(isFullScreen);
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

void chooserDialog::on_pushButton_3_clicked()
{
    SetFullScreen(!fullScreen);
    emit SwitchScreen(fullScreen);
    deleteLater();
    close();
}


void chooserDialog::SetFullScreen(bool val)
{
    fullScreen = val;
    if(fullScreen)
        ui->pushButton_3->setText("Windowed");
    else
        ui->pushButton_3->setText("Full Screen");
}
