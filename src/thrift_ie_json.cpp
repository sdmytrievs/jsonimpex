

//#include <iostream>
//#include "jsonio/jsondomfree.h"
#include "thrift_ie_json.h"

namespace jsonio17 {


StructDataIEFile* connectFormatStructDataFile( int amode, const JsonBase& object, const std::string& lua_lib_path )
{
    FormatStructDataFile fformatdata;
    readDataFromJsonSchema(  object, "FormatStructDataFile",  &fformatdata );
    //Create input structure
    StructDataIEFile*  inputIEFile=  new  StructDataIEFile( static_cast<ImpexFormatFile::ImpexMode>(amode), fformatdata.label, lua_lib_path, fformatdata );
    return inputIEFile;
}

ImpexFormatFile::formats["FormatStructDataFile"] = connectFormatStructDataFile;

//--------------------------------------------------------------------------
// Export/import of data records to/from text file in legacy JSON/YAML/XML (foreign keywords)

StructDataIEFile::StructDataIEFile( ImpexFormatFile::ImpexMode amode, const std::string& data_schema_name, const std::string& alualibPath, const FormatStructDataFile& defData ):
           AbstractIEFile( amode, data_schema_name,  alualibPath),  renderer(defData.renderer)
{
   // set up data from  FormatKeyValueFile !!!
    label = defData.label;
    comment = defData.comment;
    file_name = defData.fname;
    Nblocks = defData.Nblocks;
    Nlines = defData.Nlines;
    block = defData.block;
    structNode = JsonDomSchema::newSchemaObject( label );

    // select file type
    if(renderer == "json" || renderer == "JSON" )
      _ftype = FileTypes::Json_;
    else  if(renderer == "yaml" || renderer == "YAML" )
            _ftype = FileTypes::Yaml_;
    else if(renderer == "xml" || renderer == "XML" )
            _ftype = FileTypes::XML_;
         else _ftype = FileTypes::Undef_;
}

// Open file to import
void StructDataIEFile::openFile( const std::string& inputFname )
{

  jsonioErrIf( _mode != modeImport  , file_name, "No export to format...");

  if( !inputFname.empty() )
    file_name = inputFname;

  // read all input array to internal
  _stream.reset( new FJsonYamlXmlArray( inputFname ) );
  _stream->Open( OpenModeTypes::ReadOnly );
}

// Close internal file
void StructDataIEFile::closeFile()
{
    if (_stream->testOpen() )
      _stream->Close();
}


// Read next bson object
bool StructDataIEFile::nextFromFile( std::string& inputjson )
{
  return _stream->LoadNext( inputjson );
}

// Read data from one block in file
bool StructDataIEFile::readBlockFromFile()
{
    std::string inputjson;
    if( !nextFromFile( inputjson ) )
       return false;

    std::shared_ptr<JsonDomFree> node(JsonDomFree::newObject());
    if( !parseJsonToNode( inputjson, node.get() ) )
      return false;

    // set data to readedData
    std::string key;
    std::vector<std::string> _value;
    auto itr = block.matches.begin();
    while ( itr != block.matches.end() )
    {
        key = itr->first;
        if( node->findToArray( key, _value ) )
         externData.insert( std::pair<std::string, std::vector<std::string>>(key, _value) );
        itr++;
    }
    return true;
}

// Read data one block from bson data
bool StructDataIEFile::readBlockFromBson( const std::string& inputjson, std::string& secondKey, int ndx )
{
    if( inputjson.empty() )
       return false;

    secondKey = "";
    std::shared_ptr<JsonDomFree> node(JsonDomFree::newObject());
    if( !parseJsonToNode( inputjson, node.get() ) )
      return false;

    // set data to readedData
    std::string key, key2;
    std::vector<std::string> _value;
    auto itr = block.matches.begin();
    while ( itr != block.matches.end() )
    {
        key2 = key = itr->first;

        if( ndx>=0)
         key2 = replace( key, "%", std::to_string(ndx).c_str());

        if( node->findToArray( key2, _value ) )
        { externData.insert( std::pair<std::string, std::vector<std::string>>(key, _value) );
          if(itr->second.field == "@key" )
          {   // convert key or get substring
              _lua.runFunc(itr->second.script, _value[0]);
              secondKey += _value[0];
          }
        } else
          {
              if(key != key2 )
               return false;
          }

        itr++;
    }
    return true;
}

/// Convert free format bson to schema defined
bool StructDataIEFile::readBlock(  const std::string& inputjson, std::string& schemajson, std::string& key, int ndx )
{
   // read data from block to readedData
    externData.clear();
    if( !readBlockFromBson(inputjson, key, ndx ) )
           return false;

   // reset default structure
      structNode->clearField();
   // write data to structNode
      setReadedData();
      run_block_script(block.script, structNode );

   // return data to json string
     printNodeToJson( schemajson, structNode );
     return true;
}



} // namespace jsonio17
