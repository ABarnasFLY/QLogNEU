#ifndef RTKPOSTPROCESSPARSER_H
#define RTKPOSTPROCESSPARSER_H
#include "global_defs.h"
#include "timeutils.h"
#include <QTextStream>

class RTKPostProcessParser
{
    QFile m_file;
    QVector<CamLog_t> *m_vectCamLog;
public:
    RTKPostProcessParser(QString path, QVector<CamLog_t> *vectCamLog);
    void run();
};

#endif // RTKPOSTPROCESSPARSER_H
