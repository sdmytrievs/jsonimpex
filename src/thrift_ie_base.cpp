#include <iostream>
//#include "jsonio/jsondomfree.h"
//#include "jsonimpex/yamlxml2file.h"
#include "jsonio17/io_settings.h"
#include "jsonio17/jsonschema.h"
#include "thrift_ie_base.h"
#include "TJSONSerializer.h"

namespace jsonio17 {


void writeDataToJsonSchema( JsonBase& object,
                            const std::string& struct_name, ::apache::thrift::TBase* data )
{
    //Set the Serializator
    ::std::shared_ptr<TJSONSerializer> proto;
    proto.reset(new TJSONSerializer( ioSettings().Schema() ));
    proto->setDom( object, struct_name );
    // Get bson object from structure
    /*int  i = */data->write(proto.get());
    //std::cout << "Wrote " << i << " bytes" << std::endl;
}

void readDataFromJsonSchema( const JsonBase& object,
                             const std::string& struct_name,  ::apache::thrift::TBase* data )
{
    //Set the Serializator
    ::std::shared_ptr<TJSONSerializer> proto;
    proto.reset(new TJSONSerializer(ioSettings().Schema()));
    proto->setDom( const_cast<JsonBase&>(object), struct_name );
    // Get bson object from structure
    /*int  i = */data->read(proto.get());
    //std::cout << "Read " << i << " bytes" << std::endl;
}


void AbstractIEFile::set_readed_data()
{
    // set up predefined data
    auto itr = block.defaults.begin();
    while ( itr != block.defaults.end() )
    {
        auto fld = dynamic_cast<const JsonSchema*>( dom_node.field(itr->first) );
        if( fld == nullptr)
        {
            std::cout << "Not found " <<  itr->first << std::endl;
            itr++;
            continue;
        }
        fld->setValue(itr->second);
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

        auto idom = block.matches.find( impkey );
        if( idom == block.matches.end() ) // not converted
            continue;
        std::string domkey = idom->second.field; //idom->second.fieldkey;
        if( idom->second.ignore || domkey.empty() )  // skip ignored
            continue;

        auto fld = dom_node->field(domkey);
        if( !fld )
        {
            auto pos = domkey.find_last_of(".");
            if(pos != std::string::npos)
            {
                std::string key = std::string(domkey,  pos+1 );
                domkey = std::string(domkey, 0, pos);
                fld = dom_node->field(domkey);
                if( fld && fld->isMap() )
                {
                    run_convert( idom->second.convert, impvalue );
                    run_lua_script_string( idom->second.script, impvalue, false );
                    fld->addMapKeyValue( key, impvalue[0]  );
                }
            }
            continue;
        }

        run_convert( idom->second.convert, impvalue );
        if( fld->elemTypeId() == ::apache::thrift::protocol::T_STRING )
            run_lua_script_string( idom->second.script, impvalue, fld->isArray() );
        else
            run_lua_script( idom->second.script, impvalue, fld->isArray() );

        if( fld->isArray() && !fld->isMap() )
        {
            fld->resetArray( impvalue );
        }
        else
            if(impvalue.size()>0)
                fld->setValue(impvalue[0]);
        //     it++;
    }
}

/// change value, using convert
void AbstractIEFile::run_convert( const std::map<std::string,std::string>&  convert,
                                  std::vector<std::string>& values )
{
    if( convert.empty() )
        return;
    for(uint ii=0; ii< values.size(); ii++ )
    {
        auto itmap = convert.find( values[ii] );
        if( itmap != convert.end() )
            values[ii] = itmap->second;
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
        TArray2Base::string2value( *it++, dv );
        dvals.push_back(dv);
    }

    // run script
    bool ret;
    if( isArray )
        ret = lua_run.runFunc(luascript, dvals);
    else
        ret = lua_run.runFunc(luascript, dvals[0]);

    if( ret)   // script run ok
    {
        // convert to strings
        values.clear();
        auto itd = dvals.begin();
        while( itd != dvals.end() )
            values.push_back(TArray2Base::value2string( *itd++ ));
    }
}

// change all values use script
void AbstractIEFile::run_lua_script_string( const std::string& luascript,
                                            std::vector<std::string>& values, bool isArray )
{
    if(luascript.empty()  || values.size() < 1 )
        return;

    // run script
    if( isArray )
        lua_run.runFunc(luascript, values);
    else
        lua_run.runFunc(luascript, values[0]);
}

// Run script for operation on data values in block
void AbstractIEFile::run_block_script( const std::string& luascript, JsonDomSchema* obj )
{
    if(luascript.empty() )
        return;

    std::string recBsonText;
    printNodeToJson( recBsonText, obj );

    // run script
    std::string newjson = _lua.run( luascript, recBsonText, false );
    if( !newjson.empty() )
    {
        obj->clearField();
        parseJsonToNode( newjson, obj );
    }
}

std::string AbstractIEFile::impex_field_organization( const std::string& impex_field_key )
{
    auto itPair = block.pairs.find(impex_field_key);
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
            groupValues = extractValue( groupKey, write_data ); // get old
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

void AbstractIEFile::loadString(const std::string &)
{
    jsonioErr( "loadString", "Loading string to import from  or export to stringstream  not implemented yet.");
}


// Insert key value paies to extern list
void AbstractIEFile::addMapKeyValue( const std::string& _key, const std::vector<std::string>& vec )
{
    auto itr = externData.find(_key);
    if( itr != externData.end() )
    {
        // add only for type "group"
        if( impexFieldOrganization( _key ) == "group")
        {
            itr->second.insert( itr->second.end(), vec.begin(), vec.end() );
            return;
        }
    }
    // can overload previous
    externData[_key] = vec;
}


// Extract data from bson
void AbstractIEFile::extractDataToWrite( const std::string& blockdata, ValuesMap& writeData )
{
    std::string impexKey, impexOrganization, nodeValue;
    std::vector<std::string> nodeValues;
    externData.clear();

    // extract json data to internal structure
    structNode->clearField();
    parseJsonToNode( blockdata, structNode );
    // Run script for operation on data values in block (if need some data preparation )
    run_block_script(block.script, structNode);

    // set up predefined data
    auto itr1 = block.defaults.begin();
    while ( itr1 != block.defaults.end() )
    {
        impexKey = itr1->first;
        impexOrganization = impexFieldOrganization( impexKey );

        // we can split second to vector if type "list" | "set"
        if( impexOrganization=="list" || impexOrganization == "set"  )
            nodeValues = regexp_split( itr1->second, " "/*format.value_token_regexp*/ );
        else
            nodeValues =  { itr1->second };
        addMapKeyValue( impexKey, nodeValues );
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

        auto fld = structNode->field(domkey);
        if( !fld )
        {   idom++; continue; } // field is not present

        if( fld->isArray() )
        {
            auto sizes = fld->getArraySizes();
            nodeValues.resize(sizes[0]);
            fld->getArray(nodeValues);
        }
        else
        {
            if( !fld->getValue( nodeValue  ) )
            {   idom++; continue; }     //error extraction
            nodeValues = { nodeValue};
        }

        impexOrganization = impexFieldOrganization( impexKey );
        bool isArray = impexOrganization=="list" || impexOrganization == "set";

        run_convert( idom->second.convert, nodeValues );
        if( fld->elemTypeId() == ::apache::thrift::protocol::T_STRING )
            run_lua_script_string( idom->second.script, nodeValues, isArray );
        else
            run_lua_script( idom->second.script, nodeValues, isArray );

        addMapKeyValue( impexKey, nodeValues );
        idom++;
    }

    // add to results
    writeData.insert( external_data.begin(), external_data.end());
}


} // namespace jsonio17
