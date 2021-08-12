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

    resize(QSize(m_settings->getInt("formSettingsSize_w"), m_settings->getInt("formSettingsSize_h")));

//    foreach (ColorPicker* pick, m_settings.colors()) {
//        pick->setColor(QColor(set.value(pick->objectName().mid(3), "black").toString()));
//    }

    searchPorts();
}

frmSettings::~frmSettings()
{
    delete ui;
}

//double frmSettings::arcPrecision()
//{
//    return ui->arcDegreeMode->isChecked() ? ui->txtArcDegree->value() : ui->txtArcLength->value();
//}
QList<ColorPicker *> frmSettings::colors()
{
    return this->findChildren<ColorPicker*>();
}

QColor frmSettings::colors(QString name)
{
    ColorPicker *pick = this->findChildren<ColorPicker*>("clp" + name).at(0);
    if (pick) return pick->color(); else return QColor();
}

void frmSettings::searchPorts()
{
    ui->cboPort->clear();

    foreach (QSerialPortInfo info ,QSerialPortInfo::availablePorts()) {
        ui->cboPort->insertItem(0, info.portName());
    }
}

void frmSettings::on_cmdRefresh_clicked()
{
    searchPorts();
}

void frmSettings::on_cmdOK_clicked()
{
    Settings *s = UserSettings::getInstance();

    // Save all checkboxes
    foreach (QAbstractButton* cb, this->findChildren<QAbstractButton*>())
        s->setBool(cb->objectName(), cb->isChecked());

    // All colors
    foreach (ColorPicker* pick, this->findChildren<ColorPicker*>())
        s->set(pick->objectName(), pick->color().name());

    // All texts
    foreach (QComboBox* cb, this->findChildren<QComboBox*>())
        s->set(cb->objectName(), cb->currentText());

//    foreach (QAbstractSpinBox* sb, this->findChildren<QAbstractSpinBox*>())
//        s->set(sb->objectName(), sb->property("value").toDouble());

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

    // Set defaults
}
void frmSettings::on_cboFontSize_currentTextChanged(const QString &arg1)
{
    qApp->setStyleSheet(QString(qApp->styleSheet()).replace(QRegExp("font-size:\\s*\\d+"), "font-size: " + arg1));
}
