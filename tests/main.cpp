
#include <gtest/gtest.h>

#include "tst_jsonimpex.h"
#include "tst_yaml_xml.h"


int main(int argc, char *argv[])
{
    ioSettings().set_levels("off");
    spdlog::set_pattern("[%n] [%^%l%$] %v");

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
