#pragma once

const char* const impex_schema_str = R"({
                                     "name": "impex",
                                     "doc": "Apache Thrift IDL definition for text import-export interfaces\n",
                                     "namespaces": {
                                       "*": "impex"
                                     },
                                     "includes": [
                                     ],
                                     "enums": [
                                       {
                                         "name": "DirectionType",
                                         "doc": "Classes of direction types\n",
                                         "members": [
                                           {
                                             "name": "IMPORT",
                                             "value": 0,
                                             "doc": "import\n"
                                           },
                                           {
                                             "name": "EXPORT",
                                             "value": 1,
                                             "doc": "export\n"
                                           }
                                         ]
                                       }
                                     ],
                                     "typedefs": [
                                     ],
                                     "structs": [
                                       {
                                         "name": "FormatValue",
                                         "doc": "Definition of the data value format in imported\/exported file\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "format",
                                             "typeId": "string",
                                             "doc": "Format scanf\/printf (to string first): \"%s\" | \"in\" | \"out\" | \"endl\" | \"txel\" | \"txkw\"; \"in\" | \"out\" | \"endl\" for stream input\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 2,
                                             "name": "factor",
                                             "typeId": "double",
                                             "doc": "Factor != 0, default 1; Each num.value is multiplied (import) or divided (export) by factor\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 3,
                                             "name": "increment",
                                             "typeId": "double",
                                             "doc": "Increment, default 0; added to each numerical value (import) or subtracted from (export)\n",
                                             "required": "optional"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "FormatKeyword",
                                         "doc": "Format to read\/print keywords in key-value pair file\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "format",
                                             "typeId": "string",
                                             "doc": "scanf\/printf format for keyword\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 2,
                                             "name": "delim_begin",
                                             "typeId": "string",
                                             "doc": "delimiter for keyword begin e.g. \"\\\"\" | \"<\" | \"\"\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 3,
                                             "name": "delim_end",
                                             "typeId": "string",
                                             "doc": "delimiter for keyword end e.g. \"\\\"\" | \">\" | \"\"\n",
                                             "required": "optional"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "DataType",
                                         "doc": "Type of object from the imported or exported file (for use in keyword lookup list or map)\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "datatype",
                                             "typeId": "string",
                                             "doc": "Basis type \"string\" | \"float\" | \"double\" | \"i16\" | \"i32\" | \"bool\" | ...\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 2,
                                             "name": "organization",
                                             "typeId": "string",
                                             "doc": "Organization: \"\" | \"list\" | \"set\" | \"map\" | \"group\" | \"embedded\" | ...\n",
                                             "required": "req_out"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "DataObject",
                                         "doc": "Thrift key of data object \"8\" or \"3.8\" or \"2.3.8\" or \"\" to ignore (import); any string not starting from a digit as comment (export)\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "field",
                                             "typeId": "string",
                                             "doc": "Either Thrift key or name of the data field in recursive form (s e.g. \"4.3.1\" or name1.name2 )\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 2,
                                             "name": "ignore",
                                             "typeId": "bool",
                                             "doc": "Set to true if the corresponding value in file has to be ignored (default: false)\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 3,
                                             "name": "script",
                                             "typeId": "string",
                                             "doc": "Default \"\" or contains lua script for operation on data values in block\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 4,
                                             "name": "convert",
                                             "typeId": "map",
                                             "type": {
                                               "typeId": "map",
                                               "keyTypeId": "string",
                                               "valueTypeId": "string"
                                             },
                                             "doc": "Default empty or contains pair(s) read_value : saved_value e.g. \"e\": \"4\" (usually for setting enum values)\n",
                                             "required": "optional"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "Separators",
                                         "doc": "Definition of value, line, row, block, comment, end-of-data separators\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "v_sep",
                                             "typeId": "string",
                                             "doc": "Value separator (for arrays) \" \" | \",\" | \"\\t\" | \"integer\" (=fixed field width)\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 2,
                                             "name": "l_sep",
                                             "typeId": "string",
                                             "doc": "Line separator \"\\n\" ...\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 3,
                                             "name": "r_sep",
                                             "typeId": "string",
                                             "doc": "Row separator (table), \"\\n\" ...\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 4,
                                             "name": "c_head",
                                             "typeId": "string",
                                             "doc": "Head comment separator  e.g. \"#\" or '%'\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 5,
                                             "name": "c_end",
                                             "typeId": "string",
                                             "doc": "End comment separator e.g. \"\\n\"\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 6,
                                             "name": "eod",
                                             "typeId": "string",
                                             "doc": "string indicating end of data (as list of blocks) in file or \"\" as default ']' (end of file)\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 7,
                                             "name": "encoding",
                                             "typeId": "string",
                                             "doc": "encoding (\"\" for standard system encoding)\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 8,
                                             "name": "str_delim",
                                             "typeId": "string",
                                             "doc": "Delimiter for strings - default \"\\\"\"\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 9,
                                             "name": "bod",
                                             "typeId": "string",
                                             "doc": "string indicating begin of data (as list of blocks) in file or \"\" as default '['\n",
                                             "required": "optional"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "FormatBlock",
                                         "doc": "Text block format in file corresponding to one database document (record)\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "defaults",
                                             "typeId": "map",
                                             "type": {
                                               "typeId": "map",
                                               "keyTypeId": "string",
                                               "valueTypeId": "string"
                                             },
                                             "doc": "Default Key, Value pairs to DOM (import)  or to output (export)\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 2,
                                             "name": "pairs",
                                             "typeId": "map",
                                             "type": {
                                               "typeId": "map",
                                               "keyTypeId": "string",
                                               "valueTypeId": "struct",
                                               "valueType": {
                                                 "typeId": "struct",
                                                 "class": "DataType"
                                               }
                                             },
                                             "doc": "Lookup map of keyword-value pair format\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 3,
                                             "name": "matches",
                                             "typeId": "map",
                                             "type": {
                                               "typeId": "map",
                                               "keyTypeId": "string",
                                               "valueTypeId": "struct",
                                               "valueType": {
                                                 "typeId": "struct",
                                                 "class": "DataObject"
                                               }
                                             },
                                             "doc": ">=1 keywd, DataObject pairs connecting the block of data in file with DOM.\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 4,
                                             "name": "script",
                                             "typeId": "string",
                                             "doc": "Default \"\" or contains lua script for operation on data values in full DOM\n",
                                             "required": "optional"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "FormatTextFile",
                                         "doc": "Definition of text data file format\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "block",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "FormatBlock"
                                             },
                                             "doc": "Format definition for one or more blocks for data records - default 1 block\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 2,
                                             "name": "lines",
                                             "typeId": "list",
                                             "type": {
                                               "typeId": "list",
                                               "elemTypeId": "string"
                                             },
                                             "doc": "Will be format lines list\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 3,
                                             "name": "label",
                                             "typeId": "string",
                                             "doc": "Label of data type (vertex type), e.g. \"datasource\", \"element\" ...\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 4,
                                             "name": "separs",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "Separators"
                                             },
                                             "doc": "Definition of value, line, row, block, comment, end-of-data separators\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 5,
                                             "name": "comment",
                                             "typeId": "string",
                                             "doc": "Export: the whole comment text; Import: the comment begin markup string (to skip until endl)\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 6,
                                             "name": "file_name",
                                             "typeId": "string",
                                             "doc": "File name or \"console\" for export\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 7,
                                             "name": "Nblocks",
                                             "typeId": "i32",
                                             "doc": "number of data block in file >=1, 0 if unknown\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 8,
                                             "name": "Nlines",
                                             "typeId": "i32",
                                             "doc": "number of text lines in file (>=1), 0 if unknown\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 9,
                                             "name": "Nchars",
                                             "typeId": "i32",
                                             "doc": "total number of characters in file, 0 if unknown\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 10,
                                             "name": "direction",
                                             "typeId": "i32",
                                             "doc": "direction\n",
                                             "class": "DirectionType",
                                             "required": "req_out"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "FormatKeyValue",
                                         "doc": "Definition of key-value import\/export format\nWe use Regular Expression in case of import and Print Format in case of export.\nUse names \"head0\", ..., \"headN\", \"end0\", ..., \"endN\" to import\/export data from\/to head and end part\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "head_regexp",
                                             "typeId": "string",
                                             "doc": "Head of Block:  \"\\\\s*([^\\\\s]+)\\\\s*;\\\\s*(([\\\\w\\\\t \\\\+\\\\-\\\\(\\\\):\\\\.]+)\\\\s*=\\\\s*([^;]+))\" (import)\nor  \"\\n%head0\\n\\t%head1\\n\" (export)\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 2,
                                             "name": "end_regexp",
                                             "typeId": "string",
                                             "doc": "End of Block: \"([^\\n]+)\" (import)  or  \"%end0\\n\" (export)\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 3,
                                             "name": "key_regexp",
                                             "typeId": "string",
                                             "doc": "Keyword:  \"\\\\s*;\\\\s*\\\\-{0,1}([a-zA-Z]\\\\w*)\\\\s*\" (import)  or  \"\\t-%key\\t\" (export)\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 4,
                                             "name": "value_regexp",
                                             "typeId": "string",
                                             "doc": "Data Value(s):  \"\\\\s*([^#\\\\n;]*)\" (import)  or  \"%value\" (export)\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 5,
                                             "name": "value_next",
                                             "typeId": "string",
                                             "doc": "Key-Value pair end delimiter (used if empty value_regexp or export mode )\n",
                                             "required": "req_out",
                                             "default": "\\n"
                                           },
                                           {
                                             "key": 6,
                                             "name": "value_token_regexp",
                                             "typeId": "string",
                                             "doc": "Regular Expression to iterate over matches  (used to convert value to string list or if export mode )\n",
                                             "required": "optional",
                                             "default": " "
                                           },
                                           {
                                             "key": 7,
                                             "name": "comment_regexp",
                                             "typeId": "string",
                                             "doc": "Regular Expression for skip comments\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 8,
                                             "name": "Ndata",
                                             "typeId": "i32",
                                             "doc": "number of data items per block (0 if not set)\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 9,
                                             "name": "strvalue_exp",
                                             "typeId": "string",
                                             "doc": "Data String Value(s): only for export  \"\\'%value\\'\"\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 10,
                                             "name": "key_order",
                                             "typeId": "list",
                                             "type": {
                                               "typeId": "list",
                                               "elemTypeId": "string"
                                             },
                                             "doc": "Keyword order list:  only for export\n",
                                             "required": "optional"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "FormatKeyValueFile",
                                         "doc": "Definition of text file with key-value pair data\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "block",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "FormatBlock"
                                             },
                                             "doc": "Format for one or more blocks for data records\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 2,
                                             "name": "format",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "FormatKeyValue"
                                             },
                                             "doc": "Definition of key-value block in file\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 3,
                                             "name": "renderer",
                                             "typeId": "string",
                                             "doc": "Rendering syntax for the foreign key-value file \"GEMS3K\" | \"BIB\" | \"RIS\" | ...\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 4,
                                             "name": "label",
                                             "typeId": "string",
                                             "doc": "Label of data type (vertex type), e.g. \"datasource\", \"element\" ...\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 5,
                                             "name": "separators",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "Separators"
                                             },
                                             "doc": "Definition of value, line, row, block, comment, end-of-data separators\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 6,
                                             "name": "comment",
                                             "typeId": "string",
                                             "doc": "Export: the whole comment text; Import: the comment begin markup string (to skip until endl)\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 7,
                                             "name": "fname",
                                             "typeId": "string",
                                             "doc": "File name or \"console\" for export\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 8,
                                             "name": "Nblocks",
                                             "typeId": "i32",
                                             "doc": "number of data blocks (records) >=1, 0 if unknown\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 9,
                                             "name": "Nlines",
                                             "typeId": "i32",
                                             "doc": "total number of text lines in the file, 0 if unknown\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 10,
                                             "name": "direction",
                                             "typeId": "i32",
                                             "doc": "direction\n",
                                             "class": "DirectionType",
                                             "required": "req_out"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "FormatTable",
                                         "doc": "Definition of of the table input format\nif defined  colsizes, split by sizes\nif defined  value_regexp, split used regexpr\notherwise split by  columns delimiter\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "Nhcols",
                                             "typeId": "i32",
                                             "doc": "Number of header columns\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 2,
                                             "name": "Nhrows",
                                             "typeId": "i32",
                                             "doc": "Number of header rows (start porting from row )\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 3,
                                             "name": "headers",
                                             "typeId": "list",
                                             "type": {
                                               "typeId": "list",
                                               "elemTypeId": "string"
                                             },
                                             "doc": "Names of header columns\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 4,
                                             "name": "rowend",
                                             "typeId": "string",
                                             "doc": "Row delimiter\n",
                                             "required": "req_out",
                                             "default": "\\n"
                                           },
                                           {
                                             "key": 5,
                                             "name": "rows_one_block",
                                             "typeId": "i32",
                                             "doc": "Number of lines in one block (could be more than one)\n",
                                             "required": "req_out",
                                             "default": 1
                                           },
                                           {
                                             "key": 6,
                                             "name": "row_header_regexp",
                                             "typeId": "string",
                                             "doc": "Regular expression:  next block head in case the number of lines in one block is not fixed\n",
                                             "required": "req_out",
                                             "default": ""
                                           },
                                           {
                                             "key": 7,
                                             "name": "colends",
                                             "typeId": "string",
                                             "doc": "Columns delimiters\n",
                                             "required": "req_out",
                                             "default": "\\t "
                                           },
                                           {
                                             "key": 8,
                                             "name": "usequotes",
                                             "typeId": "bool",
                                             "doc": "Quoted field as text\n",
                                             "required": "req_out",
                                             "default": 1
                                           },
                                           {
                                             "key": 9,
                                             "name": "usemore",
                                             "typeId": "bool",
                                             "doc": "Can be more than one delimiter between columns\n",
                                             "required": "req_out",
                                             "default": 0
                                           },
                                           {
                                             "key": 10,
                                             "name": "comment_regexp",
                                             "typeId": "string",
                                             "doc": "Regular Expression for skip comments\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 11,
                                             "name": "row_size",
                                             "typeId": "i32",
                                             "doc": "Row size in case of using fixed colsizes\n",
                                             "required": "optional",
                                             "default": 0
                                           },
                                           {
                                             "key": 12,
                                             "name": "colsizes",
                                             "typeId": "list",
                                             "type": {
                                               "typeId": "list",
                                               "elemTypeId": "i32"
                                             },
                                             "doc": "Fixed size of columns for importing ( apply to all if one item )\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 13,
                                             "name": "value_regexp",
                                             "typeId": "list",
                                             "type": {
                                               "typeId": "list",
                                               "elemTypeId": "string"
                                             },
                                             "doc": "Regular Expression for column value(s) ( apply to all if one item )\n",
                                             "required": "optional"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "FormatTableFile",
                                         "doc": "Definition of table text file format\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "block",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "FormatBlock"
                                             },
                                             "doc": "Format for one or more blocks for data records\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 2,
                                             "name": "format",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "FormatTable"
                                             },
                                             "doc": "Definition of key-value block in file\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 3,
                                             "name": "renderer",
                                             "typeId": "string",
                                             "doc": "Rendering syntax for the foreign key-value file \"GEMS3K\" | \"BIB\" | \"RIS\" | ...\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 4,
                                             "name": "label",
                                             "typeId": "string",
                                             "doc": "Label of data type (vertex type), e.g. \"datasource\", \"element\" ...\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 5,
                                             "name": "separators",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "Separators"
                                             },
                                             "doc": "Definition of value, line, row, block, comment, end-of-data separators\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 6,
                                             "name": "comment",
                                             "typeId": "string",
                                             "doc": "Export: the whole comment text; Import: the comment begin markup string (to skip until endl)\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 7,
                                             "name": "fname",
                                             "typeId": "string",
                                             "doc": "File name or \"console\" for export\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 8,
                                             "name": "Nblocks",
                                             "typeId": "i32",
                                             "doc": "number of data blocks (records) >=1, 0 if unknown\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 9,
                                             "name": "Nlines",
                                             "typeId": "i32",
                                             "doc": "total number of text lines in the file, 0 if unknown\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 10,
                                             "name": "direction",
                                             "typeId": "i32",
                                             "doc": "direction\n",
                                             "class": "DirectionType",
                                             "required": "req_out"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "FormatStructDataFile",
                                         "doc": "Definition of foreign structured data JSON\/YAML\/XML text file\n",
                                         "isException": false,
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "block",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "FormatBlock"
                                             },
                                             "doc": "Format for one or more blocks for data records\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 2,
                                             "name": "renderer",
                                             "typeId": "string",
                                             "doc": "Rendering syntax for the foreign file \"JSON\" | \"YAML\" | \"XML\" | ...\n",
                                             "required": "required",
                                             "default": "JSON"
                                           },
                                           {
                                             "key": 3,
                                             "name": "label",
                                             "typeId": "string",
                                             "doc": "Label of data type (vertex type), e.g. \"datasource\", \"element\" ...\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 4,
                                             "name": "comment",
                                             "typeId": "string",
                                             "doc": "Definition of value, line, row, block, comment, end-of-data separators\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 5,
                                             "name": "fname",
                                             "typeId": "string",
                                             "doc": "File name or \"console\" for export\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 6,
                                             "name": "Nblocks",
                                             "typeId": "i32",
                                             "doc": "number of data blocks (records) >=1, 0 if unknown\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 7,
                                             "name": "Nlines",
                                             "typeId": "i32",
                                             "doc": "total number of text lines in the file, 0 if unknown\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 10,
                                             "name": "direction",
                                             "typeId": "i32",
                                             "doc": "direction\n",
                                             "class": "DirectionType",
                                             "required": "req_out"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "FormatImportExportFile",
                                         "doc": "Generalized import-export data file format\n",
                                         "isException": false,
                                         "isUnion": true,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "ff_text",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "FormatTextFile"
                                             },
                                             "doc": "Definition of text data file format\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 2,
                                             "name": "ff_table",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "FormatTableFile"
                                             },
                                             "doc": "Definition of data table file format\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 3,
                                             "name": "ff_keyvalue",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "FormatKeyValueFile"
                                             },
                                             "doc": "Definition of file format with key-value pair data\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 4,
                                             "name": "ff_stdata",
                                             "typeId": "struct",
                                             "type": {
                                               "typeId": "struct",
                                               "class": "FormatStructDataFile"
                                             },
                                             "doc": "Definition of foreign structured data JSON\/YAML\/XML file format\n",
                                             "required": "optional"
                                           }
                                         ]
                                       },
                                       {
                                         "name": "ImpexFormat",
                                         "doc": "description of import\/export format record\n",
                                         "isException": false,
                                         "to_select": [ "4", "5", "6", "7" ],
                                         "to_key": [ "6", "8", "4" ],
                                         "isUnion": false,
                                         "fields": [
                                           {
                                             "key": 1,
                                             "name": "_id",
                                             "typeId": "string",
                                             "doc": "Handle (id) of this record or 0 if unknown\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 2,
                                             "name": "_key",
                                             "typeId": "string",
                                             "doc": "ID of this record within the impex collection (part of _id)\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 3,
                                             "name": "_rev",
                                             "typeId": "string",
                                             "doc": "Code of revision (changed by the system at every update)\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 4,
                                             "name": "name",
                                             "typeId": "string",
                                             "doc": "short description\/keywd (used as key field)\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 5,
                                             "name": "impexschema",
                                             "typeId": "string",
                                             "doc": "impex schema\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 6,
                                             "name": "schema",
                                             "typeId": "string",
                                             "doc": "record schema\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 7,
                                             "name": "comment",
                                             "typeId": "string",
                                             "doc": "description of record\n",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 8,
                                             "name": "direction",
                                             "typeId": "i32",
                                             "doc": "direction\n",
                                             "class": "DirectionType",
                                             "required": "req_out"
                                           },
                                           {
                                             "key": 9,
                                             "name": "impex",
                                             "typeId": "string",
                                             "doc": "format structure\n",
                                             "required": "required"
                                           },
                                           {
                                             "key": 10,
                                             "name": "format",
                                             "typeId": "string",
                                             "doc": "Id\/description of foreign file format\n",
                                             "required": "optional"
                                           },
                                           {
                                             "key": 11,
                                             "name": "extension",
                                             "typeId": "string",
                                             "doc": "file extension\n",
                                             "required": "optional"
                                           }
                                         ]
                                       }
                                     ],
                                     "constants": [
                                     ],
                                     "services": [
                                     ]
                                   }

)" ;

