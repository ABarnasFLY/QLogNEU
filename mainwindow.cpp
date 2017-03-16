#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_defaultDir(DEFAULT_DIR),
    m_analizer(0),
    m_aThread(0),
    m_cThread(0),
    m_eThread(0)
{
    ui->setupUi(this);
    m_progresWindow = new ProgressWindow(this);
    m_progresWindow->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    m_progresWindow->hide();
    ui->tabWidget->removeTab(3);
    ui->tabWidget->removeTab(3);
    ui->table_result->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->pb_restore->setToolTip("If your fileset modification dates are corupted\n(e.g. you archive them and upack) you could restore\n them if you created backup before");
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

void MainWindow::on_pb_restore_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,QString("Open photo backup"),m_defaultDir,tr("*.pbu"));
    ui->le_pics->setText(path);
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


void MainWindow::on_pb_rin_restore_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,QString("Open photo backup"),m_defaultDir,tr("*.pbu"));
    ui->le_rin_pics->setText(path);
    m_defaultDir = QFileInfo(path).absolutePath();
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
    m_cThread = new ConverterThread();
    if(QFileInfo(ui->le_binToLog_open->text()).exists())
    {
        QBinToLog *converter = new QBinToLog(ui->le_binToLog_open->text(), ui->le_binToLog_save->text());
        connect(converter,SIGNAL(setProgressBar(int)),m_progresWindow, SLOT(setProgresBarMaxValue(int)));
        connect(converter,SIGNAL(refreshProgressBar(int)),m_progresWindow, SLOT(updateProgress(int)));
        m_progresWindow->show();
        this->setDisabled(true);
        m_cThread->setConverter(converter);
        QEventLoop loop;
        connect(m_cThread, SIGNAL(finished()),&loop,SLOT(quit()));
        m_cThread->start();
        loop.exec();
        this->setEnabled(true);
        m_progresWindow->hide();
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("Conversion done!");
        msg->exec();
        delete converter;
        delete msg;
    }
    else
    {
        QMessageBox::warning(this,"Wrong files","Selected files or paths doesn't exists");
    }
    delete m_cThread;
    m_cThread = 0;
}

