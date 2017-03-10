#ifndef EXIFWRITER_H
#define EXIFWRITER_H
#include <QString>
#include <string>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <exiv2/exiv2.hpp>
#include <cassert>

class ExifWriter :public QObject
{
    Q_OBJECT
    QString m_rPath;
    QString m_dirPath;
    QString m_copyDir;
signals:
    void updateProgress(int);
    void sendMessage(QString);
public:
    ExifWriter(QString raportPath, QString dirPath, QString copyDir);
    void writeExif();
};

#endif // EXIFWRITER_H