const char* const FormatStructDataFile_value = R"({
                                               "block" :   {
                                                    "defaults" :   {
                                                         "properties.datasources" :   "[\"Supcrt-Slop07\"]",
                                                         "properties.radioactivity.decay_type" :   "{\"0\" :   \"STABLE\"}",
                                                         "properties.radioactivity.half_life_lambda.values" :   "[ 0 ]",
                                                         "properties.sourcetdb" :   "{\"21\" :   \"SUPCRT07\"}"
                                                    },
                                                    "pairs" :   {

                                                    },
                                                    "matches" :   {
                                                         "dod.0.label" :   {
                                                              "field" :   "properties.atomic_mass.name"
                                                         },
                                                         "dod.0.val" :   {
                                                              "field" :   "properties.atomic_mass.values"
                                                         },
                                                         "dod.1.label" :   {
                                                              "field" :   "properties.entropy.name"
                                                         },
                                                         "dod.1.val" :   {
                                                              "field" :   "properties.entropy.values"
                                                         },
                                                         "dod.10.fv" :   {
                                                              "field" :   "properties.name"
                                                         },
                                                         "dod.10.val" :   {
                                                              "field" :   "properties.comment"
                                                         },
                                                         "dod.11.val" :   {
                                                              "field" :   "properties.formula"
                                                         },
                                                         "dod.12.val" :   {
                                                              "field" :   "properties.substance"
                                                         },
                                                         "dod.2.label" :   {
                                                              "field" :   "properties.heat_capacity.name"
                                                         },
                                                         "dod.2.val" :   {
                                                              "field" :   "properties.heat_capacity.values"
                                                         },
                                                         "dod.3.label" :   {
                                                              "field" :   "properties.volume.name"
                                                         },
                                                         "dod.3.val" :   {
                                                              "field" :   "properties.volume.values"
                                                         },
                                                         "dod.5.label" :   {
                                                              "field" :   "properties.isotope_abundance.name"
                                                         },
                                                         "dod.5.val" :   {
                                                              "field" :   "properties.isotope_abundance.values"
                                                         },
                                                         "dod.6.val" :   {
                                                              "field" :   "properties.valences.values"
                                                         },
                                                         "dod.7.val" :   {
                                                              "field" :   "properties.number"
                                                         },
                                                         "dod.9.val" :   {
                                                              "field" :   "properties.aggregate_state",
                                                              "convert" :   {
                                                                   "`   " :   "{\"6\" :   \"AS_IONEX\"}",
                                                                   "g   " :   "{\"0\" :   \"AS_GAS\"}",
                                                                   "l   " :   "{\"1\" :   \"AS_LIQUID\"}",
                                                                   "s   " :   "{\"3\" :   \"AS_CRYSTAL\"}"
                                                              }
                                                         },
                                                         "key.0.fv" :   {
                                                              "field" :   "properties.symbol"
                                                         },
                                                         "key.1.fv" :   {
                                                              "field" :   "properties.class_",
                                                              "convert" :   {
                                                                   "a" :   "{\"2\" :   \"LIGAND\"}",
                                                                   "e" :   "{\"0\" :   \"ELEMENT\"}",
                                                                   "h" :   "{\"0\" :   \"ELEMENT\"}",
                                                                   "o" :   "{\"0\" :   \"ELEMENT\"}",
                                                                   "v" :   "{\"5\" :   \"OTHER_EC\"}",
                                                                   "z" :   "{\"4\" :   \"CHARGE\"}"
                                                              }
                                                         }
                                                    }
                                               },
                                               "renderer" :   "JSON",
                                               "label" :   "VertexElement",
                                               "comment" :   "Import",
                                               "direction" :   0
                                          })";

