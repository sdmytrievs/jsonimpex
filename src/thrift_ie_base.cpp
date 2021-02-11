#include <iostream>
#include "jsonio17/io_settings.h"
#include "jsonio17/jsonschema.h"
#include "jsonio17/jsondump.h"
#include "thrift_ie_base.h"
#include "TJSONSerializer.h"

namespace jsonio17 {


void writeDataToJsonSchema( JsonBase& object,
                            const std::string& struct_name, ::apache::thrift::TBase* data )
{
    //Set the Serializator
    std::shared_ptr<TMemoryBuffer> buffer(new TMemoryBuffer());
    ::std::shared_ptr<TJSONSerializer> proto = std::make_shared<TJSONSerializer>( buffer, &ioSettings().Schema() );
    proto->setDom( object, struct_name );
    // Set json object to  structure
    /*int  i = */data->write(proto.get());
    //std::cout << "Wrote " << i << " bytes" << std::endl;
}

void readDataFromJsonSchema( const JsonBase& object,
                             const std::string& struct_name,  ::apache::thrift::TBase* data )
{
    //Set the Serializator
    std::shared_ptr<TMemoryBuffer> buffer(new TMemoryBuffer());
    ::std::shared_ptr<TJSONSerializer> proto = std::make_shared<TJSONSerializer>( buffer, &ioSettings().Schema() );
    proto->setDom( object, struct_name );
    // Get json object from structure
    /*int  i = */data->read(proto.get());
    //std::cout << "Read " << i << " bytes" << std::endl;
}


void AbstractIEFile::set_readed_data()
{
    // set up predefined data
    auto itr = block.defaults.begin();
    while ( itr != block.defaults.end() )
    {
        auto fld =  dom_node.field_add(itr->first);
        if( fld == nullptr)
        {
            std::cout << "Not found " <<  itr->first << std::endl;
            itr++;
            continue;
        }
        fld->loads( itr->second );
        itr++;
    }

    // set up readed data
    std::string impkey;
    std::vector<std::string> impvalue;
    auto it = external_data.begin();
    while ( it != external_data.end() )
    {
        impkey = it->first;
        impvalue = it->second;
        it++;

        if( impvalue.empty() )
            continue;
        auto idom = block.matches.find( impkey );
        if( idom == block.matches.end() ) // not converted
            continue;
        std::string domkey = idom->second.field;
        if( idom->second.ignore || domkey.empty() )  // skip ignored
            continue;

        auto fld =  dom_node.field_add(domkey);
        if( !fld )
        {
            auto pos = domkey.find_last_of(".");
            if( pos != std::string::npos )
            {
                std::string key = std::string(domkey,  pos+1 );
                domkey = std::string(domkey, 0, pos);
                fld =  dom_node.field_add(domkey);
                if( fld && fld->isMap() )
                {
                    run_convert( idom->second.convert, impvalue );
                    run_lua_script_string( idom->second.script, impvalue, false );
                    fld->load_value_via_path( key, impvalue[0]  ); // add line to map
                }
            }
            continue;
        }
        run_convert( idom->second.convert, impvalue );
        if( fld->elemType() == FieldDef::T_STRING )
            run_lua_script_string( idom->second.script, impvalue, fld->isArray() );
        else
            run_lua_script( idom->second.script, impvalue, fld->isArray() );

        //std::cout << domkey << " : " << json::dump(impvalue)  << std::endl;
        if( fld->isArray() && !fld->isMap() )
        {
            fld->loads_list_from( impvalue );
        }
        else  if( impvalue.size()>0 )
        {
           fld->loads_from( impvalue[0] );
        }
    }
}

// change value, using convert
void AbstractIEFile::run_convert( const std::map<std::string,std::string>&  convert,
                                  std::vector<std::string>& values )
{
    if( convert.empty() )
        return;

    auto copy_values = std::move(values);
    values.clear();
    for( size_t ii=0; ii< copy_values.size(); ii++ )
    {
        auto itmap = convert.find( copy_values[ii] );
        if( itmap != convert.end() )
            values.push_back( itmap->second );
    }
}

// change all values use script
void AbstractIEFile::run_lua_script( const std::string& luascript,
                                     std::vector<std::string>& values, bool isArray )
{
    if(luascript.empty()  || values.size() < 1 )
        return;

    double dv;
    std::vector<double> dvals;

    // convert to numbers
    auto it = values.begin();
    while( it != values.end() )
    {
        string2v( *it++, dv );
        dvals.push_back( dv );
    }

    // run script
    bool ret;
    if( isArray )
        ret = lua_run.runFunc( luascript, dvals );
    else
        ret = lua_run.runFunc( luascript, dvals[0] );

    if( ret)   // script run ok
    {
        // convert to strings
        values.clear();
        auto itd = dvals.begin();
        while( itd != dvals.end() )
            values.push_back( v2string( *itd++ ) );
    }
}

// change all values use script
void AbstractIEFile::run_lua_script_string( const std::string& luascript,
                                            std::vector<std::string>& values, bool isArray )
{
    if( luascript.empty()  || values.size() < 1 )
        return;

    // run script
    if( isArray )
        lua_run.runFunc( luascript, values );
    else
        lua_run.runFunc( luascript, values[0] );
}

// Run script for operation on data values in block
void AbstractIEFile::run_block_script( const std::string& luascript, JsonSchema& object )
{
    if( luascript.empty() )
        return;

    std::string json_data = object.dump( true );
    std::string newjson = lua_run.run( luascript, json_data, false );
    if( !newjson.empty() )
    {
        object.loads( newjson );
    }
}

std::string AbstractIEFile::impex_field_organization( const std::string& impex_field_key )
{
    auto itPair = block.pairs.find( impex_field_key );
    if( itPair != block.pairs.end() )
        return itPair->second.organization;
    return "";
}

std::string AbstractIEFile::impex_field_type( const std::string& impex_field_key )
{
    auto itPair = block.pairs.find(impex_field_key);
    if( itPair != block.pairs.end() )
        return itPair->second.datatype;
    return "";
}

// export part-----------------------------------------

void AbstractIEFile::assemble_groups( key_values_table_t& write_data )
{
    std::string groupKey, itKey;
    std::vector<std::string> groupValues;
    size_t groupKeySize;
    size_t ndx;

    auto itPair = block.pairs.begin();
    while( itPair != block.pairs.end() )
    {
        if( itPair->second.organization == "group" )
        {
            groupKey = itPair->first;
            groupValues = extract_value( groupKey, write_data );
            groupKeySize = groupKey.size();

            auto itv = write_data.begin();
            while( itv != write_data.end() )
            {
                itKey = itv->first;
                if( itKey.substr( 0, groupKeySize ) == groupKey )
                {
                    if( is<size_t>( ndx, itKey.substr( groupKeySize ) ) )
                    {
                        if( ndx+1 > groupValues.size() )
                            groupValues.resize(ndx+1, " ");
                        if( itv->second.size() > 0 )
                            groupValues[ndx] =  itv->second[0];
                        itv = write_data.erase(itv);
                        continue;
                    }
                }
                itv++;
            }
            write_data[groupKey]  = groupValues;
        }
        itPair++;
    }
}


// Insert key value paies to extern list
void AbstractIEFile::add_map_key_value( const std::string& field_key, const  std::vector<std::string>& values )
{
    auto itr = external_data.find( field_key );
    if( itr != external_data.end() )
    {
        // add only for type "group"
        if( impex_field_organization( field_key ) == "group" )
        {
            itr->second.insert( itr->second.end(), values.begin(), values.end() );
            return;
        }
    }
    // can overload previous
    external_data[ field_key ] = values;
}


// Extract data from internal schema based json
AbstractIEFile::AbstractIEFile(ImpexFormatFile::ImpexMode amode, const std::string &data_schema_name, const std::string &lua_lib_path):
    mode_use(amode), lua_run(lua_lib_path), label(data_schema_name), dom_node( JsonSchema::object(data_schema_name) )
{}

AbstractIEFile::~AbstractIEFile()
{
    closeFile();
}

void AbstractIEFile::extractDataToWrite( const std::string& json_block_data, key_values_table_t& data_to_write )
{
    std::string impexKey, impexOrganization, nodeValue;
    std::vector<std::string> nodeValues;
    external_data.clear();

    // extract json data to internal structure
    dom_node.loads( json_block_data );
    // Run script for operation on data values in block (if need some data preparation )
    run_block_script( block.script, dom_node );

    // set up predefined data
    auto itr1 = block.defaults.begin();
    while ( itr1 != block.defaults.end() )
    {
        impexKey = itr1->first;
        impexOrganization = impex_field_organization( impexKey );

        // we can split second to vector if type "list" | "set"
        if( impexOrganization == "list" || impexOrganization == "set"  )
            nodeValues = regexp_split( itr1->second, " "/*format.value_token_regexp*/ );
        else
            nodeValues =  { itr1->second };
        add_map_key_value( impexKey, nodeValues );
        itr1++;
    }

    // extract fields from structNode
    auto idom = block.matches.begin();
    while ( idom != block.matches.end() )
    {
        impexKey = idom->first;
        std::string domkey = idom->second.field; //idom->second.fieldkey;
        if( idom->second.ignore || domkey.empty() )  // skip ignored
        {   idom++; continue; }

        auto fld = dom_node.field( domkey );
        if( !fld )
        {   idom++; continue; } // field is not present

        if( fld->isArray() )
        {
            auto sizes = fld->array_sizes();
            nodeValues.resize( sizes[0] );
            fld->get_to_list( nodeValues );
        }
        else
        {
            if( !fld->get_to( nodeValue  ) )
            {   idom++; continue; }     //error extraction
            nodeValues = { nodeValue};
        }

        impexOrganization = impex_field_organization( impexKey );
        bool isArray = impexOrganization =="list" || impexOrganization == "set";

        run_convert( idom->second.convert, nodeValues );
        if( fld->elemType() == FieldDef::T_STRING )
            run_lua_script_string( idom->second.script, nodeValues, isArray );
        else
            run_lua_script( idom->second.script, nodeValues, isArray );

        add_map_key_value( impexKey, nodeValues );
        idom++;
    }

    // add to results
    data_to_write.insert( external_data.begin(), external_data.end());
}


} // namespace jsonio17
