TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    CTPMarket.cpp \
    BrokerAccount.cpp \
    BrokerAccountManager.cpp \
    FileHandler.cpp

HEADERS += \
    CTPMarket.h \
    ThostFtdcMdApi.h \
    ThostFtdcTraderApi.h \
    ThostFtdcUserApiStruct.h \
    ThostFtdcUserApiDataType.h \
    DLog.h \
    BrokerAccount.h \
    BrokerAccountManager.h

LIBS += -llog4cplus -lboost_filesystem -lboost_system -lboost_thread -lthosttraderapi

