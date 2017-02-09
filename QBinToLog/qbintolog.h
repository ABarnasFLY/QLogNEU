#ifndef QBINTOLOG_H
#define QBINTOLOG_H
#include "messagereader.h"
#include "messagewriter.h"
#include <QFileDialog>
class QBinToLog :public QObject
{
    Q_OBJECT
public:
    QBinToLog(QString uFrom,QString uTo); // constructor open files from path from arguments
    ~QBinToLog();
    void exec();                          // function exec the conversion
private:
    MessageWriter *m_writer;
    MessageReader *m_reader;
    QString m_urlTo;
    QString m_urlFrom;
    bool m_ready;

signals:
    void setProgressBar(int);
    void refreshProgressBar(int);
};

#endif // QBINTOLOG_H
