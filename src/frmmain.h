// This file is a part of "Candle" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QSettings>
#include <QTimer>
#include <QBasicTimer>
#include <QStringList>
#include <QList>
#include <QTime>
#include <QMenu>
#include <QDragEnterEvent>
#include <QDropEvent>

#include "parser/gcodeviewparse.h"

#include "drawers/origindrawer.h"
#include "drawers/gcodedrawer.h"
#include "drawers/tooldrawer.h"
#include "drawers/heightmapborderdrawer.h"
#include "drawers/heightmapgriddrawer.h"
#include "drawers/heightmapinterpolationdrawer.h"
#include "drawers/shaderdrawable.h"

#include "tables/gcodetablemodel.h"
#include "tables/heightmaptablemodel.h"

#include "utils/interpolation.h"
#include "utils/grblport.h"
#include "utils/usersettings.h"
#include "../libs/runtime.h"

#include "widgets/styledtoolbutton.h"

#include "frmsettings.h"
#include "frmabout.h"

#ifdef WINDOWS
    #include <QtWinExtras/QtWinExtras>
    #include "shobjidl.h"
#endif

namespace Ui {
class frmMain;
}

struct CommandAttributes {
    int length;
    int consoleIndex;
    int tableIndex;
    QString command;
};

struct CommandQueue {
    QString command;
    int tableIndex;
    bool showInConsole;
};

struct Status {
    int code;
    QString name;
    QString fcolor;
    QString bcolor;
};

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

    double toolZPosition();
    QString runPHP(QString path, QStringList params);

private slots:
    void updateHeightMapInterpolationDrawer(bool reset = false);
    void placeVisualizerButtons();

    void setWorkCoordinates(double x, double y, double z);
    void setMachinePosition(double x, double y, double z);
    void setMachineStatus(MACHINE_STATUS status);
    void resetCompleted();
    void dataReceived(QString data);
    void onError(int err_n, QString err);

    void onTimerConnection();
    void onSerialPortReadyRead();
    void onCmdJogStepClicked();
    void onVisualizatorRotationChanged();
    void onScroolBarAction(int action);
    void onJogTimer();
    void onTableInsertLine();
    void onTableDeleteLines();
    void onActRecentFileTriggered();
    void onCboCommandReturnPressed();
    void onTableCurrentChanged(QModelIndex idx1, QModelIndex idx2);
    void onConsoleResized(QSize size);
    void onPanelsSizeChanged(QSize size);

    void on_actFileExit_triggered();
    void on_cmdFileOpen_clicked();
    void on_cmdFit_clicked();
    void on_cmdFileSend_clicked();
    void onTableCellChanged(QModelIndex i1, QModelIndex i2);
    void on_actServiceSettings_triggered();
    void on_actFileOpen_triggered();
    void on_cmdCommandSend_clicked();
    void on_cmdHome_clicked();
    void on_cmdTouch_clicked();
    void on_cmdZeroXY_clicked();
    void on_cmdZeroZ_clicked();
//    void on_cmdRestoreOrigin_clicked();
    void on_cmdReset_clicked();
    void on_cmdUnlock_clicked();
