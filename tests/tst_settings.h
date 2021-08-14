#include <QtTest>
#include <QVector>
#include <QString>
#include <QSettings>
#include "../libs/settings.h"

class SettingsTests : public QObject
{
    Q_OBJECT

private slots:
    void test_case1();
    void test_double();
    void test_qsettingsFill();
    void test_qsettingsRestore();

};
