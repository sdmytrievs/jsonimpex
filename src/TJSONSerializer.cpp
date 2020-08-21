

#include "TJSONSerializer.h"
using namespace std;

namespace jsonio17 {

ThriftFieldDef TJSONSerializer::topfield = { -1, "", {T_STRUCT}, 0, "", "", "", "", DOUBLE_EMPTY, DOUBLE_EMPTY };

TJSONSerializer::TJSONSerializer( const ThriftSchema* aschema):
    TVirtualProtocol<TJSONSerializer>(nullptr),
    schema(aschema), bobj_tansport(nullptr)
{}

TJSONSerializer::~TJSONSerializer()
{}


uint32_t TJSONSerializer::writeMessageBegin(const std::string& name,
                                          const TMessageType messageType,
                                          const int32_t seqid)
{
  uint32_t result = 0;
  tranWriteStart();

  contexts_.push( TJSONWriteContext2( name.c_str(), T_LIST, /*0,*/ 4 ));
  auto arr = getDom()->appendArray( name );
  _objectStack.push(arr);
  getDom()->appendInt( "0", 1 /*kThriftVersion1*/);
  getDom()->appendString(  "1", name );
  getDom()->appendInt( "2", messageType );
  getDom()->appendInt( "3", seqid );
  return result;
}

uint32_t TJSONSerializer::writeMessageEnd()
{
 _objectStack.pop();
  contexts_.pop();
  return transWriteEnd();
}

uint32_t TJSONSerializer::writeStructBegin(const char* name)
{
  tranWriteStart();
  strSchema.push( schema->getStruct(name));
  if( contexts_.size() > 0 )
  {
      auto obj = getDom()->appendObject( contexts_.top().getKey() );
      _objectStack.push(obj);
  }
  return 0;
}

uint32_t TJSONSerializer::writeStructEnd()
{
  strSchema.pop();
  if( contexts_.size() > 0 )
    _objectStack.pop();
  return transWriteEnd();
}

uint32_t TJSONSerializer::writeFieldBegin(const char* name,
                                        const TType fieldType,
                                        const int16_t /*fieldId*/)
{
  contexts_.push( TJSONWriteContext2( name, fieldType, /*fieldId,*/ 0 ));
  return 0;
}

uint32_t TJSONSerializer::writeFieldEnd()
{
  contexts_.pop();
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
   auto obj = getDom()->appendObject( contexts_.top().getName() );
   _objectStack.push(obj);
  return 0;
}

uint32_t TJSONSerializer::writeMapEnd()
{
  _objectStack.pop();
  return 0;
}

uint32_t TJSONSerializer::writeListBegin(const TType /*elemType*/, const uint32_t /*size*/)
{
  auto arr = getDom()->appendArray( contexts_.top().getName() );
  _objectStack.push(arr);
  return 0;
}

uint32_t TJSONSerializer::writeListEnd()
{
  _objectStack.pop();
  return 0;
}

uint32_t TJSONSerializer::writeSetBegin(const TType /*elemType*/, const uint32_t /*size*/)
{
   auto arr = getDom()->appendArray( contexts_.top().getName() );
   _objectStack.push(arr);
   return 0;
}

uint32_t TJSONSerializer::writeSetEnd()
{
    _objectStack.pop();
    return 0;
}

uint32_t TJSONSerializer::writeBool(const bool value)
{
  string key = contexts_.top().getKey();
  if( key == ismapkey2)
    contexts_.top().setMapKey( to_string(value) );
  else
    getDom()->appendBool(  key, value );
  return 0;
}

uint32_t TJSONSerializer::writeByte(const int8_t byte)
{
  // writeByte() must be handled specially because boost::lexical cast sees
  // int8_t as a text type instead of an integer type
    string key = contexts_.top().getKey();
    if( key == ismapkey2)
      contexts_.top().setMapKey( to_string(byte) );
    else
      getDom()->appendInt(  key, static_cast<int16_t>(byte) );
  return 0;
}

uint32_t TJSONSerializer::writeI16(const int16_t i16)
{
    string key = contexts_.top().getKey();
    if( key == ismapkey2)
      contexts_.top().setMapKey( to_string(i16) );
    else
      getDom()->appendInt( key, i16 );
  return 0;
}

uint32_t TJSONSerializer::writeI32(const int32_t i32)
{
    string key = contexts_.top().getKey();
    if( key == ismapkey2)
      contexts_.top().setMapKey( to_string(i32) );
    else
      getDom()->appendInt( key, i32 );
  return 0;
}

uint32_t TJSONSerializer::writeI64(const int64_t i64)
{
    string key = contexts_.top().getKey();
    if( key == ismapkey2)
      contexts_.top().setMapKey( to_string(i64) );
    else
      getDom()->appendDouble( key, i64 );
  return 0;
}

uint32_t TJSONSerializer::writeDouble(const double dub)
{
    string key = contexts_.top().getKey();
    if( key == ismapkey2)
      contexts_.top().setMapKey( to_string(dub) );
    else
      getDom()->appendDouble( key, dub );
  return 0;
}

uint32_t TJSONSerializer::writeString(const std::string& str)
{
    string key = contexts_.top().getKey();
    if( key == ismapkey2)
      contexts_.top().setMapKey( str );
    else
      getDom()->appendString( key, str );
  return 0;
}

uint32_t TJSONSerializer::writeBinary(const std::string& str)
{
    getDom()->appendString( contexts_.top().getKey(), str );
    return 0;
}

/**
 * Reading functions
 */

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
  //transReadEnd();
  return 0;
}

