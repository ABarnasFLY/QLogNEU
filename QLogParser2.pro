#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T13:22:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QLogParser2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    parsemachine.cpp \
    logformat.cpp \
    timeutils.cpp \
    alglib/alglibinternal.cpp \
    alglib/alglibmisc.cpp \
    alglib/ap.cpp \
    alglib/dataanalysis.cpp \
    alglib/diffequations.cpp \
    alglib/fasttransforms.cpp \
    alglib/integration.cpp \
    alglib/interpolation.cpp \
    alglib/linalg.cpp \
    alglib/optimization.cpp \
    alglib/solvers.cpp \
    alglib/specialfunctions.cpp \
    alglib/statistics.cpp \
    DataSetAnalysis.cpp \
    QBinToLog/formatcontainer.cpp \
    QBinToLog/messagereader.cpp \
    QBinToLog/messagewriter.cpp \
    QBinToLog/qbintolog.cpp \
    progresswindow.cpp \
    converterthread.cpp \
    parserrtk.cpp \
    rinexparser.cpp \
    rtkpostprocessparser.cpp \
    rtklogparser.cpp \
    location.cpp \
    analizerthread.cpp

HEADERS  += mainwindow.h \
    parsemachine.h \
    logformat.h \
    global_defs.h \
    timeutils.h \
    alglib/alglibinternal.h \
    alglib/alglibmisc.h \
    alglib/ap.h \
    alglib/dataanalysis.h \
    alglib/diffequations.h \
    alglib/fasttransforms.h \
    alglib/integration.h \
    alglib/interpolation.h \
    alglib/linalg.h \
    alglib/optimization.h \
    alglib/solvers.h \
    alglib/specialfunctions.h \
    alglib/statistics.h \
    alglib/stdafx.h \
    DataSetAnalysis.h \
    QBinToLog/formatcontainer.h \
    QBinToLog/messagereader.h \
    QBinToLog/messagewriter.h \
    QBinToLog/qbintolog.h \
    progresswindow.h \
    converterthread.h \
    parserrtk.h \
    rinexparser.h \
    rtkpostprocessparser.h \
    rtklogparser.h \
    location.h \
    analizerthread.h

FORMS    += mainwindow.ui \
    progresswindow.ui
