#include "TJSONSerializer.h"
using namespace std;

namespace jsonio17 {

TJSONSerializer::TJSONSerializer( const SchemasData& aschema ):
    TVirtualProtocol<TJSONSerializer>( nullptr ),
    schema(aschema), top_read_object(nullptr)
{}

TJSONSerializer::~TJSONSerializer()
{}


uint32_t TJSONSerializer::writeMessageBegin(const std::string& name,
                                            const TMessageType messageType,
                                            const int32_t seqid)
{
    uint32_t result = 0;
    write_context.push( TJSONWriteContext( name, T_LIST, 4 ) );
    auto arr = getDom()->addArray( name );
    arr.addInt(  1 /*kThriftVersion1*/);
    arr.addString(  name );
    arr.addInt( messageType );
    arr.addInt(  seqid );
    builders_stack.push( std::make_shared<JsonBuilderBase>(&arr) );
    return result;
}

uint32_t TJSONSerializer::writeMessageEnd()
{
    builders_stack.pop();
    write_context.pop();
    return 0;
}

uint32_t TJSONSerializer::writeStructBegin(const char* name)
{
    struct_schemas.push( schema.getStruct(name) );
    if( write_context.size() > 0 )
    {
        auto obj = getDom()->addObject( write_context.top().getKey() );
        builders_stack.push( std:shared_ptr<JsonBuilderBase>(obj) );
    }
    return 0;
}

uint32_t TJSONSerializer::writeStructEnd()
{
    struct_schemas.pop();
    if( write_context.size() > 0 )
        builders_stack.pop();
    return 0;
}

uint32_t TJSONSerializer::writeFieldBegin( const char* name,
                                           const TType fieldType,
                                           const int16_t /*fieldId*/)
{
    write_context.push( TJSONWriteContext( name, fieldType, 0 ));
    return 0;
}

uint32_t TJSONSerializer::writeFieldEnd()
{
    write_context.pop();
    return 0;
}

uint32_t TJSONSerializer::writeFieldStop()
{
    return 0;
}

uint32_t TJSONSerializer::writeMapBegin(const TType /*keyType*/,
                                        const TType /*valType*/,
                                        const uint32_t /*size*/)
{
    auto obj = getDom()->addObject( write_context.top().getName() );
    builders_stack.push(std::make_shared<JsonBuilderBase>(obj) );
    return 0;
}

uint32_t TJSONSerializer::writeMapEnd()
{
    builders_stack.pop();
    return 0;
}

uint32_t TJSONSerializer::writeListBegin(const TType /*elemType*/, const uint32_t /*size*/)
{
    auto arr = getDom()->addArray( write_context.top().getName() );
    builders_stack.push( std::make_shared<JsonBuilderBase>(arr) );
    return 0;
}

uint32_t TJSONSerializer::writeListEnd()
{
    builders_stack.pop();
    return 0;
}

uint32_t TJSONSerializer::writeSetBegin(const TType /*elemType*/, const uint32_t /*size*/)
{
    auto arr = getDom()->addArray( write_context.top().getName() );
    builders_stack.push( std::make_shared<JsonBuilderBase>(arr) );
    return 0;
}

uint32_t TJSONSerializer::writeSetEnd()
{
    builders_stack.pop();
    return 0;
}

uint32_t TJSONSerializer::writeBool(const bool value)
{
    string key = write_context.top().getKey();
    if( key == ismapkey2 )
        write_context.top().setMapKey( to_string(value) );
    else
        getDom()->testScalar(  key, to_string(value) ); // appendBool
    return 0;
}

uint32_t TJSONSerializer::writeByte(const int8_t byte)
{
    // writeByte() must be handled specially because boost::lexical cast sees
    // int8_t as a text type instead of an integer type
    string key = write_context.top().getKey();
    if( key == ismapkey2 )
        write_context.top().setMapKey( to_string(byte) );
    else
        getDom()->testScalar(  key, to_string(static_cast<int16_t>(byte)) ); //appendInt
    return 0;
}

uint32_t TJSONSerializer::writeI16(const int16_t i16)
{
    string key = write_context.top().getKey();
    if( key == ismapkey2 )
        write_context.top().setMapKey( to_string(i16) );
    else
        getDom()->testScalar( key, to_string(i16) );  // appendInt
    return 0;
}

uint32_t TJSONSerializer::writeI32(const int32_t i32)
{
    string key = write_context.top().getKey();
    if( key == ismapkey2 )
        write_context.top().setMapKey( to_string(i32) );
    else
        getDom()->testScalar( key, to_string(i32) ); // appendInt
    return 0;
}

uint32_t TJSONSerializer::writeI64(const int64_t i64)
{
    string key = write_context.top().getKey();
    if( key == ismapkey2 )
        write_context.top().setMapKey( to_string(i64) );
    else
        getDom()->testScalar( key, to_string(i64) ); //appendDouble
    return 0;
}

uint32_t TJSONSerializer::writeDouble(const double dub)
{
    string key = write_context.top().getKey();
    if( key == ismapkey2 )
        write_context.top().setMapKey( to_string(dub) );
    else
        getDom()->testScalar( key, to_string(dub) ); //appendDouble
    return 0;
}

uint32_t TJSONSerializer::writeString(const std::string& str)
{
    string key = write_context.top().getKey();
    if( key == ismapkey2 )
        write_context.top().setMapKey( str );
    else
        getDom()->addString( key, str );
    return 0;
}

uint32_t TJSONSerializer::writeBinary(const std::string& str)
{
    getDom()->addString( write_context.top().getKey(), str );
    return 0;
}

// Reading functions -----------------------------------------------------------------

// not implemented only for structures
uint32_t TJSONSerializer::readMessageBegin(std::string& /*name*/,
                                           TMessageType& /*messageType*/,
                                           int32_t& /*seqid*/)
{
    uint32_t returns = 0; /*transReadStart();

    // get array data
    bson_iterator_next(&bitr.top());
    bson_iterator i = bitr.top();
    name = bson_iterator_key(&i);
    bson_type type = bson_iterator_type(&i);

    if ( type != BSON_ARRAY )
        throw TProtocolException(TProtocolException::INVALID_DATA,
              "Expected BSON array; field \"" + name + "\"");

   const char* arrdata = bson_iterator_value(&i);
   bson_iterator itval;
   bson_iterator_from_buffer(&itval, arrdata);
   readcntxt_.push( TJSONReadContext2( name.c_str(), itval, true,  false ) );

   // read first  4 fields
   int kThriftVersion1 = readInt();
   if ( 1 != kThriftVersion1) {
     throw TProtocolException(TProtocolException::BAD_VERSION, "Message contained bad version.");
   }
   readString( name );
   messageType = static_cast<TMessageType>(readInt());
   seqid = readInt();
*/
    return returns;
}

uint32_t TJSONSerializer::readMessageEnd()
{
    //trans_read_end();
    return 0;
}

uint32_t TJSONSerializer::readStructBegin(std::string& /*name*/)
{
    uint32_t returns = trans_read_start();
    return returns;
}

uint32_t TJSONSerializer::readStructEnd()
{
    trans_read_end();
    return 0;
}

uint32_t TJSONSerializer::readFieldBegin( std::string& name, TType& fieldType, int16_t& fieldId )
{
    if( read_struct_itr.top().hasNext() )
    {
        auto& it = read_struct_itr.top().next();
        name = it.getKey();
        // Get field definition by name
        auto fldinf = struct_schemas.top()->getField( name );
        if( fldinf == nullptr )
            JSONIO_THROW( "TJSONSerializer", 18, " expected field definition \"" + name + "\"");
        fieldId = static_cast<int16_t>( fldinf->id() );
        if( fieldId >=  0 )
            fieldType = static_cast<TType>( fldinf->type(0) );
        read_context.push( TJSONReadContext( fldinf, it ) );
    }
    else // stop process
    {
        fieldType = ::apache::thrift::protocol::T_STOP;
    }
    return 0;
}

uint32_t TJSONSerializer::readFieldEnd()
{
    read_context.pop();
    return 0;
}

uint32_t TJSONSerializer::readMapBegin(TType& keyType, TType& valType, uint32_t& size)
{
    read_context.top().getMap( keyType, valType, size );
    return 0;
}

uint32_t TJSONSerializer::readMapEnd()
{
    read_context.top().endMap();
    return 0;
}

uint32_t TJSONSerializer::readListBegin(TType& elemType, uint32_t& size)
{
    read_context.top().getList( elemType, size );
    return 0;
}

uint32_t TJSONSerializer::readListEnd()
{
    read_context.top().endList();
    return 0;
}

uint32_t TJSONSerializer::readSetBegin(TType& elemType, uint32_t& size)
{
    read_context.top().getList( elemType, size );
    return 0;
}

uint32_t TJSONSerializer::readSetEnd()
{
    read_context.top().endList();
    return 0;
}

uint32_t TJSONSerializer::readBool(bool& value)
{
    std::string mapkey;
    auto it = read_context.top().getNext( mapkey );
    if( mapkey.empty() )
        value = it->toBool();
    else
        value = std::stoi(mapkey);
    return 0;
}

int TJSONSerializer::read_int()
{
    int ii=0;
    std::string mapkey;
    auto it = read_context.top().getNext( mapkey );
    if( mapkey.empty() )
        ii =it->toInt();
    else
        ii = std::stoi(mapkey);
    return ii;
}


// readByte() must be handled properly because boost::lexical cast sees int8_t
// as a text type instead of an integer type
uint32_t TJSONSerializer::readByte(int8_t& byte)
{
    byte = static_cast<int8_t>(read_int());
    return 0;
}

uint32_t TJSONSerializer::readI16(int16_t& i16)
{
    i16 = static_cast<int16_t>(read_int());
    return 0;
}

uint32_t TJSONSerializer::readI32(int32_t& i32)
{
    i32 = read_int();
    return 0;
}

uint32_t TJSONSerializer::readI64(int64_t& i64)
{
    std::string mapkey;
    auto it = read_context.top().getNext( mapkey );
    if( mapkey.empty() )
        i64 = static_cast<int64_t>(it->toDouble());
    else
        i64 = std::stol(mapkey);
    return 0;
}

uint32_t TJSONSerializer::readDouble(double& dub)
{
    std::string mapkey;
    auto it = read_context.top().getNext( mapkey );
    if( mapkey.empty() )
        dub = it->toDouble();
    else
        dub = std::stod(mapkey);
    return 0;
}

uint32_t TJSONSerializer::readString(std::string& str)
{
    std::string mapkey;
    auto it = read_context.top().getNext( mapkey );
    if( mapkey.empty() )
        str = it->toString();
    else
        str = mapkey;
    return 0;
}

uint32_t TJSONSerializer::readBinary(std::string& str)
{
    std::string mapkey;
    auto it = read_context.top().getNext( mapkey );
    str = it->toString();
    return 0;
}

} // namespace jsonio17
