#include "core/tt_test.h"
#include "core/tt_log.h"


// JUST_RUN_TEST(log, test)
TEST(log, test)
{
    LOG_TEST("------------------------------log test--------------------------");
    LOG("hello world");
}