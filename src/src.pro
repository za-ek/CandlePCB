#-------------------------------------------------
#
# Project created by QtCreator 2014-05-23T17:51:21
#
#-------------------------------------------------

QT       = core gui opengl serialport
CONFIG += c++17
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/release/ -llibs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/debug/ -llibs
else:unix: LIBS += -L$$OUT_PWD/../libs/ -llibs

INCLUDEPATH += $$PWD/../libs
DEPENDPATH += $$PWD/../libs
LIBS += -L../libs/debug -llibs

Release:DESTDIR = release
Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:DESTDIR = debug
Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui

win32: {
    QT += winextras
    DEFINES += WINDOWS
    QMAKE_CXXFLAGS_DEBUG += -g3 -pg
    QMAKE_LFLAGS_DEBUG += -pg -lgmon
}

unix:!macx {
    DEFINES += UNIX #GL_GLEXT_PROTOTYPES
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/libs\'"
}

contains(QT_CONFIG, opengles.) {
    warning("GL ES detected. VAO will be disabled.")
    DEFINES += GLES
    INSTALLS += target
    target.path = /home/pi
}

TARGET = Candle
TEMPLATE = app
VERSION = 0.9
RC_ICONS += images/candle.ico

DEFINES += sNan=\"65536\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TRANSLATIONS += translations/candle_ru.ts translations/candle_es.ts translations/candle_fr.ts translations/candle_pt.ts

SOURCES += main.cpp\
        frmmain.cpp \
    frmsettings.cpp \
    frmabout.cpp \
    drawers/gcodedrawer.cpp \
    drawers/heightmapborderdrawer.cpp \
    drawers/heightmapgriddrawer.cpp \
    drawers/heightmapinterpolationdrawer.cpp \
    drawers/origindrawer.cpp \
    drawers/shaderdrawable.cpp \
    drawers/tooldrawer.cpp \
    parser/arcproperties.cpp \
    parser/gcodeparser.cpp \
    parser/gcodepreprocessorutils.cpp \
    parser/gcodeviewparse.cpp \
    parser/linesegment.cpp \
    parser/pointsegment.cpp \
    tables/gcodetablemodel.cpp \
    tables/heightmaptablemodel.cpp \
    utils/grblport.cpp \
    utils/usersettings.cpp \
    widgets/colorpicker.cpp \
    widgets/combobox.cpp \
    widgets/groupbox.cpp \
    widgets/scrollarea.cpp \
    widgets/styledtoolbutton.cpp \
    widgets/widget.cpp \
    widgets/glwidget.cpp

HEADERS  += frmmain.h \
    frmsettings.h \
    frmabout.h \
    drawers/gcodedrawer.h \
    drawers/heightmapborderdrawer.h \
    drawers/heightmapgriddrawer.h \
    drawers/heightmapinterpolationdrawer.h \
    drawers/origindrawer.h \
    drawers/shaderdrawable.h \
    drawers/tooldrawer.h \
    parser/arcproperties.h \
    parser/gcodeparser.h \
    parser/gcodepreprocessorutils.h \
    parser/gcodeviewparse.h \
    parser/linesegment.h \
    parser/pointsegment.h \
    tables/gcodetablemodel.h \
    tables/heightmaptablemodel.h \
    utils/grblport.h \
    utils/interpolation.h \
    utils/usersettings.h \
    utils/util.h \
    widgets/colorpicker.h \
    widgets/combobox.h \
    widgets/groupbox.h \
    widgets/scrollarea.h \
    widgets/styledtoolbutton.h \
    widgets/widget.h \
    widgets/glwidget.h

FORMS    += frmmain.ui \
    frmsettings.ui \
    frmabout.ui

DEFINES += _USE_MATH_DEFINES

RESOURCES += \
    scripts.qrc \
    shaders.qrc \
    images.qrc

CONFIG += c++11

DISTFILES += \
    CHANGELOG.md
