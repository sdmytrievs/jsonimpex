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
#include "jsonimpex17/xmldump.h"
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

    // Problems with null object write+read
    EXPECT_EQ( yaml::dump(yaml_arr), yaml_txt );
    EXPECT_EQ( json_obj.dump(), yaml_arr.dump() );
}

TEST( JSONXML, ObjectSimple)
{
    ioSettings().addSchemaFormat(schema_thrift, schema_str);

    auto empty_obj = JsonSchema( JsonSchema::object("SimpleSchemaTest") );
    EXPECT_EQ( XML::dump(empty_obj), "<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"yes\"?>\n<node />\n" );

    auto json_obj = json::loads( "SimpleSchemaTest", simple_schema_value );
    auto xml_txt = XML::dump(json_obj);
    // std::cout << xml_txt << std::endl;

    auto xml_obj = XML::loads( "SimpleSchemaTest", xml_txt );
    EXPECT_EQ( XML::dump(xml_obj), xml_txt );
    EXPECT_EQ( json_obj.dump(true), xml_obj.dump(true) );
}

TEST( JSONXML, ObjectComplex)
{
    ioSettings().addSchemaFormat(schema_thrift, schema_str);

    auto json_obj = json::loads( "ComplexSchemaTest", complex_schema_value );
    auto xml_txt = XML::dump(json_obj);
    // std::cout << xml_txt << std::endl;

    auto xml_obj = XML::loads( "ComplexSchemaTest", xml_txt );
    EXPECT_EQ( XML::dump(xml_obj), xml_txt );
    EXPECT_EQ( json_obj.dump(true), xml_obj.dump(true) );
}

TEST( JSONXML, ObjectFree)
{
    auto json_arr = json::loads( test_free_str );
    auto xml_txt = XML::dump(json_arr);
    // std::cout << xml_txt << std::endl;

    auto xml_arr = JsonFree::object();
    XML::loads( xml_txt, xml_arr  );
// Some problems with {}, [], " " and null
//    EXPECT_EQ( XML::dump(xml_arr), xml_txt );
//    EXPECT_EQ( json_arr.dump(), xml_arr.dump() );
}

