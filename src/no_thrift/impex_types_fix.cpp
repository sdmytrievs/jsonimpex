#include <algorithm>
#include <ostream>
#include "impex_types_fix.h"
#include "../TJSONSerializer.h"

namespace impex {

int _kDirectionTypeValues[] = {
  /**
   * import
   */
  DirectionType::IMPORT,
  /**
   * export
   */
  DirectionType::EXPORT,
    /**
     * update
     */
  DirectionType::UPDATE
};
const char* _kDirectionTypeNames[] = {
  /**
   * import
   */
  "IMPORT",
  /**
   * export
   */
  "EXPORT",
    /**
     * update
     */
    "UPDATE"
};
const std::map<int, const char*> _DirectionType_VALUES_TO_NAMES = {
    {T_STOP, "stop" },
    {T_VOID, "void" },
    {T_BOOL, "bool" },
    {T_BYTE, "byte" },
    {T_I08, "i8" },
    {T_I16, "i16" },
    {T_I32, "i32" },
    {T_U64, "u64" },
    {T_I64, "i64" },
    {T_DOUBLE, "double" },
    {T_STRING, "string" },
    {T_UTF7, "utf7" },
    {T_STRUCT, "struct" },
    {T_MAP, "map" },
    {T_SET, "set" },
    {T_LIST, "list" },
    {T_UTF8, "utf8" },
    {T_UTF16, "utf16" }
};

std::ostream& operator<<(std::ostream& out, const DirectionType::type& val) {
  std::map<int, const char*>::const_iterator it = _DirectionType_VALUES_TO_NAMES.find(val);
  if (it != _DirectionType_VALUES_TO_NAMES.end()) {
    out << it->second;
  } else {
    out << static_cast<int>(val);
  }
  return out;
}

std::string to_string(const DirectionType::type& val) {
  std::map<int, const char*>::const_iterator it = _DirectionType_VALUES_TO_NAMES.find(val);
  if (it != _DirectionType_VALUES_TO_NAMES.end()) {
    return std::string(it->second);
  } else {
    return std::to_string(static_cast<int>(val));
  }
}


FormatValue::~FormatValue() noexcept {
}


void FormatValue::__set_format(const std::string& val) {
  this->format = val;
}

void FormatValue::__set_factor(const double val) {
  this->factor = val;
__isset.factor = true;
}

void FormatValue::__set_increment(const double val) {
  this->increment = val;
__isset.increment = true;
}
std::ostream& operator<<(std::ostream& out, const FormatValue& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t FormatValue::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;
  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->format);
          this->__isset.format = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_DOUBLE) {
          xfer += iprot->readDouble(this->factor);
          this->__isset.factor = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_DOUBLE) {
          xfer += iprot->readDouble(this->increment);
          this->__isset.increment = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t FormatValue::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FormatValue");

  xfer += oprot->writeFieldBegin("format", ::jsonio17::impex::T_STRING, 1);
  xfer += oprot->writeString(this->format);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.factor) {
    xfer += oprot->writeFieldBegin("factor", ::jsonio17::impex::T_DOUBLE, 2);
    xfer += oprot->writeDouble(this->factor);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.increment) {
    xfer += oprot->writeFieldBegin("increment", ::jsonio17::impex::T_DOUBLE, 3);
    xfer += oprot->writeDouble(this->increment);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FormatValue &a, FormatValue &b) {
  using ::std::swap;
  swap(a.format, b.format);
  swap(a.factor, b.factor);
  swap(a.increment, b.increment);
  swap(a.__isset, b.__isset);
}

FormatValue::FormatValue(const FormatValue& other0) {
  format = other0.format;
  factor = other0.factor;
  increment = other0.increment;
  __isset = other0.__isset;
}
FormatValue& FormatValue::operator=(const FormatValue& other1) {
  format = other1.format;
  factor = other1.factor;
  increment = other1.increment;
  __isset = other1.__isset;
  return *this;
}
void FormatValue::printTo(std::ostream& out) const {
  out << "FormatValue(";
  out << "format=" << to_string(format);
  out << ", " << "factor="; (__isset.factor ? (out << to_string(factor)) : (out << "<null>"));
  out << ", " << "increment="; (__isset.increment ? (out << to_string(increment)) : (out << "<null>"));
  out << ")";
}


FormatKeyword::~FormatKeyword() noexcept {
}


void FormatKeyword::__set_format(const std::string& val) {
  this->format = val;
}

void FormatKeyword::__set_delim_begin(const std::string& val) {
  this->delim_begin = val;
__isset.delim_begin = true;
}

void FormatKeyword::__set_delim_end(const std::string& val) {
  this->delim_end = val;
__isset.delim_end = true;
}
std::ostream& operator<<(std::ostream& out, const FormatKeyword& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t FormatKeyword::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->format);
          this->__isset.format = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->delim_begin);
          this->__isset.delim_begin = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->delim_end);
          this->__isset.delim_end = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t FormatKeyword::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FormatKeyword");

  xfer += oprot->writeFieldBegin("format", ::jsonio17::impex::T_STRING, 1);
  xfer += oprot->writeString(this->format);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.delim_begin) {
    xfer += oprot->writeFieldBegin("delim_begin", ::jsonio17::impex::T_STRING, 2);
    xfer += oprot->writeString(this->delim_begin);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.delim_end) {
    xfer += oprot->writeFieldBegin("delim_end", ::jsonio17::impex::T_STRING, 3);
    xfer += oprot->writeString(this->delim_end);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FormatKeyword &a, FormatKeyword &b) {
  using ::std::swap;
  swap(a.format, b.format);
  swap(a.delim_begin, b.delim_begin);
  swap(a.delim_end, b.delim_end);
  swap(a.__isset, b.__isset);
}

FormatKeyword::FormatKeyword(const FormatKeyword& other2) {
  format = other2.format;
  delim_begin = other2.delim_begin;
  delim_end = other2.delim_end;
  __isset = other2.__isset;
}
FormatKeyword& FormatKeyword::operator=(const FormatKeyword& other3) {
  format = other3.format;
  delim_begin = other3.delim_begin;
  delim_end = other3.delim_end;
  __isset = other3.__isset;
  return *this;
}
void FormatKeyword::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "FormatKeyword(";
  out << "format=" << to_string(format);
  out << ", " << "delim_begin="; (__isset.delim_begin ? (out << to_string(delim_begin)) : (out << "<null>"));
  out << ", " << "delim_end="; (__isset.delim_end ? (out << to_string(delim_end)) : (out << "<null>"));
  out << ")";
}


DataType::~DataType() noexcept {
}


void DataType::__set_datatype(const std::string& val) {
  this->datatype = val;
}

void DataType::__set_organization(const std::string& val) {
  this->organization = val;
}
std::ostream& operator<<(std::ostream& out, const DataType& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t DataType::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->datatype);
          this->__isset.datatype = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->organization);
          this->__isset.organization = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t DataType::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("DataType");

  xfer += oprot->writeFieldBegin("datatype", ::jsonio17::impex::T_STRING, 1);
  xfer += oprot->writeString(this->datatype);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("organization", ::jsonio17::impex::T_STRING, 2);
  xfer += oprot->writeString(this->organization);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(DataType &a, DataType &b) {
  using ::std::swap;
  swap(a.datatype, b.datatype);
  swap(a.organization, b.organization);
  swap(a.__isset, b.__isset);
}

DataType::DataType(const DataType& other4) {
  datatype = other4.datatype;
  organization = other4.organization;
  __isset = other4.__isset;
}
DataType& DataType::operator=(const DataType& other5) {
  datatype = other5.datatype;
  organization = other5.organization;
  __isset = other5.__isset;
  return *this;
}
void DataType::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "DataType(";
  out << "datatype=" << to_string(datatype);
  out << ", " << "organization=" << to_string(organization);
  out << ")";
}


DataObject::~DataObject() noexcept {
}


void DataObject::__set_field(const std::string& val) {
  this->field = val;
}

void DataObject::__set_ignore(const bool val) {
  this->ignore = val;
__isset.ignore = true;
}

void DataObject::__set_script(const std::string& val) {
  this->script = val;
__isset.script = true;
}

