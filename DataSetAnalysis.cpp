#include "DataSetAnalysis.h"

DataAnalysis::DataAnalysis(QString pics, QString logPath, QObject* parent):
    QObject(parent),
    m_offset_fs(0),
    m_offset_lg(0)
{
    m_parser = new ParseMachine(logPath, this);
    createFileSet(pics);
}

DataAnalysis::DataAnalysis(QDir pics, QString logPath, QObject* parent):
    QObject(parent),
    m_offset_fs(0),
    m_offset_lg(0)
{
    createFileSet(pics);
    m_parser = new ParseMachine(logPath, this);
}

DataAnalysis::DataAnalysis(QString pics, QString logPath, QString rinexPath, QString ppRTKpath, QObject* parent):
    QObject(parent),
    m_offset_fs(0),
    m_offset_lg(0)
{
    m_parser = new ParserRTK(logPath,rinexPath, ppRTKpath, parent);
    createFileSet(pics);
}

DataAnalysis::DataAnalysis(QDir pics, QString logPath, QString rinexPath, QString ppRTKpath, QObject* parent):
    QObject(parent),
    m_offset_fs(0),
    m_offset_lg(0)
{
    createFileSet(pics);
    m_parser = new ParserRTK(logPath,rinexPath, ppRTKpath, parent);
}

void DataAnalysis::skipCam(int n)
{
    m_vectCamLog.remove(n);
    fillDelays();
    print();
}

void DataAnalysis::skipPic(int n)
{
    m_fileSet.remove((m_fileSet.begin() + n).key());
    fillDelays();
    print();
}

void DataAnalysis::Modify(QVector<int> picExclusion, QVector<int> logExclusion)
{
    for(int i = 0; i < logExclusion.size(); i++)
    {
        m_vectCamLog.remove(logExclusion[i]);
    }
    for(int i = 0; i < picExclusion.size(); i++)
    {
        m_fileSet.remove((m_fileSet.begin() + picExclusion[i]).key());
    }
    fillDelays();
    print();
}


void DataAnalysis::run()
{
    m_parser->run();
    m_vectCamLog = m_parser->getVectCamLog();
    if(dynamic_cast<ParserRTK*>(m_parser) == nullptr)
    {
        ApplyDelay();
    }
    fillDelays();
    alglib::corrr1d(m_cDelayLog,m_cDelayLog.length(),m_cDelayPic,m_cDelayPic.length(),m_crossCorr);
    m_originalCamLog = m_vectCamLog;
    m_originalFileSet = m_fileSet;
    correlationAnalysis();
    fillDelays();
    print();
}

void DataAnalysis::fillDelays()
{
    m_delayLog.clear();
    m_delayPic.clear();
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

    QVector<double>::iterator n = std::max_element(m_analizedCrossCorelation.begin(),m_analizedCrossCorelation.end());
    if(n < m_analizedCrossCorelation.begin() + m_analizedCrossCorelation.size()/2) m_offset_lg = n - m_analizedCrossCorelation.begin();
    else m_offset_fs = m_analizedCrossCorelation.end() - n;
    if (m_offset_fs != 0)
    {
        int i = m_offset_fs;
        while (i != 0)
        {
            m_fileSet.remove(m_fileSet.begin().key());
            i--;
        }
    }
    if (m_offset_lg != 0)
    {
        int i = m_offset_lg;
        m_vectCamLog.remove(0,m_offset_lg);
        i--;
    }

}


void DataAnalysis::print()
{
    m_output.clear();
    ErrorCount();
    QVector<CamLog_t>::iterator camLog_iterator= m_vectCamLog.begin();
    fileSet_t::iterator fileset_iterator = m_fileSet.begin();
    QVector<double>::iterator delayCam_iterator = m_delayPic.begin();
    QVector<double>::iterator delayLog_iterator = m_delayLog.begin();
/*    delayCam_iterator += m_offset_fs;
    delayLog_iterator += m_offset_lg;
    fileset_iterator  += m_offset_fs;
    camLog_iterator   += m_offset_lg;*/
    int offset = camLog_iterator->time - fileset_iterator.key();
    for(; camLog_iterator != m_vectCamLog.end() && fileset_iterator != m_fileSet.end(); camLog_iterator++, fileset_iterator++, delayCam_iterator++, delayLog_iterator++ )
    {
        QStringList line;
        line.push_back(QFileInfo(fileset_iterator.value()).fileName()); //filename
        line.push_back(QString::number(camLog_iterator->lat,'f',9)); // lat
        line.push_back(QString::number(camLog_iterator->lon,'f',9)); // lon
        line.push_back(QString::number(camLog_iterator->alt,'f',9)); // alt
        QString timeCam, timeLog;
        timeUtils::showTime(camLog_iterator->time, &timeLog); //time log
        line.push_back(timeLog);
        timeUtils::showTime(fileset_iterator.key() + offset, &timeCam); // time cam
        line.push_back(timeCam);
        line.push_back(QString::number(*delayLog_iterator / 1000,'f',9));
        line.push_back(QString::number(*delayCam_iterator / 1000,'f',9));
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

double DataAnalysis::meanError() const
{
    return m_meanError;
}

int DataAnalysis::outliersCount() const
{
    return m_outliersCount;
}

QVector<double> DataAnalysis::delayPic() const
{
    return m_delayPic;
}

QVector<double> DataAnalysis::delayLog() const
{
    return m_delayLog;
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

void DataAnalysis::ErrorCount()
{
    QVector<CamLog_t>::iterator itVectCamLog = m_vectCamLog.begin();
    fileSet_t::iterator itFileSet = m_fileSet.begin();
    itVectCamLog += m_offset_fs;
    itFileSet += m_offset_lg;
    double sum = 0;
    m_outliersCount = 0;
    int iterations = 0;
    double offset = itVectCamLog->time - itFileSet.key();

    for(; itVectCamLog != m_vectCamLog.end() && itFileSet != m_fileSet.end(); itVectCamLog++, itFileSet++)
    {
        sum += abs(itVectCamLog->time - itFileSet.key() - offset);
        if(abs(itVectCamLog->time - itFileSet.key() - offset) > 1400)
        {
            m_outliersCount++;
        }
        iterations++;
    }

    m_meanError = sum / (double)iterations ;
    m_meanError = m_meanError / 1000.0;
}

void DataAnalysis::ApplyDelay()
{
    QVector<CamLog_t>::iterator it_v = m_vectCamLog.begin();

    // loop over all VectorCamLog elements to apply the GPS & Camera delay corrections
    for (;	it_v != m_vectCamLog.end(); ++it_v)
    {
        Loc tempLoc;
        //convert to fixed decimal to ensure compatibility with location.cpp
        tempLoc.lat = 1.0e7 * (*it_v).lat;
        tempLoc.lon = 1.0e7 * (*it_v).lon;
        Location::loc_update(tempLoc, (*it_v).CoG , (*it_v).GS * DELAY_TRIGER_TO_PHOTO);
        (*it_v).lat = (double)(1.0e-7 * tempLoc.lat);
        (*it_v).lon = (double)(1.0e-7 * tempLoc.lon);
    }
}