const char* const FormatKeyValueFile_value = R"({
                                             "block" :   {
                                                  "defaults" :   {

                                                  },
                                                  "pairs" :   {

                                                  },
                                                  "matches" :   {
                                                       "AB" :   {
                                                            "field" :   "properties.abstracttext"
                                                       },
                                                       "AD" :   {
                                                            "field" :   "properties.address"
                                                       },
                                                       "AU" :   {
                                                            "field" :   "properties.author"
                                                       },
                                                       "BT" :   {
                                                            "field" :   "properties.booktitle"
                                                       },
                                                       "DO" :   {
                                                            "field" :   "properties.identifiers"
                                                       },
                                                       "ED" :   {
                                                            "field" :   "properties.editor"
                                                       },
                                                       "EP" :   {
                                                            "field" :   "properties.pages",
                                                            "script" :   "field = properties.pages + field;"
                                                       },
                                                       "ET" :   {
                                                            "field" :   "properties.edition"
                                                       },
                                                       "ID" :   {
                                                            "field" :   "properties.shortname"
                                                       },
                                                       "IS" :   {
                                                            "field" :   "properties.number"
                                                       },
                                                       "JO" :   {
                                                            "field" :   "properties.journal"
                                                       },
                                                       "KW" :   {
                                                            "field" :   "properties.keywords"
                                                       },
                                                       "PB" :   {
                                                            "field" :   "properties.publisher"
                                                       },
                                                       "PY" :   {
                                                            "field" :   "properties.year"
                                                       },
                                                       "SP" :   {
                                                            "field" :   "properties.pages"
                                                       },
                                                       "T1" :   {
                                                            "field" :   "properties.title"
                                                       },
                                                       "T2" :   {
                                                            "field" :   "properties.booktitle"
                                                       },
                                                       "TI" :   {
                                                            "field" :   "properties.title"
                                                       },
                                                       "TY" :   {
                                                            "field" :   "properties.bibliographic_type",
                                                            "convert" :   {
                                                                 "BOOK" :   "1",
                                                                 "CHAP" :   "4",
                                                                 "CHAPTER" :   "3",
                                                                 "CONF" :   "11",
                                                                 "INPR" :   "15",
                                                                 "JFULL" :   "0",
                                                                 "JOUR" :   "0",
                                                                 "RPRT" :   "12",
                                                                 "THES" :   "10",
                                                                 "UNPD" :   "13"
                                                            }
                                                       },
                                                       "UR" :   {
                                                            "field" :   "properties.links"
                                                       },
                                                       "VL" :   {
                                                            "field" :   "properties.volume"
                                                       }
                                                  },
                                                  "script" :   "if rintable.properties.shortname == ''  or rintable.properties.shortname == nil then rintable.properties.shortname = rintable.properties.author[1] end; if rintable.properties.bibliographic_type ~= 1 and rintable.properties.bibliographic_type ~= 2 and rintable.properties.bibliographic_type ~= 4 and rintable.properties.bibliographic_type ~= 6 then rintable.properties.booktitle = '' end; if rintable.properties.bibliographic_type == 1 then rintable.properties.booktitle = rintable.properties.title end;"
                                             },
                                             "format" :   {
                                                  "head_regexp" :   "",
                                                  "end_regexp" :   "^\\s*ER\\s*-\\s*",
                                                  "key_regexp" :   "^\\s*([A-Z0-9]{2})\\s*-\\s*",
                                                  "value_regexp" :   "",
                                                  "value_next" :   "",
                                                  "value_token_regexp" :   " ",
                                                  "Ndata" :   0,
                                                  "strvalue_exp" :   ""
                                             },
                                             "renderer" :   "RIS",
                                             "label" :   "VertexDataSource",
                                             "separators" :   {
                                                  "v_sep" :   "",
                                                  "l_sep" :   ""
                                             },
                                             "comment" :   "test",
                                             "direction" :   0
                                        }

)";

