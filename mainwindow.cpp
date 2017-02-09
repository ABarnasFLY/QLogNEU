#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_defaultDir(DEFAULT_DIR)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::on_pushButton_clicked()
{
    //ParseMachine parser;
    //parser.run();
    //ui->label->setText(QString::number(parser.getPicDone()));
    DataAnalysis analizer(QDir("D:/KL/1"), "D:/KL/120.log");
    analizer.run();
    ui->label->setText("DONE");
    QVector<double> crosscorr = analizer.analizedCrossCorelation();
    QString po = "[ ";
    for(int i = 0; i < crosscorr.length(); i++)
    {
        if (i != 0) po += ',';
        po += QString::number(crosscorr[i], 'f', 2);
    }
    po += ']';
    ui->textEdit->setText(po);
}*/

void MainWindow::on_pb_browse_log_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,QString("Open UAV log file"),m_defaultDir,tr("*.log"));
    ui->le_log->setText(path);
    m_defaultDir = QFileInfo(path).absolutePath();
}

void MainWindow::on_pb_browse_pic_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,QString("Open photo directory"),m_defaultDir);
    ui->le_pics->setText(path);
    m_defaultDir = QFileInfo(path).absolutePath();
}

void MainWindow::on_pb_rin_rinex_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,QString("Open rinex observations from RTK rover"),m_defaultDir,tr("*.obs"));
    ui->le_rin_rinex->setText(path);
    m_defaultDir = QFileInfo(path).absolutePath();
}



void MainWindow::on_pb_rin_pos_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,QString("Open RTK postoprocessing file"),m_defaultDir,tr("*.pos"));
    ui->le_rin_pos->setText(path);
    m_defaultDir = QFileInfo(path).absolutePath();
}

void MainWindow::on_pb_rin_log_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,QString("Open UAV log file"),m_defaultDir,tr("*.log"));
    ui->le_rin_log->setText(path);
    m_defaultDir = QFileInfo(path).absolutePath();
}


void MainWindow::on_pb_rin_pics_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,QString("Open photo directory"),m_defaultDir);
    ui->le_rin_pics->setText(path);
    m_defaultDir = QFileInfo(path).absoluteFilePath();
}

void MainWindow::on_pb_binToLog_open_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,QString("Open binary log file"),m_defaultDir,tr("*.bin *.BIN"));
    ui->le_binToLog_open->setText(path);
    m_defaultDir = QFileInfo(path).absolutePath();
    path.chop(3);
    ui->le_binToLog_save->setText(path + "log");
}

void MainWindow::on_pb_binToLog_save_clicked()
{
    QString path = QFileDialog::getSaveFileName(this,QString("Chose path to save text log file"),m_defaultDir,tr("*.log"));
    ui->le_binToLog_save->setText(path);
    m_defaultDir = QFileInfo(path).absolutePath();
}
