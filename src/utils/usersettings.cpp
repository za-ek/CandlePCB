#include <QApplication>
#include "settings.h"
#include "usersettings.h"
#include <QVariant>
#include <QDebug>

#include <QAbstractButton>
#include <QComboBox>
#include <QAbstractSpinBox>
#include <QDoubleSpinBox>
#include "widgets/colorpicker.h"

Settings * UserSettings::getInstance()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr) {
        _instance = new Settings({
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

             "d3_jogStep"
         });

        QString settings_path = qApp->applicationDirPath() + "/settings.ini";
        qDebug() << "Settings: " << settings_path;
        QSettings set(settings_path);
        foreach(const QString &k, set.allKeys()) {
            qDebug() << "Key: " << k;
        }
        set.setIniCodec("UTF-8");
        _instance->restoreQSettings(&set);
    }
    return _instance;
}

void UserSettings::save() {
    QString settings_path = qApp->applicationDirPath() + "/settings.ini";
    QSettings set(settings_path);
    qDebug() << "Settings: " << settings_path;
    set.setIniCodec("UTF-8");
    getInstance()->fillQSettings(&set);
}

void UserSettings::saveUI(QWidget *w) {
    Settings * s = getInstance();

    // All checkboxes
    foreach (QAbstractButton* cb, w->findChildren<QAbstractButton*>())
        s->setBool(cb->objectName(), cb->isChecked());
    // All colors
    foreach (ColorPicker* pick, w->findChildren<ColorPicker*>())
        s->set(pick->objectName(), pick->color().name());
    // All texts
    foreach (QComboBox* cb, w->findChildren<QComboBox*>())
        s->set(cb->objectName(), cb->currentText());
    // All double inputs
    foreach (QAbstractSpinBox* sb, w->findChildren<QAbstractSpinBox*>())
        s->setDouble(sb->objectName(), sb->property("value").toDouble());
}

void UserSettings::restoreUI(QWidget *w) {
    Settings * s = getInstance();

    // All checkboxes
    foreach (QAbstractButton* cb, w->findChildren<QAbstractButton*>())
        cb->setChecked(s->getBool(cb->objectName()));
    // All colors
    foreach (ColorPicker* pick, w->findChildren<ColorPicker*>())
        pick->setColor(QColor(s->get(pick->objectName())));
    // All texts
    foreach (QComboBox* cb, w->findChildren<QComboBox*>())
        cb->setCurrentIndex(s->getInt(cb->objectName()));
    // All double inputs
    foreach (QDoubleSpinBox* sb, w->findChildren<QDoubleSpinBox*>())
        sb->setValue(s->getDouble(sb->objectName()));
}
