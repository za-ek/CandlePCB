    // This file is a part of "Candle" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

//#define INITTIME //QTime time; time.start();
//#define PRINTTIME(x) //qDebug() << "time elapse" << QString("%1:").arg(x) << time.elapsed(); time.start();

#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QTextBlock>
#include <QTextCursor>
#include <QMessageBox>
#include <QComboBox>
#include <QScrollBar>
#include <QShortcut>
#include <QAction>
#include <QLayout>
#include <QMimeData>
#include <QTemporaryFile>
#include <QProcess>
#include <QTemporaryFile>
#include <QDesktopServices>
#include <QStatusBar>
#include "frmmain.h"
#include "ui_frmmain.h"
#include "utils/grblport.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cerrno>

frmMain::frmMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmMain),
    m_runtime(Runtime::getInstance())
{
    UserSettings::restoreUI(this);
    ui->setupUi(this);
    setStatusBarItems();
#ifdef WINDOWS
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
        m_taskBarButton = NULL;
        m_taskBarProgress = NULL;
    }
#endif

#ifndef UNIX
    ui->cboCommand->setStyleSheet("QComboBox {padding: 2;} QComboBox::drop-down {width: 0; border-style: none;} QComboBox::down-arrow {image: url(noimg);	border-width: 0;}");
#endif
    ui->scrollArea->updateMinimumWidth();

    m_heightMapMode = false;
    m_lastDrawnLineIndex = 0;
    m_fileProcessedCommandIndex = 0;
    m_cellChanged = false;
    m_programLoading = false;
    m_currentModel = &m_programModel;
    m_transferCompleted = true;

    ui->txtJogStep->setLocale(QLocale::C);

    ui->cmdXMinus->setBackColor(QColor(153, 180, 209));
    ui->cmdXPlus->setBackColor(ui->cmdXMinus->backColor());
    ui->cmdYMinus->setBackColor(ui->cmdXMinus->backColor());
    ui->cmdYPlus->setBackColor(ui->cmdXMinus->backColor());

    //    ui->cmdReset->setBackColor(QColor(255, 228, 181));

    ui->cmdFit->setParent(ui->glwVisualizer);
    ui->cmdIsometric->setParent(ui->glwVisualizer);
    ui->cmdTop->setParent(ui->glwVisualizer);
    ui->cmdFront->setParent(ui->glwVisualizer);
    ui->cmdLeft->setParent(ui->glwVisualizer);

    ui->cmdHeightMapBorderAuto->setMinimumHeight(ui->chkHeightMapBorderShow->sizeHint().height());
    ui->heightmapCreate->setMinimumHeight(ui->cmdFileOpen->sizeHint().height());
    ui->heightmapLoad->setMinimumHeight(ui->cmdFileOpen->sizeHint().height());
    ui->heightmapMode->setMinimumHeight(ui->cmdFileOpen->sizeHint().height());

    connect(ui->cboCommand, SIGNAL(returnPressed()), this, SLOT(onCboCommandReturnPressed()));

    m_originDrawer = new OriginDrawer();
    m_codeDrawer = new GcodeDrawer();
    m_codeDrawer->setViewParser(&m_viewParser);
    m_probeDrawer = new GcodeDrawer();
    m_probeDrawer->setViewParser(&m_probeParser);
    m_probeDrawer->setVisible(false);
    m_heightMapGridDrawer.setModel(&m_heightMapModel);
    m_currentDrawer = m_codeDrawer;
    m_toolDrawer.setToolPosition(QVector3D(0, 0, 0));

    QShortcut *insertShortcut = new QShortcut(QKeySequence(Qt::Key_Insert), ui->tblProgram);
    QShortcut *deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), ui->tblProgram);

    connect(insertShortcut, SIGNAL(activated()), this, SLOT(onTableInsertLine()));
    connect(deleteShortcut, SIGNAL(activated()), this, SLOT(onTableDeleteLines()));

    m_tableMenu = new QMenu(this);
    m_tableMenu->addAction(tr("&Insert line"), this, SLOT(onTableInsertLine()), insertShortcut->key());
    m_tableMenu->addAction(tr("&Delete lines"), this, SLOT(onTableDeleteLines()), deleteShortcut->key());

    ui->glwVisualizer->addDrawable(m_originDrawer);
    ui->glwVisualizer->addDrawable(m_codeDrawer);
    ui->glwVisualizer->addDrawable(m_probeDrawer);
    ui->glwVisualizer->addDrawable(&m_toolDrawer);
    ui->glwVisualizer->addDrawable(&m_heightMapBorderDrawer);
    ui->glwVisualizer->addDrawable(&m_heightMapGridDrawer);
    ui->glwVisualizer->addDrawable(&m_heightMapInterpolationDrawer);
    ui->glwVisualizer->fitDrawable();

    connect(&m_timerConnection, SIGNAL(timeout()), this, SLOT(onTimerConnection()));
    m_timerConnection.start(1000);

    // Changes in UI
    connect(ui->glwVisualizer, SIGNAL(rotationChanged()), this, SLOT(onVisualizatorRotationChanged()));
    connect(ui->glwVisualizer, SIGNAL(resized()), this, SLOT(placeVisualizerButtons()));
    connect(&m_programModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCellChanged(QModelIndex,QModelIndex)));
    connect(&m_programHeightmapModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCellChanged(QModelIndex,QModelIndex)));
    connect(&m_probeModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCellChanged(QModelIndex,QModelIndex)));
    connect(&m_heightMapModel, SIGNAL(dataChangedByUserInput()), this, SLOT(updateHeightMapInterpolationDrawer()));

    ui->tblProgram->setModel(&m_programModel);
    ui->tblProgram->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    connect(ui->tblProgram->verticalScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(onScroolBarAction(int)));
    clearTable();

    // Console window handling
    connect(ui->grpConsole, SIGNAL(resized(QSize)), this, SLOT(onConsoleResized(QSize)));
    connect(ui->scrollAreaWidgetContents, SIGNAL(sizeChanged(QSize)), this, SLOT(onPanelsSizeChanged(QSize)));

    // Loading settings
    loadSettings();
    ui->tblProgram->hideColumn(4);
    ui->tblProgram->hideColumn(5);
//    ui->tblProgram->showColumn(4);
    m_runtime->bind("port", [&](const QString &v) {
        this->m_serialPort.setPortName(v); m_serialPort.openPort(); updateControlsState();
    });
    m_runtime->bind("baud", [&](const QString &v) {
        this->m_serialPort.setBaudRate(v); m_serialPort.openPort(); updateControlsState();
    });


    connect(&m_serialPort, SIGNAL(dataReceived(QString)), this, SLOT(dataReceived(QString)));
    connect(&m_serialPort, SIGNAL(receivedWorkCoordinates(double,double,double)), this, SLOT(setWorkCoordinates(double,double,double)));
    connect(&m_serialPort, SIGNAL(resetCompleted()), this, SLOT(resetCompleted()));
    connect(&m_serialPort, SIGNAL(statusChanged(MACHINE_STATUS)), this, SLOT(setMachineStatus(MACHINE_STATUS)));
    connect(&m_serialPort, SIGNAL(receivedCoordinates(double,double,double)), this, SLOT(setMachinePosition(double, double, double)));
    connect(&m_serialPort, SIGNAL(receivedError(int, QString)), this, SLOT(onError(int, QString)));

    // Apply settings
    foreach (StyledToolButton* button, ui->grpJog->findChildren<StyledToolButton*>(QRegExp("cmdJogStep\\d")))
    {
        connect(button, SIGNAL(clicked(bool)), this, SLOT(onCmdJogStepClicked()));
        button->setChecked(button->text().toDouble() == ui->txtJogStep->value());
    }

    show(); // Visibility bug workaround
    applySettings();
    updateControlsState();

    this->installEventFilter(this);
    ui->tblProgram->installEventFilter(this);
    ui->splitPanels->handle(1)->installEventFilter(this);
    ui->splitPanels->installEventFilter(this);

    // Handle file drop
    if (qApp->arguments().count() > 1 && isGCodeFile(qApp->arguments().last())) {
        loadFile(qApp->arguments().last());
    }

    if(m_settings->get("lastUsedFile").length() > 0) {
        loadFile(m_settings->get("lastUsedFile"));
    }

    // Update v-sync in glformat
    QGLFormat fmt = QGLFormat::defaultFormat();
    fmt.setSwapInterval(m_settings->getBool("vsync") ? 1 : 0);
    QGLFormat::setDefaultFormat(fmt);

    m_runtime->bind("fontSize", [&](QVariant v) {
        if(v.toString().isEmpty()) v = "8";
        qApp->setStyleSheet(QString(qApp->styleSheet()).replace(QRegExp("font-size:\\s*\\d+"), "font-size: " + v.toString()));
    });

    qDebug() << "Port: " << Runtime::getInstance()->value("port");
    m_serialPort.setPortName(Runtime::getInstance()->value("port"));
    m_serialPort.setBaudRate(Runtime::getInstance()->value("baud"));
    m_serialPort.openPort();
    updateControlsState();
}

frmMain::~frmMain()
{
    delete ui;
}
void frmMain::setMachinePosition(double x, double y, double z) {
    machinePosition->setText(QString().sprintf("X: %.4f Y: %.4f Z: %.4f", x, y, z));
}
void frmMain::setStatusBarItems()
{
    machinePosition = new QLabel(this);
    statusBar()->addPermanentWidget(machinePosition);
    statusBar()->showMessage(tr("Ready"), 2000);

    machineStatusWidget = new QLabel(this);
    statusBar()->addPermanentWidget(machineStatusWidget);
}
void frmMain::setMachineStatus(MACHINE_STATUS status)
{
    qDebug() << "RECEIVED STATUS: " << status;
    if(status == ALARM) {
        ui->cmdUnlock->setStyleSheet("background-color: red");
    } else {
        ui->cmdUnlock->setStyleSheet(QString("background-color: palette(button); color: palette(text);"));
    }
    ui->cmdUnlock->repaint();

    if(status != machineStatus) {
        machineStatus = status;
        machineStatusWidget->setText(tr(machine_status[status].name.toLocal8Bit()));
        machineStatusWidget->setStyleSheet(QString("background-color: %1; color: %2;")
                                     .arg(machine_status[status].bcolor).arg(machine_status[status].fcolor));
    }
    m_timerConnection.stop();

    // Update controls
    ui->cmdRestoreOrigin->setEnabled(status == IDLE);
    ui->cmdSafePosition->setEnabled(status == IDLE);
    ui->cmdZeroXY->setEnabled(status == IDLE);
    ui->cmdZeroZ->setEnabled(status == IDLE);
    ui->chkTestMode->setEnabled(status != RUN && !m_processingFile);
    ui->chkTestMode->setChecked(status == CHECK);
    ui->cmdFilePause->setChecked(status == HOLD || status == QUEUE);
#ifdef WINDOWS
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
        if (m_taskBarProgress) m_taskBarProgress->setPaused(status == HOLD || status == QUEUE);
    }
#endif

    // Update "elapsed time" timer
    if (m_processingFile) {
        QTime time(0, 0, 0);
        int elapsed = m_startTime.elapsed();
        ui->glwVisualizer->setSpendTime(time.addMSecs(elapsed));
    }

    // Test for job complete
    if (m_processingFile && m_transferCompleted &&
            ((status == IDLE && machineStatus == RUN) || status == CHECK)) {
        qDebug() << "job completed:" << m_fileCommandIndex << m_currentModel->rowCount() - 1;

        // Shadow last segment
        GcodeViewParse *parser = m_currentDrawer->viewParser();
        QList<LineSegment*> list = parser->getLineSegmentList();
        if (m_lastDrawnLineIndex < list.count()) {
            list[m_lastDrawnLineIndex]->setDrawn(true);
            m_currentDrawer->update(QList<int>() << m_lastDrawnLineIndex);
        }

        // Update state
        m_processingFile = false;
        m_fileProcessedCommandIndex = 0;
        m_lastDrawnLineIndex = 0;
        m_storedParserStatus.clear();

        updateControlsState();

        qApp->beep();



        if(m_settings->getBool("checkedHeightMap")) {
            if(m_heightMapMode && m_heghtMapIndex < HEIGHT_MAP_NUMBER) {
                // Store current heightMap
                saveHeightMap((getTodayDirectory() + "/%1.map").arg(++m_heghtMapIndex));
                on_cmdFileSend_clicked();
            } else if (m_heightMapMode) {
                saveHeightMap((getTodayDirectory() + "/%1.map").arg(++m_heghtMapIndex));
                resetHeightmap();
                on_heightmapMode_toggled(false);

                // Generate target heightmap
                // Run php file stored in resources
                QFile f(":/scripts/map.php");
                QTemporaryFile *tmpFile = QTemporaryFile::createNativeFile(f);
                if (tmpFile->open()) {
                    QProcess *process = new QProcess(this);
                    QStringList params;
                    qDebug() << tmpFile->fileName();
                    params << tmpFile->fileName().toLocal8Bit();
                    params << getTodayDirectory();
                    qDebug() << params;
                    process->start("php", params);
                    process->waitForFinished();
                    tmpFile->close();
                } else {
                    qDebug() << "Error open temp file";
                }

                ui->heightmapUse->setChecked(true);
                useHeightmap(getTodayDirectory() + "/target.map");
            } else {
                QMessageBox::information(this, qApp->applicationDisplayName(), tr("Job done.\nTime elapsed: %1")
                                     .arg(ui->glwVisualizer->spendTime().toString("hh:mm:ss")));
            }
        } else {
            QMessageBox::information(this, qApp->applicationDisplayName(), tr("Job done.\nTime elapsed: %1")
                                 .arg(ui->glwVisualizer->spendTime().toString("hh:mm:ss")));
        }
    }

    if (m_aborting && status == IDLE) {
        if (!m_processingFile && m_resetCompleted) {
            m_aborting = false;
            restoreOffsets();
            restoreParserState();
            return;
        }
    }
    m_timerConnection.start();
}
void frmMain::setMachineStatus(QString status)
{
    machineStatus = UNKNOWN;
    statusBar()->showMessage(status, 1000);
//    machineStatusWidget->setStyleSheet(QString("background-color: palette(button); color: palette(text);"));
}

bool frmMain::isGCodeFile(QString fileName)
{
    return fileName.endsWith(".txt", Qt::CaseInsensitive)
          || fileName.endsWith(".nc", Qt::CaseInsensitive)
          || fileName.endsWith(".ncc", Qt::CaseInsensitive)
          || fileName.endsWith(".ngc", Qt::CaseInsensitive)
          || fileName.endsWith(".tap", Qt::CaseInsensitive);
}

bool frmMain::isHeightmapFile(QString fileName)
{
    return fileName.endsWith(".map", Qt::CaseInsensitive);
}

double frmMain::toolZPosition()
{
    return m_toolDrawer.toolPosition().z();
}


