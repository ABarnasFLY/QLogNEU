#include "progresswindow.h"
#include "ui_progresswindow.h"

ProgressWindow::ProgressWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressWindow)
{
    ui->setupUi(this);
    m_updateTimeLeftTimer = new QTimer(this);
    m_timeLeftTimer = new QTimer(this);
    m_updateTimeLeftTimer->setInterval(5000);
    m_timeLeftTimer->setInterval(1000);
    connect(m_updateTimeLeftTimer,SIGNAL(timeout()),this,SLOT(updateTimeLeft()));
    connect(m_timeLeftTimer,SIGNAL(timeout()),this,SLOT(TimeLeft()));
}

ProgressWindow::~ProgressWindow()
{
    delete ui;
}

void ProgressWindow::updateTimeLeft()
{
    m_ticks++;
    double maxV = ui->progressBar->maximum();
    double curV = ui->progressBar->value();
    double left = maxV - curV;
    if(left / maxV > 0.01)
    {
        QTime t(0,0);
        double timeFromStart = 5000 * m_ticks;
        m_timeLeft = t.addMSecs(timeFromStart /curV * left);
        ui->timeLeft->setText("Time left: " + m_timeLeft.toString("mm:ss"));
    }
}

void ProgressWindow::TimeLeft()
{
    m_timeLeft = m_timeLeft.addSecs(-1);
    ui->timeLeft->setText("Time left: " + m_timeLeft.toString("mm:ss"));
}

void ProgressWindow::updateProgress(int value)
{
    ui->progressBar->setValue(value);
}

void ProgressWindow::setProgresBarMaxValue(int value)
{
    ui->progressBar->setMaximum(value);
    m_updateTimeLeftTimer->start();
    m_timeLeftTimer->start();
    m_ticks = 0;
}

void ProgressWindow::showMessage(QString value)
{
    ui->label->setText(value);
}
