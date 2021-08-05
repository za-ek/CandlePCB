QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

HEADERS += ./../src/utils/Settings.h
SOURCES += ./../src/utils/Settings.cpp

TEMPLATE = app

SOURCES +=  tst_settings.cpp
