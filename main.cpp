#include <iostream>
#include "jsonio/jsonfree.h"
#include "jsonio/jsonschema.h"
#include "jsonio/jsondump.h"
#include "jsonio/schema_thrift.h"
#include "jsonio/io_settings.h"
#include "jsonimpex/xmldump.h"
#include "thrift_ie_json.h"
#include "tests/impex_schema.h"


using namespace jsonio;

void FreeSchema();


int main()
{
    try{

        ioSettings().addSchemaFormat( schema_thrift, impex_schema_str );

        FreeSchema();

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
    auto in_object = json::loads( "FormatTableFile", FormatTableFile_value );
    std::cout << in_object.dump() << "\n-------------------\n";
    FormatTableFile fformatdata;
    readDataFromJsonSchema(  in_object, "FormatTableFile",  &fformatdata );

    //fformatdata.printTo( std::cout);
    auto out_object = JsonSchema::object( "FormatTableFile" );
    writeDataToJsonSchema( out_object,"FormatTableFile",  &fformatdata );
    std::cout << out_object.dump() << "\n-------------------\n";
}