void DataObject::__set_convert(const std::map<std::string, std::string> & val) {
  this->convert = val;
__isset.convert = true;
}
std::ostream& operator<<(std::ostream& out, const DataObject& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t DataObject::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->field);
          this->__isset.field = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_BOOL) {
          xfer += iprot->readBool(this->ignore);
          this->__isset.ignore = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->script);
          this->__isset.script = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_MAP) {
          {
            this->convert.clear();
            uint32_t _size6;
            ::jsonio17::impex::TType _ktype7;
            ::jsonio17::impex::TType _vtype8;
            xfer += iprot->readMapBegin(_ktype7, _vtype8, _size6);
            uint32_t _i10;
            for (_i10 = 0; _i10 < _size6; ++_i10)
            {
              std::string _key11;
              xfer += iprot->readString(_key11);
              std::string& _val12 = this->convert[_key11];
              xfer += iprot->readString(_val12);
            }
            xfer += iprot->readMapEnd();
          }
          this->__isset.convert = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t DataObject::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("DataObject");

  xfer += oprot->writeFieldBegin("field", ::jsonio17::impex::T_STRING, 1);
  xfer += oprot->writeString(this->field);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.ignore) {
    xfer += oprot->writeFieldBegin("ignore", ::jsonio17::impex::T_BOOL, 2);
    xfer += oprot->writeBool(this->ignore);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.script) {
    xfer += oprot->writeFieldBegin("script", ::jsonio17::impex::T_STRING, 3);
    xfer += oprot->writeString(this->script);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.convert) {
    xfer += oprot->writeFieldBegin("convert", ::jsonio17::impex::T_MAP, 4);
    {
      xfer += oprot->writeMapBegin(::jsonio17::impex::T_STRING, ::jsonio17::impex::T_STRING, static_cast<uint32_t>(this->convert.size()));
      std::map<std::string, std::string> ::const_iterator _iter13;
      for (_iter13 = this->convert.begin(); _iter13 != this->convert.end(); ++_iter13)
      {
        xfer += oprot->writeString(_iter13->first);
        xfer += oprot->writeString(_iter13->second);
      }
      xfer += oprot->writeMapEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(DataObject &a, DataObject &b) {
  using ::std::swap;
  swap(a.field, b.field);
  swap(a.ignore, b.ignore);
  swap(a.script, b.script);
  swap(a.convert, b.convert);
  swap(a.__isset, b.__isset);
}

DataObject::DataObject(const DataObject& other14) {
  field = other14.field;
  ignore = other14.ignore;
  script = other14.script;
  convert = other14.convert;
  __isset = other14.__isset;
}
DataObject& DataObject::operator=(const DataObject& other15) {
  field = other15.field;
  ignore = other15.ignore;
  script = other15.script;
  convert = other15.convert;
  __isset = other15.__isset;
  return *this;
}
void DataObject::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "DataObject(";
  out << "field=" << to_string(field);
  out << ", " << "ignore="; (__isset.ignore ? (out << to_string(ignore)) : (out << "<null>"));
  out << ", " << "script="; (__isset.script ? (out << to_string(script)) : (out << "<null>"));
  out << ", " << "convert="; (__isset.convert ? (out << to_string(convert)) : (out << "<null>"));
  out << ")";
}


Separators::~Separators() noexcept {
}


void Separators::__set_v_sep(const std::string& val) {
  this->v_sep = val;
}

void Separators::__set_l_sep(const std::string& val) {
  this->l_sep = val;
}

void Separators::__set_r_sep(const std::string& val) {
  this->r_sep = val;
__isset.r_sep = true;
}

void Separators::__set_c_head(const std::string& val) {
  this->c_head = val;
__isset.c_head = true;
}

void Separators::__set_c_end(const std::string& val) {
  this->c_end = val;
__isset.c_end = true;
}

void Separators::__set_eod(const std::string& val) {
  this->eod = val;
__isset.eod = true;
}

void Separators::__set_encoding(const std::string& val) {
  this->encoding = val;
__isset.encoding = true;
}

void Separators::__set_str_delim(const std::string& val) {
  this->str_delim = val;
__isset.str_delim = true;
}

void Separators::__set_bod(const std::string& val) {
  this->bod = val;
__isset.bod = true;
}
std::ostream& operator<<(std::ostream& out, const Separators& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t Separators::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->v_sep);
          this->__isset.v_sep = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->l_sep);
          this->__isset.l_sep = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->r_sep);
          this->__isset.r_sep = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->c_head);
          this->__isset.c_head = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->c_end);
          this->__isset.c_end = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->eod);
          this->__isset.eod = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->encoding);
          this->__isset.encoding = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->str_delim);
          this->__isset.str_delim = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->bod);
          this->__isset.bod = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Separators::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Separators");

  xfer += oprot->writeFieldBegin("v_sep", ::jsonio17::impex::T_STRING, 1);
  xfer += oprot->writeString(this->v_sep);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("l_sep", ::jsonio17::impex::T_STRING, 2);
  xfer += oprot->writeString(this->l_sep);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.r_sep) {
    xfer += oprot->writeFieldBegin("r_sep", ::jsonio17::impex::T_STRING, 3);
    xfer += oprot->writeString(this->r_sep);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.c_head) {
    xfer += oprot->writeFieldBegin("c_head", ::jsonio17::impex::T_STRING, 4);
    xfer += oprot->writeString(this->c_head);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.c_end) {
    xfer += oprot->writeFieldBegin("c_end", ::jsonio17::impex::T_STRING, 5);
    xfer += oprot->writeString(this->c_end);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.eod) {
    xfer += oprot->writeFieldBegin("eod", ::jsonio17::impex::T_STRING, 6);
    xfer += oprot->writeString(this->eod);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.encoding) {
    xfer += oprot->writeFieldBegin("encoding", ::jsonio17::impex::T_STRING, 7);
    xfer += oprot->writeString(this->encoding);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.str_delim) {
    xfer += oprot->writeFieldBegin("str_delim", ::jsonio17::impex::T_STRING, 8);
    xfer += oprot->writeString(this->str_delim);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.bod) {
    xfer += oprot->writeFieldBegin("bod", ::jsonio17::impex::T_STRING, 9);
    xfer += oprot->writeString(this->bod);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Separators &a, Separators &b) {
  using ::std::swap;
  swap(a.v_sep, b.v_sep);
  swap(a.l_sep, b.l_sep);
  swap(a.r_sep, b.r_sep);
  swap(a.c_head, b.c_head);
  swap(a.c_end, b.c_end);
  swap(a.eod, b.eod);
  swap(a.encoding, b.encoding);
  swap(a.str_delim, b.str_delim);
  swap(a.bod, b.bod);
  swap(a.__isset, b.__isset);
}

Separators::Separators(const Separators& other16) {
  v_sep = other16.v_sep;
  l_sep = other16.l_sep;
  r_sep = other16.r_sep;
  c_head = other16.c_head;
  c_end = other16.c_end;
  eod = other16.eod;
  encoding = other16.encoding;
  str_delim = other16.str_delim;
  bod = other16.bod;
  __isset = other16.__isset;
}
Separators& Separators::operator=(const Separators& other17) {
  v_sep = other17.v_sep;
  l_sep = other17.l_sep;
  r_sep = other17.r_sep;
  c_head = other17.c_head;
  c_end = other17.c_end;
  eod = other17.eod;
  encoding = other17.encoding;
  str_delim = other17.str_delim;
  bod = other17.bod;
  __isset = other17.__isset;
  return *this;
}
void Separators::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "Separators(";
  out << "v_sep=" << to_string(v_sep);
  out << ", " << "l_sep=" << to_string(l_sep);
  out << ", " << "r_sep="; (__isset.r_sep ? (out << to_string(r_sep)) : (out << "<null>"));
  out << ", " << "c_head="; (__isset.c_head ? (out << to_string(c_head)) : (out << "<null>"));
  out << ", " << "c_end="; (__isset.c_end ? (out << to_string(c_end)) : (out << "<null>"));
  out << ", " << "eod="; (__isset.eod ? (out << to_string(eod)) : (out << "<null>"));
  out << ", " << "encoding="; (__isset.encoding ? (out << to_string(encoding)) : (out << "<null>"));
  out << ", " << "str_delim="; (__isset.str_delim ? (out << to_string(str_delim)) : (out << "<null>"));
  out << ", " << "bod="; (__isset.bod ? (out << to_string(bod)) : (out << "<null>"));
  out << ")";
}


FormatBlock::~FormatBlock() noexcept {
}


void FormatBlock::__set_defaults(const std::map<std::string, std::string> & val) {
  this->defaults = val;
}

void FormatBlock::__set_pairs(const std::map<std::string, DataType> & val) {
  this->pairs = val;
}

void FormatBlock::__set_matches(const std::map<std::string, DataObject> & val) {
  this->matches = val;
}

