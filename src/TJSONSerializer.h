#pragma once

#include <stack>
#include <thrift/protocol/TVirtualProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include "jsonio17/jsonschema.h"
#include "jsonio17/jsonbuilder.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

namespace jsonio17 {

const std::string ismapkey2("-");

/// Class to serve output as base JSON context
class TJSONWriteContext
{

    std::string name;
    TType fieldType;
    int16_t list_id;
    std::string mapkey;

public:

    TJSONWriteContext( const std::string& aname, const TType afieldType, const int16_t id=0 ):
        name(aname), fieldType(afieldType),  list_id(id), mapkey("")
    { }

    const std::string& getName()
    {
        return name;
    }

    std::string getKey()
    {
        std::string key = name;
        switch( fieldType )
        {
        case T_SET:
        case T_LIST:
        {
            key = std::to_string(list_id++);
            break;
        }
        case T_MAP:
            if( !list_id )
            {
                list_id++;
                key = ismapkey2;
            }
            else
            {
                list_id=0;
                key = mapkey;
            }
            break;
        default:
            break;
        }
        return key;
    }

    void setMapKey(const std::string& amapkey )
    {
        mapkey = amapkey;
    }

};


class StructItr
{
    const JsonBase& object;
    int ndx = -1;

public:

    StructItr( const JsonBase& aobject ):
        object(aobject), ndx(-1)
    {}

    const JsonBase& next()
    {
        ndx++;
        return object.child( static_cast<std::size_t>(ndx) );
    }

    const JsonBase& current()
    {
        return object.child( static_cast<std::size_t>(ndx) );
    }

    const JsonBase& data()
    {
        return object;
    }

    bool hasNext()
    {
        return ndx < static_cast<int>(object.size())-1;
    }
};


/// Class to serve input as base JSON context
class TJSONReadContext {

    const FieldDef* fld_definition;
    size_t level;                    // level in list<list<....
    std::stack<StructItr> object_iterator;
    bool isArray;
    bool isMap;
    int map_ndx;

public:

    TJSONReadContext( const FieldDef* afldinf, const JsonBase& object ):
        fld_definition( afldinf ), level(0), isArray(false), isMap(false),  map_ndx(0)
    {
        object_iterator.push( object );
    }

    std::string getName() const
    {
        return fld_definition->name();
    }

    std::string structName()
    {
        if( fld_definition->type( level ) != FieldDef::T_STRUCT )
            JSONIO_THROW( "TJSONSerializer", 11, " expected struct; field \"" + fld_definition->name() + "\"");
        return fld_definition->className();
    }

    const JsonBase* getNext( std::string& mapkey )
    {
        mapkey = "";
        const JsonBase* it = &object_iterator.top().data();
        if( isMap )
        {
            if( !map_ndx )
            {
                it = &object_iterator.top().next();
                mapkey = it->getKey();
                map_ndx = 1;
            }
            else
            {
                it = &object_iterator.top().current();
                map_ndx = 0;
            }

        }
        else if( isArray )
            it = &object_iterator.top().next();

        return it;
    }


    void getList( TType& elemType, uint32_t& size )
    {
        if ( !( fld_definition->type(level) ==  FieldDef::T_LIST || fld_definition->type(level) == FieldDef::T_SET ) )
            JSONIO_THROW( "TJSONSerializer", 12, " expected list; field \"" + fld_definition->name() + "\"" );
        std::string mapkey;
        auto it = getNext( mapkey );
        // go to next level
        level++;
        elemType = static_cast<TType>( fld_definition->type(level) );

        if( it->type() == JsonBase::Null )
        {
            size = 0;
        }
        else
        {
            size = static_cast<uint32_t>(it->size());
            isArray = true;
            object_iterator.push(*it);
        }
    }

    void endList()
    {
        object_iterator.pop();
        level--;
    }

    void getMap( TType& keyType, TType& valType, uint32_t& size )
    {
        if( !( fld_definition->type(level) ==  FieldDef::T_MAP ) )
            JSONIO_THROW( "TJSONSerializer", 13, " expected map; field \"" + fld_definition->name() + "\"");

        // go to next level
        level++;
        keyType = static_cast<TType>( fld_definition->type(level) );
        level++;
        valType = static_cast<TType>( fld_definition->type(level) );

        // calculate size
        std::string mapkey;
        auto it = getNext(mapkey);
        if( it->type() != JsonBase::Object )
        {
            size = 0;
        }
        else
        {
            size = static_cast<uint32_t>( it->size() );
            isMap = true;
            object_iterator.push(*it);
        }
    }

    void endMap()
    {
        object_iterator.pop();
        level--;
        level--;
    }

};


/**
 * json protocol for Thrift.
 *
 * Implements a protocol which uses JSON object as the wire/read-format.
 *
 * Thrift types are represented as described below:
 *
 * 1. Every Thrift integer type is represented as a int, bool.
 *
 * 2. Thrift doubles are represented as double
 *
 * 3. Thrift std::string values are emitted as string.
 *
 * 4. Thrift binary values are represented as string.
 *
 * 5. Thrift structs are represented as JSON object
 *
 * 6. Thrift lists and sets are represented as JSON arrays
 *
 * 7. Thrift maps are represented as JSON object, by the count of the Thrift pairs, followed by a
 *    JSON object containing the key-value pairs. Note that JSON keys can only
 *    be strings, which means that the key type of the Thrift map should be
 *    restricted to numeric or std::string types -- in the case of numerics, they
 *    are serialized as strings.
 *
 * 8. Thrift messages are represented as JSON arrays, with the protocol
 *    version #, the message name, the message type, and the sequence ID as
 *    the first 4 elements.
 *
 */
class TJSONSerializer : public TVirtualProtocol<TJSONSerializer>
{
public:

    TJSONSerializer( std::shared_ptr<TTransport> ptrans, const SchemasData* aschema );
    ~TJSONSerializer();


    //  Writing functions.

    uint32_t writeMessageBegin(const std::string& name,
                               const TMessageType messageType,
                               const int32_t seqid);

    uint32_t writeMessageEnd();

    uint32_t writeStructBegin(const char* name);

    uint32_t writeStructEnd();

    uint32_t writeFieldBegin(const char* name, const TType fieldType, const int16_t fieldId);

    uint32_t writeFieldEnd();

    uint32_t writeFieldStop();

    uint32_t writeMapBegin(const TType keyType, const TType valType, const uint32_t size);

    uint32_t writeMapEnd();

    uint32_t writeListBegin(const TType elemType, const uint32_t size);

    uint32_t writeListEnd();

    uint32_t writeSetBegin(const TType elemType, const uint32_t size);

    uint32_t writeSetEnd();

    uint32_t writeBool(const bool value);

    uint32_t writeByte(const int8_t byte);

    uint32_t writeI16(const int16_t i16);

    uint32_t writeI32(const int32_t i32);

    uint32_t writeI64(const int64_t i64);

    uint32_t writeDouble(const double dub);

    uint32_t writeString(const std::string& str);

    uint32_t writeBinary(const std::string& str);

    //  Reading functions

    uint32_t readMessageBegin(std::string& name, TMessageType& messageType, int32_t& seqid);

    uint32_t readMessageEnd();

    uint32_t readStructBegin(std::string& name);

    uint32_t readStructEnd();

    uint32_t readFieldBegin(std::string& name, TType& fieldType, int16_t& fieldId);

    uint32_t readFieldEnd();

    uint32_t readMapBegin(TType& keyType, TType& valType, uint32_t& size);

    uint32_t readMapEnd();

    uint32_t readListBegin(TType& elemType, uint32_t& size);

    uint32_t readListEnd();

    uint32_t readSetBegin(TType& elemType, uint32_t& size);

    uint32_t readSetEnd();

    uint32_t readBool(bool& value);

    // Provide the default readBool() implementation for std::vector<bool>
    using TVirtualProtocol<TJSONSerializer>::readBool;

    uint32_t readByte(int8_t& byte);

    uint32_t readI16(int16_t& i16);

    uint32_t readI32(int32_t& i32);

    uint32_t readI64(int64_t& i64);

    uint32_t readDouble(double& dub);

    uint32_t readString(std::string& str);

    uint32_t readBinary(std::string& str);


    /// Set up read mode
    void setDom( const JsonBase& object, const std::string& schema_name )
    {

        auto top_struct = schema->getStruct(schema_name);
        if( top_struct == nullptr )
            JSONIO_THROW( "TJSONSerializer", 14, " expected struct definition \"" + schema_name + "\"");
        struct_schemas.push( top_struct );
        top_read_object = &object;
    }

    /// Set up write mode
    void setDom( JsonBase& object, const std::string& schema_name )
    {
        auto top_struct = schema->getStruct(schema_name);
        if ( top_struct == nullptr )
            JSONIO_THROW( "TJSONSerializer", 15, " expected struct definition \"" + schema_name + "\"");
        struct_schemas.push( top_struct );
        builders_stack.push( std::make_shared<JsonObjectBuilder>( &object ) );
    }


private:

    const SchemasData* schema;
    std::stack<const StructDef*> struct_schemas;

    // write context
    std::stack< std::shared_ptr<JsonBuilderBase> > builders_stack;
    std::stack<TJSONWriteContext> write_context;

    JsonBuilderBase* getDom()
    {
        return builders_stack.top().get();
    }

    // read context
    const JsonBase* top_read_object;
    std::stack<StructItr> read_struct_itr;
    std::stack<TJSONReadContext> read_context;


    uint32_t trans_read_start()
    {
        uint32_t returns = 0;
        const JsonBase* data = top_read_object;
        if( read_struct_itr.size() > 0 )
        {
            std::string mapkey;
            data = read_context.top().getNext( mapkey );

            auto astruct = schema->getStruct( read_context.top().structName() );
            if( astruct == nullptr )
                JSONIO_THROW( "TJSONSerializer", 17, " expected struct definition \"" + read_context.top().structName() + "\"");
            struct_schemas.push( astruct );
        }
        read_struct_itr.push( *data );
        return returns;
    }

    void trans_read_end()
    {
        read_struct_itr.pop();
        if( read_struct_itr.size() > 0 )
            struct_schemas.pop();
    }

    int read_int();

};

} // namespace jsonio17


