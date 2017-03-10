#include "exifthread.h"


void ExifThread::run()
{
    if(m_writer)
        m_writer->writeExif();
}


void ExifThread::setWriter(ExifWriter *writer)
{
    m_writer = writer;
}
