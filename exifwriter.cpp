#include "exifwriter.h"
#include <qdebug.h>
ExifWriter::ExifWriter(QString raportPath, QString dirPath, QString copyDir = "")
    :
      m_rPath(raportPath),
      m_dirPath(dirPath),
      m_copyDir(copyDir)
{

}

void ExifWriter::writeExif()
{
    QFile raport(m_rPath);
    raport.open(QIODevice::ReadOnly);
    QTextStream stream(&raport);
    QString line;
    QString fileName;
    double lat,lon;
    QStringList lineSplited;
    while(!stream.atEnd())
    {
        line = stream.readLine();
        if(line !="\n" && line != "" && !line.startsWith("Filename,"))
        {
            if(line.startsWith("\r")) line.remove(0,1);
            lineSplited = line.split(',',QString::SkipEmptyParts);
            QStringList::iterator itSL = lineSplited.begin();
            fileName = *itSL;
            itSL++;
            lat = itSL->toDouble();
            itSL++;
            lon = itSL->toDouble();

            char scratchBuf[100];
            int lat_deg = (int)floor(fabs(lat)); //slice off after decimal
            long lat_min = (long)((fabs(lat) - lat_deg)*60000000); //in microseconds of arc
            int lon_deg = (int)floor(fabs(lon)); //slice off after decimal
            long lon_min = (long)((fabs(lon) - lon_deg)*60000000); //in microseconds of arc
            Exiv2::Image::AutoPtr image;
            if(fileName.endsWith(' ')) fileName.chop(1);
            if(m_copyDir == "") image = Exiv2::ImageFactory::open(m_dirPath.toStdString() + '/' + fileName.toStdString());
            else
            {

                if(QFile::copy(m_dirPath + '/' + fileName, m_copyDir + '/' + fileName))
//                QString filename111 = m_dirPath + '/' + fileName.c_str();
//                file.setFileName(m_dirPath + '/' + fileName.c_str());
//                file.copy(m_copyDir + '/' + fileName.c_str());
                image = Exiv2::ImageFactory::open(m_copyDir.toStdString() + '/' + fileName.toStdString());
            }
            assert(image.get() != 0);
            image->readMetadata();

            Exiv2::ExifData &exifData = image->exifData();

            snprintf(scratchBuf, 100, "%d/1 %ld/1000000 0/1", lat_deg, lat_min);
            exifData["Exif.GPSInfo.GPSLatitudeRef"] = (lat < 0 ) ? "S" : "N";
            exifData["Exif.GPSInfo.GPSLatitude"] = scratchBuf;

            snprintf(scratchBuf, 100, "%d/1 %ld/1000000 0/1", lon_deg, lon_min);
            exifData["Exif.GPSInfo.GPSLongitudeRef"] = (lon < 0 ) ? "W" : "E";
            exifData["Exif.GPSInfo.GPSLongitude"] = scratchBuf;

            exifData["Exif.GPSInfo.GPSMapDatum"] = "WGS-84";

            // Finally, write the modified Exif data to the image file
            image->setExifData(exifData);
            image->writeMetadata();
        }

    }
}
