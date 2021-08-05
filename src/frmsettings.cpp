// This file is a part of "Candle" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "frmsettings.h"
#include "ui_frmsettings.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QScrollBar>
#include <QColorDialog>
#include "utils/Settings.h"

frmSettings::frmSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSettings),
    Settings()
{
    ui->setupUi(this);

    this->setLocale(QLocale::C);
    m_intValidator.setBottom(1);
    m_intValidator.setTop(999);
    ui->cboFps->setValidator(&m_intValidator);
    ui->cboFontSize->setValidator(&m_intValidator);
    searchPorts();
}

frmSettings::~frmSettings()
{
    delete ui;
}

int frmSettings::exec()
{
    /**
     * When you open settings form this code stores actual values in variables m_stored*
     * Then if you click cancel the method "Undo" restore values from this variables
     **/
    // Store settings to undo
    m_storedValues.clear();
    m_storedChecks.clear();
    m_storedCombos.clear();
    m_storedColors.clear();

    foreach (QAbstractSpinBox* sb, this->findChildren<QAbstractSpinBox*>())
        m_storedValues.append(sb->property("value").toDouble());
    foreach (QAbstractButton* cb, this->findChildren<QAbstractButton*>())
        m_storedChecks.append(cb->isChecked());
    foreach (QComboBox* cb, this->findChildren<QComboBox*>())
        m_storedCombos.append(cb->currentText());
    foreach (ColorPicker* pick, this->findChildren<ColorPicker*>())
        m_storedColors.append(pick->color());

    return QDialog::exec();
}

void frmSettings::undo()
{
    foreach (QAbstractSpinBox* sb, this->findChildren<QAbstractSpinBox*>())
        sb->setProperty("value", m_storedValues.takeFirst());
    foreach (QAbstractButton* cb, this->findChildren<QAbstractButton*>())
        cb->setChecked(m_storedChecks.takeFirst());
    foreach (QComboBox* cb, this->findChildren<QComboBox*>())
        cb->setCurrentText(m_storedCombos.takeFirst());
    foreach (ColorPicker* pick, this->findChildren<ColorPicker*>())
        pick->setColor(m_storedColors.takeFirst());
}

QString frmSettings::port()
{
    return ui->cboPort->currentText();
}

void frmSettings::setPort(QString port)
{
    ui->cboPort->setCurrentText(port);
}

int frmSettings::baud()
{
    return ui->cboBaud->currentText().toInt();
}

void frmSettings::setBaud(int baud)
{
    ui->cboBaud->setCurrentText(QString::number(baud));
}

double frmSettings::toolDiameter()
{
    return ui->txtToolDiameter->value();
}

void frmSettings::setToolDiameter(double diameter)
{
    ui->txtToolDiameter->setValue(diameter);
}

double frmSettings::toolLength()
{
    return ui->txtToolLength->value();
}

void frmSettings::setToolLength(double length)
{
    ui->txtToolLength->setValue(length);
}

double frmSettings::lineWidth()
{
    return ui->txtLineWidth->value();
}

void frmSettings::setLineWidth(double lineWidth)
{
    ui->txtLineWidth->setValue(lineWidth);
}

double frmSettings::arcLength()
{
    return ui->txtArcLength->value();
}

void frmSettings::setArcLength(double arcPrecision)
{
    ui->txtArcLength->setValue(arcPrecision);
}

double frmSettings::arcDegree()
{
    return ui->txtArcDegree->value();
}

void frmSettings::setArcDegree(double arcDegree)
{
    ui->txtArcDegree->setValue(arcDegree);
}


QString frmSettings::safePositionCommand()
{
    return ui->txtSafeCommand->text();
}

void frmSettings::setSafePositionCommand(QString command)
{
    ui->txtSafeCommand->setText(command);
}

int frmSettings::restoreMode()
{
    return ui->cboRestoreMode->currentIndex();
}

void frmSettings::setRestoreMode(int value)
{
    ui->cboRestoreMode->setCurrentIndex(value);
}

int frmSettings::spindleSpeedMin()
{
    return ui->txtSpindleSpeedMin->value();
}

void frmSettings::setSpindleSpeedMin(int speed)
{
    ui->txtSpindleSpeedMin->setValue(speed);
}

int frmSettings::spindleSpeedMax()
{
    return ui->txtSpindleSpeedMax->value();
}

void frmSettings::setSpindleSpeedMax(int speed)
{
    ui->txtSpindleSpeedMax->setValue(speed);
}

int frmSettings::rapidSpeed()
{
    return ui->txtRapidSpeed->value();
}

void frmSettings::setRapidSpeed(int rapidSpeed)
{
    ui->txtRapidSpeed->setValue(rapidSpeed);
}

int frmSettings::heightmapProbingFeed()
{
    return ui->txtHeightMapProbingFeed->value();
}

void frmSettings::setHeightmapProbingFeed(int heightmapProbingFeed)
{
    ui->txtHeightMapProbingFeed->setValue(heightmapProbingFeed);
}

int frmSettings::acceleration()
{
    return ui->txtAcceleration->value();
}

void frmSettings::setAcceleration(int acceleration)
{
    ui->txtAcceleration->setValue(acceleration);
}

int frmSettings::queryStateTime()
{
    return ui->txtQueryStateTime->value();
}

void frmSettings::setQueryStateTime(int queryStateTime)
{
    ui->txtQueryStateTime->setValue(queryStateTime);
}

int frmSettings::toolType()
{
    return ui->cboToolType->currentIndex();
}

void frmSettings::setToolType(int toolType)
{
    ui->cboToolType->setCurrentIndex(toolType);
}

double frmSettings::toolAngle()
{
    return ui->txtToolAngle->value();
}

void frmSettings::setToolAngle(double toolAngle)
{
    ui->txtToolAngle->setValue(toolAngle);
}

