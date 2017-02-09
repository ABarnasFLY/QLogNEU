#ifndef MESSAGEWRITER_H
#define MESSAGEWRITER_H
#include <QFile>
#include <QTextStream>
#include <QString>

class MessageWriter
{
public:
    MessageWriter();
    MessageWriter(QString dir);
    ~MessageWriter();
    void openFile(QString dir);
    void toFile(QString message);
    bool isFileOpen() const;
private:
    bool m_fileIsOpen;
    QFile *m_fileTo;
    QTextStream *m_stream;
};

#endif // MESSAGEWRITER_H
