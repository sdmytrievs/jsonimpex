
#include <gtest/gtest.h>

#include "tst_yaml_xml.h"
#include "tst_jsonimpex.h"
#include "tst_impex.h"


int main(int argc, char *argv[])
{
    jsonio::JsonioSettings::settingsFileName = "jsonimpex-config.json";
    ioSettings().set_levels("off");
    spdlog::set_pattern("[%n] [%^%l%$] %v");

    jsonio::FormatImpexGenerator::setResourcesDirectory("Resources");

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