const char* const FormatTableFile_value = R"({
                                          "block" :   {
                                               "defaults" :   {
                                                    "properties.Pst" :   "1e05",
                                                    "properties.TPMethods" :   "[{\"method\":{\"0\" :   \"cp_ft_equation\"}, \"m_heat_capacity_ft_coeffs\": { \"limitsTP\": {\"upperT\": 0}, \"values\" : [0,0,0,0,0,0,0,0,0,0], \"names\" : [\"a0\",\"a1\",\"a2\",\"a3\",\"a4\",\"a5\",\"a6\",\"a7\",\"a8\",\"a9\",\"a10\"], \"units\" : [\"J/(mol*K)\",\"J/(mol*K^2)\",\"(J*K)/mol\",\"J/(mol*K^0.5)\",\"J/(mol*K^3)\",\"J/(mol*K^4)\",\"J/(mol*K^5)\",\"(J*K^2)/mol\",\"J/mol\",\"J/(mol*K^1.5)\",\"J/(mol*K)\"] } }]",
                                                    "properties.Tst" :   "298.15",
                                                    "properties.aggregate_state" :   "{\"2\" :   \"AS_CRYSTAL\"}",
                                                    "properties.class_" :   "{\"0\" :   \"SC_COMPONENT\"}",
                                                    "properties.sourcetdb" :   "{\"23\" :   \"SLOP16\"}"
                                               },
                                               "pairs" :   {

                                               },
                                               "matches" :   {
                                                    "Tmax" :   {
                                                         "field" :   "properties.TPMethods.0.limitsTP.upperT",
                                                         "ignore" :   false,
                                                         "script" :   ""
                                                    },
                                                    "a" :   {
                                                         "field" :   "properties.TPMethods.0.m_heat_capacity_ft_coeffs.values.0",
                                                         "ignore" :   false,
                                                         "script" :   "field = field*4.184"
                                                    },
                                                    "abbreviation" :   {
                                                         "field" :   "properties.name",
                                                         "ignore" :   false,
                                                         "script" :   ""
                                                    },
                                                    "b" :   {
                                                         "field" :   "properties.TPMethods.0.m_heat_capacity_ft_coeffs.values.1",
                                                         "ignore" :   false,
                                                         "script" :   "field = field*4.184e-3"
                                                    },
                                                    "c" :   {
                                                         "field" :   "properties.TPMethods.0.m_heat_capacity_ft_coeffs.values.2",
                                                         "ignore" :   false,
                                                         "script" :   "field = field*4.184e05"
                                                    },
                                                    "chemical_formula" :   {
                                                         "field" :   "properties.formula",
                                                         "ignore" :   false,
                                                         "script" :   "field = string.gsub(field, '%*([%d%.]*)H2O', '(H2O)%1'); field = string.gsub(field, '%*([%d%.]*)H20', '(H2O)%1');"
                                                    },
                                                    "deltaG" :   {
                                                         "field" :   "properties.sm_gibbs_energy.values",
                                                         "ignore" :   false,
                                                         "script" :   "if field[1] == 999999.0 then field[1] = 999999.0 else field[1] = field[1]*4.184 end;"
                                                    },
                                                    "deltaH" :   {
                                                         "field" :   "properties.sm_enthalpy.values",
                                                         "ignore" :   false,
                                                         "script" :   "if field[1] == 999999.0 then field[1] = 999999.0 else field[1] = field[1]*4.184 end;"
                                                    },
                                                    "entropy" :   {
                                                         "field" :   "properties.sm_entropy_abs.values",
                                                         "ignore" :   false,
                                                         "script" :   "field[1] = field[1]*4.184"
                                                    },
                                                    "name" :   {
                                                         "field" :   "properties.symbol",
                                                         "ignore" :   false,
                                                         "script" :   ""
                                                    },
                                                    "reference" :   {
                                                         "field" :   "properties.datasources",
                                                         "ignore" :   false,
                                                         "script" :   ""
                                                    },
                                                    "volume" :   {
                                                         "field" :   "properties.sm_volume.values",
                                                         "ignore" :   false,
                                                         "script" :   "field[1] = field[1]*0.1"
                                                    }
                                               },
                                               "script" :   ""
                                          },
                                          "format" :   {
                                               "Nhcols" :   14,
                                               "Nhrows" :   0,
                                               "headers" :   [
                                                    "name",
                                                    "chemical_formula",
                                                    "abbreviation",
                                                    "elemental_chemical_formula",
                                                    "reference",
                                                    "date_last_revisited",
                                                    "deltaG",
                                                    "deltaH",
                                                    "entropy",
                                                    "volume",
                                                    "a",
                                                    "b",
                                                    "c",
                                                    "Tmax"
                                               ],
                                               "rowend" :   "\\n",
                                               "rows_one_block" :   6,
                                               "row_header_regexp" :   "",
                                               "colends" :   " \\t\\n",
                                               "usequotes" :   false,
                                               "usemore" :   true,
                                               "comment_regexp" :   "",
                                               "row_size" :   0
                                          },
                                          "renderer" :   "",
                                          "label" :   "VertexSubstance",
                                          "separators" :   {
                                               "v_sep" :   "",
                                               "l_sep" :   "",
                                               "r_sep" :   "",
                                               "c_head" :   "",
                                               "c_end" :   "",
                                               "eod" :   "",
                                               "encoding" :   "",
                                               "str_delim" :   "",
                                               "bod" :   ""
                                          },
                                          "comment" :   "",
                                          "fname" :   "",
                                          "Nblocks" :   0,
                                          "Nlines" :   0,
                                          "direction" :   0
                                     }
)";
