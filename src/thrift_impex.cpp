#include "jsonio17/io_settings.h"
#include "jsonimpex17/thrift_impex.h"
#include "thrift_ie_json.h"
#include "thrift_ie_keyvalue.h"
#include "thrift_ie_table.h"

namespace jsonio17 {

std::map<std::string, IEFileFactory_f> ImpexFormatFile::formats = {};

void ImpexFormatFile::initDefaultFormats()
{
    ImpexFormatFile::formats["FormatStructDataFile"] = connectFormatStructDataFile;
    ImpexFormatFile::formats["FormatKeyValueFile"] = connectFormatKeyValueFile;
    ImpexFormatFile::formats["FormatTableFile"] = connectFormatTableFile;
}

ImpexFormatFile::ImpexFormatFile(ImpexFormatFile::ImpexMode mode_of_use, const std::string &impex_format_name,
                                 const JsonBase &object, const std::string& path_to_scripts):
    lua_lib_path(path_to_scripts), impex_format(create_format_data( mode_of_use, impex_format_name, object ))
{ }


ImpexFormatFile::~ImpexFormatFile()
{ }

const std::string&  ImpexFormatFile::dataName() const
{
    return impex_format->name();
}

void ImpexFormatFile::openFile( const std::string& file_path )
{
    impex_format->openFile( file_path );
}

void ImpexFormatFile::closeFile()
{
    impex_format->closeFile();
}

void ImpexFormatFile::loadString(const std::string &input_data)
{
    impex_format->loadString(input_data);
}

std::string ImpexFormatFile::getString()
{
    return impex_format->getString();
}

bool ImpexFormatFile::readBlock( std::string& json_block_data )
{
    return impex_format->readBlock( json_block_data );
}

bool ImpexFormatFile::readBlock(  const std::string& input_json, std::string& schema_json, std::string& key, int ndx )
{
    return impex_format->readBlock( input_json,  schema_json,  key,  ndx );
}

bool ImpexFormatFile::nextFromFile( std::string& input_json )
{
    return impex_format->nextFromFile( input_json );
}

bool ImpexFormatFile::updateBlock(DBSchemaDocument *dbdocument, std::string &json_block_data)
{
    return impex_format->updateBlock( dbdocument, json_block_data );
}

bool ImpexFormatFile::writeBlockToFile( const key_values_table_t& data_to_write )
{
    return impex_format->writeBlockToFile( data_to_write );
}

void ImpexFormatFile::extractDataToWrite(  const std::string& json_block_data, key_values_table_t& data_to_write  )
{
    impex_format->extractDataToWrite(  json_block_data,  data_to_write );
}

bool ImpexFormatFile::writeBlock( const std::string& data_to_write )
{
    return impex_format->writeBlock(  data_to_write );
}

AbstractIEFile *ImpexFormatFile::create_format_data( ImpexMode amode, const std::string &impex_format_name, const JsonBase &object)
{
    if(lua_lib_path.empty()) {
        lua_lib_path =  ioSettings().resourcesDir()+"/lua/";
        //ioSettings().directoryPath("common.LuaScriptsDirectory", std::string("/lua"))+"/";
    }
    if( impex_format_name.empty() )
        return nullptr;

    if( formats.empty() )
        initDefaultFormats();
    auto format = formats.find( impex_format_name );
    if( format != formats.end() )
        return format->second( amode, object, lua_lib_path );

    // other do not realize
    return nullptr;
}

} // namespace jsonio17

