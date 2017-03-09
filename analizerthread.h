#ifndef ANALIZERTHREAD_H
#define ANALIZERTHREAD_H

#include <QThread>
#include "DataSetAnalysis.h"

class AnalizerThread : public QThread
{
    DataAnalysis *m_analizer;
public:
     void run();
     void setAnalizer(DataAnalysis *analizer);
};

#endif // ANALIZERTHREAD_H
