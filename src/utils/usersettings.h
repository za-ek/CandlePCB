#ifndef SETTINGS_H
#define SETTINGS_H

#include "settings.h"
#include <QMap>
#include <QWidget>
#include <QVector>
#include <QVariant>
#include <mutex>

class UserSettings : Settings
{
private:
    inline static Settings* _instance{nullptr};
    inline static std::mutex _mutex;

protected:
    UserSettings() {}

public:
    ~UserSettings() {}
    UserSettings(UserSettings &a) = delete;
    void operator=(const UserSettings &) = delete;

    static Settings* getInstance();
    static void save();

    static void restoreUI(QWidget *w);
    static void saveUI(QWidget *w);

    static void restoreRuntime(QMap<QString, QString> *m);
    static void saveRuntime(QMap<QString, QString> *m);

    inline static QVector<QString> runtime_keys = {
        "port",
        "baud",
    };
    /**
      * Runtime keys are joined to settings keys
      *     by instance create
      */
    inline static QVector<QString> settings_keys = {
        "s_lastUsedFile",
        "s_projectDirectory",
        "s_pcb2gcodeArgs",

        "s_toolColor",
        "s_visualizerBackgroundColor",
        "s_visualizerTextColor",
        "s_toolpathNormalColor",
        "s_toolpathDrawnColor",
        "s_toolpathHighlightColor",
        "s_toolpathZMovementColor",

        "s_toolpathStartColor",
        "s_toolpathEndColor",
        "s_baud",
        "s_port",
        "s_touchCommand",

        "i_spindleSpeedMin",
        "i_spindleSpeedMax",
        "i_rapidSpeed",
        "i_heightmapProbingFeed",
        "i_acceleration",
        "i_toolType",
        "i_fps",
        "i_queryStateTime",
        "i_units",

        "b_restoreZeroAfterHoming",
        "b_checkedHeightMap",
        "b_antialiasing",
        "b_msaa",
        "b_vsync",
        "b_zBuffer",
        "b_simplify",
        "b_moveOnRestore",
        "b_arcDegreeMode",
        "b_showProgramCommands",
        "b_showUICommands",
        "b_autoCompletion",
        "b_panelHeightmap",
        "b_panelSpindle",
        "b_panelFeed",
        "b_panelJog",
        "b_autoScroll",
        "b_feedOverride",
        "b_keyboardControl",
        "b_heightmapBorderShow",
        "b_heightmapGridShow",
        "b_heightmapInterpolationShow",

        "d3_jogStep",
    };
};

//Settings* UserSettings::_instance{nullptr};
//std::mutex UserSettings::_mutex;

#endif // SETTINGS_H