void FormatBlock::__set_script(const std::string& val) {
  this->script = val;
__isset.script = true;
}
std::ostream& operator<<(std::ostream& out, const FormatBlock& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t FormatBlock::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;

  bool isset_matches = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_MAP) {
          {
            this->defaults.clear();
            uint32_t _size18;
            ::jsonio17::impex::TType _ktype19;
            ::jsonio17::impex::TType _vtype20;
            xfer += iprot->readMapBegin(_ktype19, _vtype20, _size18);
            uint32_t _i22;
            for (_i22 = 0; _i22 < _size18; ++_i22)
            {
              std::string _key23;
              xfer += iprot->readString(_key23);
              std::string& _val24 = this->defaults[_key23];
              xfer += iprot->readString(_val24);
            }
            xfer += iprot->readMapEnd();
          }
          this->__isset.defaults = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_MAP) {
          {
            this->pairs.clear();
            uint32_t _size25;
            ::jsonio17::impex::TType _ktype26;
            ::jsonio17::impex::TType _vtype27;
            xfer += iprot->readMapBegin(_ktype26, _vtype27, _size25);
            uint32_t _i29;
            for (_i29 = 0; _i29 < _size25; ++_i29)
            {
              std::string _key30;
              xfer += iprot->readString(_key30);
              DataType& _val31 = this->pairs[_key30];
              xfer += _val31.read(iprot);
            }
            xfer += iprot->readMapEnd();
          }
          this->__isset.pairs = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_MAP) {
          {
            this->matches.clear();
            uint32_t _size32;
            ::jsonio17::impex::TType _ktype33;
            ::jsonio17::impex::TType _vtype34;
            xfer += iprot->readMapBegin(_ktype33, _vtype34, _size32);
            uint32_t _i36;
            for (_i36 = 0; _i36 < _size32; ++_i36)
            {
              std::string _key37;
              xfer += iprot->readString(_key37);
              DataObject& _val38 = this->matches[_key37];
              xfer += _val38.read(iprot);
            }
            xfer += iprot->readMapEnd();
          }
          isset_matches = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->script);
          this->__isset.script = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_matches)
       jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected matches");
      //  throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FormatBlock::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FormatBlock");

  xfer += oprot->writeFieldBegin("defaults", ::jsonio17::impex::T_MAP, 1);
  {
    xfer += oprot->writeMapBegin(::jsonio17::impex::T_STRING, ::jsonio17::impex::T_STRING, static_cast<uint32_t>(this->defaults.size()));
    std::map<std::string, std::string> ::const_iterator _iter39;
    for (_iter39 = this->defaults.begin(); _iter39 != this->defaults.end(); ++_iter39)
    {
      xfer += oprot->writeString(_iter39->first);
      xfer += oprot->writeString(_iter39->second);
    }
    xfer += oprot->writeMapEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("pairs", ::jsonio17::impex::T_MAP, 2);
  {
    xfer += oprot->writeMapBegin(::jsonio17::impex::T_STRING, ::jsonio17::impex::T_STRUCT, static_cast<uint32_t>(this->pairs.size()));
    std::map<std::string, DataType> ::const_iterator _iter40;
    for (_iter40 = this->pairs.begin(); _iter40 != this->pairs.end(); ++_iter40)
    {
      xfer += oprot->writeString(_iter40->first);
      xfer += _iter40->second.write(oprot);
    }
    xfer += oprot->writeMapEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("matches", ::jsonio17::impex::T_MAP, 3);
  {
    xfer += oprot->writeMapBegin(::jsonio17::impex::T_STRING, ::jsonio17::impex::T_STRUCT, static_cast<uint32_t>(this->matches.size()));
    std::map<std::string, DataObject> ::const_iterator _iter41;
    for (_iter41 = this->matches.begin(); _iter41 != this->matches.end(); ++_iter41)
    {
      xfer += oprot->writeString(_iter41->first);
      xfer += _iter41->second.write(oprot);
    }
    xfer += oprot->writeMapEnd();
  }
  xfer += oprot->writeFieldEnd();

  if (this->__isset.script) {
    xfer += oprot->writeFieldBegin("script", ::jsonio17::impex::T_STRING, 4);
    xfer += oprot->writeString(this->script);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FormatBlock &a, FormatBlock &b) {
  using ::std::swap;
  swap(a.defaults, b.defaults);
  swap(a.pairs, b.pairs);
  swap(a.matches, b.matches);
  swap(a.script, b.script);
  swap(a.__isset, b.__isset);
}

FormatBlock::FormatBlock(const FormatBlock& other42) {
  defaults = other42.defaults;
  pairs = other42.pairs;
  matches = other42.matches;
  script = other42.script;
  __isset = other42.__isset;
}
FormatBlock& FormatBlock::operator=(const FormatBlock& other43) {
  defaults = other43.defaults;
  pairs = other43.pairs;
  matches = other43.matches;
  script = other43.script;
  __isset = other43.__isset;
  return *this;
}
void FormatBlock::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "FormatBlock(";
  out << "defaults=" << to_string(defaults);
  out << ", " << "pairs=" << to_string(pairs);
  out << ", " << "matches=" << to_string(matches);
  out << ", " << "script="; (__isset.script ? (out << to_string(script)) : (out << "<null>"));
  out << ")";
}


FormatTextFile::~FormatTextFile() noexcept {
}


void FormatTextFile::__set_block(const FormatBlock& val) {
  this->block = val;
}

void FormatTextFile::__set_lines(const std::vector<std::string> & val) {
  this->lines = val;
}

void FormatTextFile::__set_label(const std::string& val) {
  this->label = val;
}

void FormatTextFile::__set_separs(const Separators& val) {
  this->separs = val;
}

void FormatTextFile::__set_comment(const std::string& val) {
  this->comment = val;
__isset.comment = true;
}

void FormatTextFile::__set_file_name(const std::string& val) {
  this->file_name = val;
__isset.file_name = true;
}

void FormatTextFile::__set_Nblocks(const int32_t val) {
  this->Nblocks = val;
__isset.Nblocks = true;
}

void FormatTextFile::__set_Nlines(const int32_t val) {
  this->Nlines = val;
__isset.Nlines = true;
}

void FormatTextFile::__set_Nchars(const int32_t val) {
  this->Nchars = val;
__isset.Nchars = true;
}

void FormatTextFile::__set_direction(const DirectionType::type val) {
  this->direction = val;
}
std::ostream& operator<<(std::ostream& out, const FormatTextFile& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t FormatTextFile::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;

  bool isset_block = false;
  bool isset_lines = false;
  bool isset_label = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->block.read(iprot);
          isset_block = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_LIST) {
          {
            this->lines.clear();
            uint32_t _size44;
            ::jsonio17::impex::TType _etype47;
            xfer += iprot->readListBegin(_etype47, _size44);
            this->lines.resize(_size44);
            uint32_t _i48;
            for (_i48 = 0; _i48 < _size44; ++_i48)
            {
              xfer += iprot->readString(this->lines[_i48]);
            }
            xfer += iprot->readListEnd();
          }
          isset_lines = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->label);
          isset_label = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->separs.read(iprot);
          this->__isset.separs = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->comment);
          this->__isset.comment = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->file_name);
          this->__isset.file_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nblocks);
          this->__isset.Nblocks = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nlines);
          this->__isset.Nlines = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nchars);
          this->__isset.Nchars = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::jsonio17::impex::T_I32) {
          int32_t ecast49;
          xfer += iprot->readI32(ecast49);
          this->direction = (DirectionType::type)ecast49;
          this->__isset.direction = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_block)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected block");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_lines)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected lines");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_label)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected label");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FormatTextFile::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FormatTextFile");

  xfer += oprot->writeFieldBegin("block", ::jsonio17::impex::T_STRUCT, 1);
  xfer += this->block.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("lines", ::jsonio17::impex::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::jsonio17::impex::T_STRING, static_cast<uint32_t>(this->lines.size()));
    std::vector<std::string> ::const_iterator _iter50;
    for (_iter50 = this->lines.begin(); _iter50 != this->lines.end(); ++_iter50)
    {
      xfer += oprot->writeString((*_iter50));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("label", ::jsonio17::impex::T_STRING, 3);
  xfer += oprot->writeString(this->label);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("separs", ::jsonio17::impex::T_STRUCT, 4);
  xfer += this->separs.write(oprot);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.comment) {
    xfer += oprot->writeFieldBegin("comment", ::jsonio17::impex::T_STRING, 5);
    xfer += oprot->writeString(this->comment);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.file_name) {
    xfer += oprot->writeFieldBegin("file_name", ::jsonio17::impex::T_STRING, 6);
    xfer += oprot->writeString(this->file_name);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Nblocks) {
    xfer += oprot->writeFieldBegin("Nblocks", ::jsonio17::impex::T_I32, 7);
    xfer += oprot->writeI32(this->Nblocks);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Nlines) {
    xfer += oprot->writeFieldBegin("Nlines", ::jsonio17::impex::T_I32, 8);
    xfer += oprot->writeI32(this->Nlines);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Nchars) {
    xfer += oprot->writeFieldBegin("Nchars", ::jsonio17::impex::T_I32, 9);
    xfer += oprot->writeI32(this->Nchars);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldBegin("direction", ::jsonio17::impex::T_I32, 10);
  xfer += oprot->writeI32((int32_t)this->direction);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FormatTextFile &a, FormatTextFile &b) {
  using ::std::swap;
  swap(a.block, b.block);
  swap(a.lines, b.lines);
  swap(a.label, b.label);
  swap(a.separs, b.separs);
  swap(a.comment, b.comment);
  swap(a.file_name, b.file_name);
  swap(a.Nblocks, b.Nblocks);
  swap(a.Nlines, b.Nlines);
  swap(a.Nchars, b.Nchars);
  swap(a.direction, b.direction);
  swap(a.__isset, b.__isset);
}

