#-------------------------------------------------
#
# Project created by QtCreator 2018-12-27T23:45:18
#
#-------------------------------------------------

QT       += core gui

TARGET = SSTL
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    test.cpp

HEADERS += \
    sstl/_check.h \
    sstl/_define.h \
    sstl/_util.h \
    sstl/ArrList.h \
    sstl/PtrArray.h \
    sstl/ptrcontainer.h \
    sstl/SArray.h \
    sstl/SContainer.h \
    sstl/SList.h \
    sstl/SMap.h \
    sstl/SSet.h \
    sstl/sstl.h \
    sstl/PairList.h

CONFIG += c++11

