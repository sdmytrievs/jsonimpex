### Dependent

- jsonArango
- jsonio14

## Files descriptions

### Yaml XML parsing && imort/export ###

1. lua-run.h (cpp)

LUA-C++ Integration Class.
Internal scripts used into the import/export process.

> _source: "+";    test: "-";  example: "-"_
> Need tests and example

2. yamldump.h (cpp)

Serialize/deserialize object to Yaml format data.

```c++

   auto object = JsonFree::object();
   yaml::loads( yaml_str,  object );
   auto yaml_back = yaml::dump( object );

```

> _source: "+";    test: "-";  example: "-"_
> Need tests and example

3. xmldump.h (cpp)

Serialize/deserialize object to XML format data.

```c++

   auto object = JsonFree::object();
   XML::loads( xml_str,  object );
   auto xml_back = XML::dump( object );

```

> _source: "+";    test: "-";  example: "-"_
> Need tests and example

4. yaml_xml2file.h (cpp)

Class JsonYamlXMLFile for read/write json/yaml/xml files.
Class JsonYamlXMLArrayFile  for read/write json/yaml/xml arrays files.


> _source: "+";    test: "-";  example: "-"_
> Need tests and example

5. dbdriver_thrift.h (cpp)

Implementation of Database Driver using the Thrift server.
> Need to refresh Query structure and Server functions for changed API.
> For test would be using Python ArangoDB server from jsonArango library.
> Python server must be changed to new API too.

> _source: "+";    test: "-";  example: "-"_
> Need tests and example


## Next steps --------------------------------------------------------------------------------

To do:

- Implement examples and tests for 1-5
- Changed for new API thrift server functions and PythonArango server
- Change implementation import/export from jsonimpex
