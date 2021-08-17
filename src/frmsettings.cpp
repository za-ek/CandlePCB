// This file is a part of "Candle" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "frmsettings.h"
#include "ui_frmsettings.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QScrollBar>
#include <QColorDialog>
#include "utils/usersettings.h"
#include "../libs/runtime.h"

frmSettings::frmSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSettings)
{
    ui->setupUi(this);

    this->setLocale(QLocale::C);
    m_intValidator.setBottom(1);
    m_intValidator.setTop(999);
    ui->cboFps->setValidator(&m_intValidator);
    ui->cboFontSize->setValidator(&m_intValidator);

//    resize(QSize(m_settings->getInt("formSettingsSize_w"), m_settings->getInt("formSettingsSize_h")));

    UserSettings::restoreUI(this);
    Settings * set = UserSettings::getInstance();

    QString c;
    foreach (ColorPicker* pick, findChildren<ColorPicker*>()) {
        c = set->get(pick->objectName());
        pick->setColor(QColor(c == "" ? "white" : c));
    }

    searchPorts();
}

frmSettings::~frmSettings()
{
    delete ui;
}


void frmSettings::searchPorts()
{
    ui->port->clear();

    foreach (QSerialPortInfo info ,QSerialPortInfo::availablePorts()) {
        ui->port->insertItem(0, info.portName());
    }
}

void frmSettings::on_cmdRefresh_clicked()
{
    searchPorts();
}

void frmSettings::on_cmdOK_clicked()
{
    Runtime *r = Runtime::getInstance();
    if(r->value("port") != ui->port->currentText()) {
        r->insert("port", ui->port->currentText());
    }

    if(r->value("baud") != ui->baud->currentText()) {
        r->insert("baud", ui->baud->currentText());
    }

    UserSettings::saveUI(this);
    UserSettings::save();
    this->accept();
}

void frmSettings::on_cmdCancel_clicked()
{
    this->reject();
}

void frmSettings::on_cmdDefaults_clicked()
{
    if (QMessageBox::warning(this, qApp->applicationDisplayName(), tr("Reset settings to default values?"),
                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) != QMessageBox::Yes) return;

    // Set defaults from file
    Settings * set = UserSettings::getInstance();
    QSettings defaultIni(":/default.ini", QSettings::IniFormat);
    m_settings->mergeQSettings(&defaultIni);
    UserSettings::restoreUI(this);
}
void frmSettings::on_cboFontSize_currentTextChanged(const QString &arg1)
{
    qApp->setStyleSheet(QString(qApp->styleSheet()).replace(QRegExp("font-size:\\s*\\d+"), "font-size: " + arg1));
}

//double frmSettings::arcPrecision()
//{
//    return ui->arcDegreeMode->isChecked() ? ui->txtArcDegree->value() : ui->txtArcLength->value();
//}
