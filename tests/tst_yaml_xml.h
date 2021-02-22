#pragma once

#include <gtest/gtest.h>

#include <fstream>
#include "jsonio17/jsondump.h"
#include "jsonio17/jsonschema.h"
#include "jsonio17/io_settings.h"
#include "jsonio17/schema_thrift.h"
#include "jsonimpex17/yamldump.h"
#include "jsonimpex17/xmldump.h"
#include "jsonimpex17/yaml_xml2file.h"
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

TEST( JSONYAML, TestYamlFileWrite )
{
    std::string fpath = "testwrite.yaml";
    std::string fdata = "{\"width\":20,\"precision\":10}";
    auto obj = json::loads(fdata);

    JsonYamlXMLFile ftxt(fpath);
    EXPECT_EQ( ftxt.path(), fpath );
    EXPECT_EQ( ftxt.type(), TxtFile::Yaml );

    EXPECT_NO_THROW( ftxt.save(obj) );
    EXPECT_EQ( ftxt.load_json(), obj.toString(true) );
    EXPECT_EQ( ftxt.load_all(), "width: 20\nprecision: 10" );

    EXPECT_FALSE( ftxt.isOpened() );
}

TEST( JSONYAML, TestYamlFileLoad )
{
    std::string fpath = "test.yaml";
    std::string fdata = "width: 20\nprecision: 10";

    std::ofstream stream;
    stream.open(fpath);
    stream << fdata;
    stream.close();

    JsonYamlXMLFile ftxt(fpath);
    EXPECT_EQ( ftxt.path(), fpath );
    EXPECT_EQ( ftxt.name(), "test" );
    EXPECT_EQ( ftxt.ext(), "yaml" );
    EXPECT_EQ( ftxt.dir(), "");
    EXPECT_EQ( ftxt.type(), TxtFile::Yaml );

    EXPECT_TRUE( ftxt.exist() );
    EXPECT_EQ( ftxt.load_all(), fdata );

    auto obj = JsonFree::object();
    EXPECT_NO_THROW( ftxt.load(obj) );
    EXPECT_EQ( ftxt.load_json(), obj.toString(true) );

    EXPECT_FALSE( ftxt.isOpened() );
}

TEST( JSONYAML, TestYamlArrayFileSave )
{
    std::string fpath = "test_array_write.yaml";

    JsonYamlXMLArrayFile ftxt(fpath);
    EXPECT_EQ( ftxt.path(), fpath );
    EXPECT_EQ( ftxt.type(), TxtFile::Yaml );
    EXPECT_NO_THROW( ftxt.Open(TxtFile::WriteOnly) );

    for( size_t ii=0; ii<5; ii++)
        EXPECT_TRUE( ftxt.saveNext( std::string("{ \"key\": ")+std::to_string(ii)+"}") );

    EXPECT_TRUE( ftxt.isOpened() );
    EXPECT_NO_THROW( ftxt.Close() );
    EXPECT_EQ( ftxt.load_all(), "- key: 0\n- key: 1\n- key: 2\n- key: 3\n- key: 4" );
}

TEST( JSONYAML, TestYamlArrayFileLoad )
{
    std::string fpath = "test_array_load.yaml";
    std::string fdata = "- key: 0\n- key: 1\n- key: 2\n- key: 3\n- key: 4";
    auto obj = JsonFree::object();

    std::ofstream stream;
    stream.open(fpath);
    stream << fdata;
    stream.close();

    JsonYamlXMLArrayFile fjson(fpath);
    EXPECT_EQ( fjson.path(), fpath );
    EXPECT_EQ( fjson.type(), TxtFile::Yaml );
    EXPECT_NO_THROW( fjson.Open(TxtFile::ReadOnly) );
    EXPECT_TRUE( fjson.isOpened() );

    for( size_t ii=0; ii<5; ii++)
    {
        EXPECT_TRUE( fjson.loadNext( obj ));
        EXPECT_EQ( obj.toString(true), std::string("{\"key\":")+std::to_string(ii)+"}" );
    }
    EXPECT_NO_THROW( fjson.Close() );
    EXPECT_EQ( fjson.load_all(), fdata );
}


//---------------------------------------------------------------------------


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


