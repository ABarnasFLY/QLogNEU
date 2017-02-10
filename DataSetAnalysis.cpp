#include "DataSetAnalysis.h"

DataAnalysis::DataAnalysis(QString pics, QString logPath, QObject* parent):
    QObject(parent),
    m_parser(logPath, this),
    m_offset_fs(0),
    m_offset_lg(0)
{
    createFileSet(pics);
}

DataAnalysis::DataAnalysis(QDir pics, QString logPath, QObject* parent):
    QObject(parent),
    m_parser(logPath, this),
    m_offset_fs(0),
    m_offset_lg(0)
{
    createFileSet(pics);
}

void DataAnalysis::run()
{
    m_parser.run();
    m_vectCamLog = m_parser.getVectCamLog();
    fillDelays();
    alglib::corrr1d(m_cDelayLog,m_cDelayLog.length(),m_cDelayPic,m_cDelayPic.length(),m_crossCorr);
    correlationAnalysis();
    print();
}

void DataAnalysis::fillDelays()
{
    m_cDelayLog.setlength(m_vectCamLog.length()-1);
    m_cDelayPic.setlength(m_fileSet.size()-1);
    QVector<CamLog_t>::iterator itVectCamLog = m_vectCamLog.begin();
    QVector<CamLog_t>::iterator itVectCamLog_next = m_vectCamLog.begin() +1;
    fileSet_t::iterator itFileSet = m_fileSet.begin();
    fileSet_t::iterator itFileSet_next = m_fileSet.begin() +1;
    for(int i = 0; i < m_vectCamLog.size() - 1 && i <m_fileSet.size() - 1; i++)
    {
        double delayLog = (double) itVectCamLog_next->time - (double) itVectCamLog->time;
        double delayPic = itFileSet_next.key() - itFileSet.key();
        m_cDelayLog[i] = delayLog;
        m_cDelayPic[i] = delayPic;
        m_delayLog.push_back(delayLog);
        m_delayPic.push_back(delayPic);
        itVectCamLog++;
        itVectCamLog_next++;
        itFileSet++;
        itFileSet_next++;
    }
}

void DataAnalysis::correlationAnalysis()
{
    for(int i = 0; i < m_crossCorr.length(); i++)
    {
        if (abs(m_crossCorr.length()/2 - i) > 0.2 * m_crossCorr.length()) m_analizedCrossCorelation.push_back(m_crossCorr[i] / (abs(m_crossCorr.length()/2 - i)));
    }
}


void DataAnalysis::print()
{
    m_output.clear();
    QVector<CamLog_t>::iterator camLog_iterator= m_vectCamLog.begin();
    fileSet_t::iterator fileset_iterator = m_fileSet.begin();
    QVector<double>::iterator delayCam_iterator = m_delayPic.begin();
    QVector<double>::iterator delayLog_iterator = m_delayLog.begin();
    delayCam_iterator += m_offset_fs;
    delayLog_iterator += m_offset_lg;
    fileset_iterator  += m_offset_fs;
    camLog_iterator   += m_offset_lg;
    int offset = camLog_iterator->time - fileset_iterator.key();
    for(; camLog_iterator != m_vectCamLog.end() && fileset_iterator != m_fileSet.end(); camLog_iterator++, fileset_iterator++ )
    {
        QStringList line;
        line.push_back(fileset_iterator.value()); //filename
        line.push_back(QString::number(camLog_iterator->lat)); // lat
        line.push_back(QString::number(camLog_iterator->lon)); // lon
        line.push_back(QString::number(camLog_iterator->alt)); // alt
        QString timeCam, timeLog;
        timeUtils::showTime(camLog_iterator->time, &timeLog); //time log
        line.push_back(timeLog);
        timeUtils::showTime(fileset_iterator.key() + offset, &timeCam); // time cam
        line.push_back(timeCam);
        line.push_back(QString::number(*delayLog_iterator));
        line.push_back(QString::number(*delayCam_iterator));
        m_output.push_back(line);
    }
}

alglib::real_1d_array DataAnalysis::crossCorr() const
{
    return m_crossCorr;
}

QVector<double> DataAnalysis::analizedCrossCorelation() const
{
    return m_analizedCrossCorelation;
}

QVector<QStringList> DataAnalysis::output() const
{
    return m_output;
}

void DataAnalysis::createFileSet(QDir picDir)
{
    if ( picDir.exists() )
    {
        QDirIterator dir_iter(picDir);
        while(dir_iter.hasNext())
        {
            dir_iter.next();
            QFileInfo fInfo(dir_iter.filePath());
            if ( fInfo.isFile() )
            {
                QDateTime	picTimeCAM(fInfo.lastModified()); // picture time as measured by the camera
                QDateTime	picTimeUTC(picTimeCAM);           // picture time UTC
                uint picTimeArdu(timeUtils::convTimeUnix2Ardu(picTimeUTC.toTime_t()));	// picture time as stored by ArduPilot 2.5
                m_fileSet.insert(picTimeArdu, dir_iter.filePath());
            }
        }
    }
    timeUtils::CreatePictureBackupFile(picDir,0, QString("PictureBackup") + picDir.dirName());
}
void DataAnalysis::createFileSet(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString line = "line";
    while(!line.isNull())
    {
        line = stream.readLine();
        QStringList SList = line.split(';');
        if(SList.size() == 2)
        {
            m_fileSet.insert(SList[1].toLong(),SList[0]);
        }
    }
}
