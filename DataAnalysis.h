#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H
#include "global_defs.h"
#include "timeutils.h"
#include "parsemachine.h"

class DataAnalysis
{
    fileSet_t m_fileSet;
    ParseMachine m_parser;
    QVector<CamLog_t> m_vectCamLog;
    //alglib complex vector delay1, delay2


    /////////////////////////////////////
    /// \brief createFileSet
    /// \param picDir
    /// Fill fileset multimap with data from selected directory
    void createFileSet(QDir picDir);
    /////////////////////////////////////
    /// \brief createFileSet
    /// \param filePath
    /// Fill fileset multimap with data from selected backup file
    void createFileSet(QString filePath);
    /////////////////////////////////////
    /// \brief fillDelays
    /// fill delays vector to corelate each other and find proper ofset between
    /// fileset and log stamps
    void fillDelays();

public:
    DataAnalysis(QDir picDir, QString logPath);
    DataAnalysis(QString picBackupPath, QString logPath);

};

#endif // DATAANALYSIS_H
