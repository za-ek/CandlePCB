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
    foreach(const QString & s, values.keys()) set->setValue(s, values.value(s));
    foreach(const QString & s, values_bool.keys()) set->setValue(s, values_bool.value(s));
    foreach(const QString & s, values_int.keys()) set->setValue(s, values_int.value(s));
    foreach(const QString & s, values_double.keys()) set->setValue(s, values_double.value(s));
    foreach(const QString & k, values_t.keys())
        foreach(const QString & s, values_t[values_t.find(k).key()].keys()) {
            set->setValue(k + "_" + s, values_t[values_t.find(k).key()].value(s));
        }
}

void Settings::restoreQSettings(QSettings *setObj)
{
    fillByKeys(originalKeys, setObj);
//    foreach(const QString &s, setObj->allKeys()) {
//        if(originalKeys.indexOf(s) == -1) {
//            qDebug() << "!!!!!!!!!!!" << s;
////            setT(s.mid(0,1), s.mid(2), setObj->value(s));
//        }
//    }
}

void Settings::mergeQSettings(QSettings * s) {
    fillByKeys(s->allKeys().toVector(), s);
}

void Settings::fillByKeys(QVector<QString> list, QSettings *setObj) {
    QVector<QString>::const_iterator i = list.constBegin();
    while(i != list.constEnd()) {
        QString val;
        val += i;
        switch((int)i->mid(0, 1).toLocal8Bit()[0]) {
            case 105: setInt(i->mid(2), setObj->value(val).toInt()); break; // i (int)
            case 108: setList(i->mid(2), setObj->value(val).toString().split(",")); // l (list)
            case 98: setBool(i->mid(2), setObj->value(val).toBool()); break; // (bool)
            case 100: setDouble(i->mid(1,1) == "_" ? i->mid(2) : i->mid(3), setObj->value(val).toDouble()); break; // d (double)
            case 115: set(i->mid(2), setObj->value(val).toString()); break; // s (string)
        default:
            setT(i->mid(0,1), i->mid(2), setObj->value(val));
            break;
        }
        ++i;
    }
}

bool Settings::keyExists(QString option) {
    return originalKeys.contains(option) ||
            values_list.contains("l_" + option) ||
            values_int.contains("i_" + option) ||
            values_bool.contains("b_" + option) ||
            values_double.contains("d_" + option) ||
            values.contains("s_" + option);
}