FormatTextFile::FormatTextFile(const FormatTextFile& other51) {
  block = other51.block;
  lines = other51.lines;
  label = other51.label;
  separs = other51.separs;
  comment = other51.comment;
  file_name = other51.file_name;
  Nblocks = other51.Nblocks;
  Nlines = other51.Nlines;
  Nchars = other51.Nchars;
  direction = other51.direction;
  __isset = other51.__isset;
}
FormatTextFile& FormatTextFile::operator=(const FormatTextFile& other52) {
  block = other52.block;
  lines = other52.lines;
  label = other52.label;
  separs = other52.separs;
  comment = other52.comment;
  file_name = other52.file_name;
  Nblocks = other52.Nblocks;
  Nlines = other52.Nlines;
  Nchars = other52.Nchars;
  direction = other52.direction;
  __isset = other52.__isset;
  return *this;
}
void FormatTextFile::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "FormatTextFile(";
  out << "block=" << to_string(block);
  out << ", " << "lines=" << to_string(lines);
  out << ", " << "label=" << to_string(label);
  out << ", " << "separs=" << to_string(separs);
  out << ", " << "comment="; (__isset.comment ? (out << to_string(comment)) : (out << "<null>"));
  out << ", " << "file_name="; (__isset.file_name ? (out << to_string(file_name)) : (out << "<null>"));
  out << ", " << "Nblocks="; (__isset.Nblocks ? (out << to_string(Nblocks)) : (out << "<null>"));
  out << ", " << "Nlines="; (__isset.Nlines ? (out << to_string(Nlines)) : (out << "<null>"));
  out << ", " << "Nchars="; (__isset.Nchars ? (out << to_string(Nchars)) : (out << "<null>"));
  out << ", " << "direction=" << to_string(direction);
  out << ")";
}


FormatKeyValue::~FormatKeyValue() noexcept {
}


void FormatKeyValue::__set_head_regexp(const std::string& val) {
  this->head_regexp = val;
}

void FormatKeyValue::__set_end_regexp(const std::string& val) {
  this->end_regexp = val;
}

void FormatKeyValue::__set_key_regexp(const std::string& val) {
  this->key_regexp = val;
}

void FormatKeyValue::__set_value_regexp(const std::string& val) {
  this->value_regexp = val;
}

void FormatKeyValue::__set_value_next(const std::string& val) {
  this->value_next = val;
}

void FormatKeyValue::__set_value_token_regexp(const std::string& val) {
  this->value_token_regexp = val;
__isset.value_token_regexp = true;
}

void FormatKeyValue::__set_comment_regexp(const std::string& val) {
  this->comment_regexp = val;
__isset.comment_regexp = true;
}

void FormatKeyValue::__set_Ndata(const int32_t val) {
  this->Ndata = val;
__isset.Ndata = true;
}

void FormatKeyValue::__set_strvalue_exp(const std::string& val) {
  this->strvalue_exp = val;
}

void FormatKeyValue::__set_key_order(const std::vector<std::string> & val) {
  this->key_order = val;
__isset.key_order = true;
}
std::ostream& operator<<(std::ostream& out, const FormatKeyValue& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t FormatKeyValue::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;

  bool isset_head_regexp = false;
  bool isset_end_regexp = false;
  bool isset_key_regexp = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->head_regexp);
          isset_head_regexp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->end_regexp);
          isset_end_regexp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->key_regexp);
          isset_key_regexp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->value_regexp);
          this->__isset.value_regexp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->value_next);
          this->__isset.value_next = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->value_token_regexp);
          this->__isset.value_token_regexp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->comment_regexp);
          this->__isset.comment_regexp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Ndata);
          this->__isset.Ndata = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->strvalue_exp);
          this->__isset.strvalue_exp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::jsonio17::impex::T_LIST) {
          {
            this->key_order.clear();
            uint32_t _size53;
            ::jsonio17::impex::TType _etype56;
            xfer += iprot->readListBegin(_etype56, _size53);
            this->key_order.resize(_size53);
            uint32_t _i57;
            for (_i57 = 0; _i57 < _size53; ++_i57)
            {
              xfer += iprot->readString(this->key_order[_i57]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.key_order = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_head_regexp)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected head_regexp");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_end_regexp)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected end_regexp");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_key_regexp)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected key_regexp");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FormatKeyValue::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FormatKeyValue");

  xfer += oprot->writeFieldBegin("head_regexp", ::jsonio17::impex::T_STRING, 1);
  xfer += oprot->writeString(this->head_regexp);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("end_regexp", ::jsonio17::impex::T_STRING, 2);
  xfer += oprot->writeString(this->end_regexp);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("key_regexp", ::jsonio17::impex::T_STRING, 3);
  xfer += oprot->writeString(this->key_regexp);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value_regexp", ::jsonio17::impex::T_STRING, 4);
  xfer += oprot->writeString(this->value_regexp);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value_next", ::jsonio17::impex::T_STRING, 5);
  xfer += oprot->writeString(this->value_next);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.value_token_regexp) {
    xfer += oprot->writeFieldBegin("value_token_regexp", ::jsonio17::impex::T_STRING, 6);
    xfer += oprot->writeString(this->value_token_regexp);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.comment_regexp) {
    xfer += oprot->writeFieldBegin("comment_regexp", ::jsonio17::impex::T_STRING, 7);
    xfer += oprot->writeString(this->comment_regexp);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Ndata) {
    xfer += oprot->writeFieldBegin("Ndata", ::jsonio17::impex::T_I32, 8);
    xfer += oprot->writeI32(this->Ndata);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldBegin("strvalue_exp", ::jsonio17::impex::T_STRING, 9);
  xfer += oprot->writeString(this->strvalue_exp);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.key_order) {
    xfer += oprot->writeFieldBegin("key_order", ::jsonio17::impex::T_LIST, 10);
    {
      xfer += oprot->writeListBegin(::jsonio17::impex::T_STRING, static_cast<uint32_t>(this->key_order.size()));
      std::vector<std::string> ::const_iterator _iter58;
      for (_iter58 = this->key_order.begin(); _iter58 != this->key_order.end(); ++_iter58)
      {
        xfer += oprot->writeString((*_iter58));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FormatKeyValue &a, FormatKeyValue &b) {
  using ::std::swap;
  swap(a.head_regexp, b.head_regexp);
  swap(a.end_regexp, b.end_regexp);
  swap(a.key_regexp, b.key_regexp);
  swap(a.value_regexp, b.value_regexp);
  swap(a.value_next, b.value_next);
  swap(a.value_token_regexp, b.value_token_regexp);
  swap(a.comment_regexp, b.comment_regexp);
  swap(a.Ndata, b.Ndata);
  swap(a.strvalue_exp, b.strvalue_exp);
  swap(a.key_order, b.key_order);
  swap(a.__isset, b.__isset);
}

FormatKeyValue::FormatKeyValue(const FormatKeyValue& other59) {
  head_regexp = other59.head_regexp;
  end_regexp = other59.end_regexp;
  key_regexp = other59.key_regexp;
  value_regexp = other59.value_regexp;
  value_next = other59.value_next;
  value_token_regexp = other59.value_token_regexp;
  comment_regexp = other59.comment_regexp;
  Ndata = other59.Ndata;
  strvalue_exp = other59.strvalue_exp;
  key_order = other59.key_order;
  __isset = other59.__isset;
}
FormatKeyValue& FormatKeyValue::operator=(const FormatKeyValue& other60) {
  head_regexp = other60.head_regexp;
  end_regexp = other60.end_regexp;
  key_regexp = other60.key_regexp;
  value_regexp = other60.value_regexp;
  value_next = other60.value_next;
  value_token_regexp = other60.value_token_regexp;
  comment_regexp = other60.comment_regexp;
  Ndata = other60.Ndata;
  strvalue_exp = other60.strvalue_exp;
  key_order = other60.key_order;
  __isset = other60.__isset;
  return *this;
}
void FormatKeyValue::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "FormatKeyValue(";
  out << "head_regexp=" << to_string(head_regexp);
  out << ", " << "end_regexp=" << to_string(end_regexp);
  out << ", " << "key_regexp=" << to_string(key_regexp);
  out << ", " << "value_regexp=" << to_string(value_regexp);
  out << ", " << "value_next=" << to_string(value_next);
  out << ", " << "value_token_regexp="; (__isset.value_token_regexp ? (out << to_string(value_token_regexp)) : (out << "<null>"));
  out << ", " << "comment_regexp="; (__isset.comment_regexp ? (out << to_string(comment_regexp)) : (out << "<null>"));
  out << ", " << "Ndata="; (__isset.Ndata ? (out << to_string(Ndata)) : (out << "<null>"));
  out << ", " << "strvalue_exp=" << to_string(strvalue_exp);
  out << ", " << "key_order="; (__isset.key_order ? (out << to_string(key_order)) : (out << "<null>"));
  out << ")";
}


FormatKeyValueFile::~FormatKeyValueFile() noexcept {
}


void FormatKeyValueFile::__set_block(const FormatBlock& val) {
  this->block = val;
}

void FormatKeyValueFile::__set_format(const FormatKeyValue& val) {
  this->format = val;
}

void FormatKeyValueFile::__set_renderer(const std::string& val) {
  this->renderer = val;
}

void FormatKeyValueFile::__set_label(const std::string& val) {
  this->label = val;
}

void FormatKeyValueFile::__set_separators(const Separators& val) {
  this->separators = val;
}

void FormatKeyValueFile::__set_comment(const std::string& val) {
  this->comment = val;
__isset.comment = true;
}

void FormatKeyValueFile::__set_fname(const std::string& val) {
  this->fname = val;
__isset.fname = true;
}

void FormatKeyValueFile::__set_Nblocks(const int32_t val) {
  this->Nblocks = val;
__isset.Nblocks = true;
}

void FormatKeyValueFile::__set_Nlines(const int32_t val) {
  this->Nlines = val;
__isset.Nlines = true;
}

void FormatKeyValueFile::__set_direction(const DirectionType::type val) {
  this->direction = val;
}
std::ostream& operator<<(std::ostream& out, const FormatKeyValueFile& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t FormatKeyValueFile::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;

  bool isset_block = false;
  bool isset_format = false;
  bool isset_renderer = false;
  bool isset_label = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->block.read(iprot);
          isset_block = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->format.read(iprot);
          isset_format = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->renderer);
          isset_renderer = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->label);
          isset_label = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->separators.read(iprot);
          this->__isset.separators = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->comment);
          this->__isset.comment = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->fname);
          this->__isset.fname = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nblocks);
          this->__isset.Nblocks = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nlines);
          this->__isset.Nlines = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::jsonio17::impex::T_I32) {
          int32_t ecast61;
          xfer += iprot->readI32(ecast61);
          this->direction = (DirectionType::type)ecast61;
          this->__isset.direction = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_block)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected block");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_format)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected format");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_renderer)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected renderer");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_label)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected label");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FormatKeyValueFile::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FormatKeyValueFile");

  xfer += oprot->writeFieldBegin("block", ::jsonio17::impex::T_STRUCT, 1);
  xfer += this->block.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("format", ::jsonio17::impex::T_STRUCT, 2);
  xfer += this->format.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("renderer", ::jsonio17::impex::T_STRING, 3);
  xfer += oprot->writeString(this->renderer);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("label", ::jsonio17::impex::T_STRING, 4);
  xfer += oprot->writeString(this->label);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("separators", ::jsonio17::impex::T_STRUCT, 5);
  xfer += this->separators.write(oprot);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.comment) {
    xfer += oprot->writeFieldBegin("comment", ::jsonio17::impex::T_STRING, 6);
    xfer += oprot->writeString(this->comment);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.fname) {
    xfer += oprot->writeFieldBegin("fname", ::jsonio17::impex::T_STRING, 7);
    xfer += oprot->writeString(this->fname);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Nblocks) {
    xfer += oprot->writeFieldBegin("Nblocks", ::jsonio17::impex::T_I32, 8);
    xfer += oprot->writeI32(this->Nblocks);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Nlines) {
    xfer += oprot->writeFieldBegin("Nlines", ::jsonio17::impex::T_I32, 9);
    xfer += oprot->writeI32(this->Nlines);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldBegin("direction", ::jsonio17::impex::T_I32, 10);
  xfer += oprot->writeI32((int32_t)this->direction);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FormatKeyValueFile &a, FormatKeyValueFile &b) {
  using ::std::swap;
  swap(a.block, b.block);
  swap(a.format, b.format);
  swap(a.renderer, b.renderer);
  swap(a.label, b.label);
  swap(a.separators, b.separators);
  swap(a.comment, b.comment);
  swap(a.fname, b.fname);
  swap(a.Nblocks, b.Nblocks);
  swap(a.Nlines, b.Nlines);
  swap(a.direction, b.direction);
  swap(a.__isset, b.__isset);
}