void MainWindow::on_pb_run_clicked()
{
    m_aThread = new AnalizerThread();
    if(QFileInfo(ui->le_log->text()).exists())
    {
        if(QDir(ui->le_pics->text()).exists())
        {
            m_photoProcessed = ui->le_pics->text();
            if(!m_analizer) m_analizer = new DataAnalysis(QDir(ui->le_pics->text()),ui->le_log->text(),this);
            connect(m_analizer,SIGNAL(setProgressBar(int)),m_progresWindow,SLOT(setProgresBarMaxValue(int)));
            connect(m_analizer,SIGNAL(updateProgressBar(int)),m_progresWindow,SLOT(updateProgress(int)));
            connect(m_analizer,SIGNAL(updateStatus(QString)),m_progresWindow,SLOT(showMessage(QString)));

            m_aThread->setAnalizer(m_analizer);
            m_progresWindow->show();
            this->setDisabled(true);

            QEventLoop loop;
            connect(m_aThread,SIGNAL(finished()),&loop,SLOT(quit()));
            m_aThread->start(); //m_analizer->run();
            loop.exec();
            this->setEnabled(true);
            m_progresWindow->hide();
            showDone();
            ui->table_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
        else if(ui->le_pics->text().endsWith(".pbu"))
        {
            if(!m_analizer) m_analizer = new DataAnalysis(ui->le_pics->text(),ui->le_log->text(),this);
            connect(m_analizer,SIGNAL(setProgressBar(int)),m_progresWindow,SLOT(setProgresBarMaxValue(int)));
            connect(m_analizer,SIGNAL(updateProgressBar(int)),m_progresWindow,SLOT(updateProgress(int)));
            connect(m_analizer,SIGNAL(updateStatus(QString)),m_progresWindow,SLOT(showMessage(QString)));

            m_aThread->setAnalizer(m_analizer);
            m_progresWindow->show();
            this->setDisabled(true);

            QEventLoop loop;
            connect(m_aThread,SIGNAL(finished()),&loop,SLOT(quit()));
            m_aThread->start(); //m_analizer->run();
            loop.exec();
            this->setEnabled(true);
            m_progresWindow->hide();
            showDone();
            ui->table_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->pb_exif->setDisabled(true);
            ui->pb_photoBackup->setDisabled(true);
        }
        else
        {
            QMessageBox::warning(this,"Wrong picture dir","Selected files or paths don't exist");
        }
    }
    else QMessageBox::warning(this,"Wrong log","Selected files or paths don't exist");

    delete m_aThread;
    m_aThread = 0;
}

void MainWindow::on_pb_rin_run_clicked()
{
    m_aThread = new AnalizerThread();
    if(QFileInfo(ui->le_rin_log->text()).exists() && QFileInfo(ui->le_rin_pos->text()).exists())
    {
        if(QDir(ui->le_rin_pics->text()).exists())
        {
            m_progresWindow->show();
            m_progresWindow->showMessage("Processing...");
            m_photoProcessed = ui->le_rin_pics->text();
            if(!m_analizer) m_analizer = new DataAnalysis(QDir(ui->le_rin_pics->text()),ui->le_rin_log->text(), ui->le_rin_pos->text(), this);
            connect(m_analizer,SIGNAL(setProgressBar(int)),m_progresWindow,SLOT(setProgresBarMaxValue(int)));
            connect(m_analizer,SIGNAL(updateProgressBar(int)),m_progresWindow,SLOT(updateProgress(int)));
            connect(m_analizer,SIGNAL(updateStatus(QString)),m_progresWindow,SLOT(showMessage(QString)));
            m_aThread->setAnalizer(m_analizer);
            QEventLoop loop;
            connect(m_aThread,SIGNAL(finished()),&loop,SLOT(quit()));
            m_aThread->start();
            loop.exec();
            this->setEnabled(true);
            m_progresWindow->hide();
            showDone();
            ui->table_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
        else if(ui->le_rin_pics->text().endsWith(".pbu"))
        {
            if(!m_analizer) m_analizer = new DataAnalysis(ui->le_rin_pics->text(),ui->le_rin_log->text(), ui->le_rin_pos->text(), this);
            connect(m_analizer,SIGNAL(setProgressBar(int)),m_progresWindow,SLOT(setProgresBarMaxValue(int)));
            connect(m_analizer,SIGNAL(updateProgressBar(int)),m_progresWindow,SLOT(updateProgress(int)));
            connect(m_analizer,SIGNAL(updateStatus(QString)),m_progresWindow,SLOT(showMessage(QString)));

            m_aThread->setAnalizer(m_analizer);
            m_progresWindow->show();
            this->setDisabled(true);
            QEventLoop loop;
            connect(m_aThread,SIGNAL(finished()),&loop,SLOT(quit()));
            m_aThread->start();
            loop.exec();
            this->setEnabled(true);
            m_progresWindow->hide();
            showDone();
            ui->table_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->pb_exif->setDisabled(true);
            ui->pb_photoBackup->setDisabled(true);
        }
        else
        {
            QMessageBox::warning(this,"Wrong picture dir","Selected files or paths don't exist");
        }
    }
    else QMessageBox::warning(this,"Wrong log","Selected files or paths don't exist");

    delete m_aThread;
    m_aThread = 0;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(m_aThread)
    {
        delete m_aThread;
    }
    if(m_cThread)
    {
        delete m_cThread;
    }
    if(m_eThread)
    {
        delete m_eThread;
    }

}

void MainWindow::refreshResult()
{
    ui->label_outCount->setText(QString::number(m_analizer->outliersCount()));
    ui->label_meanValue->setText(QString::number(m_analizer->meanError()));
    QVector<QStringList> vectorOutput = m_analizer->output();
    QVector<double> vectorLog = m_analizer->delayLog();
    QVector<double> vectorCam = m_analizer->delayPic();
    m_skipCam.clear();
    m_skipPic.clear();
    ui->table_result->clearContents();
    ui->table_result->setRowCount(vectorOutput.size());
    for(int i = 0; i < ui->table_result->rowCount(); i++)
    {
        ui->table_result->setItem(i,0,new QTableWidgetItem(vectorOutput[i][0]));
        ui->table_result->setItem(i,1, new QTableWidgetItem(vectorOutput[i][8]));
        if(i < vectorCam.size() && i < vectorLog.size())
        {
            ui->table_result->setItem(i,2,new QTableWidgetItem(QString::number(vectorLog[i]/1000,'f',1)));
            ui->table_result->setItem(i,3,new QTableWidgetItem(QString::number(vectorCam[i]/1000,'f',1)));
            ui->table_result->setItem(i,4,new QTableWidgetItem(QString::number(fabs(vectorLog[i] - vectorCam[i])/1000,'f',1)));
            ui->table_result->setItem(i,5,new QTableWidgetItem(QString("skip")));
            ui->table_result->setItem(i,6,new QTableWidgetItem(QString("skip")));
        }
    }
}

void MainWindow::showResult()
{
    ui->label_outCount->setText(QString::number(m_analizer->outliersCount()));
    ui->label_meanValue->setText(QString::number(m_analizer->meanError()));
    ui->tabWidget->removeTab(0);
    //ui->tabWidget->removeTab(0);
    //ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(ui->tab_Results,"Result");
    m_skipCam.clear();
    m_skipPic.clear();
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
                ui->table_result->setItem(i,5,new QTableWidgetItem(QString("skip")));
                ui->table_result->setItem(i,6,new QTableWidgetItem(QString("skip")));
            }
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

void MainWindow::showDone()
{
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(ui->tab_done,"Done");
    if(m_analizer->meanError() > 1 || m_analizer->outliersCount() > 10) ui->label_info->setText("Georeferencing finished, sets don't fit. Try manual edition");
    else ui->label_info->setText("Georeferencing successfull");
    ui->label_done_mean_val->setText(QString::number(m_analizer->meanError()));
    ui->label_done_out_val->setText(QString::number(m_analizer->outliersCount()));
}

void MainWindow::on_pb_reset_clicked()
{
    if(m_analizer) delete m_analizer;
    m_analizer = 0;
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(ui->tab_paths_no_rtk, "Parser");
    ui->tabWidget->addTab(ui->tab_paths_rtk, "Parser RTK");
    ui->tabWidget->addTab(ui->tab_binToLog, "Bin to log converter");
    ui->pb_exif->setDisabled(false);
    ui->pb_photoBackup->setDisabled(false);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->table_result->horizontalHeader()->resizeSections(QHeaderView::Stretch);
}

void MainWindow::on_pb_confirmExclusions_clicked()
{
    showDone();
}

void MainWindow::on_table_result_cellClicked(int row, int column)
{
    if(column == 5)
    {
        m_analizer->skipPic(row);
        refreshResult();
    }
    if(column == 6)
    {
        m_analizer->skipCam(row);
        refreshResult();
    }
}



void MainWindow::on_pb_undo_clicked()
{
    m_analizer->undo();
    refreshResult();
}

void MainWindow::on_pb_manualEdit_clicked()
{
    showResult();
}

void MainWindow::on_pb_newSession_clicked()
{
    if(m_analizer) delete m_analizer;
    m_analizer = 0;
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(ui->tab_paths_no_rtk, "Parser");
    ui->tabWidget->addTab(ui->tab_paths_rtk, "Parser RTK");
    ui->tabWidget->addTab(ui->tab_binToLog, "COnvert binary to text log");
}

void MainWindow::on_pb_raport_clicked()
{
    m_analizer->printToFile(QFileDialog::getSaveFileName(this,"Save raport",m_defaultDir,tr("*.txt")));
}

void MainWindow::on_pb_exif_clicked()
{
    m_analizer->printToFile("tmpRaport.txt");
    m_eThread = new ExifThread();
    ExifWriter exif("tmpRaport.txt",m_photoProcessed,QFileDialog::getExistingDirectory(this,"Copy directory", m_defaultDir));
    m_progresWindow->show();
    connect(&exif,SIGNAL(updateProgress(int)),m_progresWindow,SLOT(updateProgress(int)));
    connect(&exif,SIGNAL(sendMessage(QString)),m_progresWindow,SLOT(showMessage(QString)));
    m_progresWindow->setProgresBarMaxValue(m_analizer->output().size());
    m_eThread->setWriter(&exif);
    QEventLoop loop;
    connect(m_eThread,SIGNAL(finished()),&loop,SLOT(quit()));
    m_progresWindow->show();
    this->setDisabled(true);
    m_eThread->start();
    loop.exec();
    m_progresWindow->hide();
    this->setDisabled(false);
    delete m_eThread;
    m_eThread = 0;
}

void MainWindow::on_pb_photoBackup_clicked()
{
    timeUtils::CreatePictureBackupFile(QDir(m_photoProcessed),0,QFileDialog::getSaveFileName(this,"Create backup of your photo directory", m_defaultDir,tr("*.pbu")));
}
