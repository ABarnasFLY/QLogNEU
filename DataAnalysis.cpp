#include "DataAnalysis.h"

DataAnalysis::DataAnalysis(QDir picDir, QString logPath):
    m_parser(logPath)
{
    createFileSet(picDir);
}

DataAnalysis::DataAnalysis(QString picBackupPath, QString logPath):
    m_parser(logPath)
{
    createFileSet(picBackupPath);
}

void DataAnalysis::fillDelays()
{
    //Fill delays Alglib needed
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
                picTimeUTC.addSecs(cam2utcClkOffs);
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
            fileSet.insert(SList[1].toLong(),SList[0]);
        }
    }
}