FormatKeyValueFile::FormatKeyValueFile(const FormatKeyValueFile& other62) {
  block = other62.block;
  format = other62.format;
  renderer = other62.renderer;
  label = other62.label;
  separators = other62.separators;
  comment = other62.comment;
  fname = other62.fname;
  Nblocks = other62.Nblocks;
  Nlines = other62.Nlines;
  direction = other62.direction;
  __isset = other62.__isset;
}
FormatKeyValueFile& FormatKeyValueFile::operator=(const FormatKeyValueFile& other63) {
  block = other63.block;
  format = other63.format;
  renderer = other63.renderer;
  label = other63.label;
  separators = other63.separators;
  comment = other63.comment;
  fname = other63.fname;
  Nblocks = other63.Nblocks;
  Nlines = other63.Nlines;
  direction = other63.direction;
  __isset = other63.__isset;
  return *this;
}
void FormatKeyValueFile::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "FormatKeyValueFile(";
  out << "block=" << to_string(block);
  out << ", " << "format=" << to_string(format);
  out << ", " << "renderer=" << to_string(renderer);
  out << ", " << "label=" << to_string(label);
  out << ", " << "separators=" << to_string(separators);
  out << ", " << "comment="; (__isset.comment ? (out << to_string(comment)) : (out << "<null>"));
  out << ", " << "fname="; (__isset.fname ? (out << to_string(fname)) : (out << "<null>"));
  out << ", " << "Nblocks="; (__isset.Nblocks ? (out << to_string(Nblocks)) : (out << "<null>"));
  out << ", " << "Nlines="; (__isset.Nlines ? (out << to_string(Nlines)) : (out << "<null>"));
  out << ", " << "direction=" << to_string(direction);
  out << ")";
}


FormatTable::~FormatTable() noexcept {
}


void FormatTable::__set_Nhcols(const int32_t val) {
  this->Nhcols = val;
}

void FormatTable::__set_Nhrows(const int32_t val) {
  this->Nhrows = val;
}

void FormatTable::__set_headers(const std::vector<std::string> & val) {
  this->headers = val;
}

void FormatTable::__set_rowend(const std::string& val) {
  this->rowend = val;
}

void FormatTable::__set_rows_one_block(const int32_t val) {
  this->rows_one_block = val;
}

void FormatTable::__set_row_header_regexp(const std::string& val) {
  this->row_header_regexp = val;
}

void FormatTable::__set_colends(const std::string& val) {
  this->colends = val;
}

void FormatTable::__set_usequotes(const bool val) {
  this->usequotes = val;
}

void FormatTable::__set_usemore(const bool val) {
  this->usemore = val;
}

void FormatTable::__set_comment_regexp(const std::string& val) {
  this->comment_regexp = val;
__isset.comment_regexp = true;
}

void FormatTable::__set_row_size(const int32_t val) {
  this->row_size = val;
__isset.row_size = true;
}

void FormatTable::__set_colsizes(const std::vector<int32_t> & val) {
  this->colsizes = val;
__isset.colsizes = true;
}

