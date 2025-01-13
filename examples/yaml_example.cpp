#include <iostream>
#include "jsonio/jsonfree.h"
#include "jsonio/jsonschema.h"
#include "jsonio/jsondump.h"
#include "jsonio/schema_thrift.h"
#include "jsonio/io_settings.h"
#include "jsonimpex/yamldump.h"
#include "tests/example_schema.h"

using namespace jsonio;

void FreeSchema();

void SimpleSchema();

void ComplexSchema();


int main()
{
    jsonio::JsonioSettings::settingsFileName = "jsonimpex-config.json";
    try{

        ioSettings().addSchemaFormat( schema_thrift, schema_str );
        FreeSchema();
        SimpleSchema();
        ComplexSchema();

    }
    catch(jsonio::jsonio_exception& e)
    {
        std::cout <<   e.what() <<  std::endl;
    }
    catch(std::exception& e)
    {
        std::cout <<   "std::exception: " << e.what() <<  std::endl;
    }
    catch(...)
    {
        std::cout <<  "unknown exception" <<  std::endl;
    }

    return 0;

}

void FreeSchema()
{
    auto json_arr = json::loads( "{\"about\":{\"version\":1},\"values\":[[1,2],[3,4]]}" );
    auto yaml_txt = yaml::dump(json_arr);
    std::cout << yaml_txt << std::endl << "------------" << std::endl;

    auto yaml_arr = JsonFree::array();
    yaml::loads( yaml_txt, yaml_arr  );
    std::cout << yaml::dump(yaml_arr) << std::endl << "------------" << std::endl;

    std::cout << yaml::Yaml2Json(yaml_txt) << std::endl << "------------" << std::endl;
}

void SimpleSchema()
{
    auto json_obj = json::loads( "SimpleSchemaTest", simple_schema_value );
    auto yaml_txt = yaml::dump(json_obj);
    std::cout << yaml::dump(json_obj) << std::endl << "------------" << std::endl;

    auto yaml_obj = yaml::loads( "SimpleSchemaTest", yaml_txt );
    std::cout << yaml::dump(yaml_obj) << std::endl << "------------" << std::endl;
}


void ComplexSchema()
{
    auto json_obj = json::loads( "ComplexSchemaTest", complex_schema_value );
    auto yaml_txt = yaml::dump(json_obj);
    std::cout << yaml::dump(json_obj) << std::endl << std::endl;

    auto yaml_obj = yaml::loads( "ComplexSchemaTest", yaml_txt );
    std::cout << yaml::dump(yaml_obj) << std::endl << "------------" << std::endl;
}
