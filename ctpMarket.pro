#-------------------------------------------------
#
# Project created by QtCreator 2014-05-28T17:56:42
#
#-------------------------------------------------

QT       -= core gui

TARGET = ctpMarket
TEMPLATE = lib

DEFINES += CTPMARKET_LIBRARY

INCLUDEPATH += ../../ $(APOLLO_INCLUDE_PATH)
INCLUDEPATH += ../../lib/CTP/

CONFIG += c++11
CONFIG -= qt
unix:QMAKE_CXXFLAGS_WARN_ON = -Wall  -Wno-unknown-pragmas

SOURCES += CTPMarket.cpp \
    CTPUtil.cpp \
    OrderIdUtil.cpp \
    PluginInfo.cpp

HEADERS += CTPMarket.h\
        ctpmarket_global.h \
    CTPUtil.h \
    OrderIdUtil.h \
    UniformMap.hpp


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/CTP/release/ -lthosttraderapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/CTP/debug/ -lthosttraderapi
else:unix: LIBS += -L$$PWD/../../lib/CTP/ -lthosttraderapi

INCLUDEPATH += $$PWD/../../lib/CTP
DEPENDPATH += $$PWD/../../lib/CTP


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../../core/ -lcore

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core


target.path = /lib/plugins
INSTALLS += target
unix:LIBS += -ltcmalloc
win32:DEFINES += BOOST_ALL_NO_LIB
CONFIG(debug, debug|release){
    win32: LIBS += -llibboost_thread-mt-gd-1_55 -llibboost_filesystem-mt-gd-1_55 -llibboost_system-mt-gd-1_55
    win32: LIBS += -llibboost_date_time-mt-gd-1_55 -llibboost_chrono-mt-gd-1_55 -llog4cplus
} else {
    win32: LIBS += -llibboost_thread-mt-1_55 -llibboost_system-mt-1_55
    win32: LIBS += -llibboost_date_time-mt-1_55 -llibboost_chrono-mt-1_55 -llog4cplus
}
