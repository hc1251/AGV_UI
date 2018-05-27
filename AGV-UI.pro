#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T12:41:41
#
#-------------------------------------------------
QT       += core gui network sql
QT       += multimedia charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = AGV-UI
TEMPLATE = app
include(agv/agv.pri)
include(database/database.pri)
include(mapshow/mapshow.pri)
include(util/util.pri)
include(station/station.pri)
include(css/css.pri)
include(pages/pages.pri)
SOURCES += main.cpp\
    LoginDialog.cpp \
    RecordWindow.cpp \
    DebugWindow.cpp \
    MainWindow.cpp \
    ModifyPwdWindow.cpp \
    qcustomplot.cpp

HEADERS  += \
    LoginDialog.h \
    RecordWindow.h \
    DebugWindow.h \
    MainWindow.h \
    ModifyPwdWindow.h \
    qcustomplot.h

FORMS    += \
    LoginDialog.ui \
    RecordWindow.ui \
    AboutDialog.ui \
    DebugWindow.ui \
    ModifyPwdWindow.ui \
    MainWindow.ui

RESOURCES += \
    img.qrc \
    rc.qrc

DISTFILES += \
    resource/song.mp3 \
    resource/config.json \
    resource/agv.db \
RC_FILE+=
    exeIcon.rc