int frmSettings::fps()
{
    return ui->cboFps->currentText().toInt();
}

void frmSettings::setFps(int fps)
{
    ui->cboFps->setCurrentText(QString::number(fps));
}


int frmSettings::units()
{
    return ui->cboUnits->currentIndex();
}

void frmSettings::setUnits(int units)
{
    ui->cboUnits->setCurrentIndex(units);
}

QString frmSettings::touchCommand()
{
    return ui->txtTouchCommand->text();
}

void frmSettings::setTouchCommand(QString touchCommand)
{
    ui->txtTouchCommand->setText(touchCommand);
}

double frmSettings::simplifyPrecision()
{
    return ui->txtSimplifyPrecision->value();
}

void frmSettings::setSimplifyPrecision(double simplifyPrecision)
{
    ui->txtSimplifyPrecision->setValue(simplifyPrecision);
}
QString frmSettings::getOptionString(QString opt)
{
    return m_optionsString.value(opt);
}
void frmSettings::setOptionString(QString opt, QString value)
{
    m_optionsString.insert(opt, value);
}
bool frmSettings::getOption(QString opt)
{
    return m_settings.at(opt);
}
void frmSettings::setOption(QString opt, bool value)
{
    m_settings.insert({opt, value});
    QCheckBox * c = findChild<QCheckBox*>(opt);
    if(c != nullptr) c->setChecked(value);
}
double frmSettings::arcPrecision()
{
    return ui->arcDegreeMode->isChecked() ? ui->txtArcDegree->value() : ui->txtArcLength->value();
}
QList<ColorPicker *> frmSettings::colors()
{
    return this->findChildren<ColorPicker*>();
}

QColor frmSettings::colors(QString name)
{
    ColorPicker *pick = this->findChildren<ColorPicker*>("clp" + name).at(0);
    if (pick) return pick->color(); else return QColor();
}

int frmSettings::fontSize()
{
    return ui->cboFontSize->currentText().toInt();
}

void frmSettings::setFontSize(int fontSize)
{
    ui->cboFontSize->setCurrentText(QString::number(fontSize));
}

void frmSettings::showEvent(QShowEvent *se)
{
    Q_UNUSED(se)

//    ui->scrollSettings->updateMinimumWidth();
}

void frmSettings::searchPorts()
{
    ui->cboPort->clear();

    foreach (QSerialPortInfo info ,QSerialPortInfo::availablePorts()) {
//        ui->cboPort->addItem(info.portName());
        ui->cboPort->insertItem(0, info.portName());
    }
}

void frmSettings::on_cmdRefresh_clicked()
{
    searchPorts();
}

void frmSettings::on_cmdOK_clicked()
{
    this->accept();
}

void frmSettings::on_cmdCancel_clicked()
{
    this->reject();
}

void frmSettings::on_cboToolType_currentIndexChanged(int index)
{
    ui->lblToolAngle->setEnabled(index == 1);
    ui->txtToolAngle->setEnabled(index == 1);
}

void frmSettings::on_cmdDefaults_clicked()
{
    if (QMessageBox::warning(this, qApp->applicationDisplayName(), tr("Reset settings to default values?"),
                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) != QMessageBox::Yes) return;

    setPort("");
    setBaud(115200);

    setQueryStateTime(40);
    setRapidSpeed(2000);
    setAcceleration(100);
    setSpindleSpeedMin(0);
    setSpindleSpeedMax(10000);
    setTouchCommand("G21G91G38.2Z-30F100; G0Z1; G38.2Z-2F10");
    setSafePositionCommand("G21G90; G53G0Z0");
//    setMoveOnRestore(false);
    setRestoreMode(0);
    setHeightmapProbingFeed(10);
    setUnits(0);

    setArcLength(0.0);
//    setArcDegreeMode(true);
    setArcDegree(5.0);

    setLineWidth(1.5);
//    setAntialiasing(true);
//    setMsaa(true);
//    setSimplify(true);
    setSimplifyPrecision(0.0);
    setFps(60);
//    setZBuffer(false);

    setToolType(1);
    setToolAngle(15.0);
    setToolDiameter(3.0);
    setToolLength(30.0);

//    setShowProgramCommands(false);
//    setAutoCompletion(true);

//    setPanelFeed(true);
//    setPanelHeightmap(true);
//    setPanelJog(true);
//    setPanelSpindle(true);

    ui->clpTool->setColor(QColor(255, 153, 0));

    ui->clpVisualizerBackground->setColor(QColor(255, 255, 255));
    ui->clpVisualizerText->setColor(QColor(0, 0, 0));

    ui->clpToolpathNormal->setColor(QColor(0, 0, 0));
    ui->clpToolpathDrawn->setColor(QColor(217, 217, 217));
    ui->clpToolpathHighlight->setColor(QColor(145, 130, 230));
    ui->clpToolpathZMovement->setColor(QColor(255, 0, 0));
    ui->clpToolpathStart->setColor(QColor(255, 0, 0));
    ui->clpToolpathEnd->setColor(QColor(0, 255, 0));

    setFontSize(9);
}
bool frmSettings::showUICommands() {
    return getOption("showUICommands");
}
void frmSettings::on_cboFontSize_currentTextChanged(const QString &arg1)
{
    qApp->setStyleSheet(QString(qApp->styleSheet()).replace(QRegExp("font-size:\\s*\\d+"), "font-size: " + arg1));
}

void frmSettings::on_restoreZeroAfterHoming_stateChanged(int arg1)
{
    setOption("restoreZeroAfterHoming", arg1);
}

void frmSettings::on_pcb2gcodeArgs_textChanged()
{
    setOptionString("pcb2gcodeArgs", ui->pcb2gcodeArgs->toPlainText());
}