void frmMain::loadSettings()
{
    Settings * set = UserSettings::getInstance();

    m_settingsLoading = true;
    m_projectDirectory = set->get("projectDirectory");

    ui->txtJogStep->setValue(set->getDouble("jogStep"));

    m_programSpeed = true;
    ui->sliSpindleSpeed->setValue(set->getInt("spindleSpeed"));
    m_programSpeed = false;


    ui->grpConsole->setMinimumHeight(set->getInt("consoleMinHeight"));

    ui->chkAutoScroll->setChecked(set->getBool("autoScroll"));
    ui->sliSpindleSpeed->setValue(set->getInt("spindleSpeed") / 100);
    ui->txtSpindleSpeed->setValue(set->getInt("spindleSpeed"));
    ui->chkFeedOverride->setChecked(set->getBool("feedOverride"));
    ui->sliFeed->setValue(set->getInt("feed"));

    m_storedX = set->getDouble("storedX");
    m_storedY = set->getDouble("storedY");
    m_storedZ = set->getDouble("storedZ");

//    ui->cmdRestoreOrigin->setToolTip(QString(tr("Restore origin:\n%1, %2, %3")).arg(m_storedX).arg(m_storedY).arg(m_storedZ));

    m_recentFiles = set->get("recentFiles").split(",");
    m_recentHeightmaps = set->get("recentHeightmaps").split(",");

    this->restoreGeometry(QByteArray(set->get("formGeometry").toLocal8Bit()));
    QByteArray splitterState = QByteArray(set->get("splitter").toLocal8Bit());

    if (splitterState.length() == 0) {
        ui->splitter->setStretchFactor(0, 1);
        ui->splitter->setStretchFactor(1, 1);
    } else ui->splitter->restoreState(splitterState);

    ui->chkAutoScroll->setVisible(ui->splitter->sizes()[1]);
    resizeCheckBoxes();

    ui->cboCommand->setMinimumHeight(ui->cboCommand->height());
    ui->cmdClearConsole->setFixedHeight(ui->cboCommand->height());
    ui->cmdCommandSend->setFixedHeight(ui->cboCommand->height());

    m_storedKeyboardControl = set->getBool("keyboardControl");
    ui->cboCommand->setCurrentIndex(-1);

    updateRecentFilesMenu();

    ui->tblProgram->horizontalHeader()->restoreState(set->get("header").toLocal8Bit());

    m_settingsLoading = false;
}


bool frmMain::saveChanges(bool heightMapMode)
{
    if ((!heightMapMode && m_fileChanged)) {
        int res = QMessageBox::warning(this, this->windowTitle(), tr("G-code program file was changed. Save?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (res == QMessageBox::Cancel) return false;
        else if (res == QMessageBox::Yes) on_actFileSave_triggered();
        m_fileChanged = false;
    }

    if (m_heightMapChanged) {
        int res = QMessageBox::warning(this, this->windowTitle(), tr("Heightmap file was changed. Save?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (res == QMessageBox::Cancel) return false;
        else if (res == QMessageBox::Yes) {
            m_heightMapMode = true;
            on_actFileSave_triggered();
            m_heightMapMode = heightMapMode;
            updateRecentFilesMenu(); // Restore g-code files recent menu
        }

        m_fileChanged = false;
    }

    return true;
}

void frmMain::updateControlsState() {
    bool portOpened = m_serialPort.isOpen();
    qDebug() << "Port is open: " << portOpened;

    QVector<QString> enableOnPortOpen({"grpState", "grpControl", "widgetSpindle", "cmdCommandSend"});
    foreach(const QString &s, enableOnPortOpen) {
        QWidget *x = findChild<QWidget*>(s);
        if(x == nullptr) {
            qDebug() << "nullptr: " << s;
        } else {
            x->setEnabled(portOpened);
        }
    }

    QVector<QString> disableWhileProcessFile({
                         "chkTestMode", "cmdHome", "cmdTouch", "cmdZeroXY",
                         "cmdZeroZ", "cmdRestoreOrigin", "cmdSafePosition", "cmdUnlock",
                         "cmdSpindle", "actFileNew", "actFileOpen", "cmdFileOpen",
                         "cmdFileAbort", "cmdFileOpen"
                     });
    foreach(const QString & s, disableWhileProcessFile) {
        QWidget *x = findChild<QWidget*>(s);
        if(x == nullptr) {
            qDebug() << "nullptr: " << s;
        } else {
            x->setEnabled(!m_processingFile);
        }
    }

    ui->widgetJog->setEnabled(portOpened && !m_processingFile);
    ui->cboCommand->setEnabled(portOpened && (!ui->chkKeyboardControl->isChecked()));

    ui->chkTestMode->setEnabled(portOpened && !m_processingFile);

    ui->cmdFileReset->setEnabled(!m_processingFile && m_programModel.rowCount() > 1);
    ui->cmdFileSend->setEnabled(portOpened && !m_processingFile && m_programModel.rowCount() > 1);
    ui->cmdFilePause->setEnabled(m_processingFile && !ui->chkTestMode->isChecked());
    ui->mnuRecent->setEnabled(!m_processingFile && ((m_recentFiles.count() > 0 && !m_heightMapMode)
                                                      || (m_recentHeightmaps.count() > 0 && m_heightMapMode)));
    ui->actFileSave->setEnabled(m_programModel.rowCount() > 1);
    ui->actFileSaveAs->setEnabled(m_programModel.rowCount() > 1);

    ui->tblProgram->setEditTriggers(m_processingFile ? QAbstractItemView::NoEditTriggers :
                                                         QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked
                                                         | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed);

    if (!portOpened) {
        setMachineStatus(tr("Not connected"));
    }

    this->setWindowTitle(m_programFileName.isEmpty() ? qApp->applicationDisplayName()
                                                     : m_programFileName.mid(m_programFileName.lastIndexOf("/") + 1) + " - " + qApp->applicationDisplayName());

    if (!m_processingFile) ui->chkKeyboardControl->setChecked(m_storedKeyboardControl);

#ifdef WINDOWS
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
        if (!m_processingFile && m_taskBarProgress) m_taskBarProgress->hide();
    }
#endif

    style()->unpolish(ui->cmdFileOpen);
    style()->unpolish(ui->cmdFileReset);
    style()->unpolish(ui->cmdFileSend);
    style()->unpolish(ui->cmdFilePause);
    style()->unpolish(ui->cmdFileAbort);
    ui->cmdFileOpen->ensurePolished();
    ui->cmdFileReset->ensurePolished();
    ui->cmdFileSend->ensurePolished();
    ui->cmdFilePause->ensurePolished();
    ui->cmdFileAbort->ensurePolished();

    // Heightmap
    m_heightMapBorderDrawer.setVisible(ui->chkHeightMapBorderShow->isChecked() && m_heightMapMode);
    m_heightMapGridDrawer.setVisible(ui->chkHeightMapGridShow->isChecked() && m_heightMapMode);
    m_heightMapInterpolationDrawer.setVisible(ui->chkHeightMapInterpolationShow->isChecked() && m_heightMapMode);

    ui->grpProgram->setTitle(m_heightMapMode ? tr("Heightmap") : tr("G-code program"));
    ui->grpProgram->setProperty("overrided", m_heightMapMode);
    style()->unpolish(ui->grpProgram);
    ui->grpProgram->ensurePolished();

    ui->grpHeightmapSettings->setVisible(m_heightMapMode);
    ui->grpHeightmapSettings->setEnabled(!m_processingFile);

    ui->chkTestMode->setVisible(!m_heightMapMode);
    ui->chkAutoScroll->setVisible(ui->splitter->sizes()[1] && !m_heightMapMode);

    ui->tblHeightMap->setVisible(m_heightMapMode);
    ui->tblProgram->setVisible(!m_heightMapMode);

    ui->widgetHeightmap->setEnabled(!m_processingFile && m_programModel.rowCount() > 1);
    ui->heightmapMode->setEnabled(!ui->heightmapFile->text().isEmpty());

    ui->cmdFileSend->setText(m_heightMapMode ? tr("Probe") : tr("Send"));

    ui->heightmapUse->setEnabled(!m_heightMapMode && !ui->heightmapFile->text().isEmpty());

    ui->actFileSaveTransformedAs->setVisible(ui->heightmapUse->isChecked());
}

void frmMain::sendCommand(QString command, int tableIndex, bool showInConsole)
{
    if (!m_serialPort.isOpen() || !m_resetCompleted) return;

    command = command.toUpper();

    // Commands queue
    if ((bufferLength() + command.length() + 1) > BUFFERLENGTH) {

        CommandQueue cq;

        cq.command = command;
        cq.tableIndex = tableIndex;
        cq.showInConsole = showInConsole;

        m_queue.append(cq);
        return;
    }

    CommandAttributes ca;

//    if (!(command == "$G" && tableIndex < -1) && !(command == "$#" && tableIndex < -1)
//            && (!m_transferringFile || (m_transferringFile && m_showAllCommands) || tableIndex < 0)) {
    if (showInConsole) {
        ui->txtConsole->appendPlainText(command);
        ca.consoleIndex = ui->txtConsole->blockCount() - 1;
    } else {
        ca.consoleIndex = -1;
    }

    ca.command = command;
    ca.length = command.length() + 1;
    ca.tableIndex = tableIndex;

    m_commands.append(ca);

    // Processing spindle speed only from g-code program
    QRegExp s("[Ss]0*(\\d+)");
    if (s.indexIn(command) != -1 && ca.tableIndex > -2) {
        int speed = s.cap(1).toInt();
        if (ui->txtSpindleSpeed->value() != speed) {
            ui->txtSpindleSpeed->setValue(speed);
            m_programSpeed = true;
            ui->sliSpindleSpeed->setValue(speed / 100);
            m_programSpeed = false;
        }
    }

    // Set M2 & M30 commands sent flag
    if (command.contains(QRegExp("M0*2|M30"))) {
        m_fileEndSent = true;
    }

    m_serialPort.write(command + "\r");
}

void frmMain::resetCompleted()
{
    qDebug() << "grbl reset";

    m_processingFile = false;
    m_transferCompleted = true;
    m_fileCommandIndex = 0;

    m_homing = false;
    m_resetCompleted = false;
    m_updateSpindleSpeed = true;
    m_lastGrblStatus = -1;
    m_statusReceived = true;

    // Drop all remaining commands in buffer
    m_commands.clear();
    m_queue.clear();

    // Prepare reset response catch
    CommandAttributes ca;
    ca.command = "[CTRL+X]";

    if (m_settings->getBool("showUICommands")) ui->txtConsole->appendPlainText(ca.command);
    ca.consoleIndex = m_settings->getBool("showUICommands") ? ui->txtConsole->blockCount() - 1 : -1;
    ca.tableIndex = -1;
    ca.length = ca.command.length() + 1;
    m_commands.append(ca);

    updateControlsState();
}

int frmMain::bufferLength()
{
    int length = 0;

    foreach (CommandAttributes ca, m_commands) {
        length += ca.length;
    }

    return length;
}
QString frmMain::getTodayDirectory()
{
    return m_projectDirectory + "/" + QDateTime::currentDateTime().toString("yyMMdd");
}
void frmMain::onSerialPortReadyRead()
{
}

void frmMain::onTimerConnection()
{
    if (!m_serialPort.isOpen()) {
        m_serialPort.openPort();
    } else if (!m_homing && !ui->cmdFilePause->isChecked() && m_queue.length() == 0) {
        if (m_updateSpindleSpeed) {
            m_updateSpindleSpeed = false;
            sendCommand(QString("S%1").arg(ui->txtSpindleSpeed->value()), -2, m_settings->getBool("showUICommands"));
        }
        if (m_updateParserStatus) {
            m_updateParserStatus = false;
            sendCommand("$G", -3, false);
        }
        if (m_updateFeed) {
            m_updateFeed = false;
            sendCommand(QString("F%1").arg(ui->chkFeedOverride->isChecked() ?
                m_originalFeed / 100 * ui->txtFeed->value() : m_originalFeed), -1, m_settings->getBool("showUICommands"));
        }
    }
}

void frmMain::onCmdJogStepClicked()
{
    ui->txtJogStep->setValue(static_cast<QPushButton*>(sender())->text().toDouble());

    foreach (StyledToolButton* button, ui->grpJog->findChildren<StyledToolButton*>(QRegExp("cmdJogStep\\d")))
    {
        button->setChecked(false);
    }
    static_cast<QPushButton*>(sender())->setChecked(true);
}

void frmMain::onVisualizatorRotationChanged()
{
    ui->cmdIsometric->setChecked(false);
}

void frmMain::onScroolBarAction(int action)
{
    Q_UNUSED(action)

    if (m_processingFile) ui->chkAutoScroll->setChecked(false);
}

void frmMain::onJogTimer()
{
    m_jogBlock = false;
}

void frmMain::placeVisualizerButtons()
{
    ui->cmdIsometric->move(ui->glwVisualizer->width() - ui->cmdIsometric->width() - 8, 8);
    ui->cmdTop->move(ui->cmdIsometric->geometry().left() - ui->cmdTop->width() - 8, 8);
    ui->cmdLeft->move(ui->glwVisualizer->width() - ui->cmdLeft->width() - 8, ui->cmdIsometric->geometry().bottom() + 8);
    ui->cmdFront->move(ui->cmdLeft->geometry().left() - ui->cmdFront->width() - 8, ui->cmdIsometric->geometry().bottom() + 8);
    ui->cmdFit->move(ui->glwVisualizer->width() - ui->cmdFit->width() - 8, ui->cmdLeft->geometry().bottom() + 8);
}

void frmMain::showEvent(QShowEvent *se)
{
    Q_UNUSED(se)

    placeVisualizerButtons();

#ifdef WINDOWS
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
        if (m_taskBarButton == NULL) {
            m_taskBarButton = new QWinTaskbarButton(this);
            m_taskBarButton->setWindow(this->windowHandle());
            m_taskBarProgress = m_taskBarButton->progress();
        }
    }
#endif

    ui->glwVisualizer->setUpdatesEnabled(true);

    resizeCheckBoxes();
}

void frmMain::hideEvent(QHideEvent *he)
{
    Q_UNUSED(he)

    ui->glwVisualizer->setUpdatesEnabled(false);
}

void frmMain::resizeEvent(QResizeEvent *re)
{
    Q_UNUSED(re)

    placeVisualizerButtons();
    resizeCheckBoxes();
    resizeTableHeightMapSections();
}

void frmMain::resizeTableHeightMapSections()
{
    if (ui->tblHeightMap->horizontalHeader()->defaultSectionSize()
            * ui->tblHeightMap->horizontalHeader()->count() < ui->glwVisualizer->width())
        ui->tblHeightMap->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); else {
        ui->tblHeightMap->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
}

void frmMain::resizeCheckBoxes()
{
    static int widthCheckMode = ui->chkTestMode->sizeHint().width();
    static int widthAutoScroll = ui->chkAutoScroll->sizeHint().width();

    // Transform checkboxes

    this->setUpdatesEnabled(false);

    updateLayouts();

    if (ui->chkTestMode->sizeHint().width() > ui->chkTestMode->width()) {
        widthCheckMode = ui->chkTestMode->sizeHint().width();
        ui->chkTestMode->setText(tr("Check"));
        ui->chkTestMode->setMinimumWidth(ui->chkTestMode->sizeHint().width());
        updateLayouts();
    }

    if (ui->chkAutoScroll->sizeHint().width() > ui->chkAutoScroll->width()
            && ui->chkTestMode->text() == tr("Check")) {
        widthAutoScroll = ui->chkAutoScroll->sizeHint().width();
        ui->chkAutoScroll->setText(tr("Scroll"));
        ui->chkAutoScroll->setMinimumWidth(ui->chkAutoScroll->sizeHint().width());
        updateLayouts();
    }

    if (ui->spacerBot->geometry().width() + ui->chkAutoScroll->sizeHint().width()
            - ui->spacerBot->sizeHint().width() > widthAutoScroll && ui->chkAutoScroll->text() == tr("Scroll")) {
        ui->chkAutoScroll->setText(tr("Autoscroll"));
        updateLayouts();
    }

    if (ui->spacerBot->geometry().width() + ui->chkTestMode->sizeHint().width()
            - ui->spacerBot->sizeHint().width() > widthCheckMode && ui->chkTestMode->text() == tr("Check")) {
        ui->chkTestMode->setText(tr("Check mode"));
        updateLayouts();
    }

    this->setUpdatesEnabled(true);
    this->repaint();
}

void frmMain::timerEvent(QTimerEvent *te)
{
    ui->glwVisualizer->setBufferState(QString(tr("Buffer: %1 / %2")).arg(bufferLength()).arg(m_queue.length()));

    if (te->timerId() == m_timerToolAnimation.timerId()) {
        m_toolDrawer.rotate((m_spindleCW ? -40 : 40) * (double)(ui->txtSpindleSpeed->value())
                            / (ui->txtSpindleSpeed->maximum()));
    } else {
        QMainWindow::timerEvent(te);
    }
}

void frmMain::closeEvent(QCloseEvent *ce)
{
    bool mode = m_heightMapMode;
    m_heightMapMode = false;

    if (!saveChanges(m_heightMapMode)) {
        ce->ignore();
        m_heightMapMode = mode;
        return;
    }

    if (m_processingFile && QMessageBox::warning(this, this->windowTitle(), tr("File sending in progress. Terminate and exit?"),
                                                   QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        ce->ignore();
        m_heightMapMode = mode;
        return;
    }

    if (m_queue.length() > 0) {
        m_commands.clear();
        m_queue.clear();
    }
}

void frmMain::dragEnterEvent(QDragEnterEvent *dee)
{
    if (dee->mimeData()->hasFormat("text/plain") && !m_heightMapMode) dee->acceptProposedAction();
    else if (dee->mimeData()->hasFormat("text/uri-list") && dee->mimeData()->urls().count() == 1) {
        QString fileName = dee->mimeData()->urls().at(0).toLocalFile();

        if ((!m_heightMapMode && isGCodeFile(fileName))
        || (m_heightMapMode && isHeightmapFile(fileName)))
            dee->acceptProposedAction();
    }
}

void frmMain::dropEvent(QDropEvent *de)
{
    QString fileName = de->mimeData()->urls().at(0).toLocalFile();

    if (!m_heightMapMode) {
        if (!saveChanges(false)) return;

        // Load dropped g-code file
        if (!fileName.isEmpty()) {
            addRecentFile(fileName);
            updateRecentFilesMenu();
            loadFile(fileName);
        // Load dropped text
        } else {
            m_programFileName.clear();
            m_fileChanged = true;
            loadFile(de->mimeData()->text().split("\n"));
        }
    } else {
        if (!saveChanges(true)) return;

        // Load dropped heightmap file
        addRecentHeightmap(fileName);
        updateRecentFilesMenu();
        loadHeightMap(fileName);
    }
}

void frmMain::on_actFileExit_triggered()
{
    close();
}

void frmMain::on_cmdFileOpen_clicked()
{
    if (!m_heightMapMode) {
        if (!saveChanges(false)) return;

        QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), getTodayDirectory(), tr("G-Code files (*.nc *.ncc *.ngc *.tap *.txt);;All files (*.*)"));

        if (fileName != "") {
            addRecentFile(fileName);
            updateRecentFilesMenu();

            loadFile(fileName);
        }
    } else {
        if (!saveChanges(true)) return;

        QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), getTodayDirectory(), tr("Heightmap files (*.map)"));

        if (fileName != "") {
            addRecentHeightmap(fileName);
            updateRecentFilesMenu();
            loadHeightMap(fileName);
        }
    }
}

