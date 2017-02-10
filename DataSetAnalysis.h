#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H
#include "global_defs.h"
#include "timeutils.h"
#include "parsemachine.h"
#include "alglib/fasttransforms.h"
#include <QObject>

class DataAnalysis : public QObject
{
    Q_OBJECT
    fileSet_t m_fileSet;
    ParseMachine m_parser;
    QVector<CamLog_t> m_vectCamLog;
    alglib::real_1d_array m_cDelayPic, m_cDelayLog, m_crossCorr;
    QVector<double> m_delayPic, m_delayLog, m_analizedCrossCorelation;
    QVector<QStringList> m_output;
    int m_offset_fs;
    int m_offset_lg;
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
    void correlationAnalysis();
    void calculateClockOffset();
    void print();
public:
    DataAnalysis(QString pics, QString logPath, QObject* parent);
    DataAnalysis(QDir pics, QString logPath, QObject *parent);
    void run();
    alglib::real_1d_array crossCorr() const;
    QVector<double> analizedCrossCorelation() const;
    QVector<QStringList> output() const;
};

#endif // DATAANALYSIS_H
