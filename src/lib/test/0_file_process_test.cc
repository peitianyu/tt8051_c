#include "core/tt_test.h"
#include "core/tt_log.h"

#include "stc/file_preprocess.h"

JUST_RUN_TEST(file_preprocess, test)
TEST(file_preprocess, test)
{
    LOG_TEST("------------------------------file_preprocess test--------------------------");
    
    FilePreprocess file_preprocess;
    file_preprocess.run("../data/test_0.tt");
    file_preprocess.output("../out/test_0_out.tt");
}