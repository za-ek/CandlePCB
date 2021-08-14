#include <QtTest>
#include <QVector>
#include <QString>
#include <QSettings>
#include "../libs/settings.h"
#include "tst_settings.h"

// add necessary includes here

void SettingsTests::test_case1()
{
    QVector<QString> keys({"key", "boolKey", "intKey", "doubleKey"});
    Settings s(keys);

    s.set("key", "value");
    QVERIFY(s.get("key") == "value");

    s.setBool("boolKey", true);
    QVERIFY(s.getBool("boolKey") == true);

    s.setInt("intKey", 15);
    QVERIFY(s.getInt("intKey") == 15);

}
void SettingsTests::test_double()
{
    Settings s({"key", "boolKey", "intKey", "d1_doubleKey", "dblKey"});

    s.setDouble("doubleKey", 15.404);
    QVERIFY(s.getDouble("doubleKey") == 15.4);

    s.setDouble("doubleKey", 15.504);
    QVERIFY(s.getDouble("doubleKey") == 15.5);

    s.setDouble("dblKey", 15.504);
    QVERIFY(s.getDouble("dblKey") == 15.504);

}

void SettingsTests::test_qsettingsFill()
{
    Settings s({"s_key", "b_boolKey", "i_intKey", "d1_doubleKey"});

    s.set("key", "value");
    s.setBool("boolKey", true);
    s.setBool("falseKey", false);
    s.setInt("intKey", 15);
    s.setDouble("doubleKey", 15.504);

    QSettings settings(nullptr);
    settings.clear();
    s.fillQSettings(&settings);

    QVERIFY(settings.childKeys().length() == 5);

    QVERIFY(settings.value("s_key").toString() == "value");
    QVERIFY(settings.value("b_boolKey").toBool() == true);
    QVERIFY(settings.value("b_falseKey").toBool() == false);
    QVERIFY(settings.value("i_intKey").toInt() == 15);
    QVERIFY(settings.value("d1_doubleKey").toDouble() == 15.5);
}

void SettingsTests::test_qsettingsRestore()
{
    QSettings s;
    s.clear();

    s.setValue("s_key", "value");
    s.setValue("b_boolKey", true);
    s.setValue("b_falseKey", false);
    s.setValue("i_intKey", 15);
    s.setValue("d1_doubleKey", 15.504);

    Settings settings({"s_key", "b_boolKey", "i_intKey", "d1_doubleKey"});

    settings.restoreQSettings(&s);

    QVERIFY(settings.get("key") == "value");
    QVERIFY(settings.getBool("boolKey") == true);
    QVERIFY(settings.getBool("falseKey") == false);
    QVERIFY(settings.getInt("intKey") == 15);
    QVERIFY(settings.getDouble("doubleKey") == 15.5);
}
