#include <iostream>
#include "jsonio17/jsonfree.h"
#include "jsonio17/jsonschema.h"
#include "jsonio17/jsondump.h"
#include "jsonio17/schema_thrift.h"
#include "jsonio17/io_settings.h"
#include "jsonimpex17/xmldump.h"
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
    auto json_arr = json::loads( "{\"about\":{\"version\":1},\"values\":[[1,2],[3,4]]}" );
    auto xml_txt = XML::dump(json_arr);
    std::cout << xml_txt << std::endl << "------------" << std::endl;

    auto xml_arr = JsonFree::array();
    XML::loads( xml_txt, xml_arr  );
    std::cout << XML::dump(xml_arr) << std::endl << "------------" << std::endl;

}

void SimpleSchema()
{
    auto json_obj = json::loads( "SimpleSchemaTest", simple_schema_value );
    auto xml_txt = XML::dump(json_obj);
    std::cout << xml_txt << std::endl << "------------" << std::endl;

    auto xml_obj = XML::loads( "SimpleSchemaTest", xml_txt );
    std::cout << XML::dump(xml_obj) << std::endl << "------------" << std::endl;
}


void ComplexSchema()
{
    auto json_obj = json::loads( "ComplexSchemaTest", complex_schema_value );
    auto xml_txt = XML::dump(json_obj);
    std::cout << xml_txt << std::endl << std::endl;

    auto xml_obj = XML::loads( "ComplexSchemaTest", xml_txt );
    std::cout << XML::dump(xml_obj) << std::endl << "------------" << std::endl;
}
