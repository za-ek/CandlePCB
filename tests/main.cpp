#ifndef TEST_H
#define TEST_H

#include <QtTest>
#include "../libs/runtime.h"
#include "tst_runtime.h"
#include "tst_settings.h"

int main(int argc, char** argv)
{
   int status = 0;
   auto ASSERT_TEST = [&status, argc, argv](QObject* obj) {
     status |= QTest::qExec(obj, argc, argv);
     delete obj;
   };

   ASSERT_TEST(new SettingsTests());
   ASSERT_TEST(new RuntimeTests());

   return status;
}

#endif // TEST_H
