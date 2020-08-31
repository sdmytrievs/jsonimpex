#include <iostream>
#include "jsonio17/jsonfree.h"
#include "jsonio17/jsonschema.h"
#include "jsonio17/jsondump.h"
#include "jsonio17/schema_thrift.h"
#include "jsonio17/io_settings.h"
#include "jsonimpex17/xmldump.h"
#include "thrift_ie_json.h"
#include "tests/impex_schema.h"


using namespace jsonio17;

void FreeSchema();


int main()
{
    try{

        ioSettings().addSchemaFormat( schema_thrift, impex_schema_str );

        FreeSchema();

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
    auto in_object = json::loads( "FormatTableFile", FormatTableFile_value );
    std::cout << in_object.dump() << "\n-------------------\n";
    FormatTableFile fformatdata;
    readDataFromJsonSchema(  in_object, "FormatTableFile",  &fformatdata );

    //fformatdata.printTo( std::cout);
    auto out_object = JsonSchema::object( "FormatTableFile" );
    writeDataToJsonSchema( out_object,"FormatTableFile",  &fformatdata );
    std::cout << out_object.dump() << "\n-------------------\n";
}

