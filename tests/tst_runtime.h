#include <QtTest>

class RuntimeTests : public QObject
{
    Q_OBJECT

public:
    RuntimeTests();
    ~RuntimeTests();

private slots:
    void test_bind();
};
