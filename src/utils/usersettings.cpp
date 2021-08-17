#include <QApplication>
#include "settings.h"
#include "usersettings.h"
#include <QVariant>
#include <QDebug>
#include "runtime.h"

#include <QAbstractButton>
#include <QComboBox>
#include <QLineEdit>
#include <QAbstractSpinBox>
#include <QDoubleSpinBox>
#include "widgets/colorpicker.h"

Settings * UserSettings::getInstance()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr) {
        foreach(const QString &s, runtime_keys) {
            settings_keys.append("r_" + s);
        }
        _instance = new Settings(settings_keys);

        QString settings_path = qApp->applicationDirPath() + "/settings.ini";
        QSettings set(settings_path, QSettings::IniFormat);
        set.setIniCodec("UTF-8");

        _instance->restoreQSettings(&set);
        Runtime * r = Runtime::getInstance();
        foreach(const QString &k, runtime_keys) {
            qDebug() << k << _instance->getT("r", k);
            r->insert(k, _instance->getT("r", k).toString());
        }
        Runtime::bind([&](const QString k, const QString v) { getInstance()->setT("r", k, v); });
    }
    return _instance;
}

void UserSettings::save() {
    QString settings_path = qApp->applicationDirPath() + "/settings.ini";
    QSettings set(settings_path, QSettings::IniFormat);
    set.setIniCodec("UTF-8");
    getInstance()->fillQSettings(&set);
}

void UserSettings::saveUI(QWidget *w) {
    Settings * s = getInstance();

    // All checkboxes
    foreach (QAbstractButton* cb, w->findChildren<QAbstractButton*>())
        if(s->keyExists(cb->objectName()))
            s->setBool(cb->objectName(), cb->isChecked());
    // All colors
    foreach (ColorPicker* pick, w->findChildren<ColorPicker*>())
        if(s->keyExists(pick->objectName()))
            s->set(pick->objectName(), pick->color().name());
    // All texts
    foreach (QComboBox* cb, w->findChildren<QComboBox*>())
        if(s->keyExists(cb->objectName()))
            s->set(cb->objectName(), cb->currentText());
    // All double inputs
    foreach (QAbstractSpinBox* sb, w->findChildren<QAbstractSpinBox*>())
        if(s->keyExists(sb->objectName()))
            s->setDouble(sb->objectName(), sb->property("value").toDouble());
}

void UserSettings::restoreUI(QWidget *w) {
    Settings * s = getInstance();
    qDebug() << s->get("touchCommand") << s->keyExists("touchCommand");

    // All checkboxes
    foreach (QAbstractButton* cb, w->findChildren<QAbstractButton*>())
        cb->setChecked(s->getBool(cb->objectName()));
    // All colors
    foreach (ColorPicker* pick, w->findChildren<ColorPicker*>())
        pick->setColor(QColor(s->get(pick->objectName())));
    // All texts
    foreach (QLineEdit* le, w->findChildren<QLineEdit*>()) {
        if(s->keyExists(le->objectName())) le->setText(s->get(le->objectName()));
        else qDebug() << "Key not found: " << le->objectName();
    }
    foreach (QComboBox* cb, w->findChildren<QComboBox*>()) {
        QString v = s->get(cb->objectName());
        for(int i = 0; i < cb->count(); i++) {
            qDebug() << i << ") " << cb->itemText(i) << " " << v << " " << cb->objectName();
            if(cb->itemText(i) == v) {
                cb->setCurrentIndex(i);
            }
        }
    }
    // All double inputs
    foreach (QDoubleSpinBox* sb, w->findChildren<QDoubleSpinBox*>())
        sb->setValue(s->getDouble(sb->objectName()));
}

void UserSettings::saveRuntime(QMap<QString, QString> *m) {
    Settings * s = getInstance();

    foreach(const QString &k, runtime_keys) {
        s->setT("r", k, m->value(k));
    }
}