//    void on_cmdSafePosition_clicked();
    void on_cmdSpindle_toggled(bool checked);
    void on_txtSpindleSpeed_editingFinished();
    void on_sliSpindleSpeed_valueChanged(int value);
    void on_cmdYPlus_clicked();
    void on_cmdYMinus_clicked();
    void on_cmdXPlus_clicked();
    void on_cmdXMinus_clicked();
    void on_cmdZPlus_clicked();
    void on_cmdZMinus_clicked();
    void on_chkTestMode_clicked(bool checked);
    void on_cmdFilePause_clicked(bool checked);
    void on_cmdFileReset_clicked();
    void on_actFileNew_triggered();
    void on_cmdClearConsole_clicked();
    void on_actFileSaveAs_triggered();
    void on_actFileSave_triggered();
    void on_actFileSaveTransformedAs_triggered();
    void on_cmdTop_clicked();
    void on_cmdFront_clicked();
    void on_cmdLeft_clicked();
    void on_cmdIsometric_clicked();
    void on_actAbout_triggered();
    void on_txtFeed_editingFinished();
    void on_sliFeed_valueChanged(int value);
    void on_chkFeedOverride_toggled(bool checked);
    void on_grpFeed_toggled(bool checked);
    void on_grpSpindle_toggled(bool checked);
    void on_grpJog_toggled(bool checked);
    void on_chkKeyboardControl_toggled(bool checked);
    void on_tblProgram_customContextMenuRequested(const QPoint &pos);
    void on_splitter_splitterMoved(int pos, int index);
    void on_actRecentClear_triggered();
    void on_grpHeightmap_toggled(bool arg1);
    void on_heightmapBorderShow_toggled(bool checked);
    void on_heightmapBorderX_valueChanged(double arg1);
    void on_heightmapBorderWidth_valueChanged(double arg1);
    void on_heightmapBorderY_valueChanged(double arg1);
    void on_heightmapBorderHeight_valueChanged(double arg1);
    void on_heightmapGridShow_toggled(bool checked);
    void on_heightmapGridX_valueChanged(double arg1);
    void on_heightmapGridY_valueChanged(double arg1);
    void on_heightmapGridZBottom_valueChanged(double arg1);
    void on_heightmapGridZTop_valueChanged(double arg1);
    void on_heightmapMode_toggled(bool checked);
    void on_heightmapInterpolationShow_toggled(bool checked);
    void on_heightmapLoad_clicked();
    void on_heightmapInterpolationStepX_valueChanged(double arg1);
    void on_heightmapInterpolationStepY_valueChanged(double arg1);
    void on_heightmapUse_clicked(bool checked);
    void on_heightmapCreate_clicked();
    void on_heightmapBorderAuto_clicked();
    void on_cmdFileAbort_clicked();

    void on_actionGenerate_g_code_triggered();

    void on_actionOpen_directory_triggered();

protected:
    void showEvent(QShowEvent *se);
    void hideEvent(QHideEvent *he);
    void resizeEvent(QResizeEvent *re);
    void timerEvent(QTimerEvent *);
    void closeEvent(QCloseEvent *ce);
    void dragEnterEvent(QDragEnterEvent *dee);
    void dropEvent(QDropEvent *de);

private:
    frmAbout _frmAbout;
    frmSettings _frmSettings;

    const int BUFFERLENGTH = 127;
    const unsigned short HEIGHT_MAP_NUMBER = 0;

    unsigned short m_heghtMapIndex = HEIGHT_MAP_NUMBER;
    QString m_projectDirectory;
    QString getTodayDirectory();

    Ui::frmMain *ui;
    GcodeViewParse m_viewParser;
    GcodeViewParse m_probeParser;

    OriginDrawer *m_originDrawer;

    GcodeDrawer *m_codeDrawer;
    GcodeDrawer *m_probeDrawer;
    GcodeDrawer *m_currentDrawer;

    ToolDrawer m_toolDrawer;
    HeightMapBorderDrawer m_heightMapBorderDrawer;
    HeightMapGridDrawer m_heightMapGridDrawer;
    HeightMapInterpolationDrawer m_heightMapInterpolationDrawer;

    GCodeTableModel m_programModel;
    GCodeTableModel m_probeModel;
    GCodeTableModel m_programHeightmapModel;

    HeightMapTableModel m_heightMapModel;

    bool m_programLoading;
    bool m_settingsLoading;

    GRBLPort m_serialPort;
    Runtime *m_runtime;
    Settings *m_settings = UserSettings::getInstance();

    QString m_settingsFileName;
    QString m_programFileName;
    QString m_heightMapFileName;

    bool m_fileChanged = false;
    bool m_heightMapChanged = false;

    QTimer m_timerConnection;
    QBasicTimer m_timerToolAnimation;

    QStringList m_status;
    QStringList m_statusCaptions;
    QStringList m_statusBackColors;
    QStringList m_statusForeColors;

#ifdef WINDOWS
    QWinTaskbarButton *m_taskBarButton;
    QWinTaskbarProgress *m_taskBarProgress;