void FormatTable::__set_value_regexp(const std::vector<std::string> & val) {
  this->value_regexp = val;
__isset.value_regexp = true;
}
std::ostream& operator<<(std::ostream& out, const FormatTable& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t FormatTable::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nhcols);
          this->__isset.Nhcols = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nhrows);
          this->__isset.Nhrows = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_LIST) {
          {
            this->headers.clear();
            uint32_t _size64;
            ::jsonio17::impex::TType _etype67;
            xfer += iprot->readListBegin(_etype67, _size64);
            this->headers.resize(_size64);
            uint32_t _i68;
            for (_i68 = 0; _i68 < _size64; ++_i68)
            {
              xfer += iprot->readString(this->headers[_i68]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.headers = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->rowend);
          this->__isset.rowend = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->rows_one_block);
          this->__isset.rows_one_block = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->row_header_regexp);
          this->__isset.row_header_regexp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->colends);
          this->__isset.colends = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::jsonio17::impex::T_BOOL) {
          xfer += iprot->readBool(this->usequotes);
          this->__isset.usequotes = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::jsonio17::impex::T_BOOL) {
          xfer += iprot->readBool(this->usemore);
          this->__isset.usemore = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->comment_regexp);
          this->__isset.comment_regexp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 11:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->row_size);
          this->__isset.row_size = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 12:
        if (ftype == ::jsonio17::impex::T_LIST) {
          {
            this->colsizes.clear();
            uint32_t _size69;
            ::jsonio17::impex::TType _etype72;
            xfer += iprot->readListBegin(_etype72, _size69);
            this->colsizes.resize(_size69);
            uint32_t _i73;
            for (_i73 = 0; _i73 < _size69; ++_i73)
            {
              xfer += iprot->readI32(this->colsizes[_i73]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.colsizes = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 13:
        if (ftype == ::jsonio17::impex::T_LIST) {
          {
            this->value_regexp.clear();
            uint32_t _size74;
            ::jsonio17::impex::TType _etype77;
            xfer += iprot->readListBegin(_etype77, _size74);
            this->value_regexp.resize(_size74);
            uint32_t _i78;
            for (_i78 = 0; _i78 < _size74; ++_i78)
            {
              xfer += iprot->readString(this->value_regexp[_i78]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.value_regexp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t FormatTable::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FormatTable");

  xfer += oprot->writeFieldBegin("Nhcols", ::jsonio17::impex::T_I32, 1);
  xfer += oprot->writeI32(this->Nhcols);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Nhrows", ::jsonio17::impex::T_I32, 2);
  xfer += oprot->writeI32(this->Nhrows);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("headers", ::jsonio17::impex::T_LIST, 3);
  {
    xfer += oprot->writeListBegin(::jsonio17::impex::T_STRING, static_cast<uint32_t>(this->headers.size()));
    std::vector<std::string> ::const_iterator _iter79;
    for (_iter79 = this->headers.begin(); _iter79 != this->headers.end(); ++_iter79)
    {
      xfer += oprot->writeString((*_iter79));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("rowend", ::jsonio17::impex::T_STRING, 4);
  xfer += oprot->writeString(this->rowend);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("rows_one_block", ::jsonio17::impex::T_I32, 5);
  xfer += oprot->writeI32(this->rows_one_block);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("row_header_regexp", ::jsonio17::impex::T_STRING, 6);
  xfer += oprot->writeString(this->row_header_regexp);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("colends", ::jsonio17::impex::T_STRING, 7);
  xfer += oprot->writeString(this->colends);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("usequotes", ::jsonio17::impex::T_BOOL, 8);
  xfer += oprot->writeBool(this->usequotes);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("usemore", ::jsonio17::impex::T_BOOL, 9);
  xfer += oprot->writeBool(this->usemore);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.comment_regexp) {
    xfer += oprot->writeFieldBegin("comment_regexp", ::jsonio17::impex::T_STRING, 10);
    xfer += oprot->writeString(this->comment_regexp);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.row_size) {
    xfer += oprot->writeFieldBegin("row_size", ::jsonio17::impex::T_I32, 11);
    xfer += oprot->writeI32(this->row_size);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.colsizes) {
    xfer += oprot->writeFieldBegin("colsizes", ::jsonio17::impex::T_LIST, 12);
    {
      xfer += oprot->writeListBegin(::jsonio17::impex::T_I32, static_cast<uint32_t>(this->colsizes.size()));
      std::vector<int32_t> ::const_iterator _iter80;
      for (_iter80 = this->colsizes.begin(); _iter80 != this->colsizes.end(); ++_iter80)
      {
        xfer += oprot->writeI32((*_iter80));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.value_regexp) {
    xfer += oprot->writeFieldBegin("value_regexp", ::jsonio17::impex::T_LIST, 13);
    {
      xfer += oprot->writeListBegin(::jsonio17::impex::T_STRING, static_cast<uint32_t>(this->value_regexp.size()));
      std::vector<std::string> ::const_iterator _iter81;
      for (_iter81 = this->value_regexp.begin(); _iter81 != this->value_regexp.end(); ++_iter81)
      {
        xfer += oprot->writeString((*_iter81));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FormatTable &a, FormatTable &b) {
  using ::std::swap;
  swap(a.Nhcols, b.Nhcols);
  swap(a.Nhrows, b.Nhrows);
  swap(a.headers, b.headers);
  swap(a.rowend, b.rowend);
  swap(a.rows_one_block, b.rows_one_block);
  swap(a.row_header_regexp, b.row_header_regexp);
  swap(a.colends, b.colends);
  swap(a.usequotes, b.usequotes);
  swap(a.usemore, b.usemore);
  swap(a.comment_regexp, b.comment_regexp);
  swap(a.row_size, b.row_size);
  swap(a.colsizes, b.colsizes);
  swap(a.value_regexp, b.value_regexp);
  swap(a.__isset, b.__isset);
}

FormatTable::FormatTable(const FormatTable& other82) {
  Nhcols = other82.Nhcols;
  Nhrows = other82.Nhrows;
  headers = other82.headers;
  rowend = other82.rowend;
  rows_one_block = other82.rows_one_block;
  row_header_regexp = other82.row_header_regexp;
  colends = other82.colends;
  usequotes = other82.usequotes;
  usemore = other82.usemore;
  comment_regexp = other82.comment_regexp;
  row_size = other82.row_size;
  colsizes = other82.colsizes;
  value_regexp = other82.value_regexp;
  __isset = other82.__isset;
}
FormatTable& FormatTable::operator=(const FormatTable& other83) {
  Nhcols = other83.Nhcols;
  Nhrows = other83.Nhrows;
  headers = other83.headers;
  rowend = other83.rowend;
  rows_one_block = other83.rows_one_block;
  row_header_regexp = other83.row_header_regexp;
  colends = other83.colends;
  usequotes = other83.usequotes;
  usemore = other83.usemore;
  comment_regexp = other83.comment_regexp;
  row_size = other83.row_size;
  colsizes = other83.colsizes;
  value_regexp = other83.value_regexp;
  __isset = other83.__isset;
  return *this;
}
void FormatTable::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "FormatTable(";
  out << "Nhcols=" << to_string(Nhcols);
  out << ", " << "Nhrows=" << to_string(Nhrows);
  out << ", " << "headers=" << to_string(headers);
  out << ", " << "rowend=" << to_string(rowend);
  out << ", " << "rows_one_block=" << to_string(rows_one_block);
  out << ", " << "row_header_regexp=" << to_string(row_header_regexp);
  out << ", " << "colends=" << to_string(colends);
  out << ", " << "usequotes=" << to_string(usequotes);
  out << ", " << "usemore=" << to_string(usemore);
  out << ", " << "comment_regexp="; (__isset.comment_regexp ? (out << to_string(comment_regexp)) : (out << "<null>"));
  out << ", " << "row_size="; (__isset.row_size ? (out << to_string(row_size)) : (out << "<null>"));
  out << ", " << "colsizes="; (__isset.colsizes ? (out << to_string(colsizes)) : (out << "<null>"));
  out << ", " << "value_regexp="; (__isset.value_regexp ? (out << to_string(value_regexp)) : (out << "<null>"));
  out << ")";
}


FormatTableFile::~FormatTableFile() noexcept {
}


void FormatTableFile::__set_block(const FormatBlock& val) {
  this->block = val;
}

void FormatTableFile::__set_format(const FormatTable& val) {
  this->format = val;
}

void FormatTableFile::__set_renderer(const std::string& val) {
  this->renderer = val;
}

void FormatTableFile::__set_label(const std::string& val) {
  this->label = val;
}

void FormatTableFile::__set_separators(const Separators& val) {
  this->separators = val;
}

void FormatTableFile::__set_comment(const std::string& val) {
  this->comment = val;
__isset.comment = true;
}

void FormatTableFile::__set_fname(const std::string& val) {
  this->fname = val;
__isset.fname = true;
}

void FormatTableFile::__set_Nblocks(const int32_t val) {
  this->Nblocks = val;
__isset.Nblocks = true;
}

void FormatTableFile::__set_Nlines(const int32_t val) {
  this->Nlines = val;
__isset.Nlines = true;
}

void FormatTableFile::__set_direction(const DirectionType::type val) {
  this->direction = val;
}
std::ostream& operator<<(std::ostream& out, const FormatTableFile& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t FormatTableFile::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;

  bool isset_block = false;
  bool isset_format = false;
  bool isset_renderer = false;
  bool isset_label = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->block.read(iprot);
          isset_block = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->format.read(iprot);
          isset_format = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->renderer);
          isset_renderer = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->label);
          isset_label = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->separators.read(iprot);
          this->__isset.separators = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->comment);
          this->__isset.comment = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->fname);
          this->__isset.fname = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nblocks);
          this->__isset.Nblocks = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nlines);
          this->__isset.Nlines = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::jsonio17::impex::T_I32) {
          int32_t ecast84;
          xfer += iprot->readI32(ecast84);
          this->direction = (DirectionType::type)ecast84;
          this->__isset.direction = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_block)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected block");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_format)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected format");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_renderer)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected renderer");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_label)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected label");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FormatTableFile::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FormatTableFile");

  xfer += oprot->writeFieldBegin("block", ::jsonio17::impex::T_STRUCT, 1);
  xfer += this->block.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("format", ::jsonio17::impex::T_STRUCT, 2);
  xfer += this->format.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("renderer", ::jsonio17::impex::T_STRING, 3);
  xfer += oprot->writeString(this->renderer);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("label", ::jsonio17::impex::T_STRING, 4);
  xfer += oprot->writeString(this->label);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("separators", ::jsonio17::impex::T_STRUCT, 5);
  xfer += this->separators.write(oprot);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.comment) {
    xfer += oprot->writeFieldBegin("comment", ::jsonio17::impex::T_STRING, 6);
    xfer += oprot->writeString(this->comment);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.fname) {
    xfer += oprot->writeFieldBegin("fname", ::jsonio17::impex::T_STRING, 7);
    xfer += oprot->writeString(this->fname);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Nblocks) {
    xfer += oprot->writeFieldBegin("Nblocks", ::jsonio17::impex::T_I32, 8);
    xfer += oprot->writeI32(this->Nblocks);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Nlines) {
    xfer += oprot->writeFieldBegin("Nlines", ::jsonio17::impex::T_I32, 9);
    xfer += oprot->writeI32(this->Nlines);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldBegin("direction", ::jsonio17::impex::T_I32, 10);
  xfer += oprot->writeI32((int32_t)this->direction);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FormatTableFile &a, FormatTableFile &b) {
  using ::std::swap;
  swap(a.block, b.block);
  swap(a.format, b.format);
  swap(a.renderer, b.renderer);
  swap(a.label, b.label);
  swap(a.separators, b.separators);
  swap(a.comment, b.comment);
  swap(a.fname, b.fname);
  swap(a.Nblocks, b.Nblocks);
  swap(a.Nlines, b.Nlines);
  swap(a.direction, b.direction);
  swap(a.__isset, b.__isset);
}

FormatTableFile::FormatTableFile(const FormatTableFile& other85) {
  block = other85.block;
  format = other85.format;
  renderer = other85.renderer;
  label = other85.label;
  separators = other85.separators;
  comment = other85.comment;
  fname = other85.fname;
  Nblocks = other85.Nblocks;
  Nlines = other85.Nlines;
  direction = other85.direction;
  __isset = other85.__isset;
}
FormatTableFile& FormatTableFile::operator=(const FormatTableFile& other86) {
  block = other86.block;
  format = other86.format;
  renderer = other86.renderer;
  label = other86.label;
  separators = other86.separators;
  comment = other86.comment;
  fname = other86.fname;
  Nblocks = other86.Nblocks;
  Nlines = other86.Nlines;
  direction = other86.direction;
  __isset = other86.__isset;
  return *this;
}
void FormatTableFile::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "FormatTableFile(";
  out << "block=" << to_string(block);
  out << ", " << "format=" << to_string(format);
  out << ", " << "renderer=" << to_string(renderer);
  out << ", " << "label=" << to_string(label);
  out << ", " << "separators=" << to_string(separators);
  out << ", " << "comment="; (__isset.comment ? (out << to_string(comment)) : (out << "<null>"));
  out << ", " << "fname="; (__isset.fname ? (out << to_string(fname)) : (out << "<null>"));
  out << ", " << "Nblocks="; (__isset.Nblocks ? (out << to_string(Nblocks)) : (out << "<null>"));
  out << ", " << "Nlines="; (__isset.Nlines ? (out << to_string(Nlines)) : (out << "<null>"));
  out << ", " << "direction=" << to_string(direction);
  out << ")";
}


FormatStructDataFile::~FormatStructDataFile() noexcept {
}


void FormatStructDataFile::__set_block(const FormatBlock& val) {
  this->block = val;
}

void FormatStructDataFile::__set_renderer(const std::string& val) {
  this->renderer = val;
}

void FormatStructDataFile::__set_label(const std::string& val) {
  this->label = val;
}

void FormatStructDataFile::__set_comment(const std::string& val) {
  this->comment = val;
__isset.comment = true;
}

void FormatStructDataFile::__set_fname(const std::string& val) {
  this->fname = val;
__isset.fname = true;
}

void FormatStructDataFile::__set_Nblocks(const int32_t val) {
  this->Nblocks = val;
__isset.Nblocks = true;
}

void FormatStructDataFile::__set_Nlines(const int32_t val) {
  this->Nlines = val;
__isset.Nlines = true;
}

void FormatStructDataFile::__set_direction(const DirectionType::type val) {
  this->direction = val;
}
std::ostream& operator<<(std::ostream& out, const FormatStructDataFile& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t FormatStructDataFile::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;

  bool isset_block = false;
  bool isset_renderer = false;
  bool isset_label = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->block.read(iprot);
          isset_block = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->renderer);
          isset_renderer = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->label);
          isset_label = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->comment);
          this->__isset.comment = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->fname);
          this->__isset.fname = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nblocks);
          this->__isset.Nblocks = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::jsonio17::impex::T_I32) {
          xfer += iprot->readI32(this->Nlines);
          this->__isset.Nlines = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::jsonio17::impex::T_I32) {
          int32_t ecast87;
          xfer += iprot->readI32(ecast87);
          this->direction = (DirectionType::type)ecast87;
          this->__isset.direction = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_block)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected block");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_renderer)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected renderer");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_label)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected label");
