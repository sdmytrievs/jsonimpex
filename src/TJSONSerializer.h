#pragma once

#include <stack>
#include <thrift/protocol/TVirtualProtocol.h>
//#include "jsonio/thrift_schema.h"
#include "jsonio17/jsonschema.h"


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

namespace jsonio17 {

const std::string ismapkey2("-");

/**
 * Class to serve output as base BSON context
 */
class TJSONWriteContext2 {

    std::string name;
    TType fieldType;
    //int16_t fieldId;
    int16_t list_id;
    std::string mapkey;

public:

  TJSONWriteContext2( const char* aname, const TType afieldType,
                /*const int16_t afieldId,*/ const int16_t id=0 ):
      name(aname), fieldType(afieldType), /*fieldId(afieldId),*/
      list_id(id), mapkey("")
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
         { key = std::to_string(list_id++);
           break;
         }
     case T_MAP:
          if( !list_id )
          {  list_id++;
             key = ismapkey2;
           } else
            { list_id=0;
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
  JsonBase* object;
  int ndx = -1;

public:

  StructItr(JsonBase* aobject):
   object(aobject), ndx(-1)
  {}

  JsonBase* next()
  {
    ndx++;
    return object->getChild( static_cast<std::size_t>(ndx));
  }

  JsonBase* current()
  {
    return object->getChild(static_cast<std::size_t>(ndx));
  }

  JsonBase* data()
  {
    return object;
  }

};


/**
 * Class to serve input as base BSON context
 */
class TJSONReadContext2 {

    //std::string name;
    ThriftFieldDef fldinf;
    uint level;              // level in list<list<....
    std::stack<StructItr> itval;
    //bson_iterator itval;
    bool isArray;
    bool isMap;
    int mapndx;

public:

  TJSONReadContext2( const ThriftFieldDef& afldinf, JsonBase* aitval ):
      fldinf(afldinf), level(0), isArray(false), isMap(false),  mapndx(0)
  {
      itval.push(aitval);
  }

  const std::string& getName() const
  {
    return fldinf.fName;
  }

  const std::string& structName()
  {
      if ( !( fldinf.fTypeId.size()>(level)  &&
              (fldinf.fTypeId[level] ==  T_STRUCT ) ))
      {
       throw TProtocolException(TProtocolException::INVALID_DATA,
        "Expected struct; field \"" + fldinf.fName + "\"");
      }
      return fldinf.className;

  }

  JsonBase* getNext( std::string& mapkey )
  {
    mapkey = "";
    JsonBase* it = itval.top().data();
    if( isMap )
    {
        if( !mapndx )
        {
           it = itval.top().next();
           if( it != nullptr )
              mapkey = it->getKey();
           mapndx = 1;
        } else
           {
             it = itval.top().current();
             mapndx = 0;
            }

    }
    else if( isArray )
       it = itval.top().next();

   return it;
  }



  void getList( TType& elemType, uint32_t& size )
  {
      if ( !( fldinf.fTypeId.size()>(level+1)  &&
              (fldinf.fTypeId[level] ==  T_LIST || fldinf.fTypeId[level] == T_SET) ))
      {
       throw TProtocolException(TProtocolException::INVALID_DATA,
        "Expected list; field \"" + fldinf.fName + "\"");
      }
      std::string mapkey;
      auto it = getNext(mapkey);
      // go to next level
      level++;
      elemType = static_cast<TType>(fldinf.fTypeId[level]);

      if( it->getType() == JSON_NULL )
      {
        size = 0;
      }
      else
      {
        size = static_cast<uint32_t>(it->getChildrenCount());
        isArray = true;
        itval.push(it);
      }
  }

  void endList()
  {
     itval.pop();
     level--;
  }

