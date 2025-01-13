### Dependent

- arango-cpp
- jsonio

## Files descriptions

### Yaml XML parsing && imort/export ###

1. lua-run.h (cpp)

LUA-C++ Integration Class.
Internal scripts used into the import/export process.

> _source: "+";   test: "+";  example: "internal"_

2. yamldump.h (cpp)

Serialize/deserialize object to Yaml format data.

```c++

   auto object = JsonFree::object();
   yaml::loads( yaml_str,  object );
   auto yaml_back = yaml::dump( object );

```

> _source: "+";   test: "+";  example: "+"_

3. xmldump.h (cpp)

Serialize/deserialize object to XML format data.

```c++

   auto object = JsonFree::object();
   XML::loads( xml_str,  object );
   auto xml_back = XML::dump( object );

```

> _source: "+";   test: "+";  example: "+"_
>  Some problems with {}, [], " " and null

4. yaml_xml2file.h (cpp)

Class JsonYamlXMLFile for read/write json/yaml/xml files.
Class JsonYamlXMLArrayFile  for read/write json/yaml/xml arrays files.


> _source: "+";    test: "+";  example: "-"_

5. dbdriver_thrift.h (cpp)

Implementation of Database Driver using the Thrift server.
> Need to refresh Query structure and Server functions for changed API.
> For test would be using Python ArangoDB server from jsonArango library.
> Python server must be changed to new API too.

> _source: "+";    test: "-";  example: "-"_

6. TJSONSerializer.h (cpp)

Class TJSONSerializer json protocol for Thrift.
Implements a protocol which uses JSON object as the wire/read-format.

> _source: "+";    test: "+";  example: "internal"_

7. thrift_impex.h (cpp)

Class ImpexFormatFile - top level implementation of import/export to/from foreign format files.
Prepared an "impex.thrift" file describing 3 file formats for import/export of data
to internal store schema-based JSON object.

> _source: "+";    test: "-";  example: "-"_

8. thrift_ie_base.h (cpp)

Class AbstractIEFile - love level abstract interface of import/export to/from foreign format files.

> _source: "+";    test: "-";  example: "-"_
> Need tests and example

9. thrift_ie_json.h (cpp)

Class StructDataIEFile - export/import of data records to/from text file in legacy JSON/YAML/XML (foreign keywords).

```c++

/** Definition of foreign structured data JSON/YAML/XML text file */
struct FormatStructDataFile {
   /** Format for one or more blocks for data records */
   1: required FormatBlock block
   /** Rendering syntax for the foreign file "JSON" | "YAML" | "XML" | ... */
   2: required string renderer = "JSON"
   /** Label of data type (vertex type), e.g. "datasource", "element" ... */
   3: required string label
   /** Definition of value, line, row, block, comment, end-of-data separators */
   4: optional string comment
   /** File name or "console" for export */
   5: optional string fname
   /** number of data blocks (records) >=1, 0 if unknown */
   6: optional i32 Nblocks
   /** total number of text lines in the file, 0 if unknown */
   7: optional i32 Nlines
   /** direction */
   10: DirectionType direction
}

```

> _source: "+";    test: "-";  example: "internal"_
> Need tests and example

10. thrift_ie_keyvalue.h (cpp)

Class KeyValueIEFile - export/import of data records to/from text file with key-value pair data.

```c++

/** Definition of text file with key-value pair data */
struct FormatKeyValueFile {
   /** Format for one or more blocks for data records */
   1: required FormatBlock block
   /** Definition of key-value block in file */
   2: required  FormatKeyValue format
   /** Rendering syntax for the foreign key-value file "GEMS3K" | "BIB" | "RIS" | ... */
   3: required string renderer
   /** Label of data type (vertex type), e.g. "datasource", "element" ... */
   4: required string label
   /** Definition of value, line, row, block, comment, end-of-data separators */
   5: Separators separators
   /** Export: the whole comment text; Import: the comment begin markup string (to skip until endl) */
   6: optional string comment
   /** File name or "console" for export */
   7: optional string fname
   /** number of data blocks (records) >=1, 0 if unknown */
   8: optional i32 Nblocks
   /** total number of text lines in the file, 0 if unknown */
   9: optional i32 Nlines
   /** direction */
   10: DirectionType direction
}

```


> _source: "+";    test: "-";  example: "internal"_
> Need tests and example

11. thrift_ie_table.h (cpp)

Class TableIEFile - export / import data records to / from a text file with table data.

```c++

/** Definition of table text file format */
struct FormatTableFile {
   /** Format for one or more blocks for data records */
   1: required FormatBlock block
   /** Definition of key-value block in file */
   2: required  FormatTable format
   /** Rendering syntax for the foreign key-value file "GEMS3K" | "BIB" | "RIS" | ... */
   3: required string renderer
   /** Label of data type (vertex type), e.g. "datasource", "element" ... */
   4: required string label
   /** Definition of value, line, row, block, comment, end-of-data separators */
   5: Separators separators
   /** Export: the whole comment text; Import: the comment begin markup string (to skip until endl) */
   6: optional string comment
   /** File name or "console" for export */
   7: optional string fname
   /** number of data blocks (records) >=1, 0 if unknown */
   8: optional i32 Nblocks
   /** total number of text lines in the file, 0 if unknown */
   9: optional i32 Nlines
   /** direction */
   10: DirectionType direction
}

```

> _source: "+";    test: "-";  example: "internal"_
> Need tests and example


## Next steps --------------------------------------------------------------------------------

To do:

- Implement examples and tests for 7-11

- Changed for new API thrift server functions and PythonArango server

