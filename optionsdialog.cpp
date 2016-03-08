#include "optionsdialog.h"
#include "ui_optionsdialog.h"


OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setReadOnly(true);
    model->setFilter(QDir::AllDirs);

    ui->dirTreeView->setModel(model);
    QModelIndex idx = model->index(QStandardPaths::standardLocations( QStandardPaths::PicturesLocation)[0]);

    ui->dirTreeView->expand(idx);
    ui->dirTreeView->scrollTo(idx);
    ui->dirTreeView->setCurrentIndex(idx);
    ui->dirTreeView->resizeColumnToContents(0);

    ui->comboBox->setCurrentIndex(0);
    ui->spinBox->setValue(30);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::setTimeOut(int interval)
{
    timeout = interval;
    ui->spinBox->setValue(timeout);
}

void OptionsDialog::setBlurValue( int val )
{
    blurVal = val;
    ui->blurSpinBox->setValue(blurVal);
}

void OptionsDialog::on_buttonBox_accepted()
{
    timeout = ui->spinBox->value();
    imageCrop = static_cast<IMAGE>(ui->comboBox->currentIndex());
    blurVal = ui->blurSpinBox->value();

    QModelIndex index = ui->dirTreeView->currentIndex();

    if( index.isValid() && model->fileInfo(index).isDir() )
        topDir = model->filePath(index);
    else
        topDir = QDir("");
    emit DialogOK();
}

void OptionsDialog::on_buttonBox_rejected()
{
    emit DialogCancel();
}

void OptionsDialog::setImageCount( int count )
{
    ui->pictureCount->setText(QString::number(count));
}

void OptionsDialog::setCurrentRandNum( int num )
{
    ui->randNum->setText(QString::number(num));
}