void frmMain::resetHeightmap()
{
    delete m_heightMapInterpolationDrawer.data();
    m_heightMapInterpolationDrawer.setData(NULL);
//    updateHeightMapInterpolationDrawer();

    ui->tblHeightMap->setModel(NULL);
    m_heightMapModel.resize(1, 1);

    ui->heightmapFile->clear();
    m_heightMapFileName.clear();
    m_heightMapChanged = false;
}

void frmMain::loadFile(QList<QString> data)
{
    Settings * s = UserSettings::getInstance();
    QTime time;
    time.start();

    // Reset tables
    clearTable();
    m_probeModel.clear();
    m_programHeightmapModel.clear();
    m_currentModel = &m_programModel;

    // Reset parsers
    m_viewParser.reset();
    m_probeParser.reset();

    // Reset code drawer
    m_currentDrawer = m_codeDrawer;

    // Update interface
    ui->heightmapUse->setChecked(false);
    ui->grpHeightmap->setProperty("overrided", false);
    style()->unpolish(ui->grpHeightmap);
    ui->grpHeightmap->ensurePolished();

    // Reset tableview
    QByteArray headerState = ui->tblProgram->horizontalHeader()->saveState();
    ui->tblProgram->setModel(NULL);

    // Prepare parser
    GcodeParser gp;
    gp.setTraverseSpeed(s->getDouble("rapidSpeed"));

    qDebug() << "Prepared to load:" << time.elapsed();
    time.start();

    // Block parser updates on table changes
    m_programLoading = true;

    QString command;
    QString stripped;
    QList<QString> args;

    while (!data.isEmpty())
    {
        command = data.takeFirst();

        // Trim & split command
        stripped = GcodePreprocessorUtils::removeComment(command);
        args = GcodePreprocessorUtils::splitCommand(stripped);

        PointSegment *ps = gp.addCommand(args);
        // Quantum line (if disable pointsegment check some points will have qQNaN() number on raspberry)
        // code alignment?
        if (ps && (qIsNaN(ps->point()->x()) || qIsNaN(ps->point()->y()) || qIsNaN(ps->point()->z())))
                   qDebug() << "nan point segment added:" << *ps->point();

        m_programModel.setData(m_programModel.index(m_programModel.rowCount() - 1, 1), command);
        m_programModel.setData(m_programModel.index(m_programModel.rowCount() - 2, 2), tr("In queue"));
        m_programModel.setData(m_programModel.index(m_programModel.rowCount() - 2, 4), gp.getCommandNumber());
        // Store splitted args to speed up future parser updates
        m_programModel.setData(m_programModel.index(m_programModel.rowCount() - 2, 5), QVariant(args));
    }

    updateProgramEstimatedTime(m_viewParser.getLinesFromParser(&gp, s->getDouble("arcPrecision"), s->getBool("arcDegreeMode")));

    qDebug() << "model filled:" << time.elapsed();
    time.start();

    m_programLoading = false;

    // Set table model
    ui->tblProgram->setModel(&m_programModel);
    ui->tblProgram->horizontalHeader()->restoreState(headerState);

    // Update tableview
    connect(ui->tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));
    ui->tblProgram->selectRow(0);

    qDebug() << "view parser filled:" << time.elapsed();

    //  Update code drawer
    m_codeDrawer->update();
    ui->glwVisualizer->fitDrawable(m_codeDrawer);

    resetHeightmap();
    updateControlsState();
}

void frmMain::loadFile(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, this->windowTitle(), tr("Can't open file:\n") + fileName);
        return;
    }

    qDebug() << "Load file " << fileName;
    m_settings->set("lastUsedFile", fileName);
    // Set filename
    m_programFileName = fileName;

    // Prepare text stream
    QTextStream textStream(&file);

    // Read lines
    QList<QString> data;
    while (!textStream.atEnd()) data.append(textStream.readLine());

    // Load lines
    loadFile(data);
}

QTime frmMain::updateProgramEstimatedTime(QList<LineSegment*> lines)
{
    double time = 0;

    for (int i = 0; i < lines.count(); i++) {
        LineSegment *ls = lines[i];
    //    foreach (LineSegment *ls, lines) {
        double length = (ls->getEnd() - ls->getStart()).length();

        if (!qIsNaN(length) && !qIsNaN(ls->getSpeed()) && ls->getSpeed() != 0) time +=
                length / ((ui->chkFeedOverride->isChecked() && !ls->isFastTraverse())
                          ? (ls->getSpeed() * ui->txtFeed->value() / 100) : ls->getSpeed());

//        qDebug() << "length/time:" << length << ((ui->chkFeedOverride->isChecked() && !ls->isFastTraverse())
//                                                 ? (ls->getSpeed() * ui->txtFeed->value() / 100) : ls->getSpeed())
//                 << time;

        if (qIsNaN(length)) qDebug() << "length nan:" << i << ls->getLineNumber() << ls->getStart() << ls->getEnd();
        if (qIsNaN(ls->getSpeed())) qDebug() << "speed nan:" << ls->getSpeed();
    }

    time *= 60;

    QTime t;

    t.setHMS(0, 0, 0);
    t = t.addSecs(time);

    ui->glwVisualizer->setSpendTime(QTime(0, 0, 0));
    ui->glwVisualizer->setEstimatedTime(t);

    return t;
}

void frmMain::clearTable()
{
    m_programModel.clear();
    m_programModel.insertRow(0);
}

void frmMain::on_cmdFit_clicked()
{
    ui->glwVisualizer->fitDrawable(m_currentDrawer);
}

void frmMain::on_cmdFileSend_clicked()
{
    if(m_settings->getBool("checkedHeightMap") && m_projectDirectory.length() == 0) {
        m_projectDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                               m_projectDirectory,
                                                               QFileDialog::ShowDirsOnly
                                                               | QFileDialog::DontResolveSymlinks);
        m_settings->set("projectDirectory", m_projectDirectory);
    }
    if (m_currentModel->rowCount() == 1) return;
    on_cmdFileReset_clicked();

    m_storedKeyboardControl = ui->chkKeyboardControl->isChecked();
    ui->chkKeyboardControl->setChecked(false);

    if (!ui->chkTestMode->isChecked()) storeOffsets(); // Allready stored on check
    storeParserState();

#ifdef WINDOWS
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
        if (m_taskBarProgress) {
            m_taskBarProgress->setMaximum(m_currentModel->rowCount() - 2);
            m_taskBarProgress->setValue(0);
            m_taskBarProgress->show();
        }
    }
#endif

    updateControlsState();
    ui->cmdFilePause->setFocus();

    m_startTime.start();

    if(m_heightMapMode && m_settings->getBool("checkedHeightMap")) {
        // Some staff
        m_heghtMapIndex = 0;
        sendNextFileCommands();
        qDebug() << "---- Finish";
    } else {
        sendNextFileCommands();
    }
}

void frmMain::on_cmdFileAbort_clicked()
{
    m_aborting = true;
    if (!ui->chkTestMode->isChecked()) {
        m_serialPort.write("!");
    } else {
        m_serialPort.resetDevice();
    }
}

void frmMain::storeParserState()
{
    m_storedParserStatus = ui->glwVisualizer->parserStatus().remove(
                QRegExp("\\[|\\]|G[01234]\\s|M[0345]+\\s|\\sF[\\d\\.]+|\\sS[\\d\\.]+"));
}

void frmMain::restoreParserState()
{
    if (!m_storedParserStatus.isEmpty()) sendCommand(m_storedParserStatus, -1, m_settings->getBool("showUICommands"));
}

void frmMain::storeOffsets()
{
//    sendCommand("$#", -2, m_settings->getBool("showUICommands"));
}

void frmMain::restoreOffsets()
{
    // Still have pre-reset working position
    sendCommand(QString("G21G53G90X%1Y%2Z%3").arg(toMetric(m_serialPort.getCoordinate(X)))
                                       .arg(toMetric(m_serialPort.getCoordinate(Y)))
                                       .arg(toMetric(m_serialPort.getCoordinate(Z))), -1, m_settings->getBool("showUICommands"));
    sendCommand(QString("G21G92X%1Y%2Z%3").arg(toMetric(m_serialPort.getCoordinate(X)))
                                       .arg(toMetric(m_serialPort.getCoordinate(Y)))
                                       .arg(toMetric(m_serialPort.getCoordinate(Z))), -1, m_settings->getBool("showUICommands"));
}

void frmMain::sendNextFileCommands() {
    if (m_queue.length() > 0) return;

    // Moved from cmdFileSend_click
    m_transferCompleted = false;
    m_fileEndSent = false;
    m_processingFile = true;

    QString command = feedOverride(m_currentModel->data(m_currentModel->index(m_fileCommandIndex, 1)).toString());

    while ((bufferLength() + command.length() + 1) <= BUFFERLENGTH
           && m_fileCommandIndex < m_currentModel->rowCount() - 1
           && !(!m_commands.isEmpty() && m_commands.last().command.contains(QRegExp("M0*2|M30")))) {
        m_currentModel->setData(m_currentModel->index(m_fileCommandIndex, 2), tr("Sent"));
        sendCommand(command, m_fileCommandIndex, m_settings->getBool("showProgramCommands"));
        m_fileCommandIndex++;
        command = feedOverride(m_currentModel->data(m_currentModel->index(m_fileCommandIndex, 1)).toString());
    }
}

void frmMain::onTableCellChanged(QModelIndex i1, QModelIndex i2)
{
    Q_UNUSED(i2)

    GCodeTableModel *model = (GCodeTableModel*)sender();

    if (i1.column() != 1) return;
    // Inserting new line at end
    if (i1.row() == (model->rowCount() - 1) && model->data(model->index(i1.row(), 1)).toString() != "") {
        model->setData(model->index(model->rowCount() - 1, 2), tr("In queue"));
        model->insertRow(model->rowCount());
        if (!m_programLoading) ui->tblProgram->setCurrentIndex(model->index(i1.row() + 1, 1));
    // Remove last line
    } /*else if (i1.row() != (model->rowCount() - 1) && model->data(model->index(i1.row(), 1)).toString() == "") {
        ui->tblProgram->setCurrentIndex(model->index(i1.row() + 1, 1));
        m_tableModel.removeRow(i1.row());
    }*/

    if (!m_programLoading) {

        // Clear cached args
        model->setData(model->index(i1.row(), 5), QVariant());

        // Drop heightmap cache
        if (m_currentModel == &m_programModel) m_programHeightmapModel.clear();

        // Update visualizer
        updateParser();

        // Hightlight w/o current cell changed event (double hightlight on current cell changed)
        QList<LineSegment*> list = m_viewParser.getLineSegmentList();
        for (int i = 0; i < list.count() && list[i]->getLineNumber() <= m_currentModel->data(m_currentModel->index(i1.row(), 4)).toInt(); i++) {
            list[i]->setIsHightlight(true);
        }
    }
}

void frmMain::onTableCurrentChanged(QModelIndex idx1, QModelIndex idx2)
{
    // Update toolpath hightlighting
    if (idx1.row() > m_currentModel->rowCount() - 2) idx1 = m_currentModel->index(m_currentModel->rowCount() - 2, 0);
    if (idx2.row() > m_currentModel->rowCount() - 2) idx2 = m_currentModel->index(m_currentModel->rowCount() - 2, 0);

    // Update linesegments on cell changed
    if (!m_currentDrawer->geometryUpdated()) {
        QList<LineSegment*> list = m_viewParser.getLineSegmentList();
        for (int i = 0; i < list.count(); i++) {
            list[i]->setIsHightlight(list[i]->getLineNumber() <= m_currentModel->data(m_currentModel->index(idx1.row(), 4)).toInt());
        }
    // Update vertices on current cell changed
    } else {
        GcodeViewParse *parser = m_currentDrawer->viewParser();
        QList<LineSegment*> list = parser->getLineSegmentList();
        int segmentLine;
        int modelLine = m_currentModel->data(m_currentModel->index(idx1.row(), 4)).toInt();
        int modelLinePrevious = m_currentModel->data(m_currentModel->index(idx2.row(), 4)).toInt();
        QList<int> indexes;

        for (int i = 0; i < list.count(); i++) {
            segmentLine = list[i]->getLineNumber();
            // Highlight
            if (idx1.row() > idx2.row()) {
                if (segmentLine > modelLinePrevious && segmentLine <= modelLine) {
                    list[i]->setIsHightlight(true);
                    indexes.append(i);
                }
            // Reset
            } else {
                if (segmentLine <= modelLinePrevious && segmentLine > modelLine) {
                    list[i]->setIsHightlight(false);
                    indexes.append(i);
                }
            }
        }

        if (!indexes.isEmpty()) m_currentDrawer->update(indexes);
    }
}