//    throw TProtocolException(TProtocolException::INVALID_DATA);

  return xfer;
}

uint32_t FormatStructDataFile::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FormatStructDataFile");

  xfer += oprot->writeFieldBegin("block", ::jsonio17::impex::T_STRUCT, 1);
  xfer += this->block.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("renderer", ::jsonio17::impex::T_STRING, 2);
  xfer += oprot->writeString(this->renderer);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("label", ::jsonio17::impex::T_STRING, 3);
  xfer += oprot->writeString(this->label);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.comment) {
    xfer += oprot->writeFieldBegin("comment", ::jsonio17::impex::T_STRING, 4);
    xfer += oprot->writeString(this->comment);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.fname) {
    xfer += oprot->writeFieldBegin("fname", ::jsonio17::impex::T_STRING, 5);
    xfer += oprot->writeString(this->fname);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Nblocks) {
    xfer += oprot->writeFieldBegin("Nblocks", ::jsonio17::impex::T_I32, 6);
    xfer += oprot->writeI32(this->Nblocks);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Nlines) {
    xfer += oprot->writeFieldBegin("Nlines", ::jsonio17::impex::T_I32, 7);
    xfer += oprot->writeI32(this->Nlines);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldBegin("direction", ::jsonio17::impex::T_I32, 10);
  xfer += oprot->writeI32((int32_t)this->direction);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FormatStructDataFile &a, FormatStructDataFile &b) {
  using ::std::swap;
  swap(a.block, b.block);
  swap(a.renderer, b.renderer);
  swap(a.label, b.label);
  swap(a.comment, b.comment);
  swap(a.fname, b.fname);
  swap(a.Nblocks, b.Nblocks);
  swap(a.Nlines, b.Nlines);
  swap(a.direction, b.direction);
  swap(a.__isset, b.__isset);
}

FormatStructDataFile::FormatStructDataFile(const FormatStructDataFile& other88) {
  block = other88.block;
  renderer = other88.renderer;
  label = other88.label;
  comment = other88.comment;
  fname = other88.fname;
  Nblocks = other88.Nblocks;
  Nlines = other88.Nlines;
  direction = other88.direction;
  __isset = other88.__isset;
}
FormatStructDataFile& FormatStructDataFile::operator=(const FormatStructDataFile& other89) {
  block = other89.block;
  renderer = other89.renderer;
  label = other89.label;
  comment = other89.comment;
  fname = other89.fname;
  Nblocks = other89.Nblocks;
  Nlines = other89.Nlines;
  direction = other89.direction;
  __isset = other89.__isset;
  return *this;
}
void FormatStructDataFile::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "FormatStructDataFile(";
  out << "block=" << to_string(block);
  out << ", " << "renderer=" << to_string(renderer);
  out << ", " << "label=" << to_string(label);
  out << ", " << "comment="; (__isset.comment ? (out << to_string(comment)) : (out << "<null>"));
  out << ", " << "fname="; (__isset.fname ? (out << to_string(fname)) : (out << "<null>"));
  out << ", " << "Nblocks="; (__isset.Nblocks ? (out << to_string(Nblocks)) : (out << "<null>"));
  out << ", " << "Nlines="; (__isset.Nlines ? (out << to_string(Nlines)) : (out << "<null>"));
  out << ", " << "direction=" << to_string(direction);
  out << ")";
}


FormatImportExportFile::~FormatImportExportFile() noexcept {
}


void FormatImportExportFile::__set_ff_text(const FormatTextFile& val) {
  this->ff_text = val;
__isset.ff_text = true;
}

void FormatImportExportFile::__set_ff_table(const FormatTableFile& val) {
  this->ff_table = val;
__isset.ff_table = true;
}

void FormatImportExportFile::__set_ff_keyvalue(const FormatKeyValueFile& val) {
  this->ff_keyvalue = val;
__isset.ff_keyvalue = true;
}

