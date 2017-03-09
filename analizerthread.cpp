#include "analizerthread.h"


void AnalizerThread::run()
{
    if(m_analizer)
        m_analizer->run();
}


void AnalizerThread::setAnalizer(DataAnalysis *analizer)
{
    m_analizer = analizer;
}
