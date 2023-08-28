#pragma once

#include <gtest/gtest.h>
#include "jsonio17/schema_thrift.h"
#include "jsonio17/io_settings.h"
#include "jsonimpex17/impex_generator.h"
#include "impex_schema.h"
#include "example_schema.h"
#include "impex_schema_example.h"

using namespace testing;
using namespace jsonio17;

// Testing import and export external data implementation

const char* const simple_data_string = R"([
     {
          "vbool" :   false,
          "vint" :   0,
          "vdouble" :   0,
          "vstring" :   "input-0"
     },
     {
          "vbool" :   true,
          "vint" :   1,
          "vdouble" :   10.01,
          "vstring" :   "input-1",
          "vlist" :   [
               1,
               2,
               3
          ]
     },
     {
          "vbool" :   false,
          "vint" :   2,
          "vdouble" :   20.02,
          "vstring" :   "input-2",
          "vlist" :   [
               2,
               4,
               6
          ]
     },
     {
          "vbool" :   true,
          "vint" :   3,
          "vdouble" :   30.03,
          "vstring" :   "input-3",
          "vlist" :   [
               3,
               6,
               9
          ]
     },
     {
          "vbool" :   false,
          "vint" :   4,
          "vdouble" :   40.04,
          "vstring" :   "input-4",
          "vlist" :   [
               4,
               8,
               12
          ]
     },
     {
          "vbool" :   true,
          "vint" :   5,
          "vdouble" :   50.05,
          "vstring" :   "input-5",
          "vlist" :   [
               5,
               10,
               15
          ]
     },
     {
          "vbool" :   false,
          "vint" :   6,
          "vdouble" :   60.06,
          "vstring" :   "input-6",
          "vlist" :   [
               6,
               12,
               18
          ]
     },
     {
          "vbool" :   true,
          "vint" :   7,
          "vdouble" :   70.07,
          "vstring" :   "input-7",
          "vlist" :   [
               7,
               14,
               21
          ]
     },
     {
          "vbool" :   false,
          "vint" :   8,
          "vdouble" :   80.08,
          "vstring" :   "input-8",
          "vlist" :   [
               8,
               16,
               24
          ]
     },
     {
          "vbool" :   true,
          "vint" :   9,
          "vdouble" :   90.09,
          "vstring" :   "input-9",
          "vlist" :   [
               9,
               18,
               27
          ]
     }
]
)";

const char* const import_simple_result =
        "[{\"vint\":0,\"vdouble\":0,\"vstring\":\"input-0\",\"vlist\":[0.1,2,3]},"
         "{\"vint\":1,\"vdouble\":10.01,\"vstring\":\"input-1\",\"vlist\":[1,2,3]},"
         "{\"vint\":2,\"vdouble\":20.02,\"vstring\":\"input-2\",\"vlist\":[2,2,3]},"
         "{\"vint\":3,\"vdouble\":30.03,\"vstring\":\"input-3\",\"vlist\":[3,2,3]},"
         "{\"vint\":4,\"vdouble\":40.04,\"vstring\":\"input-4\",\"vlist\":[4,2,3]},"
         "{\"vint\":5,\"vdouble\":50.05,\"vstring\":\"input-5\",\"vlist\":[5,2,3]},"
         "{\"vint\":6,\"vdouble\":60.06,\"vstring\":\"input-6\",\"vlist\":[6,2,3]},"
         "{\"vint\":7,\"vdouble\":70.07,\"vstring\":\"input-7\",\"vlist\":[7,2,3]},"
         "{\"vint\":8,\"vdouble\":80.08,\"vstring\":\"input-8\",\"vlist\":[8,2,3]},"
         "{\"vint\":9,\"vdouble\":90.09,\"vstring\":\"input-9\",\"vlist\":[9,2,3]}]";


TEST( EXPORT_IMPORT, FormatTableFile )
{
    //ioSettings().addSchemaFormat(schema_thrift, schema_str);
    //ioSettings().addSchemaFormat(schema_thrift, impex_schema_str);

    // no database connection
    std::shared_ptr<jsonio17::DataBase> database(nullptr);
    jsonio17::FormatImpexGenerator worker(database, true, jsonio17::FormatImpexGenerator::String);

    std::string other_format_string;
    worker.exportStringFromString( "FormatTableFile", export_simple_FormatTableFile_script,
                other_format_string, simple_data_string);

    EXPECT_EQ( other_format_string, import_simple_FormatTableFile_data );

    std::string schema_format_string;
    worker.importStringToString( "FormatTableFile", import_simple_FormatTableFile_script,
                import_simple_FormatTableFile_data, schema_format_string);

    EXPECT_EQ( schema_format_string, import_simple_result );
}

