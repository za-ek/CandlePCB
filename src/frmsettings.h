// This file is a part of "Candle" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#ifndef FRMSETTINGS_H
#define FRMSETTINGS_H

#include <QDialog>
#include <QVector>
#include <QMessageBox>
#include <QListWidgetItem>
#include <utils/Settings.h>
#include "widgets/colorpicker.h"

namespace Ui {
class frmSettings;
}

class frmSettings : public QDialog, Settings
{
    Q_OBJECT

public:
    explicit frmSettings(QWidget *parent = 0);
    ~frmSettings();

    int exec();
    void undo();

    QString port();
    void setPort(QString port);
    int baud();
    void setBaud(int baud);
    double toolDiameter();
    void setToolDiameter(double diameter);
    double toolLength();
    void setToolLength(double length);
    double lineWidth();
    void setLineWidth(double lineWidth);
    double arcLength();
    void setArcLength(double arcLength);
    double arcDegree();
    void setArcDegree(double arcDegree);
    double arcPrecision();
    QString safePositionCommand();
    void setSafePositionCommand(QString command);
    int restoreMode();
    void setRestoreMode(int value);
    int spindleSpeedMin();
    void setSpindleSpeedMin(int speed);
    int spindleSpeedMax();
    void setSpindleSpeedMax(int speed);
    int rapidSpeed();
    void setRapidSpeed(int rapidSpeed);
    int heightmapProbingFeed();
    void setHeightmapProbingFeed(int heightmapProbingFeed);
    int acceleration();
    void setAcceleration(int acceleration);
    int queryStateTime();
    void setQueryStateTime(int queryStateTime);
    int toolType();
    void setToolType(int toolType);
    double toolAngle();
    void setToolAngle(double toolAngle);
    int fps();
    void setFps(int fps);
    int units();
    void setUnits(int units);
    QString touchCommand();
    void setTouchCommand(QString touchCommand);
    bool simplify();
    void setSimplify(bool simplify);
    double simplifyPrecision();
    void setSimplifyPrecision(double simplifyPrecision);
    QList<ColorPicker*> colors();
    QColor colors(QString name);
    int fontSize();
    void setFontSize(int fontSize);
    bool showUICommands();

    bool getOption(QString opt);
    void setOption(QString opt, bool value);

    std::map<QString, bool> m_settings;

    constexpr const inline static char * options[] = {
        "restoreZeroAfterHoming",
        "checkedHeightMap",
        "antialiasing",
        "msaa",
        "vsync",
        "zBuffer",
        "simplify",
        "moveOnRestore",
        "arcDegreeMode",
        "showProgramCommands",
        "showUICommands",
        "autoCompletion",
        "panelHeightmap",
        "panelSpindle",
        "panelFeed",
        "panelJog",
        "autoScroll",
        "feedOverride",
        "keyboardControl",
        "heightmapBorderShow",
        "heightmapGridShow",
        "heightmapInterpolationShow"
    };

    constexpr const inline static char * optionsInt[] = {
        "spindleSpeedMin",
        "spindleSpeedMax",
        "rapidSpeed",
        "heightmapProbingFeed",
        "acceleration",
        "toolType",
        "fps",
        "queryStateTime",
        "units"
    };

    QString getOptionString(QString opt);
    void setOptionString(QString opt, QString value);
    constexpr inline static char * optionsStringKeys[] = {"lastUsedFile", "projectDirectory", "pcb2gcodeArgs"};

protected:
    void showEvent(QShowEvent *se);
    QVector<QString> originalKeys = {
        "s_property"
    };
private slots:
    void on_cmdRefresh_clicked();
    void on_cmdOK_clicked();
    void on_cmdCancel_clicked();
    void on_cboToolType_currentIndexChanged(int index);
    void on_cmdDefaults_clicked();

    void on_cboFontSize_currentTextChanged(const QString &arg1);

    void on_restoreZeroAfterHoming_stateChanged(int arg1);

    void on_pcb2gcodeArgs_textChanged();

private:
    Ui::frmSettings *ui;
    void searchPorts();

    QList<double> m_storedValues;
    QList<bool> m_storedChecks;
    QList<QString> m_storedCombos;
    QList<QColor> m_storedColors;

    QIntValidator m_intValidator;

    QHash<QString, QString> m_optionsString;
};

#endif // FRMSETTINGS_H
