#ifndef GLOBAL_DEFS
#define GLOBAL_DEFS
#include <QString>
#include <QVector>
#include <QStringList>
#include <QFileSystemModel>
#include <QFile>
#include <QMultiMap>

typedef QMultiMap<uint, QString> fileSet_t;

typedef struct {
    unsigned long	time;
    float			alt;
    double          lat;
    double          lon;
    float           roll;
    float           pitch;
    float           yaw;
    float			CoG;		//GPS Course over Ground
    float			GS;			//GPS Ground Speed [m/s]
} CamLog_t;

typedef struct {
    QString	lastATT;
    QString	lastGPS;
    QString	CAM;
    QString	nextATT;
    QString	nextGPS;
} NearLogs_t;


#endif // GLOBAL_DEFS

