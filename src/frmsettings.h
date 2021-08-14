// This file is a part of "Candle" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#ifndef FRMSETTINGS_H
#define FRMSETTINGS_H

#include <QDialog>
#include <QVector>
#include <QMessageBox>
#include <QListWidgetItem>
#include <utils/usersettings.h>
#include "widgets/colorpicker.h"

namespace Ui {
class frmSettings;
}

class frmSettings : public QDialog
{
    Q_OBJECT

public:
    explicit frmSettings(QWidget *parent = 0);
    ~frmSettings();

    void undo();

    QList<ColorPicker*> colors();
    QColor colors(QString name);
    bool showUICommands();

    bool getOption(QString opt);
    void setOption(QString opt, bool value);

    QString getOptionString(QString opt);
    void setOptionString(QString opt, QString value);

protected:
    QVector<QString> originalKeys = {
        "s_property"
    };
private slots:
    void on_cmdRefresh_clicked();
    void on_cmdOK_clicked();
    void on_cmdCancel_clicked();
    void on_cmdDefaults_clicked();

    void on_cboFontSize_currentTextChanged(const QString &arg1);

private:
    Ui::frmSettings *ui;
    void searchPorts();
    QIntValidator m_intValidator;
    Settings* m_settings = UserSettings::getInstance();
};

#endif // FRMSETTINGS_H
