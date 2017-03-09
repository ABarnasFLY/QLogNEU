#ifndef RTKLOGPARSER_H
#define RTKLOGPARSER_H
#include "global_defs.h"
#include <QTextStream>
#include <QObject>

#define PSTATE RTKLogParser::State

class RTKLogParser :public QObject
{
    Q_OBJECT
    QFile *m_log;
    QVector<CamLog_t> *m_vectCamLog;
    QTextStream m_logStream;
    long m_timeDifference;
    unsigned long m_currentTimeMark;
    enum State
    {
        INIT = 0,
        FORMAT_DETECTION,
        SYNCHRONIZATION,
        CONVERSION,
        SEARCHING,
        DONE,
        ERROR
    };

    QVector<CamLog_t>::iterator m_it_vectCamLog;
    int m_rollPos, m_pitchPos, m_yawPos, m_GPSTimePos;
    //State functions
    //
    //
    State on_Init();
    State on_FormatDetection();
    State on_Synchronization();
    State on_Conversion();
    State on_Searching();
    State on_Error();
signals:
    void updateProgress(int);
    void setProgressMax(int);

public:
    RTKLogParser(QFile *log, QVector<CamLog_t> *vectCamLog);
    void run();
};

#endif // RTKLOGPARSER_H
