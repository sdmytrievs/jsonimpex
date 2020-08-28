#pragma once

#include <gtest/gtest.h>

#include <list>
#include <map>
#include <unordered_map>
#include "jsonio17/jsondump.h"
#include "jsonio17/jsonschema.h"
#include "jsonio17/jsonschema.h"
#include "jsonio17/io_settings.h"
#include "jsonio17/schema_thrift.h"
#include "jsonimpex17/yamldump.h"
#include "example_schema.h"

using namespace testing;
using namespace jsonio17;


TEST( JSONYAML, ObjectSimple)
{
    ioSettings().addSchemaFormat(schema_thrift, schema_str);

    auto empty_obj = JsonSchema( JsonSchema::object("SimpleSchemaTest") );
    EXPECT_EQ( yaml::dump(empty_obj), "{}" );

    auto json_obj = json::loads( "SimpleSchemaTest", simple_schema_value );
    EXPECT_EQ( yaml::dump(json_obj), simple_yaml_value );
    //std::cout << " <" << yaml::dump(json_obj) << ">" << std::endl;

    auto yaml_obj = yaml::loads( "SimpleSchemaTest", simple_yaml_value );
    EXPECT_EQ( yaml::dump(yaml_obj), simple_yaml_value );
    EXPECT_EQ( json_obj.dump(true), yaml_obj.dump(true) );
}

TEST( JSONYAML, ObjectComplex)
{
    ioSettings().addSchemaFormat(schema_thrift, schema_str);

    auto json_obj = json::loads( "ComplexSchemaTest", complex_schema_value );
    auto yaml_txt = yaml::dump(json_obj);
    // std::cout << yaml_txt << std::endl;

    auto yaml_obj = yaml::loads( "ComplexSchemaTest", yaml_txt );
    EXPECT_EQ( yaml::dump(yaml_obj), yaml_txt );
    EXPECT_EQ( json_obj.dump(true), yaml_obj.dump(true) );
}

TEST( JSONYAML, ObjectFree)
{
    auto json_obj = json::loads( test_free_str );
    auto yaml_txt = yaml::dump(json_obj);
    // std::cout << yaml_txt << std::endl;

    auto yaml_arr = JsonFree::array();
    yaml::loads( yaml_txt, yaml_arr  );
    EXPECT_EQ( yaml::dump(yaml_arr), yaml_txt );
    EXPECT_EQ( json_obj.dump(), yaml_arr.dump() );
}
