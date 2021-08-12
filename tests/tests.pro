QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

#HEADERS += ./../libs/settings.h
#SOURCES += ./../libs/settings.cpp

TEMPLATE = app

SOURCES +=  tst_settings.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/release/ -llibs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/debug/ -llibs
else:unix: LIBS += -L$$OUT_PWD/../libs/ -llibs

INCLUDEPATH += $$PWD/../libs
DEPENDPATH += $$PWD/../libs
LIBS += -L../libs/debug -llibs
