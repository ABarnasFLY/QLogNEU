#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_defaultDir(DEFAULT_DIR),
    m_analizer(0)
{
    ui->setupUi(this);
    m_progresWindow = new ProgressWindow(this);
    m_progresWindow->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    m_progresWindow->hide();
    ui->tabWidget->removeTab(3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

void MainWindow::on_pb_binToLog_convert_clicked()
{
    QBinToLog *converter = new QBinToLog(ui->le_binToLog_open->text(), ui->le_binToLog_save->text());
    connect(converter,SIGNAL(setProgressBar(int)),m_progresWindow, SLOT(setProgresBarMaxValue(int)));
    connect(converter,SIGNAL(refreshProgressBar(int)),m_progresWindow, SLOT(updateProgress(int)));
    m_progresWindow->show();
    this->setDisabled(true);
    ConverterThread *cThread = new ConverterThread();
    cThread->setConverter(converter);
    QEventLoop loop;
    connect(cThread, SIGNAL(finished()),&loop,SLOT(quit()));
    cThread->start();
    loop.exec();
    this->setEnabled(true);
    m_progresWindow->hide();
    QMessageBox *msg = new QMessageBox(this);
    msg->setText("Conversion done!");
    msg->exec();
    delete cThread;
    delete converter;
    delete msg;

}

void MainWindow::on_pb_run_clicked()
{
    if(!m_analizer) m_analizer = new DataAnalysis(QDir(ui->le_pics->text()),ui->le_log->text(),this);
    m_analizer->run();
    showResult();
    ui->table_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_pb_rin_run_clicked()
{
    if(!m_analizer) m_analizer = new DataAnalysis(QDir(ui->le_rin_pics->text()),ui->le_rin_log->text(), ui->le_rin_rinex->text(), ui->le_rin_pos->text(), this);
    m_analizer->run();
    QVector<QStringList> vec = m_analizer->output();
    double error = m_analizer->meanError();
    int outl = m_analizer->outliersCount();
    int debug = m_analizer->debug;
    vec.clear();
}

void MainWindow::showResult()
{
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(ui->tab_Results,"Result");
    if(m_analizer)
    {
        QVector<QStringList> vectorOutput = m_analizer->output();
        QVector<double> vectorLog = m_analizer->delayLog();
        QVector<double> vectorCam = m_analizer->delayPic();
        ui->table_result->setRowCount(vectorOutput.size());
        for(int i = 0; i < ui->table_result->rowCount(); i++)
        {
            ui->table_result->setItem(i,0,new QTableWidgetItem(vectorOutput[i][0]));
            ui->table_result->setItem(i,1, new QTableWidgetItem(vectorOutput[i][4]));
            if(i < vectorCam.size() && i < vectorLog.size())
            {
                ui->table_result->setItem(i,2,new QTableWidgetItem(QString::number(vectorLog[i]/1000,'f',1)));
                ui->table_result->setItem(i,3,new QTableWidgetItem(QString::number(vectorCam[i]/1000,'f',1)));
                ui->table_result->setItem(i,4,new QTableWidgetItem(QString::number(fabs(vectorLog[i] - vectorCam[i])/1000,'f',1)));
            }
           // ui->table_result->setItem(i,5,new QTableWidgetItem(vectorOutput[i][j]));
        }

    }
    else
    {
        ui->table_result->setRowCount(1);
        ui->table_result->setColumnCount(1);
        ui->table_result->horizontalHeader()->resizeSections(QHeaderView::Stretch);
        ui->table_result->setItem(0,0,new QTableWidgetItem("Error occured while generating output!"));

    }
}

void MainWindow::on_pb_reset_clicked()
{
    if(m_analizer) delete m_analizer;
    m_analizer = 0;
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(ui->tab_paths_no_rtk, "Parser");
    ui->tabWidget->addTab(ui->tab_paths_rtk, "Parser RTK");
    ui->tabWidget->addTab(ui->tab_binToLog, "COnvert binary to text log");
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
     ui->table_result->horizontalHeader()->resizeSections(QHeaderView::Stretch);
}
