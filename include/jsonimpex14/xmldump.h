#pragma once

#include "jsonio14/jsonbase.h"

namespace jsonio14 {

class JsonFree;
class JsonSchema;

namespace XML {

/// @brief Dump object to XML string.
std::string dump( const JsonBase& object );

/// Serialize object as a XML formatted stream.
void dump( std::ostream& os, const JsonBase& object );

/// Deserialize a XML string to a object.
void loads( const std::string& xml_str, JsonBase& object );

/// Deserialize a XML document to a free format json object.
JsonFree loads( const std::string& xml_str );
/// Deserialize a XML document to a schema define json object.
JsonSchema loads( const std::string& aschema_name, const std::string& xml_str );


} // namespace XML

} // namespace jsonio14

