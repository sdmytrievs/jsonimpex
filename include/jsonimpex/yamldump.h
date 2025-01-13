#pragma once

#include "jsonio/jsonbase.h"

namespace jsonio {

class JsonFree;
class JsonSchema;

namespace yaml {

/// @brief Dump object to YAML string.
std::string dump( const JsonBase& object );

/// Serialize object as a YAML formatted stream.
void dump( std::ostream& os, const JsonBase& object );

/// Deserialize a YAML string to a object.
void loads( const std::string& yaml_str, JsonBase& object );

/// Deserialize a YAML document to a free format json object.
JsonFree loads( const std::string& yaml_str );
/// Deserialize a YAML document to a schema define json object.
JsonSchema loads( const std::string& aschema_name, const std::string& yaml_str );

/// Parse YAML string to json string
std::string Yaml2Json( const std::string& yaml_str );

} // namespace yaml

} // namespace jsonio

