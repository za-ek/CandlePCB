#include "settings.h"
#include <QDebug>
#include <QMap>
#include <QVector>
#include <QString>
#include <QSettings>
#include <cmath>
#include <math.h>

Settings::Settings() {}
Settings::~Settings() {}


void Settings::setDouble(QString option, double value) {
    QString k = getDoubleKey(option);
    if(k.mid(1,1) == "_") values_double.insert(k, value);
    else {
        double multiplier = std::pow(10.0, k.mid(1,1).toInt());
        values_double.insert(k, std::round(value * multiplier) / multiplier);
    }
}

QString Settings::getDoubleKey(QString doubleKey)
{
    QVector<QString>::const_iterator i = originalKeys.constBegin();
    while(i != originalKeys.constEnd()) {
        // With defined precision
        if(i->mid(0,1) == "d" && i->mid(1,1) != "_" && i->mid(3) == doubleKey) {
            return i->mid(0);
        }
        ++i;
    }

    return "d_" + doubleKey;
}

void Settings::fillQSettings(QSettings *set)
{
    QMap<QString, QString>::const_iterator i = values.constBegin();
    while (i != values.constEnd()) {set->setValue(i.key(), i.value());++i;}

    QMap<QString, bool>::const_iterator i2 = values_bool.constBegin();
    while (i2 != values_bool.constEnd()) {set->setValue(i2.key(), i2.value());++i2;}

    QMap<QString, int>::const_iterator i3 = values_int.constBegin();
    while (i3 != values_int.constEnd()) {set->setValue(i3.key(), i3.value());++i3;}

    QMap<QString, double>::const_iterator i4 = values_double.constBegin();
    while (i4 != values_double.constEnd()) {set->setValue(i4.key(), i4.value());++i4;}
}

void Settings::restoreQSettings(QSettings *setObj)
{
    QVector<QString>::const_iterator i = originalKeys.constBegin();
    while(i != originalKeys.constEnd()) {
        QString val;
        val += i;
        switch((int)i->mid(0, 1).toLocal8Bit()[0]) {
            case 105: setInt(i->mid(2), setObj->value(val).toInt()); break; // i (int)
            case 98: setBool(i->mid(2), setObj->value(val).toBool()); break; // (bool)
            case 100: setDouble(i->mid(1,1) == "_" ? i->mid(2) : i->mid(3), setObj->value(val).toDouble()); break; // d (double)
            case 115: set(i->mid(2), setObj->value(val).toString()); break; // s (string)
            break;
        }
        ++i;
    }
}
