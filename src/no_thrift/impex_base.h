#pragma once

#include <memory>

namespace jsonio {

class TJSONSerializer;

namespace impex {

/**
 * Enumerated definition of the types that the Thrift protocol supports.
 * Take special note of the T_END type which is used specifically to mark
 * the end of a sequence of fields.
 */
enum TType {
  T_STOP       = 0,
  T_VOID       = 1,
  T_BOOL       = 2,
  T_BYTE       = 3,
  T_I08        = 3,
  T_I16        = 6,
  T_I32        = 8,
  T_U64        = 9,
  T_I64        = 10,
  T_DOUBLE     = 4,
  T_STRING     = 11,
  T_UTF7       = 11,
  T_STRUCT     = 12,
  T_MAP        = 13,
  T_SET        = 14,
  T_LIST       = 15,
  T_UTF8       = 16,
  T_UTF16      = 17
};

/**
 * Enumerated definition of the message types that the Thrift protocol
 * supports.
 */
enum TMessageType {
  T_CALL       = 1,
  T_REPLY      = 2,
  T_EXCEPTION  = 3,
  T_ONEWAY     = 4
};



class TBase {
public:
  virtual ~TBase() = default;
  virtual uint32_t read(TJSONSerializer* iprot) = 0;
  virtual uint32_t write(TJSONSerializer* oprot) const = 0;
};

}

} // jsonio