  void getMap( TType& keyType, TType& valType, uint32_t& size )
  {
      if ( !( fldinf.fTypeId.size()>(level+2)  &&
              (fldinf.fTypeId[level] ==  T_MAP ) ))
      {
       throw TProtocolException(TProtocolException::INVALID_DATA,
        "Expected map; field \"" + fldinf.fName + "\"");
      }

      // go to next level
      level++;
      keyType = static_cast<TType>(fldinf.fTypeId[level]);
      level++;
      valType = static_cast<TType>(fldinf.fTypeId[level]);

      // calculate size
      std::string mapkey;
      auto it = getNext(mapkey);
      if( it->getType() != JSON_OBJECT )
      {
        size = 0;
      }
      else
      {
        size = static_cast<uint32_t>(it->getChildrenCount());
        isMap = true;
        itval.push(it);
      }
  }

  void endMap()
  {
     itval.pop();
     level--;
     level--;
  }


};


/**
 * bson protocol for Thrift.
 *
 * Implements a protocol which uses bson as the wire-format.
 *
 * Thrift types are represented as described below:
 *
 * 1. Every Thrift integer type is represented as a bson_iterator_int, bson_iterator_bool.
 *
 * 2. Thrift doubles are represented as bson_iterator_double
 *
 * 3. Thrift std::string values are emitted as bson_iterator_std::string.
 *
 * 4. Thrift binary values are represented as bson_iterator_bin_data.
 *
 * 5. Thrift structs are represented as BSON object
 *
 * 6. Thrift lists and sets are represented as BSON arrays
 *
 * 7. Thrift maps are represented as BSON object, by the count of the Thrift pairs, followed by a
 *    BSON object containing the key-value pairs. Note that BSON keys can only
 *    be strings, which means that the key type of the Thrift map should be
 *    restricted to numeric or std::string types -- in the case of numerics, they
 *    are serialized as strings.
 *
 * 8. Thrift messages are represented as BSON arrays, with the protocol
 *    version #, the message name, the message type, and the sequence ID as
 *    the first 4 elements.
 *
 */
class TJSONSerializer : public TVirtualProtocol<TJSONSerializer>
{
public:

  TJSONSerializer( const SchemasData& aschema);
  ~TJSONSerializer();

private:

  void tranWriteStart()
  {
      if( contexts_.size() == 0 )
      {
        bobj_tansport->clearField();
      }
  }


  uint32_t transWriteEnd()
  {
    uint32_t returns = 0;
    //if( contexts_.size() == 0 )
    //{ }
    return returns;
  }

  uint32_t transReadStart()
  {
      uint32_t returns = 0;
      JsonBase* data = bobj_tansport;
      if( bitr.size() > 0 )
      {
          std::string mapkey;
          data = readcntxt_.top().getNext(mapkey);
          StructDef* strc_ = schema->getStruct(readcntxt_.top().structName());
          if ( strc_ == nullptr )
          {
                  throw TProtocolException(TProtocolException::INVALID_DATA,
                   "Expected struct definition; field \"" + readcntxt_.top().structName() + "\"");
          }
          strSchema.push( strc_);
      }
      bitr.push(data);
      bitrndx = 0;
      return returns;
  }


  void transReadEnd()
  {
    bitr.pop();
    bitrndx =0;
    if( bitr.size() > 0 )
      strSchema.pop();
  }

  int readInt();

public:

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


  void setDom( JsonBase& obj, const std::string& strname )
    {

       StructDef* strc_ = schema->getStruct(strname);
       if ( strc_ == nullptr )
       {
              throw TProtocolException(TProtocolException::INVALID_DATA,
               "Expected struct definition; field \"" + strname + "\"");
       }
       strSchema.push( strc_ );
       bobj_tansport = obj;
       _objectStack.push(bobj_tansport);

   }

  private:

  static ThriftFieldDef topfield;

  const SchemasData* schema;
  JsonBase* bobj_tansport;
  std::stack<JsonBase*> _objectStack;
  //bson* bobj_tansport;

  // write context
  std::stack<TJSONWriteContext2> contexts_;

  // read context
  std::stack<StructDef*> strSchema;
  std::stack<StructItr> bitr;
  int bitrndx =0;
  std::stack<TJSONReadContext2> readcntxt_;

  JsonBase* getDom()
      {
        return _objectStack.top();
      }
};

} // namespace jsonio17

