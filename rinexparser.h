#ifndef RINEXPARSER_H
#define RINEXPARSER_H

#include "timeutils.h"
#include "global_defs.h"
#include <QTextStream>

class RinexParser
{
    QFile m_rinex;
    QVector<CamLog_t> *m_vectCamLog;
public:
    RinexParser(QVector<CamLog_t> *vectCamLog, QString rinexPath);
    void run();
};

#endif // RINEXPARSER_H
