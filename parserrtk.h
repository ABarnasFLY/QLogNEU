#ifndef PARSERRTK_H
#define PARSERRTK_H
#include "parsemachine.h"
#include "rtkeventlogparser.h"
#include "rtklogparser.h"


class ParserRTK : public ParseMachine
{
    RTKEventLogParser m_event;
    RTKLogParser m_log;
public:
    ParserRTK(QString logPath, QString posPath, QObject *parent);
    void run();
};

#endif // PARSERRTK_H