void FormatImportExportFile::__set_ff_stdata(const FormatStructDataFile& val) {
  this->ff_stdata = val;
__isset.ff_stdata = true;
}
std::ostream& operator<<(std::ostream& out, const FormatImportExportFile& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t FormatImportExportFile::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->ff_text.read(iprot);
          this->__isset.ff_text = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->ff_table.read(iprot);
          this->__isset.ff_table = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->ff_keyvalue.read(iprot);
          this->__isset.ff_keyvalue = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_STRUCT) {
          xfer += this->ff_stdata.read(iprot);
          this->__isset.ff_stdata = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t FormatImportExportFile::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FormatImportExportFile");

  if (this->__isset.ff_text) {
    xfer += oprot->writeFieldBegin("ff_text", ::jsonio17::impex::T_STRUCT, 1);
    xfer += this->ff_text.write(oprot);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.ff_table) {
    xfer += oprot->writeFieldBegin("ff_table", ::jsonio17::impex::T_STRUCT, 2);
    xfer += this->ff_table.write(oprot);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.ff_keyvalue) {
    xfer += oprot->writeFieldBegin("ff_keyvalue", ::jsonio17::impex::T_STRUCT, 3);
    xfer += this->ff_keyvalue.write(oprot);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.ff_stdata) {
    xfer += oprot->writeFieldBegin("ff_stdata", ::jsonio17::impex::T_STRUCT, 4);
    xfer += this->ff_stdata.write(oprot);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FormatImportExportFile &a, FormatImportExportFile &b) {
  using ::std::swap;
  swap(a.ff_text, b.ff_text);
  swap(a.ff_table, b.ff_table);
  swap(a.ff_keyvalue, b.ff_keyvalue);
  swap(a.ff_stdata, b.ff_stdata);
  swap(a.__isset, b.__isset);
}

FormatImportExportFile::FormatImportExportFile(const FormatImportExportFile& other90) {
  ff_text = other90.ff_text;
  ff_table = other90.ff_table;
  ff_keyvalue = other90.ff_keyvalue;
  ff_stdata = other90.ff_stdata;
  __isset = other90.__isset;
}
FormatImportExportFile& FormatImportExportFile::operator=(const FormatImportExportFile& other91) {
  ff_text = other91.ff_text;
  ff_table = other91.ff_table;
  ff_keyvalue = other91.ff_keyvalue;
  ff_stdata = other91.ff_stdata;
  __isset = other91.__isset;
  return *this;
}
void FormatImportExportFile::printTo(std::ostream& out) const {
  //using ::apache::thrift::to_string;
  out << "FormatImportExportFile(";
  out << "ff_text="; (__isset.ff_text ? (out << to_string(ff_text)) : (out << "<null>"));
  out << ", " << "ff_table="; (__isset.ff_table ? (out << to_string(ff_table)) : (out << "<null>"));
  out << ", " << "ff_keyvalue="; (__isset.ff_keyvalue ? (out << to_string(ff_keyvalue)) : (out << "<null>"));
  out << ", " << "ff_stdata="; (__isset.ff_stdata ? (out << to_string(ff_stdata)) : (out << "<null>"));
  out << ")";
}


ImpexFormat::~ImpexFormat() noexcept {
}


void ImpexFormat::__set__id(const std::string& val) {
  this->_id = val;
}

void ImpexFormat::__set__key(const std::string& val) {
  this->_key = val;
}

void ImpexFormat::__set__rev(const std::string& val) {
  this->_rev = val;
}

void ImpexFormat::__set_name(const std::string& val) {
  this->name = val;
}

void ImpexFormat::__set_impexschema(const std::string& val) {
  this->impexschema = val;
}

void ImpexFormat::__set_schema(const std::string& val) {
  this->schema = val;
}

void ImpexFormat::__set_comment(const std::string& val) {
  this->comment = val;
}

void ImpexFormat::__set_direction(const DirectionType::type val) {
  this->direction = val;
}

void ImpexFormat::__set_impex(const std::string& val) {
  this->impex = val;
}

void ImpexFormat::__set_format(const std::string& val) {
  this->format = val;
__isset.format = true;
}

void ImpexFormat::__set_extension(const std::string& val) {
  this->extension = val;
__isset.extension = true;
}
std::ostream& operator<<(std::ostream& out, const ImpexFormat& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t ImpexFormat::read(::jsonio17::TJSONSerializer* iprot) {

  //::jsonio17::impex::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::jsonio17::impex::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  //using ::jsonio17::TJSONSerializerException;

  bool isset__id = false;
  bool isset__key = false;
  bool isset__rev = false;
  bool isset_name = false;
  bool isset_impexschema = false;
  bool isset_impex = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::jsonio17::impex::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->_id);
          isset__id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->_key);
          isset__key = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->_rev);
          isset__rev = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->name);
          isset_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->impexschema);
          isset_impexschema = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->schema);
          this->__isset.schema = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->comment);
          this->__isset.comment = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::jsonio17::impex::T_I32) {
          int32_t ecast92;
          xfer += iprot->readI32(ecast92);
          this->direction = (DirectionType::type)ecast92;
          this->__isset.direction = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->impex);
          isset_impex = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->format);
          this->__isset.format = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 11:
        if (ftype == ::jsonio17::impex::T_STRING) {
          xfer += iprot->readString(this->extension);
          this->__isset.extension = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset__id)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected _id");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset__key)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected _key");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset__rev)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected _rev");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_name)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected name");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_impexschema)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected impexschema");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_impex)
      jsonio17::JSONIO_THROW( "TJSONSerializer", 101, " expected impex");
//    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t ImpexFormat::write(::jsonio17::TJSONSerializer* oprot) const {
  uint32_t xfer = 0;
  //::jsonio17::impex::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("ImpexFormat");

  xfer += oprot->writeFieldBegin("_id", ::jsonio17::impex::T_STRING, 1);
  xfer += oprot->writeString(this->_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("_key", ::jsonio17::impex::T_STRING, 2);
  xfer += oprot->writeString(this->_key);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("_rev", ::jsonio17::impex::T_STRING, 3);
  xfer += oprot->writeString(this->_rev);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("name", ::jsonio17::impex::T_STRING, 4);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("impexschema", ::jsonio17::impex::T_STRING, 5);
  xfer += oprot->writeString(this->impexschema);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("schema", ::jsonio17::impex::T_STRING, 6);
  xfer += oprot->writeString(this->schema);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("comment", ::jsonio17::impex::T_STRING, 7);
  xfer += oprot->writeString(this->comment);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("direction", ::jsonio17::impex::T_I32, 8);
  xfer += oprot->writeI32((int32_t)this->direction);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("impex", ::jsonio17::impex::T_STRING, 9);
  xfer += oprot->writeString(this->impex);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.format) {
    xfer += oprot->writeFieldBegin("format", ::jsonio17::impex::T_STRING, 10);
    xfer += oprot->writeString(this->format);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.extension) {
    xfer += oprot->writeFieldBegin("extension", ::jsonio17::impex::T_STRING, 11);
    xfer += oprot->writeString(this->extension);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ImpexFormat &a, ImpexFormat &b) {
  using ::std::swap;
  swap(a._id, b._id);
  swap(a._key, b._key);
  swap(a._rev, b._rev);
  swap(a.name, b.name);
  swap(a.impexschema, b.impexschema);
  swap(a.schema, b.schema);
  swap(a.comment, b.comment);
  swap(a.direction, b.direction);
  swap(a.impex, b.impex);
  swap(a.format, b.format);
  swap(a.extension, b.extension);
  swap(a.__isset, b.__isset);
}

ImpexFormat::ImpexFormat(const ImpexFormat& other93) {
  _id = other93._id;
  _key = other93._key;
  _rev = other93._rev;
  name = other93.name;
  impexschema = other93.impexschema;
  schema = other93.schema;
  comment = other93.comment;
  direction = other93.direction;
  impex = other93.impex;
  format = other93.format;
  extension = other93.extension;
  __isset = other93.__isset;
}
ImpexFormat& ImpexFormat::operator=(const ImpexFormat& other94) {
  _id = other94._id;
  _key = other94._key;
  _rev = other94._rev;
  name = other94.name;
  impexschema = other94.impexschema;
  schema = other94.schema;
  comment = other94.comment;
  direction = other94.direction;
  impex = other94.impex;
  format = other94.format;
  extension = other94.extension;
  __isset = other94.__isset;
  return *this;
}
void ImpexFormat::printTo(std::ostream& out) const {
  out << "ImpexFormat(";
  out << "_id=" << to_string(_id);
  out << ", " << "_key=" << to_string(_key);
  out << ", " << "_rev=" << to_string(_rev);
  out << ", " << "name=" << to_string(name);
  out << ", " << "impexschema=" << to_string(impexschema);
  out << ", " << "schema=" << to_string(schema);
  out << ", " << "comment=" << to_string(comment);
  out << ", " << "direction=" << to_string(direction);
  out << ", " << "impex=" << to_string(impex);
  out << ", " << "format="; (__isset.format ? (out << to_string(format)) : (out << "<null>"));
  out << ", " << "extension="; (__isset.extension ? (out << to_string(extension)) : (out << "<null>"));
  out << ")";
}

} // namespac

