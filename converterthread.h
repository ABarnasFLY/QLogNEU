#ifndef CONVERTERTHREAD_H
#define CONVERTERTHREAD_H
#include <QThread>
#include "QBinToLog/qbintolog.h"

class ConverterThread : public QThread
{
    QBinToLog *m_converter;
public:
    void setConverter(QBinToLog *value);
    void run();
};

#endif // CONVERTERTHREAD_H