TEST( JSONXML, TestXMLFileWrite )
{
    std::string fpath = "testwrite.xml";
    std::string fdata = "{\"width\":20,\"precision\":10}";
    auto obj = json::loads(fdata);

    JsonYamlXMLFile ftxt(fpath);
    EXPECT_EQ( ftxt.path(), fpath );
    EXPECT_EQ( ftxt.type(), TxtFile::XML );

    EXPECT_NO_THROW( ftxt.save(obj) );
    EXPECT_EQ( ftxt.load_json(), obj.toString(true) );
    EXPECT_EQ( ftxt.load_all(), "<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"yes\"?>\n<node>\n\t<width>20</width>\n\t<precision>10</precision>\n</node>\n" );

    EXPECT_FALSE( ftxt.isOpened() );
}

TEST( JSONXML, TestXMLFileLoad )
{
    std::string fpath = "test.xml";
    std::string fdata = "<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"yes\"?>\n<node>\n\t<width>20</width>\n\t<precision>10</precision>\n</node>\n";

    std::ofstream stream;
    stream.open(fpath);
    stream << fdata;
    stream.close();

    JsonYamlXMLFile ftxt(fpath);
    EXPECT_EQ( ftxt.path(), fpath );
    EXPECT_EQ( ftxt.name(), "test" );
    EXPECT_EQ( ftxt.ext(), "xml" );
    EXPECT_EQ( ftxt.dir(), "");
    EXPECT_EQ( ftxt.type(), TxtFile::XML );

    EXPECT_TRUE( ftxt.exist() );
    EXPECT_EQ( ftxt.load_all(), fdata );

    auto obj = JsonFree::object();
    EXPECT_NO_THROW( ftxt.load(obj) );
    EXPECT_EQ( ftxt.load_json(), obj.toString(true) );

    EXPECT_FALSE( ftxt.isOpened() );
}

TEST( JSONXML, TestXMLArrayFileSave )
{
    std::string fpath = "test_array_write.xml";

    JsonYamlXMLArrayFile ftxt(fpath);
    EXPECT_EQ( ftxt.path(), fpath );
    EXPECT_EQ( ftxt.type(), TxtFile::XML );
    EXPECT_NO_THROW( ftxt.Open(TxtFile::WriteOnly) );

    for( size_t ii=0; ii<5; ii++)
        EXPECT_TRUE( ftxt.saveNext( std::string("{ \"key\": ")+std::to_string(ii)+"}") );

    EXPECT_TRUE( ftxt.isOpened() );
    EXPECT_NO_THROW( ftxt.Close() );
    std::string xml_txt = "<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"yes\"?>\n<node isarray=\"true\">"
                          "\n\t<element>\n\t\t<key>0</key>\n\t</element>\n\t<element>\n\t\t<key>1</key>\n\t</element>\n\t"
                          "<element>\n\t\t<key>2</key>\n\t</element>\n\t<element>\n\t\t<key>3</key>\n\t</element>\n\t"
                          "<element>\n\t\t<key>4</key>\n\t</element>\n</node>\n";
    EXPECT_EQ( ftxt.load_all(), xml_txt );
}

TEST( JSONXML, TestXMLArrayFileLoad )
{
    //std::string fpath = "test_array_load.xml";
    std::string xml_txt = "<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"yes\"?>\n<node isarray=\"true\">"
                          "\n\t<element>\n\t\t<key>0</key>\n\t</element>\n\t<element>\n\t\t<key>1</key>\n\t</element>\n\t"
                          "<element>\n\t\t<key>2</key>\n\t</element>\n\t<element>\n\t\t<key>3</key>\n\t</element>\n\t"
                          "<element>\n\t\t<key>4</key>\n\t</element>\n</node>\n";
    auto obj = JsonFree::object();

    // load from string
    JsonYamlXMLArrayFile fjson("");
    EXPECT_EQ( fjson.path(), "" );
    fjson.set_type(TxtFile::XML);
    EXPECT_EQ( fjson.type(), TxtFile::XML );

    EXPECT_NO_THROW( fjson.loadString(xml_txt) );
    EXPECT_FALSE( fjson.isOpened() );

    for( size_t ii=0; ii<5; ii++)
    {
        EXPECT_TRUE( fjson.loadNext( obj ));
        EXPECT_EQ( obj.toString(true), std::string("{\"key\":")+std::to_string(ii)+"}" );
    }
}
