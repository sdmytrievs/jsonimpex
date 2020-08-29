#include <iomanip>
#include "pugixml.hpp"

#include "jsonio17/jsonfree.h"
#include "jsonio17/jsonschema.h"
#include "jsonio17/jsonbuilder.h"
#include "jsonio17/service.h"
#include "jsonimpex17/xmldump.h"


namespace jsonio17 {

namespace XML {

/// Print internal Json object to XML structure
void object_emitter( pugi::xml_node& out, const JsonBase& object );

/// Parse XML node to the JSON object
void parse( const pugi::xml_node& node, JsonBuilderBase& builder  );
/// Add ',' separated array
void add_array( const std::string& key, const std::string& value, JsonBuilderBase& builder );


//--------------------------------------------------------------

std::string dump( const JsonBase &object )
{
    std::stringstream os;
    XML::dump( os, object );
    return os.str();
}

void dump( std::ostream &os, const JsonBase &object )
{
    pugi::xml_document doc;
    pugi::xml_node decl = doc.append_child( pugi::node_declaration );
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-16";
    decl.append_attribute("standalone") = "yes";

    // define root node
    pugi::xml_node fld = doc.append_child( "node" );
    if( object.type() == JsonBase::Array )
        fld.append_attribute("isarray") = true;
    object_emitter( fld, object );

    pugi::xml_writer_stream xml_stream(os);
    doc.save( xml_stream );
}

void loads( const std::string &xml_str, JsonBase &object )
{
    // read and parse xml document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string( xml_str.c_str() );
    if (!result)
    {
        std::stringstream str;
        str << "XML parsed with errors: \n";
        str << "Error description: " << result.description() << "\n";
        str << "Error offset: " << result.offset << " (error at [..." << (result.offset) << "]\n\n";
        JSONIO_THROW( "XML2Json", 15, str.str() );
    }

    // parse xml to bson data
    auto node = doc.child("node");
    bool isarray = false;
    if( node.attribute("isarray").as_bool() == true )
        isarray = true;


    if( !isarray )
    {
        JsonObjectBuilder jsBuilder(&object);
        int ii=0;
        for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it, ++ii )
        {
            if( it->type() != pugi::node_element )
                continue;
            parse( *it, jsBuilder  );
        }
    }
    else
    {
        JsonArrayBuilder jsBuilder(&object);
        int ii=0;
        for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it, ++ii )
        {
            if( it->type() != pugi::node_element )
                continue;
            it->set_name( std::to_string(ii).c_str() );
            parse( *it, jsBuilder  );
        }
    }
}

JsonFree loads( const std::string &xml_str )
{
    auto object = JsonFree::object();
    XML::loads( xml_str, object );
    return object;
}


JsonSchema loads( const std::string& schema_name,const std::string &xml_str )
{
    auto object = JsonSchema::object(schema_name);
    XML::loads( xml_str, object );
    return object;
}


//--------------------------------------------------------------

void object_emitter( pugi::xml_node& out, const JsonBase& object )
{
    pugi::xml_node ch1=out;
    int objtype = object.type();
    size_t objsize = object.size();

    for( size_t ii=0; ii<objsize; ii++ )
    {
        auto& childobj = object.child( ii);
        // before print
        switch( objtype )
        {
        case JsonBase::Object:
            ch1 = out.append_child(childobj.getKey().c_str());
            break;
        case JsonBase::Array:
            ch1 = out.append_child("element");
            break;
        default:
            break;
        }

        switch(childobj.type())
        {
        // impotant datatypes
        case JsonBase::Null:
            ch1.append_child(pugi::node_null);
            break;
        case JsonBase::Bool:
        case JsonBase::Int:
        case JsonBase::Double:
            ch1.text().set( childobj.toString().c_str() );
            break;
        case JsonBase::String: // use "" ?
            ch1.text().set( childobj.toString().c_str() );
            break;
            // main constructions
        case JsonBase::Object:
            object_emitter( ch1, childobj );
            break;
        case JsonBase::Array:
            ch1.append_attribute("isarray") = true;
            object_emitter( ch1, childobj );
            break;
        default:
            ch1.text().set( "can't print type : " );
        }
    }
}

// add ',' separated array
void add_array( const std::string& key, const std::string& value,  JsonBuilderBase& builder )
{
    std::string val = "",str = value;
    size_t pos = str.find( ',' );
    int ii=0;

    auto childobj = builder.addArray( key );
    while( pos != std::string::npos )
    {
        val = str.substr(0, pos);
        str = str.substr( pos+1 );
        pos = str.find( ',' );
        //trim(val, " \n\t\r\'\"");
        childobj.testScalar( std::to_string(ii++), val );
    }
    //trim(str, " \n\t\r\'\"");
    childobj.testScalar( std::to_string(ii), str );
}


void parse( const pugi::xml_node& node,  JsonBuilderBase& builder  )
{
    if( !node.first_child() )
    {
        builder.testScalar( node.name(), "null" );
        return;
    }

    std::vector<std::string> keys;
    bool isobject = true;  // object list (all node_pcdata will be ignored )
    bool isarray = false;  // isobject ==  true and all names is same or attribut isarray == true
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        if( it->type() == pugi::node_element )
            keys.push_back( it->name() );
    }

    if( keys.empty() )
        isobject = false;
    else
        if( node.attribute("isarray").as_bool() == true )
        {
            isarray = true;
            isobject = false;
        }
        else  if( keys.size()>1 )
        {   auto pred = [&](const std::string& str ){ return str==keys[0]; };
            if ( std::all_of(keys.begin()+1, keys.end(),  pred ) )
            {
                isarray = true;
                isobject = false;
            }
        }

    if( !isarray && !isobject ) // only internal text
    {
        std::string value = node.text().as_string();
        //if( !value.empty() )
        { if( node.attribute("isarray").as_bool() == true )
                add_array( node.name(), value, builder );
            else
            {
                //trim(value, " \n\t\r\'\"");
                builder.testScalar( node.name(), value );
            }
        }
    }
    else if( isobject )
    {
        auto childobj =  builder.addObject(  node.name() );
        for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
        {
            if( it->type() != pugi::node_element )
                continue;
            parse( *it, childobj  );
        }
    }
    else if( isarray )
    {
        auto childobj =  builder.addArray( node.name() );
        int ii=0;
        for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it, ++ii)
        {
            if( it->type() != pugi::node_element )
                continue;
            it->set_name( std::to_string(ii).c_str() );
            parse( *it, childobj  );
        }
    }
}

} // XML namespace

} // namespace jsonio17

