#ifndef RTKEVENTLOGPARSER_H
#define RTKEVENTLOGPARSER_H

#include <QVector>
#include <QString>
#include <QTextStream>
#include <QFile>
#include "global_defs.h"
#include "timeutils.h"


class RTKEventLogParser
{
    QFile m_file;
    QVector<CamLog_t> *m_vectCamLog;
public:
    RTKEventLogParser(QString path, QVector<CamLog_t> *vectCamLog);
    void run();
};

#endif // RTKEVENTLOGPARSER_H