void frmMain::onTableInsertLine()
{
    if (ui->tblProgram->selectionModel()->selectedRows().count() == 0 || m_processingFile) return;

    int row = ui->tblProgram->selectionModel()->selectedRows()[0].row();

    m_currentModel->insertRow(row);
    m_currentModel->setData(m_currentModel->index(row, 2), tr("In queue"));

    updateParser();
    m_cellChanged = true;
    ui->tblProgram->selectRow(row);
}

void frmMain::onTableDeleteLines()
{
    if (ui->tblProgram->selectionModel()->selectedRows().count() == 0 || m_processingFile ||
            QMessageBox::warning(this, this->windowTitle(), tr("Delete lines?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return;

    QModelIndex firstRow = ui->tblProgram->selectionModel()->selectedRows()[0];
    int rowsCount = ui->tblProgram->selectionModel()->selectedRows().count();

    for (int i = 0; i < rowsCount && firstRow.row() != m_currentModel->rowCount() - 1; i++) {
        m_currentModel->removeRow(firstRow.row());
    }

    // Drop heightmap cache
    if (m_currentModel == &m_programModel) m_programHeightmapModel.clear();

    updateParser();
    m_cellChanged = true;
    ui->tblProgram->selectRow(firstRow.row());
}

void frmMain::on_actServiceSettings_triggered()
{
    _frmSettings.exec();
}

bool buttonLessThan(StyledToolButton *b1, StyledToolButton *b2)
{
    return b1->text().toDouble() < b2->text().toDouble();
}

void frmMain::applySettings() {
    Settings *s = UserSettings::getInstance();

    m_originDrawer->setLineWidth(s->getDouble("lineWidth"));
    m_toolDrawer.setToolDiameter(s->getDouble("toolDiameter"));
    m_toolDrawer.setToolLength(s->getDouble("toolLength"));
    m_toolDrawer.setLineWidth(s->getDouble("lineWidth"));
    m_codeDrawer->setLineWidth(s->getDouble("lineWidth"));
    m_heightMapBorderDrawer.setLineWidth(s->getDouble("lineWidth"));
    m_heightMapGridDrawer.setLineWidth(0.2);
    m_heightMapInterpolationDrawer.setLineWidth(s->getDouble("lineWidth"));
    ui->glwVisualizer->setLineWidth(s->getDouble("lineWidth"));

    m_toolDrawer.setToolAngle(s->getInt("toolType") == 0 ? 180 : s->getDouble("toolAngle"));
    m_toolDrawer.setColor(QColor(s->get("toolColor")));
    m_toolDrawer.update();

    ui->glwVisualizer->setAntialiasing(s->getBool("antialiasing"));
    ui->glwVisualizer->setMsaa(s->getBool("msaa"));
    ui->glwVisualizer->setZBuffer(s->getBool("zBuffer"));
    ui->glwVisualizer->setVsync(s->getBool("vsync"));
    ui->glwVisualizer->setFps(s->getInt("fps"));
    ui->glwVisualizer->setColorBackground(QColor(s->get("visualizerBackgroundColor")));
    ui->glwVisualizer->setColorText(QColor(s->get("visualizerTextColor")));

    ui->txtSpindleSpeed->setMinimum(s->getInt("spindleSpeedMin"));
    ui->txtSpindleSpeed->setMaximum(s->getInt("spindleSpeedMax"));
    ui->sliSpindleSpeed->setMinimum(ui->txtSpindleSpeed->minimum() / 100);
    ui->sliSpindleSpeed->setMaximum(ui->txtSpindleSpeed->maximum() / 100);

    ui->scrollArea->setVisible(s->getBool("panelHeightmap") || s->getBool("panelFeed")
                               || s->getBool("panelJog") || s->getBool("panelSpindle"));

    ui->grpHeightmap->setVisible(s->getBool("panelHeightmap"));
    ui->grpSpindle->setVisible(s->getBool("panelSpindle"));
    ui->grpFeed->setVisible(s->getBool("panelFeed"));
    ui->grpJog->setVisible(s->getBool("panelJog"));

    ui->cboCommand->setAutoCompletion(s->getBool("autoCompletion"));

    m_codeDrawer->setSimplify(s->getBool("simplify"));
    m_codeDrawer->setSimplifyPrecision(s->getBool("simplify"));
    m_codeDrawer->setColorNormal(QColor(s->get("toolpathNormalColor")));
    m_codeDrawer->setColorDrawn(QColor(s->get("toolpathDrawnColor")));
    m_codeDrawer->setColorHighlight(QColor(s->get("toolpathHighlightColor")));
    m_codeDrawer->setColorZMovement(QColor(s->get("toolpathZMovementColor")));
    m_codeDrawer->setColorStart(QColor(s->get("toolpathStartColor")));
    m_codeDrawer->setColorEnd(QColor(s->get("toolpathEndColor")));
    m_codeDrawer->update();

    // Adapt visualizer buttons colors
    const int LIGHTBOUND = 127;
    const int NORMALSHIFT = 40;
    const int HIGHLIGHTSHIFT = 80;

    QColor base = QColor(s->get("visualizerBackgroundColor"));
    bool light = base.value() > LIGHTBOUND;
    QColor normal, highlight;

    normal.setHsv(base.hue(), base.saturation(), base.value() + (light ? -NORMALSHIFT : NORMALSHIFT));
    highlight.setHsv(base.hue(), base.saturation(), base.value() + (light ? -HIGHLIGHTSHIFT : HIGHLIGHTSHIFT));

    ui->glwVisualizer->setStyleSheet(QString("QToolButton {border: 1px solid %1; \
                background-color: %3} QToolButton:hover {border: 1px solid %2;}")
                .arg(normal.name()).arg(highlight.name())
                .arg(base.name()));

    ui->cmdFit->setIcon(QIcon(":/images/fit_1.png"));
    ui->cmdIsometric->setIcon(QIcon(":/images/cube.png"));
    ui->cmdFront->setIcon(QIcon(":/images/cubeFront.png"));
    ui->cmdLeft->setIcon(QIcon(":/images/cubeLeft.png"));
    ui->cmdTop->setIcon(QIcon(":/images/cubeTop.png"));

    if (!light) {
        Util::invertButtonIconColors(ui->cmdFit);
        Util::invertButtonIconColors(ui->cmdIsometric);
        Util::invertButtonIconColors(ui->cmdFront);
        Util::invertButtonIconColors(ui->cmdLeft);
        Util::invertButtonIconColors(ui->cmdTop);
    }

    ui->cboCommand->setMinimumHeight(ui->cboCommand->height());
    ui->cmdClearConsole->setFixedHeight(ui->cboCommand->height());
    ui->cmdCommandSend->setFixedHeight(ui->cboCommand->height());
}

void frmMain::updateParser()
{
    Settings * s = UserSettings::getInstance();
    QTime time;

    qDebug() << "updating parser:" << m_currentModel << m_currentDrawer;
    time.start();

    GcodeViewParse *parser = m_currentDrawer->viewParser();

    GcodeParser gp;
    gp.setTraverseSpeed(s->getInt("rapidSpeed"));

    ui->tblProgram->setUpdatesEnabled(false);

    QString stripped;
    QList<QString> args;

    for (int i = 0; i < m_currentModel->rowCount() - 1; i++) {
        // Get stored args
        args = m_currentModel->data(m_currentModel->index(i, 5)).toStringList();

        // Store args if none
        if (args.isEmpty()) {
            qDebug() << "updating args" << i;
            stripped = GcodePreprocessorUtils::removeComment(m_currentModel->data(m_currentModel->index(i, 1)).toString());
            args = GcodePreprocessorUtils::splitCommand(stripped);
            m_currentModel->setData(m_currentModel->index(i, 5), QVariant(args));
        }

        // Add command to parser
        gp.addCommand(args);

        // Update table model
        m_currentModel->setData(m_currentModel->index(i, 2), tr("In queue"));
        m_currentModel->setData(m_currentModel->index(i, 3), "");
        m_currentModel->setData(m_currentModel->index(i, 4), gp.getCommandNumber());
    }

    ui->tblProgram->setUpdatesEnabled(true);

    parser->reset();

    updateProgramEstimatedTime(parser->getLinesFromParser(&gp, s->getDouble("arcPrecision"), s->getBool("arcDegreeMode")));
    m_currentDrawer->update();
    ui->glwVisualizer->updateExtremes(m_currentDrawer);
    updateControlsState();

    if (m_currentModel == &m_programModel) m_fileChanged = true;

    qDebug() << "Update parser time: " << time.elapsed();
}

void frmMain::on_cmdCommandSend_clicked()
{
    QString command = ui->cboCommand->currentText();
    if (command.isEmpty()) return;

    ui->cboCommand->storeText();
    ui->cboCommand->setCurrentText("");
    sendCommand(command, -1);
}

void frmMain::on_actFileOpen_triggered()
{
    on_cmdFileOpen_clicked();
}

void frmMain::on_cmdHome_clicked()
{
    m_homing = true;
    m_updateSpindleSpeed = true;
    sendCommand("$H", -1, m_settings->getBool("showUICommands"));
}

void frmMain::on_cmdTouch_clicked()
{
//    m_homing = true;

    QStringList list = UserSettings::getInstance()->get("touchCommand").split(";");

    foreach (QString cmd, list) {
        sendCommand(cmd.trimmed(), -1, m_settings->getBool("showUICommands"));
    }
}

void frmMain::on_cmdZeroXY_clicked()
{
    m_settingZeroXY = true;
    sendCommand("G92X0Y0", -1, m_settings->getBool("showUICommands"));
    sendCommand("$#", -2, m_settings->getBool("showUICommands"));
}

void frmMain::on_cmdZeroZ_clicked()
{
    m_settingZeroZ = true;
    sendCommand("G92Z0", -1, m_settings->getBool("showUICommands"));
    sendCommand("$#", -2, m_settings->getBool("showUICommands"));
}

//void frmMain::on_cmdRestoreOrigin_clicked()
//{
//    // Restore offset
//    sendCommand(QString("G21"), -1, m_settings->getBool("showUICommands"));
//    sendCommand(QString("G53G90G0X%1Y%2Z%3").arg(toMetric(m_serialPort.getCoordinate(X)))
//                                            .arg(toMetric(m_serialPort.getCoordinate(Y)))
//                                            .arg(toMetric(m_serialPort.getCoordinate(Z))), -1, m_settings->getBool("showUICommands"));
//    sendCommand(QString("G92X%1Y%2Z%3").arg(toMetric(m_serialPort.getCoordinate(X)) - m_storedX)
//                                        .arg(toMetric(m_serialPort.getCoordinate(Y)) - m_storedY)
//                                        .arg(toMetric(m_serialPort.getCoordinate(Z)) - m_storedZ), -1, m_settings->getBool("showUICommands"));

//    // Move tool
//    if (s->getBool("moveOnRestore")) switch (m_settings.restoreMode()) {
//    case 0:
//        sendCommand("G0X0Y0", -1, m_settings->getBool("showUICommands"));
//        break;
//    case 1:
//        sendCommand("G0X0Y0Z0", -1, m_settings->getBool("showUICommands"));
//        break;
//    }
//}

void frmMain::on_cmdReset_clicked()
{
    m_serialPort.resetDevice();
}

void frmMain::on_cmdUnlock_clicked()
{
    m_updateSpindleSpeed = true;
    sendCommand("$X", -1, m_settings->getBool("showUICommands"));
}

//void frmMain::on_cmdSafePosition_clicked()
//{
//    QStringList list = m_settings.safePositionCommand().split(";");

//    foreach (QString cmd, list) {
//        sendCommand(cmd.trimmed(), -1, m_settings->getBool("showUICommands"));
//    }
//}

void frmMain::on_cmdSpindle_toggled(bool checked)
{
    if (!m_programSpeed) sendCommand(checked ? QString("M3 S%1").arg(ui->txtSpindleSpeed->text()) : "M5", -1, m_settings->getBool("showUICommands"));
    ui->grpSpindle->setProperty("overrided", checked);
    style()->unpolish(ui->grpSpindle);
    ui->grpSpindle->ensurePolished();

    if (checked) {
        if (!ui->grpSpindle->isChecked()) ui->grpSpindle->setTitle(tr("Spindle") + QString(tr(" (%1)")).arg(ui->txtSpindleSpeed->text()));
    } else {
        ui->grpSpindle->setTitle(tr("Spindle"));
    }
}

void frmMain::on_txtSpindleSpeed_editingFinished()
{
    ui->txtSpindleSpeed->setStyleSheet("color: red;");
    m_programSpeed = true;
    ui->sliSpindleSpeed->setValue(ui->txtSpindleSpeed->value() / 100);
    m_programSpeed = false;
    m_updateSpindleSpeed = true;
}

void frmMain::on_sliSpindleSpeed_valueChanged(int value)
{
    Q_UNUSED(value)

    if (!m_programSpeed) {
        ui->txtSpindleSpeed->setValue(ui->sliSpindleSpeed->value() * 100);
//        sendCommand(QString("S%1").arg(ui->sliSpindleSpeed->value() * 100), -2);
        m_updateSpindleSpeed = true;
    }

    ui->txtSpindleSpeed->setStyleSheet("color: red;");

    if (!ui->grpSpindle->isChecked() && ui->cmdSpindle->isChecked())
        ui->grpSpindle->setTitle(tr("Spindle") + QString(tr(" (%1)")).arg(ui->txtSpindleSpeed->text()));
}

void frmMain::on_cmdYPlus_clicked()
{
    // Query parser state to restore coordinate system, hide from table and console
    sendCommand("$G", -2, m_settings->getBool("showUICommands"));
    sendCommand("G91G0Y" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
}

void frmMain::on_cmdYMinus_clicked()
{
    sendCommand("$G", -2, m_settings->getBool("showUICommands"));
    sendCommand("G91G0Y-" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
}

void frmMain::on_cmdXPlus_clicked()
{
    sendCommand("$G", -2, m_settings->getBool("showUICommands"));
    sendCommand("G91G0X" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
}

void frmMain::on_cmdXMinus_clicked()
{
    sendCommand("$G", -2, m_settings->getBool("showUICommands"));
    sendCommand("G91G0X-" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
}

void frmMain::on_cmdZPlus_clicked()
{
    sendCommand("$G", -2, m_settings->getBool("showUICommands"));
    sendCommand("G91G0Z" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
}

void frmMain::on_cmdZMinus_clicked()
{
    sendCommand("$G", -2, m_settings->getBool("showUICommands"));
    sendCommand("G91G0Z-" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
}

void frmMain::on_chkTestMode_clicked(bool checked)
{
    if (checked) {
        storeOffsets();
        storeParserState();
        sendCommand("$C", -1, m_settings->getBool("showUICommands"));
    } else {
        m_aborting = true;
        m_serialPort.resetDevice();
    };
}

void frmMain::on_cmdFilePause_clicked(bool checked)
{
    m_serialPort.write(checked ? "!" : "~");
}

void frmMain::on_cmdFileReset_clicked()
{
    m_fileCommandIndex = 0;
    m_fileProcessedCommandIndex = 0;
    m_lastDrawnLineIndex = 0;
    m_probeIndex = -1;

    if (!m_heightMapMode) {
        QTime time;

        time.start();

        QList<LineSegment*> list = m_viewParser.getLineSegmentList();

        QList<int> indexes;
        for (int i = m_lastDrawnLineIndex; i < list.count(); i++) {
            list[i]->setDrawn(false);
            indexes.append(i);
        }
        m_codeDrawer->update(indexes);

        qDebug() << "drawn false:" << time.elapsed();

        time.start();

        ui->tblProgram->setUpdatesEnabled(false);
        for (int i = 0; i < m_currentModel->rowCount() - 1; i++) {
            m_currentModel->setData(m_currentModel->index(i, 2), tr("In queue"));
            m_currentModel->setData(m_currentModel->index(i, 3), "");
        }
        ui->tblProgram->setUpdatesEnabled(true);

        qDebug() << "table updated:" << time.elapsed();

        ui->tblProgram->scrollTo(m_currentModel->index(0, 0));
        ui->tblProgram->clearSelection();
        ui->tblProgram->selectRow(0);

        ui->glwVisualizer->setSpendTime(QTime(0, 0, 0));
    } else {
        ui->heightmapGridX->setEnabled(true);
        ui->heightmapGridY->setEnabled(true);
        ui->heightmapGridZBottom->setEnabled(true);
        ui->heightmapGridZTop->setEnabled(true);

        delete m_heightMapInterpolationDrawer.data();
        m_heightMapInterpolationDrawer.setData(NULL);

        m_heightMapModel.clear();
        updateHeightMapGrid();
    }
}

void frmMain::on_actFileNew_triggered()
{
    qDebug() << "changes:" << m_fileChanged << m_heightMapChanged;

    if (!saveChanges(m_heightMapMode)) return;

    if (!m_heightMapMode) {
        // Reset tables
        clearTable();
        m_probeModel.clear();
        m_programHeightmapModel.clear();
        m_currentModel = &m_programModel;

        // Reset parsers
        m_viewParser.reset();
        m_probeParser.reset();

        // Reset code drawer
        m_codeDrawer->update();
        m_currentDrawer = m_codeDrawer;
        ui->glwVisualizer->fitDrawable();
        ui->glwVisualizer->setSpendTime(QTime(0, 0, 0));
        ui->glwVisualizer->setEstimatedTime(QTime(0, 0, 0));

        m_programFileName = "";
        ui->heightmapUse->setChecked(false);
        ui->grpHeightmap->setProperty("overrided", false);
        style()->unpolish(ui->grpHeightmap);
        ui->grpHeightmap->ensurePolished();

        // Reset tableview
        QByteArray headerState = ui->tblProgram->horizontalHeader()->saveState();
        ui->tblProgram->setModel(NULL);

        // Set table model
        ui->tblProgram->setModel(&m_programModel);
        ui->tblProgram->horizontalHeader()->restoreState(headerState);

        // Update tableview
        connect(ui->tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));
        ui->tblProgram->selectRow(0);

        resetHeightmap();
    } else {
        m_heightMapModel.clear();
        on_cmdFileReset_clicked();
        ui->heightmapFile->setText(tr("Untitled"));
        m_heightMapFileName.clear();

        updateHeightMapBorderDrawer();
        updateHeightMapGrid();

        m_heightMapChanged = false;
    }

    updateControlsState();
}

void frmMain::on_cmdClearConsole_clicked()
{
    ui->txtConsole->clear();
}

bool frmMain::saveProgramToFile(QString fileName, GCodeTableModel *model)
{
    QFile file(fileName);
    QDir dir;

    qDebug() << "Saving program";

    if (file.exists()) dir.remove(file.fileName());
    if (!file.open(QIODevice::WriteOnly)) return false;

    QTextStream textStream(&file);

    for (int i = 0; i < model->rowCount() - 1; i++) {
        textStream << model->data(model->index(i, 1)).toString() << "\r\n";
    }

    file.close();

    return true;
}

void frmMain::on_actFileSaveTransformedAs_triggered()
{
    QString fileName = (QFileDialog::getSaveFileName(this, tr("Save file as"), "", tr("G-Code files (*.nc *.ncc *.ngc *.tap *.txt)")));

    if (!fileName.isEmpty()) {
        saveProgramToFile(fileName, &m_programHeightmapModel);
    }
}

void frmMain::on_actFileSaveAs_triggered()
{
    if (!m_heightMapMode) {
        QString fileName = (QFileDialog::getSaveFileName(this, tr("Save file as"), "", tr("G-Code files (*.nc *.ncc *.ngc *.tap *.txt)")));

        if (!fileName.isEmpty()) if (saveProgramToFile(fileName, &m_programModel)) {
            m_programFileName = fileName;
            m_fileChanged = false;

            addRecentFile(fileName);
            updateRecentFilesMenu();

            updateControlsState();
        }
    } else {
        QString fileName = (QFileDialog::getSaveFileName(this, tr("Save file as"), "", tr("Heightmap files (*.map)")));

        if (!fileName.isEmpty()) if (saveHeightMap(fileName)) {
            ui->heightmapFile->setText(fileName.mid(fileName.lastIndexOf("/") + 1));
            m_heightMapFileName = fileName;
            m_heightMapChanged = false;

            addRecentHeightmap(fileName);
            updateRecentFilesMenu();

            updateControlsState();
        }
    }
}

void frmMain::on_actFileSave_triggered()
{
    if (!m_heightMapMode) {
        // G-code saving
        if (m_programFileName.isEmpty()) on_actFileSaveAs_triggered(); else {
            saveProgramToFile(m_programFileName, &m_programModel);
            m_fileChanged = false;
        }
    } else {
        // Height map saving
        if (m_heightMapFileName.isEmpty()) on_actFileSaveAs_triggered(); else saveHeightMap(m_heightMapFileName);
    }
}

void frmMain::on_cmdTop_clicked()
{
    ui->glwVisualizer->setTopView();
}

void frmMain::on_cmdFront_clicked()
{
    ui->glwVisualizer->setFrontView();
}

void frmMain::on_cmdLeft_clicked()
{
    ui->glwVisualizer->setLeftView();
}

void frmMain::on_cmdIsometric_clicked()
{
    ui->glwVisualizer->setIsometricView();
}

void frmMain::on_actAbout_triggered()
{
    _frmAbout.exec();
}

bool frmMain::dataIsEnd(QString data) {
    QStringList ends;

    ends << "ok";
    ends << "error";
//    ends << "Reset to continue";
//    ends << "'$' for help";
//    ends << "'$H'|'$X' to unlock";
//    ends << "Caution: Unlocked";
//    ends << "Enabled";
//    ends << "Disabled";
//    ends << "Check Door";
//    ends << "Pgm End";

    foreach (QString str, ends) {
        if (data.contains(str)) return true;
    }

    return false;
}

bool frmMain::dataIsFloating(QString data) {
    QStringList ends;

    ends << "Reset to continue";
    ends << "'$H'|'$X' to unlock";
    ends << "ALARM: Soft limit";
    ends << "ALARM: Hard limit";
    ends << "Check Door";

    foreach (QString str, ends) {
        if (data.contains(str)) return true;
    }

    return false;
}

QString frmMain::feedOverride(QString command)
{
    // Feed override if not in heightmap probing mode
    if (!ui->heightmapMode->isChecked()) command = GcodePreprocessorUtils::overrideSpeed(command, ui->chkFeedOverride->isChecked() ?
        ui->txtFeed->value() : 100, &m_originalFeed);

    return command;
}

void frmMain::on_txtFeed_editingFinished()
{
    ui->sliFeed->setValue(ui->txtFeed->value());
}

void frmMain::on_sliFeed_valueChanged(int value)
{
    ui->txtFeed->setValue(value);
    updateProgramEstimatedTime(m_currentDrawer->viewParser()->getLineSegmentList());
    if (m_processingFile && ui->chkFeedOverride->isChecked()) {
        ui->txtFeed->setStyleSheet("color: red;");
        m_updateFeed = true;
    }
}

void frmMain::on_chkFeedOverride_toggled(bool checked)
{
    ui->grpFeed->setProperty("overrided", checked);
    style()->unpolish(ui->grpFeed);
    ui->grpFeed->ensurePolished();
    updateProgramEstimatedTime(m_currentDrawer->viewParser()->getLineSegmentList());
    if (m_processingFile) {
        ui->txtFeed->setStyleSheet("color: red;");
        m_updateFeed = true;
    }
}

void frmMain::on_grpFeed_toggled(bool checked)
{
    if (checked) {
        ui->grpFeed->setTitle(tr("Feed"));
    } else if (ui->chkFeedOverride->isChecked()) {
        ui->grpFeed->setTitle(tr("Feed") + QString(tr(" (%1)")).arg(ui->txtFeed->text()));
    }
    updateLayouts();

    ui->widgetFeed->setVisible(checked);
}

void frmMain::on_grpSpindle_toggled(bool checked)
{
    if (checked) {
        ui->grpSpindle->setTitle(tr("Spindle"));
    } else if (ui->cmdSpindle->isChecked()) {
        ui->grpSpindle->setTitle(tr("Spindle") + QString(tr(" (%1)")).arg(ui->txtSpindleSpeed->text()));
    }
    updateLayouts();

    ui->widgetSpindle->setVisible(checked);
}

void frmMain::on_grpJog_toggled(bool checked)
{
    if (checked) {
        ui->grpJog->setTitle(tr("Jog"));
    } else if (ui->chkKeyboardControl->isChecked()) {
        ui->grpJog->setTitle(tr("Jog") + QString(tr(" (%1)")).arg(ui->txtJogStep->text()));
    }
    updateLayouts();

    ui->widgetJog->setVisible(checked);
}

void frmMain::blockJogForRapidMovement(bool repeated) {
    m_jogBlock = true;

    Settings *set = UserSettings::getInstance();

    const double acc = set->getDouble("acceleration");    // Acceleration mm/sec^2
    double v = set->getInt("rapidSpeed") / 60;         // Rapid speed mm/sec
    double at = v / acc;                                // Acceleration time
    double s = acc * at * at / 2;                       // Distance on acceleration
    double time;
    double step = ui->txtJogStep->text().toDouble();

    if (repeated) {
        time = step / v;
    } else {
        if (2 * s > step) {
            time = sqrt(step / acc);
        } else {
            time = (step - 2 * s) / v + 1 * at;
        }
    }

//    qDebug() << QString("acc: %1; v: %2; at: %3; s: %4; time: %5").arg(acc).arg(v).arg(at).arg(s).arg(time);
    QTimer::singleShot(time * 1000, Qt::PreciseTimer, this, SLOT(onJogTimer()));
}

bool frmMain::eventFilter(QObject *obj, QEvent *event)
{
    // Main form events
    if (obj == this || obj == ui->tblProgram) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if (!m_processingFile && keyEvent->key() == Qt::Key_ScrollLock && obj == this) {
                ui->chkKeyboardControl->toggle();
            }

            if (!m_processingFile && ui->chkKeyboardControl->isChecked()) {
                // Block only autorepeated keypresses
                if (keyIsMovement(keyEvent->key()) && !(m_jogBlock && keyEvent->isAutoRepeat())) {
                    blockJogForRapidMovement(keyEvent->isAutoRepeat());

                    switch (keyEvent->key()) {
                    case Qt::Key_4:
                        sendCommand("G91G0X-" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
                        break;
                    case Qt::Key_6:
                        sendCommand("G91G0X" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
                        break;
                    case Qt::Key_8:
                        sendCommand("G91G0Y" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
                        break;
                    case Qt::Key_2:
                        sendCommand("G91G0Y-" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
                        break;
                    case Qt::Key_9:
                        sendCommand("G91G0Z" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
                        break;
                    case Qt::Key_3:
                        sendCommand("G91G0Z-" + ui->txtJogStep->text(), -1, m_settings->getBool("showUICommands"));
                        break;
                    }
                }
                else if (keyEvent->key() == Qt::Key_5 || keyEvent->key() == Qt::Key_Period) {
                    QList<StyledToolButton*> stepButtons = ui->grpJog->findChildren<StyledToolButton*>(QRegExp("cmdJogStep\\d"));
                    std::sort(stepButtons.begin(), stepButtons.end(), buttonLessThan);

                    for (int i = 0; i < stepButtons.count(); i++) {
                        if (stepButtons[i]->isChecked()) {

                            StyledToolButton *button = stepButtons[keyEvent->key() == Qt::Key_5
                                    ? (i == stepButtons.length() - 1 ? stepButtons.length() - 1 : i + 1)
                                    : (i == 0 ? 0 : i - 1)];

                            ui->txtJogStep->setValue(button->text().toDouble());
                            foreach (StyledToolButton* button, ui->grpJog->findChildren<StyledToolButton*>(QRegExp("cmdJogStep\\d")))
                            {
                                button->setChecked(false);
                            }
                            button->setChecked(true);

                            if (!ui->grpJog->isChecked()) {
                                ui->grpJog->setTitle(tr("Jog") + QString(tr(" (%1)")).arg(ui->txtJogStep->text()));
                            }
                            break;
                        }
                    }
                } else if (keyEvent->key() == Qt::Key_0) {
                    ui->cmdSpindle->toggle();
                } else if (keyEvent->key() == Qt::Key_7) {
                    ui->sliSpindleSpeed->setValue(ui->sliSpindleSpeed->value() + 1);
                } else if (keyEvent->key() == Qt::Key_1) {
                    ui->sliSpindleSpeed->setValue(ui->sliSpindleSpeed->value() - 1);
                }
            }

            if (obj == ui->tblProgram && m_processingFile) {
                if (keyEvent->key() == Qt::Key_PageDown || keyEvent->key() == Qt::Key_PageUp
                            || keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Up) {
                    ui->chkAutoScroll->setChecked(false);
                }
            }
        }

    // Splitter events
    } else if (obj == ui->splitPanels && event->type() == QEvent::Resize) {
        // Resize splited widgets
        onPanelsSizeChanged(ui->scrollAreaWidgetContents->sizeHint());

    // Splitter handle events
    } else if (obj == ui->splitPanels->handle(1)) {
        int minHeight = getConsoleMinHeight();
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            // Store current console group box minimum & real heights
            m_storedConsoleMinimumHeight = ui->grpConsole->minimumHeight();
            m_storedConsoleHeight = ui->grpConsole->height();

            // Update splited sizes
            ui->splitPanels->setSizes(QList<int>() << ui->scrollArea->height() << ui->grpConsole->height());

            // Set new console mimimum height
            ui->grpConsole->setMinimumHeight(qMax(minHeight, ui->splitPanels->height()
                - ui->scrollAreaWidgetContents->sizeHint().height() - ui->splitPanels->handleWidth() - 4));
            break;
        case QEvent::MouseButtonRelease:
            // Store new console minimum height if height was changed with split handle
            if (ui->grpConsole->height() != m_storedConsoleHeight) {
                ui->grpConsole->setMinimumHeight(ui->grpConsole->height());
            } else {
                ui->grpConsole->setMinimumHeight(m_storedConsoleMinimumHeight);
            }
            break;
        case QEvent::MouseButtonDblClick:
            // Switch to min/max console size
            if (ui->grpConsole->height() == minHeight || !ui->scrollArea->verticalScrollBar()->isVisible()) {
                ui->splitPanels->setSizes(QList<int>() << ui->scrollArea->minimumHeight()
                << ui->splitPanels->height() - ui->splitPanels->handleWidth() - ui->scrollArea->minimumHeight());
            } else {
                ui->grpConsole->setMinimumHeight(minHeight);
                onPanelsSizeChanged(ui->scrollAreaWidgetContents->sizeHint());
            }
            break;
        default:
            break;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

int frmMain::getConsoleMinHeight()
{
    return ui->grpConsole->height() - ui->grpConsole->contentsRect().height()
            + ui->spacerConsole->geometry().height() + ui->grpConsole->layout()->margin() * 2
            + ui->cboCommand->height();
}

void frmMain::onConsoleResized(QSize size)
{
    Q_UNUSED(size)

    int minHeight = getConsoleMinHeight();
    bool visible = ui->grpConsole->height() > minHeight;
    if (ui->txtConsole->isVisible() != visible) {
        ui->txtConsole->setVisible(visible);
    }
}

void frmMain::onPanelsSizeChanged(QSize size)
{
    ui->splitPanels->setSizes(QList<int>() << size.height() + 4
                              << ui->splitPanels->height() - size.height()
                              - 4 - ui->splitPanels->handleWidth());
}

bool frmMain::keyIsMovement(int key)
{
    return key == Qt::Key_4 || key == Qt::Key_6 || key == Qt::Key_8 || key == Qt::Key_2 || key == Qt::Key_9 || key == Qt::Key_3;
}

void frmMain::on_chkKeyboardControl_toggled(bool checked)
{
    ui->grpJog->setProperty("overrided", checked);
    style()->unpolish(ui->grpJog);
    ui->grpJog->ensurePolished();

    // Store/restore coordinate system
    if (checked) {
        sendCommand("$G", -2, m_settings->getBool("showUICommands"));
        if (!ui->grpJog->isChecked()) ui->grpJog->setTitle(tr("Jog") + QString(tr(" (%1)")).arg(ui->txtJogStep->text()));
    } else {
        if (m_absoluteCoordinates) sendCommand("G90", -1, m_settings->getBool("showUICommands"));
        ui->grpJog->setTitle(tr("Jog"));
    }

    if (!m_processingFile) m_storedKeyboardControl = checked;

    updateControlsState();
}

void frmMain::on_tblProgram_customContextMenuRequested(const QPoint &pos)
{
    if (m_processingFile) return;

    if (ui->tblProgram->selectionModel()->selectedRows().count() > 0) {
        m_tableMenu->actions().at(0)->setEnabled(true);
        m_tableMenu->actions().at(1)->setEnabled(ui->tblProgram->selectionModel()->selectedRows()[0].row() != m_currentModel->rowCount() - 1);
    } else {
        m_tableMenu->actions().at(0)->setEnabled(false);
        m_tableMenu->actions().at(1)->setEnabled(false);
    }
    m_tableMenu->popup(ui->tblProgram->viewport()->mapToGlobal(pos));
}

void frmMain::on_splitter_splitterMoved(int pos, int index)
{
    Q_UNUSED(pos)
    Q_UNUSED(index)

    static bool tableCollapsed = ui->splitter->sizes()[1] == 0;

    if ((ui->splitter->sizes()[1] == 0) != tableCollapsed) {
        this->setUpdatesEnabled(false);
        ui->chkAutoScroll->setVisible(ui->splitter->sizes()[1] && !m_heightMapMode);
        updateLayouts();
        resizeCheckBoxes();

        this->setUpdatesEnabled(true);
        ui->chkAutoScroll->repaint();

        // Store collapsed state
        tableCollapsed = ui->splitter->sizes()[1] == 0;
    }
}

void frmMain::updateLayouts()
{
    this->update();
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

void frmMain::addRecentFile(QString fileName)
{
    m_recentFiles.removeAll(fileName);
    m_recentFiles.append(fileName);
    if (m_recentFiles.count() > 5) m_recentFiles.takeFirst();
}

void frmMain::addRecentHeightmap(QString fileName)
{
    m_recentHeightmaps.removeAll(fileName);
    m_recentHeightmaps.append(fileName);
    if (m_recentHeightmaps.count() > 5) m_recentHeightmaps.takeFirst();
}

void frmMain::onActRecentFileTriggered()
{
    QAction *action = static_cast<QAction*>(sender());
    QString fileName = action->text();

    if (action != NULL) {
        if (!saveChanges(m_heightMapMode)) return;
        if (!m_heightMapMode) loadFile(fileName); else loadHeightMap(fileName);
    }
}

void frmMain::onCboCommandReturnPressed()
{
    QString command = ui->cboCommand->currentText();
    if (command.isEmpty()) return;

    ui->cboCommand->setCurrentText("");
    sendCommand(command, -1);
}

void frmMain::updateRecentFilesMenu()
{
    foreach (QAction * action, ui->mnuRecent->actions()) {
        if (action->text() == "") break; else {
            ui->mnuRecent->removeAction(action);
            delete action;
        }
    }

    foreach (QString file, !m_heightMapMode ? m_recentFiles : m_recentHeightmaps) {
        QAction *action = new QAction(file, this);
        connect(action, SIGNAL(triggered()), this, SLOT(onActRecentFileTriggered()));
        ui->mnuRecent->insertAction(ui->mnuRecent->actions()[0], action);
    }

    updateControlsState();
}

void frmMain::on_actRecentClear_triggered()
{
    if (!m_heightMapMode) m_recentFiles.clear(); else m_recentHeightmaps.clear();
    updateRecentFilesMenu();
}

double frmMain::toMetric(double value)
{
    return UserSettings::getInstance()->get("units") == 0 ? value : value * 25.4;
}

void frmMain::on_grpHeightmap_toggled(bool arg1)
{
    ui->widgetHeightmap->setVisible(arg1);
}

QRectF frmMain::borderRectFromTextboxes()
{
    QRectF rect;

    rect.setX(ui->heightmapBorderX->value());
    rect.setY(ui->heightmapBorderY->value());
    rect.setWidth(ui->heightmapBorderWidth->value());
    rect.setHeight(ui->heightmapBorderHeight->value());

    return rect;
}

QRectF frmMain::borderRectFromExtremes()
{
    QRectF rect;

    rect.setX(m_codeDrawer->getMinimumExtremes().x());
    rect.setY(m_codeDrawer->getMinimumExtremes().y());
    rect.setWidth(m_codeDrawer->getSizes().x());
    rect.setHeight(m_codeDrawer->getSizes().y());

    return rect;
}

void frmMain::updateHeightMapBorderDrawer()
{
    if (m_settingsLoading) return;

    qDebug() << "updating border drawer";

    m_heightMapBorderDrawer.setBorderRect(borderRectFromTextboxes());
}

void frmMain::updateHeightMapGrid(double arg1)
{
    if (sender()->property("previousValue").toDouble() != arg1 && !updateHeightMapGrid())
        static_cast<QDoubleSpinBox*>(sender())->setValue(sender()->property("previousValue").toDouble());
    else sender()->setProperty("previousValue", arg1);
}

bool frmMain::updateHeightMapGrid()
{
    if (m_settingsLoading) return true;

    qDebug() << "updating heightmap grid drawer";

    // Grid map changing warning
    bool nan = true;
    for (int i = 0; i < m_heightMapModel.rowCount(); i++)
        for (int j = 0; j < m_heightMapModel.columnCount(); j++)
            if (!qIsNaN(m_heightMapModel.data(m_heightMapModel.index(i, j), Qt::UserRole).toDouble())) {
                nan = false;
                break;
            }
    if (!nan && QMessageBox::warning(this, this->windowTitle(), tr("Changing grid settings will reset probe data. Continue?"),
                                                           QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return false;

    // Update grid drawer
    QRectF borderRect = borderRectFromTextboxes();
    m_heightMapGridDrawer.setBorderRect(borderRect);
    m_heightMapGridDrawer.setGridSize(QPointF(ui->heightmapGridX->value(), ui->heightmapGridY->value()));
    m_heightMapGridDrawer.setZBottom(ui->heightmapGridZBottom->value());
    m_heightMapGridDrawer.setZTop(ui->heightmapGridZTop->value());

    // Reset model
//    int gridPointsX = trunc(borderRect.width() / ui->heightmapGridX->value()) + 1;
//    int gridPointsY = trunc(borderRect.height() / ui->heightmapGridY->value()) + 1;
    int gridPointsX = ui->heightmapGridX->value();
    int gridPointsY = ui->heightmapGridY->value();

    m_heightMapModel.resize(gridPointsX, gridPointsY);
    ui->tblHeightMap->setModel(NULL);
    ui->tblHeightMap->setModel(&m_heightMapModel);
    resizeTableHeightMapSections();

    // Update interpolation
    updateHeightMapInterpolationDrawer(true);

    // Generate probe program
    double gridStepX = gridPointsX > 1 ? borderRect.width() / (gridPointsX - 1) : 0;
    double gridStepY = gridPointsY > 1 ? borderRect.height() / (gridPointsY - 1) : 0;

    qDebug() << "generating probe program";

    m_programLoading = true;
    m_probeModel.clear();
    m_probeModel.insertRow(0);

    m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G21G90F%1G0Z%2").
                         arg(UserSettings::getInstance()->getInt("heightmapProbingFeed")).arg(ui->heightmapGridZTop->value()));
    m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G0X0Y0"));
//                         .arg(ui->heightmapGridZTop->value()));
    m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G38.2Z%1")
                         .arg(ui->heightmapGridZBottom->value()));
    m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G0Z%1")
                         .arg(ui->heightmapGridZTop->value()));

    double x, y;

    for (int i = 0; i < gridPointsY; i++) {
        y = borderRect.top() + gridStepY * i;
        for (int j = 0; j < gridPointsX; j++) {
            x = borderRect.left() + gridStepX * (i % 2 ? gridPointsX - 1 - j : j);
            m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G0X%1Y%2")
                                 .arg(x, 0, 'f', 3).arg(y, 0, 'f', 3));
            m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G38.2Z%1")
                                 .arg(ui->heightmapGridZBottom->value()));
            m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G0Z%1")
                                 .arg(ui->heightmapGridZTop->value()));
        }
    }

    m_programLoading = false;

    if (m_currentDrawer == m_probeDrawer) updateParser();

    m_heightMapChanged = true;
    return true;
}

void frmMain::updateHeightMapInterpolationDrawer(bool reset)
{
    if (m_settingsLoading) return;

    qDebug() << "Updating interpolation";

    QRectF borderRect = borderRectFromTextboxes();
    m_heightMapInterpolationDrawer.setBorderRect(borderRect);

    QVector<QVector<double>> *interpolationData = new QVector<QVector<double>>;

    int interpolationPointsX = ui->heightmapInterpolationStepX->value();// * (ui->heightmapGridX->value() - 1) + 1;
    int interpolationPointsY = ui->heightmapInterpolationStepY->value();// * (ui->heightmapGridY->value() - 1) + 1;

    double interpolationStepX = interpolationPointsX > 1 ? borderRect.width() / (interpolationPointsX - 1) : 0;
    double interpolationStepY = interpolationPointsY > 1 ? borderRect.height() / (interpolationPointsY - 1) : 0;

    for (int i = 0; i < interpolationPointsY; i++) {
        QVector<double> row;
        for (int j = 0; j < interpolationPointsX; j++) {

            double x = interpolationStepX * j + borderRect.x();
            double y = interpolationStepY * i + borderRect.y();

            row.append(reset ? qQNaN() : Interpolation::bicubicInterpolate(borderRect, &m_heightMapModel, x, y));
        }
        interpolationData->append(row);
    }

    if (m_heightMapInterpolationDrawer.data() != NULL) {
        delete m_heightMapInterpolationDrawer.data();
    }
    m_heightMapInterpolationDrawer.setData(interpolationData);

    // Update grid drawer
    m_heightMapGridDrawer.update();

    // Heightmap changed by table user input
    if (sender() == &m_heightMapModel) m_heightMapChanged = true;

    // Reset heightmapped program model
    m_programHeightmapModel.clear();
}

void frmMain::on_heightmapBorderShow_toggled(bool checked)
{
    Q_UNUSED(checked)

    updateControlsState();
}

void frmMain::on_heightmapBorderX_valueChanged(double arg1)
{
    updateHeightMapBorderDrawer();
    updateHeightMapGrid(arg1);
}

void frmMain::on_heightmapBorderWidth_valueChanged(double arg1)
{
    updateHeightMapBorderDrawer();
    updateHeightMapGrid(arg1);
}

void frmMain::on_heightmapBorderY_valueChanged(double arg1)
{
    updateHeightMapBorderDrawer();
    updateHeightMapGrid(arg1);
}

void frmMain::on_heightmapBorderHeight_valueChanged(double arg1)
{
    updateHeightMapBorderDrawer();
    updateHeightMapGrid(arg1);
}

void frmMain::on_heightmapGridShow_toggled(bool checked)
{
    Q_UNUSED(checked)

    updateControlsState();
}

void frmMain::on_heightmapGridX_valueChanged(double arg1)
{
    updateHeightMapGrid(arg1);
}

void frmMain::on_heightmapGridY_valueChanged(double arg1)
{
    updateHeightMapGrid(arg1);
}

void frmMain::on_heightmapGridZBottom_valueChanged(double arg1)
{
    updateHeightMapGrid(arg1);
}

void frmMain::on_heightmapGridZTop_valueChanged(double arg1)
{
    updateHeightMapGrid(arg1);
}

void frmMain::on_heightmapMode_toggled(bool checked)
{
    // Update flag
    m_heightMapMode = checked;

    // Reset file progress
    m_fileCommandIndex = 0;
    m_fileProcessedCommandIndex = 0;
    m_lastDrawnLineIndex = 0;

    // Reset/restore g-code program modification on edit mode enter/exit
    if (ui->heightmapUse->isChecked()) {
        on_heightmapUse_clicked(!checked); // Update gcode program parser
//        m_codeDrawer->updateData(); // Force update data to properly shadowing
    }

    if (m_heightMapMode) {
        ui->tblProgram->setModel(&m_probeModel);
        resizeTableHeightMapSections();
        m_currentModel = &m_probeModel;
        m_currentDrawer = m_probeDrawer;
        updateParser();  // Update probe program parser
    } else {
        m_probeParser.reset();
        if (!ui->heightmapUse->isChecked()) {
            ui->tblProgram->setModel(&m_programModel);
            connect(ui->tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));
            ui->tblProgram->selectRow(0);

            resizeTableHeightMapSections();
            m_currentModel = &m_programModel;
            m_currentDrawer = m_codeDrawer;

            if (!ui->heightmapUse->isChecked()) {
                ui->glwVisualizer->updateExtremes(m_codeDrawer);
                updateProgramEstimatedTime(m_currentDrawer->viewParser()->getLineSegmentList());
            }
        }
    }

    // Shadow toolpath
    QList<LineSegment*> list = m_viewParser.getLineSegmentList();
    QList<int> indexes;
    for (int i = m_lastDrawnLineIndex; i < list.count(); i++) {
        list[i]->setDrawn(checked);
        list[i]->setIsHightlight(false);
        indexes.append(i);
    }
    // Update only vertex color.
    // If chkHeightMapUse was checked codeDrawer updated via updateParser
    if (!ui->heightmapUse->isChecked()) m_codeDrawer->update(indexes);

    updateRecentFilesMenu();
    updateControlsState();
}

bool frmMain::saveHeightMap(QString fileName)
{
    QFile file(fileName);
    QDir dir;

    if (file.exists()) dir.remove(file.fileName());
    if (!file.open(QIODevice::WriteOnly)) return false;

    QTextStream textStream(&file);
    textStream << ui->heightmapBorderX->text() << ";"
               << ui->heightmapBorderY->text() << ";"
               << ui->heightmapBorderWidth->text() << ";"
               << ui->heightmapBorderHeight->text() << "\r\n";
    textStream << ui->heightmapGridX->text() << ";"
               << ui->heightmapGridY->text() << ";"
               << ui->heightmapGridZBottom->text() << ";"
               << ui->heightmapGridZTop->text() << "\r\n";
    textStream << ui->heightmapInterpolationType->currentIndex() << ";"
               << ui->heightmapInterpolationStepX->text() << ";"
                << ui->heightmapInterpolationStepY->text() << "\r\n";

    for (int i = 0; i < m_heightMapModel.rowCount(); i++) {
        for (int j = 0; j < m_heightMapModel.columnCount(); j++) {
            textStream << m_heightMapModel.data(m_heightMapModel.index(i, j), Qt::UserRole).toString() << ((j == m_heightMapModel.columnCount() - 1) ? "" : ";");
        }
        textStream << "\r\n";
    }

    file.close();

    m_heightMapChanged = false;

    return true;
}

void frmMain::loadHeightMap(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, this->windowTitle(), tr("Can't open file:\n") + fileName);
        return;
    }
    QTextStream textStream(&file);

    m_settingsLoading = true;

    // Storing previous values
    ui->heightmapBorderX->setValue(qQNaN());
    ui->heightmapBorderY->setValue(qQNaN());
    ui->heightmapBorderWidth->setValue(qQNaN());
    ui->heightmapBorderHeight->setValue(qQNaN());

    ui->heightmapGridX->setValue(qQNaN());
    ui->heightmapGridY->setValue(qQNaN());
    ui->heightmapGridZBottom->setValue(qQNaN());
    ui->heightmapGridZTop->setValue(qQNaN());

    QList<QString> list = textStream.readLine().split(";");
    ui->heightmapBorderX->setValue(list[0].toDouble());
    ui->heightmapBorderY->setValue(list[1].toDouble());
    ui->heightmapBorderWidth->setValue(list[2].toDouble());
    ui->heightmapBorderHeight->setValue(list[3].toDouble());

    list = textStream.readLine().split(";");
    ui->heightmapGridX->setValue(list[0].toDouble());
    ui->heightmapGridY->setValue(list[1].toDouble());
    ui->heightmapGridZBottom->setValue(list[2].toDouble());
    ui->heightmapGridZTop->setValue(list[3].toDouble());

    m_settingsLoading = false;

    updateHeightMapBorderDrawer();

    m_heightMapModel.clear();   // To avoid probe data wipe message
    updateHeightMapGrid();

    list = textStream.readLine().split(";");

//    ui->chkHeightMapBorderAuto->setChecked(false);
//    ui->chkHeightMapBorderAuto->setEnabled(false);
//    ui->heightmapBorderX->setEnabled(false);
//    ui->heightmapBorderY->setEnabled(false);
//    ui->heightmapBorderWidth->setEnabled(false);
//    ui->heightmapBorderHeight->setEnabled(false);

//    ui->heightmapGridX->setEnabled(false);
//    ui->heightmapGridY->setEnabled(false);
//    ui->heightmapGridZBottom->setEnabled(false);
//    ui->heightmapGridZTop->setEnabled(false);

    for (int i = 0; i < m_heightMapModel.rowCount(); i++) {
        QList<QString> row = textStream.readLine().split(";");
        for (int j = 0; j < m_heightMapModel.columnCount(); j++) {
            m_heightMapModel.setData(m_heightMapModel.index(i, j), row[j].toDouble(), Qt::UserRole);
        }
    }

    file.close();

    ui->heightmapFile->setText(fileName.mid(fileName.lastIndexOf("/") + 1));
    m_heightMapFileName = fileName;
    m_heightMapChanged = false;

    ui->heightmapInterpolationType->setCurrentIndex(list[0].toInt());
    ui->heightmapInterpolationStepX->setValue(list[1].toDouble());
    ui->heightmapInterpolationStepY->setValue(list[2].toDouble());

    updateHeightMapInterpolationDrawer();
}

void frmMain::on_heightmapInterpolationShow_toggled(bool checked)
{
    Q_UNUSED(checked)

    updateControlsState();
}

void frmMain::on_heightmapLoad_clicked()
{
    if (!saveChanges(true)) {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), "", tr("Heightmap files (*.map)"));

    useHeightmap(fileName);
}

void frmMain::useHeightmap(QString fileName)
{
    if (fileName != "") {
        addRecentHeightmap(fileName);
        loadHeightMap(fileName);

        // If using heightmap
        if (ui->heightmapUse->isChecked() && !m_heightMapMode) {
            // Restore original file
            on_heightmapUse_clicked(false);
            // Apply heightmap
            on_heightmapUse_clicked(true);
        }

        updateRecentFilesMenu();
        updateControlsState(); // Enable 'cmdHeightMapMode' button
    }
}

void frmMain::on_heightmapInterpolationStepX_valueChanged(double arg1)
{
    Q_UNUSED(arg1)

    updateHeightMapInterpolationDrawer();
}

void frmMain::on_heightmapInterpolationStepY_valueChanged(double arg1)
{
    Q_UNUSED(arg1)

    updateHeightMapInterpolationDrawer();
}

void frmMain::on_heightmapUse_clicked(bool checked)
{
//    static bool fileChanged;

    ui->grpHeightmap->setProperty("overrided", checked);
    style()->unpolish(ui->grpHeightmap);
    ui->grpHeightmap->ensurePolished();

    if (checked) {

        // Performance test
        QTime time;

        // Store fileChanged state
//        fileChanged = m_fileChanged;

        // Reset table view
        QByteArray headerState = ui->tblProgram->horizontalHeader()->saveState();
        ui->tblProgram->setModel(NULL);

        // Set current model to prevent reseting heightmap cache
        m_currentModel = &m_programHeightmapModel;

        // Update heightmap-modificated program if not cached
        if (m_programHeightmapModel.rowCount() == 0) {

            // Modifying linesegments
            QList<LineSegment*> *list = m_viewParser.getLines();
            QRectF borderRect = borderRectFromTextboxes();
            double x, y, z;

            time.start();

            for (int i = 0; i < list->count(); i++) {
                if (!list->at(i)->isZMovement()) {
                    QList<LineSegment*> subSegments = subdivideSegment(list->at(i));

                    if (subSegments.count() > 0) {
                        delete list->at(i);
                        list->removeAt(i);
                        foreach (LineSegment* subSegment, subSegments) list->insert(i++, subSegment);
                        i--;
                    }
                }
            }

            qDebug() << "Subdivide time: " << time.elapsed();

            time.start();

            for (int i = 0; i < list->count(); i++) {
                if (i == 0) {
                    x = list->at(i)->getStart().x();
                    y = list->at(i)->getStart().y();
                    z = list->at(i)->getStart().z() + Interpolation::bicubicInterpolate(borderRect, &m_heightMapModel, x, y);
                    list->at(i)->setStart(QVector3D(x, y, z));
                } else list->at(i)->setStart(list->at(i - 1)->getEnd());

                x = list->at(i)->getEnd().x();
                y = list->at(i)->getEnd().y();
                z = list->at(i)->getEnd().z() + Interpolation::bicubicInterpolate(borderRect, &m_heightMapModel, x, y);
                list->at(i)->setEnd(QVector3D(x, y, z));
            }

            qDebug() << "Z update time (interpolation): " << time.elapsed();

            time.start();

            // Modifying g-code program
            int lastSegmentIndex = 0;
            int commandIndex;
            int lastCommandIndex = -1;
            QString command;
            QString newCommandPrefix;
            QStringList codes;

            m_programLoading = true;
        //        m_programHeightmapModel.clear();
            m_programHeightmapModel.insertRow(0);

            for (int i = 0; i < m_programModel.rowCount() - 1; i++) {
                commandIndex = m_programModel.data(m_programModel.index(i, 4)).toInt();
                command = m_programModel.data(m_programModel.index(i, 1)).toString();

                if (commandIndex < 0 || commandIndex == lastCommandIndex || lastSegmentIndex == list->count() - 1) {
                    m_programHeightmapModel.setData(m_programHeightmapModel.index(m_programHeightmapModel.rowCount() - 1, 1), command);
                } else {
                    // Get command codes
//                    codes = GcodePreprocessorUtils::splitCommand(command);
//                    qDebug() << "codes from split:" << codes;
                    // from cache
                    codes = m_programModel.data(m_programModel.index(i, 5)).toStringList();
//                    qDebug() << "codes from cache:" << codes;
                    newCommandPrefix.clear();

                    // Remove coordinates codes
                    foreach (QString code, codes) if (code.contains(QRegExp("[XxYyZzIiJjKkRr]+"))) codes.removeOne(code);
                    else newCommandPrefix.append(code);

                    // Replace arcs with lines
                    newCommandPrefix.replace(QRegExp("[Gg]0*2|[Gg]0*3"), "G1");

                    // Last motion code
                    static QString lastCode;

                    // Find first linesegment by command index
                    for (int j = lastSegmentIndex; j < list->count(); j++) {
                        if (list->at(j)->getLineNumber() == commandIndex) {

        //                        qDebug() << "Updating line:" << commandIndex << list->at(j)->getEnd().length()
        //                                 << newCommandPrefix << lastCode;

                            // If command is G0 or G1
                            if (!qIsNaN(list->at(j)->getEnd().length())
                                    && (newCommandPrefix.contains(QRegExp("[Gg]0+|[Gg]0*1"))
                                        || (!newCommandPrefix.contains(QRegExp("[Gg]|[Mm]"))
                                            && lastCode.contains(QRegExp("[Gg]0+|[Gg]0*1"))))) {

                                // Store motion code
                                if (newCommandPrefix.contains(QRegExp("[Gg]0+"))) lastCode = "G0";
                                else if (newCommandPrefix.contains(QRegExp("[Gg]0*1"))) lastCode = "G1";

                                // Create new commands for each linesegment with given command index
                                while ((j < list->count()) && (list->at(j)->getLineNumber() == commandIndex)) {
                                    x = list->at(j)->getEnd().x();
                                    y = list->at(j)->getEnd().y();
                                    z = list->at(j)->getEnd().z();

                                    if (!list->at(j)->isAbsolute()) {
                                        x -= list->at(j)->getStart().x();
                                        y -= list->at(j)->getStart().y();
                                        z -= list->at(j)->getStart().z();
                                    }

                                    if (!list->at(j)->isMetric()) {
                                        x /= 25.4;
                                        y /= 25.4;
                                        z /= 25.4;
                                    }

                                    m_programHeightmapModel.setData(m_programHeightmapModel.index(m_programHeightmapModel.rowCount() - 1, 1)
                                                           , newCommandPrefix + QString("X%1Y%2Z%3")
                                                           .arg(x, 0, 'f', 3)
                                                           .arg(y, 0, 'f', 3)
                                                           .arg(z, 0, 'f', 3));
                                    if (!newCommandPrefix.isEmpty()) newCommandPrefix.clear();
                                    j++;
                                }
                            // Copy original command if not G0 or G1
                            } else {
                                m_programHeightmapModel.setData(m_programHeightmapModel.index(m_programHeightmapModel.rowCount() - 1, 1), command);
                            }

                            lastSegmentIndex = j;
                            break;
                        }
                    }
                }
                lastCommandIndex = commandIndex;
            }
        }
        qDebug() << "Model modification time: " << time.elapsed();

        ui->tblProgram->setModel(&m_programHeightmapModel);
        ui->tblProgram->horizontalHeader()->restoreState(headerState);

        connect(ui->tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));
        ui->tblProgram->selectRow(0);

        m_programLoading = false;

        // Update parser
        m_currentDrawer = m_codeDrawer;
        updateParser();
    } else {
        QByteArray headerState = ui->tblProgram->horizontalHeader()->saveState();
        ui->tblProgram->setModel(NULL);
//        m_programHeightmapModel.clear();

        m_currentModel = &m_programModel;

        ui->tblProgram->setModel(&m_programModel);
        ui->tblProgram->horizontalHeader()->restoreState(headerState);

        connect(ui->tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));
        ui->tblProgram->selectRow(0);

        bool fileChanged = m_fileChanged;
        updateParser();
        m_fileChanged = fileChanged;
    }

    // Update menu
    ui->actFileSaveTransformedAs->setVisible(checked);
}

QList<LineSegment*> frmMain::subdivideSegment(LineSegment* segment)
{
    QList<LineSegment*> list;

    QRectF borderRect = borderRectFromTextboxes();

    double interpolationStepX = borderRect.width() / (ui->heightmapInterpolationStepX->value() - 1);
    double interpolationStepY = borderRect.height() / (ui->heightmapInterpolationStepY->value() - 1);

    double length;

    QVector3D vec = segment->getEnd() - segment->getStart();

    if (qIsNaN(vec.length())) return QList<LineSegment*>();

    if (fabs(vec.x()) / fabs(vec.y()) < interpolationStepX / interpolationStepY) length = interpolationStepY / (vec.y() / vec.length());
    else length = interpolationStepX / (vec.x() / vec.length());

    length = fabs(length);

    if (qIsNaN(length)) {
        qDebug() << "ERROR length:" << segment->getStart() << segment->getEnd();
        return QList<LineSegment*>();
    }

    QVector3D seg = vec.normalized() * length;
    int count = trunc(vec.length() / length);

    if (count == 0) return QList<LineSegment*>();

    for (int i = 0; i < count; i++) {
        LineSegment* line = new LineSegment(segment);
        line->setStart(i == 0 ? segment->getStart() : list[i - 1]->getEnd());
        line->setEnd(line->getStart() + seg);
        list.append(line);
    }

    if (list.count() > 0 && list.last()->getEnd() != segment->getEnd()) {
        LineSegment* line = new LineSegment(segment);
        line->setStart(list.last()->getEnd());
        line->setEnd(segment->getEnd());
        list.append(line);
    }

    return list;
}

void frmMain::on_heightmapCreate_clicked()
{
    ui->heightmapMode->setChecked(true);
    on_actFileNew_triggered();
}

void frmMain::on_heightmapBorderAuto_clicked()
{
    QRectF rect = borderRectFromExtremes();

    if (!qIsNaN(rect.width()) && !qIsNaN(rect.height())) {
        ui->heightmapBorderX->setValue(rect.x());
        ui->heightmapBorderY->setValue(rect.y());
        ui->heightmapBorderWidth->setValue(rect.width());
        ui->heightmapBorderHeight->setValue(rect.height());
    }
}

void frmMain::on_actionGenerate_g_code_triggered()
{
    QByteArray projectDir = m_projectDirectory.toLocal8Bit();

    /**
     * Base pcb2gcode params
     * --tile-x=1 --tile-y=1 --metric=true --metricoutput=true --nog64=true --optimise=false --tolerance=0.0100 --vectorial=true --zchange=12.0000 --zchange-absolute=false --zero-start=true --zsafe=2.0000 --extra-passes=1 --mill-feed=400 --mill-speed=10000 --offset=0.2000 --voronoi=false --zwork=-0.0500 --drill-feed=200 --drill-side=front --drill-speed=10000 --milldrill=false --nog81=true --nog91-1=false --onedrill=false --zdrill=-1.8000 --bridges=0.5000 --bridgesnum=4 --cut-feed=200 --cut-infeed=0.4000 --cut-side=front --cut-speed=10000 --cutter-diameter=1.0000 --fill-outline=true --zbridges=-0.6000 --zcut=-1.8000
     */
    QStringList baseParams = QString(
                "--tile-x=1 --tile-y=1 --metric=true --metricoutput=true --nog64=true --optimise=false \
                --tolerance=0.0100 --vectorial=true --zchange=12.0000 --zchange-absolute=false --zero-start=true \
                --zsafe=2.0000 --extra-passes=1 --mill-feed=400 --mill-speed=10000 --offset=0.2000 --voronoi=false \
                --zwork=-0.0500 --drill-feed=200 --drill-side=front --drill-speed=10000 --milldrill=false --nog81=true \
                --nog91-1=false --onedrill=false --zdrill=-1.8000 --bridges=0.5000 --bridgesnum=4 --cut-feed=200 \
                --cut-infeed=0.4000 --cut-side=front --cut-speed=10000 --cutter-diameter=1.0000 --fill-outline=true \
                --zbridges=-0.6000 --zcut=-1.8000"
            ).split(" ");

    /**
     * Generate files:
     *  - front.ngc (milling)
     *  - back.ngc (milling)
     *  - drill.ngc (holes)
     *  - outline.ngc (edge cuts)
     */
    QStringList params;
    params << "--back=" + projectDir + "/HBridge-B_Cu.gbr";
    params << "--drill=" + projectDir + "/HBridge.drl";
    params << "--front=" + projectDir + "/HBridge-F_Cu.gbr";
    params << "--outline=" + projectDir + "/HBridge-Edge_Cuts.gbr";

    params << "--front-output="  + getTodayDirectory().toLocal8Bit() + "/front_mill.ngc";
    params << "--back-output="  + getTodayDirectory().toLocal8Bit() + "/back_mill.ngc";
    params << "--milldrill-output="  + getTodayDirectory().toLocal8Bit() + "/milldrill.ngc";
    params << "--outline-output="  + getTodayDirectory().toLocal8Bit() + "/outline.ngc";

    params += baseParams;

    QProcess *process = new QProcess(this);
    process->start("pcb2gcode", params);
    process->waitForFinished();

    qDebug() << process->readAllStandardOutput();

    /**
     * Generate mask files
     */
    params.clear();
    params << "--front=" + projectDir + "/HBridge-F_Mask.gbr";
    params << "--back=" + projectDir + "/HBridge-F_Mask.gbr";
    params << "--output-dir=" + getTodayDirectory().toLocal8Bit();
    params += baseParams;

    process->start("pcb2gcode", params);
    process->waitForFinished();

    qDebug() << process->readAllStandardOutput();

    /**
     * Make svg file grayscale
     */
    runPHP(":/scripts/grayscale_svg.php", QStringList(getTodayDirectory() + "/outp0_original_back.svg"));
    runPHP(":/scripts/grayscale_svg.php", QStringList(getTodayDirectory() + "/outp1_original_front.svg"));
}

void frmMain::on_actionOpen_directory_triggered()
{
    QDesktopServices::openUrl(getTodayDirectory());
}
QString frmMain::runPHP(QString path, QStringList params)
{
    // Run php file stored in resources
    QFile f(path);
    QTemporaryFile *tmpFile = QTemporaryFile::createNativeFile(f);
    if (tmpFile->open()) {
        QProcess *process = new QProcess(this);
        qDebug() << params;
        process->start("php", QStringList(tmpFile->fileName()) + params);
        process->waitForFinished();
        tmpFile->close();
        return process->readAllStandardOutput();
    } else {
        qDebug() << "Error open temp file";
        return "";
    }
}

void frmMain::setWorkCoordinates(double x, double y, double z)
{
    ui->txtWPosX->setText(QString::number(x));
    ui->txtWPosY->setText(QString::number(y));
    ui->txtWPosZ->setText(QString::number(z));

    // Update tool position
    if (!(machineStatus == CHECK && m_fileProcessedCommandIndex < m_currentModel->rowCount() - 1)) {
        m_toolDrawer.setToolPosition(QVector3D(toMetric(ui->txtWPosX->text().toDouble()),
                                               toMetric(ui->txtWPosY->text().toDouble()),
                                               toMetric(ui->txtWPosZ->text().toDouble())));
    }
    // toolpath shadowing
    if (m_processingFile && machineStatus != CHECK) {
        GcodeViewParse *parser = m_currentDrawer->viewParser();

        bool toolOntoolpath = false;

        QList<int> drawnLines;
        QList<LineSegment*> list = parser->getLineSegmentList();

        for (int i = m_lastDrawnLineIndex; i < list.count()
             && list[i]->getLineNumber()
             <= (m_currentModel->data(m_currentModel->index(m_fileProcessedCommandIndex, 4)).toInt() + 1); i++) {
            if (list[i]->contains(m_toolDrawer.toolPosition())) {
                toolOntoolpath = true;
                m_lastDrawnLineIndex = i;
                break;
            }
            drawnLines << i;
        }

        if (toolOntoolpath) {
            foreach (int i, drawnLines) {
                list[i]->setDrawn(true);
            }
            if (!drawnLines.isEmpty()) m_currentDrawer->update(drawnLines);
        } else if (m_lastDrawnLineIndex < list.count()) {
            qDebug() << "tool missed:" << list[m_lastDrawnLineIndex]->getLineNumber()
                     << m_currentModel->data(m_currentModel->index(m_fileProcessedCommandIndex, 4)).toInt()
                     << m_fileProcessedCommandIndex;
        }
    }
}

void frmMain::dataReceived(QString data) {
    // Processed commands
    if (m_commands.length() > 0 && !dataIsFloating(data)
            && !(m_commands[0].command != "[CTRL+X]" && dataIsReset(data))) {

        static QString response; // Full response string

        if ((m_commands[0].command != "[CTRL+X]" && dataIsEnd(data))
                || (m_commands[0].command == "[CTRL+X]" && dataIsReset(data))) {

            response.append(data);

            // Take command from buffer
            CommandAttributes ca = m_commands.takeFirst();
            QTextBlock tb = ui->txtConsole->document()->findBlockByNumber(ca.consoleIndex);
            QTextCursor tc(tb);

            // Restore absolute/relative coordinate system after jog
            if (ca.command.toUpper() == "$G" && ca.tableIndex == -2) {
                if (ui->chkKeyboardControl->isChecked()) m_absoluteCoordinates = response.contains("G90");
                else if (response.contains("G90")) sendCommand("G90", -1, m_settings->getBool("showUICommands"));
            }

            // Process parser status
            if (ca.command.toUpper() == "$G" && ca.tableIndex == -3) {
                // Update status in visualizer window
                ui->glwVisualizer->setParserStatus(response.left(response.indexOf("; ")));

                // Store parser status
                if (m_processingFile) storeParserState();

                // Process spindle state
                if (!response.contains("M5")) {
                    m_spindleCW = response.contains("M3");
                    m_timerToolAnimation.start(25, this);
                    m_programSpeed = true;
                    ui->cmdSpindle->setChecked(true);
                    m_programSpeed = false;
                } else {
                    m_timerToolAnimation.stop();
                    m_programSpeed = true;
                    ui->cmdSpindle->setChecked(false);
                    m_programSpeed = false;
                }

                // Spindle speed
                QRegExp rx(".*S([\\d\\.]+)");
                if (rx.indexIn(response) != -1) {
                    double speed = toMetric(rx.cap(1).toDouble()); //RPM in imperial?
                    if (fabs(ui->txtSpindleSpeed->value() - speed) < 2.54) ui->txtSpindleSpeed->setStyleSheet("color: palette(text);");
                }

                // Feed
                rx.setPattern(".*F([\\d\\.]+)");
                if (rx.indexIn(response) != -1) {
                    double feed = toMetric(rx.cap(1).toDouble());
                    double set = ui->chkFeedOverride->isChecked() ? m_originalFeed / 100 * ui->txtFeed->value()
                                                                  : m_originalFeed;
                    if (response.contains("G20")) set *= 25.4;
                    if (fabs(feed - set) < 2.54) ui->txtFeed->setStyleSheet("color: palette(text);");
                }

                m_updateParserStatus = true;
            }

            // Store origin
            if (ca.command == "$#" && ca.tableIndex == -2) {
                qDebug() << "Received offsets:" << response;
                QRegExp rx(".*G92:([^,]*),([^,]*),([^\\]]*)");

                if (rx.indexIn(response) != -1) {
                    if (m_settingZeroXY) {
                        m_settingZeroXY = false;
                        m_storedX = toMetric(rx.cap(1).toDouble());
                        m_storedY = toMetric(rx.cap(2).toDouble());
                    } else if (m_settingZeroZ) {
                        m_settingZeroZ = false;
                        m_storedZ = toMetric(rx.cap(3).toDouble());
                    }
                    ui->cmdRestoreOrigin->setToolTip(QString(tr("Restore origin:\n%1, %2, %3")).arg(m_storedX).arg(m_storedY).arg(m_storedZ));
                }
            }

            // Homing response
            if ((ca.command.toUpper() == "$H" || ca.command.toUpper() == "$T") && m_homing) {
                m_homing = false;
                if(m_settings->getBool("restoreZeroAfterHoming")) {
                    m_settingZeroXY = true;
                    sendCommand("G92X0Y0", -1, m_settings->getBool("showUICommands"));
                    sendCommand("$#", -2, m_settings->getBool("showUICommands"));
                }
            }

            // Reset complete
            if (ca.command == "[CTRL+X]") {
                m_resetCompleted = true;
                m_updateParserStatus = true;
            }

            // Clear command buffer on "M2" & "M30" command (old firmwares)
            if ((ca.command.contains("M2") || ca.command.contains("M30")) && response.contains("ok") && !response.contains("[Pgm End]")) {
                m_commands.clear();
                m_queue.clear();
            }

            // Process probing on heightmap mode only from table commands
            if (ca.command.contains("G38.2") && m_heightMapMode && ca.tableIndex > -1) {
                // Get probe Z coordinate
                // "[PRB:0.000,0.000,0.000:0];ok"
                QRegExp rx(".*PRB:([^,]*),([^,]*),([^]^:]*)");
                double z = qQNaN();
                if (rx.indexIn(response) != -1) {
                    qDebug() << "probing coordinates:" << rx.cap(1) << rx.cap(2) << rx.cap(3);
                    z = toMetric(rx.cap(3).toDouble());
                }

                static double firstZ;
                if (m_probeIndex == -1) {
                    firstZ = z;
                    z = 0;
                } else {
                    // Calculate delta Z
                    z -= firstZ;

                    // Calculate table indexes
                    int row = trunc(m_probeIndex / m_heightMapModel.columnCount());
                    int column = m_probeIndex - row * m_heightMapModel.columnCount();
                    if (row % 2) column = m_heightMapModel.columnCount() - 1 - column;

                    // Store Z in table
                    m_heightMapModel.setData(m_heightMapModel.index(row, column), z, Qt::UserRole);
                    ui->tblHeightMap->update(m_heightMapModel.index(m_heightMapModel.rowCount() - 1 - row, column));
                    updateHeightMapInterpolationDrawer();
                }

                m_probeIndex++;
            }

            // Change state query time on check mode on
//            if (ca.command.contains(QRegExp("$[cC]"))) {
//                m_timerStateQuery.setInterval(response.contains("Enable") ? 1000 : UserSettings::getInstance()->getInt("queryStateTime"));
//            }

            // Add response to console
            if (tb.isValid() && tb.text() == ca.command) {

                bool scrolledDown = ui->txtConsole->verticalScrollBar()->value() == ui->txtConsole->verticalScrollBar()->maximum();

                // Update text block numbers
                int blocksAdded = response.count("; ");

                if (blocksAdded > 0) for (int i = 0; i < m_commands.count(); i++) {
                    if (m_commands[i].consoleIndex != -1) m_commands[i].consoleIndex += blocksAdded;
                }

                tc.beginEditBlock();
                tc.movePosition(QTextCursor::EndOfBlock);

                tc.insertText(" < " + QString(response).replace("; ", "\r\n"));
                tc.endEditBlock();

                if (scrolledDown) ui->txtConsole->verticalScrollBar()->setValue(ui->txtConsole->verticalScrollBar()->maximum());
            }

            // Check queue
            if (m_queue.length() > 0) {
                CommandQueue cq = m_queue.takeFirst();
                while ((bufferLength() + cq.command.length() + 1) <= BUFFERLENGTH) {
                    sendCommand(cq.command, cq.tableIndex, cq.showInConsole);
                    if (m_queue.isEmpty()) break; else cq = m_queue.takeFirst();
                }
            }

            // Add response to table, send next program commands
            if (m_processingFile) {

                // Only if command from table
                if (ca.tableIndex > -1) {
                    m_currentModel->setData(m_currentModel->index(ca.tableIndex, 2), tr("Processed"));
                    m_currentModel->setData(m_currentModel->index(ca.tableIndex, 3), response);

                    m_fileProcessedCommandIndex = ca.tableIndex;

                    if (ui->chkAutoScroll->isChecked() && ca.tableIndex != -1) {
                        ui->tblProgram->scrollTo(m_currentModel->index(ca.tableIndex + 1, 0));
                        ui->tblProgram->setCurrentIndex(m_currentModel->index(ca.tableIndex, 1));
                    }
                }

                // Update taskbar progress
#ifdef WINDOWS
                if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
                    if (m_taskBarProgress) m_taskBarProgress->setValue(m_fileProcessedCommandIndex);
                }
#endif

                // Check transfer complete (last row always blank, last command row = rowcount - 2)
                if (m_fileProcessedCommandIndex == m_currentModel->rowCount() - 2
                        || ca.command.contains(QRegExp("M0*2|M30"))) m_transferCompleted = true;
                // Send next program commands
                else if (!m_fileEndSent && (m_fileCommandIndex < m_currentModel->rowCount())) sendNextFileCommands();
            }

            // Scroll to first line on "M30" command
            if (ca.command.contains("M30")) ui->tblProgram->setCurrentIndex(m_currentModel->index(0, 1));

            // Toolpath shadowing on check mode
            if (machineStatus == CHECK) {
                GcodeViewParse *parser = m_currentDrawer->viewParser();
                QList<LineSegment*> list = parser->getLineSegmentList();

                if (!m_transferCompleted && m_fileProcessedCommandIndex < m_currentModel->rowCount() - 1) {
                    int i;
                    QList<int> drawnLines;

                    for (i = m_lastDrawnLineIndex; i < list.count()
                         && list[i]->getLineNumber()
                         <= (m_currentModel->data(m_currentModel->index(m_fileProcessedCommandIndex, 4)).toInt()); i++) {
                        drawnLines << i;
                    }

                    if (!drawnLines.isEmpty() && (i < list.count())) {
                        m_lastDrawnLineIndex = i;
                        QVector3D vec = list[i]->getEnd();
                        m_toolDrawer.setToolPosition(vec);
                    }

                    foreach (int i, drawnLines) {
                        list[i]->setDrawn(true);
                    }
                    if (!drawnLines.isEmpty()) m_currentDrawer->update(drawnLines);
                } else {
                    foreach (LineSegment* s, list) {
                        if (!qIsNaN(s->getEnd().length())) {
                            m_toolDrawer.setToolPosition(s->getEnd());
                            break;
                        }
                    }
                }
            }

            response.clear();
        } else {
            response.append(data + "; ");
        }

    } else {
        // Unprocessed responses
        qDebug() << "Unprocessed response:" << data;

        // Handle hardware reset
        if (dataIsReset(data)) {
            qDebug() << "hardware reset";

            m_processingFile = false;
            m_transferCompleted = true;
            m_fileCommandIndex = 0;

            m_reseting = false;
            m_homing = false;
            m_lastGrblStatus = -1;

            m_updateParserStatus = true;
            m_statusReceived = true;

            m_commands.clear();
            m_queue.clear();

            updateControlsState();
        }
        ui->txtConsole->appendPlainText(data);
    }
}

bool frmMain::dataIsReset(QString data) {
    return QRegExp("^GRBL|GCARVIN\\s\\d\\.\\d.").indexIn(data.toUpper()) != -1;
}

void frmMain::onError(int err_n, QString err) {
    ui->txtConsole->appendPlainText(tr("Serial port error ") + QString::number(err_n) + ": " + err);
}
