#include <iostream>
#include "jsonio17/jsonfree.h"
#include "jsonio17/jsonschema.h"
#include "jsonio17/jsondump.h"
#include "jsonio17/schema_thrift.h"
#include "jsonio17/io_settings.h"
#include "jsonimpex17/yamldump.h"
#include "tests/example_schema.h"

using namespace jsonio17;

void FreeSchema();

void SimpleSchema();

void ComplexSchema();


int main()
{
    try{

        ioSettings().addSchemaFormat( schema_thrift, schema_str );

        FreeSchema();

        SimpleSchema();

        ComplexSchema();

    }
    catch(jsonio17::jsonio_exception& e)
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
    auto json_arr = json::loads( test_free_str );
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
