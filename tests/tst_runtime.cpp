#include <QtTest>
#include "../libs/runtime.h"
#include "tst_runtime.h"

RuntimeTests::RuntimeTests() {}
RuntimeTests::~RuntimeTests(){}

void RuntimeTests::test_bind()
{
    QString port = "";

    Runtime *r = Runtime::getInstance();

    Runtime::bind("port", [&](const QString v) {
        port = v;
    });

    r->insert("port", "hello");

    QVERIFY(port == "hello");
    QVERIFY(r->value("port") == port);

    r->insert("port", "qwe");
    QVERIFY(port == "qwe");
}
