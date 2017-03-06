#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H

#define DELAY_TRIGER_TO_PHOTO 0.2
#include "global_defs.h"
#include "timeutils.h"
#include "parsemachine.h"
#include "alglib/fasttransforms.h"
#include "parserrtk.h"
#include "location.h"
#include <QObject>

class DataAnalysis : public QObject
{
    Q_OBJECT
    fileSet_t m_fileSet;
    fileSet_t m_originalFileSet;
    ParseMachine *m_parser;
    QVector<CamLog_t> m_vectCamLog;
    QVector<CamLog_t> m_originalCamLog;
    alglib::real_1d_array m_cDelayPic, m_cDelayLog, m_crossCorr;
    double m_meanError;
    int m_outliersCount;
    QVector<double> m_delayPic, m_delayLog, m_analizedCrossCorelation;
    QVector<QStringList> m_output;
    int m_offset_fs;
    int m_offset_lg;

    void createFileSet(QDir picDir);

    void createFileSet(QString filePath);

    void fillDelays();

    void correlationAnalysis();

    void calculateClockOffset();

    void print();

    void ErrorCount();
    void ApplyDelay();
public:
    DataAnalysis(QString pics, QString logPath, QObject* parent);
    DataAnalysis(QDir pics, QString logPath, QObject *parent);
    void run();
    alglib::real_1d_array crossCorr() const;
    QVector<double> analizedCrossCorelation() const;
    QVector<QStringList> output() const;
    double meanError() const;
    int outliersCount() const;
    DataAnalysis(QDir pics, QString logPath, QString rinexPath, QString ppRTKpath, QObject *parent);
    DataAnalysis(QString pics, QString logPath, QString rinexPath, QString ppRTKpath, QObject *parent);
    QVector<double> delayPic() const;
    QVector<double> delayLog() const;
    void Modify(QVector<int> picExclusions, QVector<int> logExclusion);
};

#endif // DATAANALYSIS_H