uint32_t TJSONSerializer::readStructBegin(std::string& /*name*/)
{
    uint32_t returns = transReadStart();
    return returns;
}

uint32_t TJSONSerializer::readStructEnd()
{
    transReadEnd();
    return 0;
}

uint32_t TJSONSerializer::readFieldBegin(std::string& name, TType& fieldType, int16_t& fieldId)
{
   auto it = bitr.top().next();
   if( it != nullptr )
   {
       name = it->getKey();
       // Get field definition by name
       ThriftFieldDef fldinf = strSchema.top()->getSchema( name );
       fieldId = static_cast<int16_t>(fldinf.fId);
       if( fieldId >=  0 )
         fieldType = static_cast<TType>(fldinf.fTypeId[0]);

       readcntxt_.push( TJSONReadContext2( fldinf, it ) );

   } else // stop process
      {
        fieldType = ::apache::thrift::protocol::T_STOP;
      }
  return 0;
}

uint32_t TJSONSerializer::readFieldEnd()
{
  readcntxt_.pop();
  return 0;
}

uint32_t TJSONSerializer::readMapBegin(TType& keyType, TType& valType, uint32_t& size)
{
   readcntxt_.top().getMap( keyType, valType, size );
   return 0;
}

uint32_t TJSONSerializer::readMapEnd()
{
  readcntxt_.top().endMap();
  return 0;
}

uint32_t TJSONSerializer::readListBegin(TType& elemType, uint32_t& size)
{
   readcntxt_.top().getList( elemType, size );
   return 0;
}

uint32_t TJSONSerializer::readListEnd()
{
  readcntxt_.top().endList();
  return 0;
}

uint32_t TJSONSerializer::readSetBegin(TType& elemType, uint32_t& size)
{
  readcntxt_.top().getList( elemType, size );
  return 0;
}

uint32_t TJSONSerializer::readSetEnd()
{
  readcntxt_.top().endList();
  return 0;
}

uint32_t TJSONSerializer::readBool(bool& value)
{
    std::string mapkey;
    auto it = readcntxt_.top().getNext( mapkey );
    if( mapkey.empty() )
     value = it->toBool();
    else
     value = std::stoi(mapkey);
    return 0;
}

int TJSONSerializer::readInt()
{
    int ii=0;
    std::string mapkey;
    auto it = readcntxt_.top().getNext( mapkey );
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
  byte = static_cast<int8_t>(readInt());
  return 0;
}

uint32_t TJSONSerializer::readI16(int16_t& i16)
{
  i16 = static_cast<int16_t>(readInt());
  return 0;
}

uint32_t TJSONSerializer::readI32(int32_t& i32)
{
  i32 = readInt();
  return 0;
}

uint32_t TJSONSerializer::readI64(int64_t& i64)
{
    std::string mapkey;
     auto it = readcntxt_.top().getNext( mapkey );
    if( mapkey.empty() )
     i64 = static_cast<int64_t>(it->toDouble());
    else
     i64 = std::stol(mapkey);
  return 0;
}

uint32_t TJSONSerializer::readDouble(double& dub)
{
    std::string mapkey;
     auto it = readcntxt_.top().getNext( mapkey );
    if( mapkey.empty() )
     dub = it->toDouble();
    else
     dub = std::stod(mapkey);
  return 0;
}

uint32_t TJSONSerializer::readString(std::string& str)
{
   std::string mapkey;
    auto it = readcntxt_.top().getNext( mapkey );
   if( mapkey.empty() )
      str = it->toString();
   else
    str = mapkey;
   return 0;
}

uint32_t TJSONSerializer::readBinary(std::string& str)
{
    std::string mapkey;
    auto it = readcntxt_.top().getNext( mapkey );
    str = it->toString();
    return 0;
}

} // namespace jsonio17