const char* const import_simple_result_key_value =
        "[{\"vbool\":true,\"vint\":0,\"vdouble\":0,\"vstring\":\"input-0\"},"
        "{\"vbool\":true,\"vint\":1,\"vdouble\":10.01,\"vstring\":\"input-1\",\"vlist\":[1,2,3]},"
        "{\"vbool\":true,\"vint\":2,\"vdouble\":20.02,\"vstring\":\"input-2\",\"vlist\":[2,4,6]},"
        "{\"vbool\":true,\"vint\":3,\"vdouble\":30.03,\"vstring\":\"input-3\",\"vlist\":[3,6,9]},"
        "{\"vbool\":true,\"vint\":4,\"vdouble\":40.04,\"vstring\":\"input-4\",\"vlist\":[4,8,12]},"
        "{\"vbool\":true,\"vint\":5,\"vdouble\":50.05,\"vstring\":\"input-5\",\"vlist\":[5,10,15]},"
        "{\"vbool\":true,\"vint\":6,\"vdouble\":60.06,\"vstring\":\"input-6\",\"vlist\":[6,12,18]},"
        "{\"vbool\":true,\"vint\":7,\"vdouble\":70.07,\"vstring\":\"input-7\",\"vlist\":[7,14,21]},"
        "{\"vbool\":true,\"vint\":8,\"vdouble\":80.08,\"vstring\":\"input-8\",\"vlist\":[8,16,24]},"
        "{\"vbool\":true,\"vint\":9,\"vdouble\":90.09,\"vstring\":\"input-9\",\"vlist\":[9,18,27]}]";

TEST( EXPORT_IMPORT, FormatKeyValueFile )
{
    //ioSettings().addSchemaFormat(schema_thrift, schema_str);
    //ioSettings().addSchemaFormat(schema_thrift, impex_schema_str);

    std::shared_ptr<jsonio17::DataBase> database(nullptr);
    jsonio17::FormatImpexGenerator worker(database, true, jsonio17::FormatImpexGenerator::String);

    std::string other_format_string;
    worker.exportStringFromString( "FormatKeyValueFile", export_simple_FormatKeyValueFile_script,
                other_format_string, simple_data_string);

    EXPECT_EQ( other_format_string, import_simple_FormatKeyValueFile_data );

    std::string schema_format_string;
    worker.importStringToString( "FormatKeyValueFile", import_simple_FormatKeyValueFile_script,
                import_simple_FormatKeyValueFile_data, schema_format_string);

    EXPECT_EQ( schema_format_string, import_simple_result_key_value );
}

const char* const import_simple_result_struct =
        "[{\"vbool\":true,\"vint\":0,\"vdouble\":0,\"vstring\":\"yaml-0\",\"vlist\":[0,0,0]},"
        "{\"vbool\":true,\"vint\":1,\"vdouble\":0.03,\"vstring\":\"yaml-1\",\"vlist\":[11,22,33]},"
        "{\"vbool\":true,\"vint\":2,\"vdouble\":0.06,\"vstring\":\"yaml-2\",\"vlist\":[22,44,66]},"
        "{\"vbool\":true,\"vint\":3,\"vdouble\":0.09,\"vstring\":\"yaml-3\",\"vlist\":[33,66,99]},"
        "{\"vbool\":true,\"vint\":4,\"vdouble\":0.12,\"vstring\":\"yaml-4\",\"vlist\":[44,88,132]}]";

TEST( IMPORT, FormatStructDataFile )
{
    //ioSettings().addSchemaFormat(schema_thrift, schema_str);
    //ioSettings().addSchemaFormat(schema_thrift, impex_schema_str);

    std::shared_ptr<jsonio17::DataBase> database(nullptr);
    jsonio17::FormatImpexGenerator worker(database, true, jsonio17::FormatImpexGenerator::String);

    std::string schema_format_string;
    worker.importStringToString( "FormatStructDataFile", import_simple_FormatStructDataFile_script,
                import_simple_FormatStructDataFile_data, schema_format_string);

    EXPECT_EQ( schema_format_string, import_simple_result_struct );
}

