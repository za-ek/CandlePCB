#ifndef LIB_SETTINGS_H
#define LIB_SETTINGS_H

#include "libs_global.h"
#include <QSettings>
#include <QVector>
#include <QDebug>

/**
 * @brief The settings class
 * Settings keys are store as %TYPE_%OPTION
 * Where %TYPE% could be:
 *  - d : double (with present precision letter d follows by digits (up to 9))
 *  - i : int
 *  - s : string
 *  - b : bool
 *  - l : list
 *
 */

class LIBS_EXPORT Settings
{
public:
    Settings();
    Settings(QVector<QString> keys) : originalKeys(keys) {}
    ~Settings();

    void set        (QString option, QString value) { values.insert("s_" + option, value); }
    void setBool    (QString option, bool value) { values_bool.insert("b_" + option, value); }
    void setInt     (QString option, int value) { values_int.insert("i_" + option, value); }
    void setDouble  (QString option, double  value);
    void setList    (QString option, QStringList value) { values_list.insert("l_" + option, value.join(",")); }
    void setT       (QString prefix, QString option, QVariant value) {
        if(!values_t.contains(prefix)) {
            values_t.insert(prefix, QMap<QString, QVariant>());
        }
        values_t[values_t.find(prefix).key()].insert(option, value);
    }

    QString get     (QString option) { return values.value("s_" + option); }
    bool getBool    (QString option) { return values_bool.value("b_" + option); }
    int getInt      (QString option) { return values_int.value("i_" + option); }
    double getDouble(QString option) { return values_double.value(getDoubleKey(option)); }
    QStringList getList(QString option) { return values_list.value("l_" + option).split(","); }
    QVariant getT   (QString prefix, QString option) { return values_t.value(prefix).value(option); }

    bool keyExists  (QString option);

    void fillQSettings (QSettings *set);
    void restoreQSettings(QSettings *set);
    void mergeQSettings(QSettings *set);

    void setKeys(QVector<QString> keys) {originalKeys = keys;}
private:
    QMap<QString, QString>  values;
    QMap<QString, QString>  values_list;
    QMap<QString, int>      values_int;
    QMap<QString, double>   values_double;
    QMap<QString, bool>     values_bool;
    QMap<QString, QMap<QString, QVariant>> values_t;

    QString getDoubleKey(QString doubleKey);
    void fillByKeys(QVector<QString> list, QSettings * s);
protected:
    QVector<QString> originalKeys;
};

#endif // LIB_SETTINGS_H
