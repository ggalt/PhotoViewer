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

void OptionsDialog::setTimeOut(int interval)
{
    timeout = interval;
    ui->spinBox->setValue(timeout);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_buttonBox_accepted()
{
    timeout = ui->spinBox->value();
    imageCrop = static_cast<IMAGE>(ui->comboBox->currentIndex());

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