#endif

    QMenu *m_tableMenu;
    QList<CommandAttributes> m_commands;
    QList<CommandQueue> m_queue;
    QTime m_startTime;

    double m_storedX = 0;
    double m_storedY = 0;
    double m_storedZ = 0;
    QString m_storedParserStatus;

    // Console window
    int m_storedConsoleMinimumHeight;
    int m_storedConsoleHeight;
    int m_consolePureHeight;

    // Flags
    bool m_settingZeroXY = false;
    bool m_settingZeroZ = false;
    bool m_homing = false;
    bool m_programSpeed = false;
    bool m_updateSpindleSpeed = false;
    bool m_updateParserStatus = false;
    bool m_updateFeed = false;

    bool m_reseting = false;
    bool m_resetCompleted = true;
    bool m_aborting = false;
    bool m_statusReceived = false;

    bool m_processingFile = false;
    bool m_transferCompleted = false;
    bool m_fileEndSent = false;

    /**
     * @brief m_heightMapMode
     * HeightMap mode toggles by clicking on button "cmdHeightMapMode"
     * HeightMap process starts by event on_cmdFileSend_clicked
     *
     * on_cmdFileSend_clicked calls on_cmdFileReset_clicked
     *  which set heightmap gcode to m_probeModel
     *
     * sendNextFileCommands sends current gcode table
     *  (m_currentModel (alias for m_probeModel in on_cmdHeightMapMode_toggled))
     *
     * updateHeightMapGrid sets data to m_probeModel
     *
     */
    bool m_heightMapMode;
    bool m_cellChanged;

    // Indices
    int m_fileCommandIndex;
    int m_fileProcessedCommandIndex;
    /*
     * Index in heightmap table
     * Only resets by "reset" button
     * The fQirst index is -1 which means relative point for rest of heightmap
     */
    int m_probeIndex;

    // Current values
    int m_lastDrawnLineIndex;
    int m_lastGrblStatus;
    double m_originalFeed;

    // Keyboard
    bool m_keyPressed = false;
    bool m_jogBlock = false;
    bool m_absoluteCoordinates;
    bool m_storedKeyboardControl;

    // Spindle
    bool m_spindleCW = true;
    bool m_spindleCommandSpeed = false;

    QStringList m_recentFiles;
    QStringList m_recentHeightmaps;

    void loadFile(QString fileName);
    void loadFile(QList<QString> data);
    void clearTable();
    void loadSettings();
    void saveSettings();
    bool saveChanges(bool heightMapMode);
    void updateControlsState();
    void sendCommand(QString command, int tableIndex = -1, bool showInConsole = true);
    int bufferLength();
    void sendNextFileCommands();
    void applySettings();
    void updateParser();
    bool dataIsFloating(QString data);
    bool dataIsEnd(QString data);
    bool dataIsReset(QString data);

    void setStatusBarItems();
    void useHeightmap(QString fileName);

    QTime updateProgramEstimatedTime(QList<LineSegment *> lines);
    bool saveProgramToFile(QString fileName, GCodeTableModel *model);
    QString feedOverride(QString command);

    bool eventFilter(QObject *obj, QEvent *event);
    void blockJogForRapidMovement(bool repeated = false);
    bool keyIsMovement(int key);
    void resizeCheckBoxes();
    void updateLayouts();
    void updateRecentFilesMenu();
    void addRecentFile(QString fileName);
    void addRecentHeightmap(QString fileName);
    double toMetric(double value);

    QRectF borderRectFromTextboxes();
    QRectF borderRectFromExtremes();
    void updateHeightMapBorderDrawer();
    bool updateHeightMapGrid();
    void loadHeightMap(QString fileName);
    bool saveHeightMap(QString fileName);

    GCodeTableModel *m_currentModel;
    QList<LineSegment *> subdivideSegment(LineSegment *segment);
    void resizeTableHeightMapSections();
    void updateHeightMapGrid(double arg1);
    void resetHeightmap();
    void storeParserState();
    void restoreParserState();
    void storeOffsets();
    void restoreOffsets();
    bool isGCodeFile(QString fileName);
    bool isHeightmapFile(QString fileName);
    int getConsoleMinHeight();

    double getMachinePosition(coordinates axis);
    double machine_coordinates[3] = {0, 0, 0};
    QLabel *machinePosition;

    void setMachineStatus(QString status);
    void setMachineStatus(int status);
    MACHINE_STATUS machineStatus = UNKNOWN;
    QLabel*machineStatusWidget = nullptr;

    const Status machine_status[9] = {
        {UNKNOWN, "Unknown", "red", "white"},
        {IDLE, "Idle", "palette(button)", "palette(text)"},
        {ALARM, "Alarm", "red", "white"},
        {RUN, "Run", "lime", "black"},
        {HOME, "Home", "lime", "black"},
        {HOLD, "Hold", "yellow", "black"},
        {QUEUE, "Queue", "yellow", "black"},
        {CHECK, "Check",  "palette(button)", "palette(text)"},
        {DOOR, "Door", "red", "white"}
    };
};

#endif // FRMMAIN_H
