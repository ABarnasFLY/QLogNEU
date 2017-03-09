#include "progresswindow.h"
#include "ui_progresswindow.h"

ProgressWindow::ProgressWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressWindow)
{
    ui->setupUi(this);
}

ProgressWindow::~ProgressWindow()
{
    delete ui;
}

void ProgressWindow::updateProgress(int value)
{
    ui->progressBar->setValue(value);
}

void ProgressWindow::setProgresBarMaxValue(int value)
{
    ui->progressBar->setMaximum(value);
}

void ProgressWindow::showMessage(QString value)
{
    ui->label->setText(value);
}
