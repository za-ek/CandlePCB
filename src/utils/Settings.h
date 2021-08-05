#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QSettings>

/**
 * @brief The settings class
 * Settings keys are store as %TYPE_%OPTION
 * Where %TYPE% could be:
 *  - d : double (with present precision letter d follows by digits (up to 9))
 *  - i : int
 *  - s : string
 *  - b : bool
 *
 */
class Settings {
public:
    Settings();
    Settings(QVector<QString> keys) : originalKeys(keys) {}
    ~Settings();

    QString get     (QString option);
    int getInt      (QString option);
    bool getBool    (QString option);
    double getDouble(QString option);

    void set        (QString option, QString value);
    void setInt     (QString option, int     value);
    void setBool    (QString option, bool    value);
    void setDouble  (QString option, double  value);

    void fillQSettings (QSettings *set);
    void restoreQSettings(QSettings *set);
private:
    QMap<QString, QString>  values;
    QMap<QString, int>      values_int;
    QMap<QString, double>   values_double;
    QMap<QString, bool>     values_bool;

    QString getDoubleKey(QString doubleKey);
protected:
    QVector<QString> originalKeys;
};

#endif // SETTINGS_H
