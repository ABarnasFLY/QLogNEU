#ifndef EXIFTHREAD_H
#define EXIFTHREAD_H

#include <QThread>
#include "exifwriter.h"

class ExifThread : public QThread
{
    ExifWriter *m_writer;
public:
     void run();
     void setWriter(ExifWriter *analizer);
};

#endif // EXIFTHREAD_H
