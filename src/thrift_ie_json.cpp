
#include "jsonio17/jsonfree.h"
#include "jsonio17/jsondump.h"
#include "thrift_ie_json.h"

namespace jsonio17 {


StructDataIEFile* connectFormatStructDataFile( int amode, const JsonBase& object, const std::string& lua_lib_path )
{
    FormatStructDataFile fformatdata;
    readDataFromJsonSchema(  object, "FormatStructDataFile",  &fformatdata );
    //Create input structure
    StructDataIEFile*  inputIEFile =  new  StructDataIEFile( static_cast<ImpexFormatFile::ImpexMode>(amode),
                                                             fformatdata.label, lua_lib_path, fformatdata );
    return inputIEFile;
}

StructDataIEFile::StructDataIEFile( ImpexFormatFile::ImpexMode amode, const std::string& data_schema_name,
                                    const std::string& lua_lib_path, const FormatStructDataFile& format_description_data ):
    AbstractIEFile( amode, data_schema_name,  lua_lib_path),  renderer(format_description_data.renderer)
{

    // set up data from  FormatStructDataFile
    //label = format_description_data.label;
    comment = format_description_data.comment;
    file_name = format_description_data.fname;
    Nblocks = format_description_data.Nblocks;
    Nlines = format_description_data.Nlines;
    block = format_description_data.block;

    // select file type
    std::transform(renderer.begin(), renderer.end(), renderer.begin(), [](unsigned char c){ return tolower(c); });
    foreign_data_type = JsonYamlXMLFile::type_from_extension( renderer );

}

// Open file to import
void StructDataIEFile::openFile( const std::string& file_path )
{
    if( !file_path.empty() )
        file_name = file_path;

    // read all input array to internal
    ie_stream.reset( new JsonYamlXMLArrayFile( file_path ) );
    ie_stream->set_type( foreign_data_type );
    TxtFile::OpenModeTypes file_mode = ( mode_use!=ImpexFormatFile::modeExport ? TxtFile::ReadOnly : TxtFile::WriteOnly );
    ie_stream->Open( file_mode );
}

// Close internal file
void StructDataIEFile::closeFile()
{
    if( ie_stream->isOpened() )
        ie_stream->Close();
}

void StructDataIEFile::loadString( const std::string& input_data )
{
    ie_stream.reset( new JsonYamlXMLArrayFile( "empty" ) );
    ie_stream->set_type( foreign_data_type );
    if( mode_use==ImpexFormatFile::modeImport ||
        mode_use==ImpexFormatFile::modeUpdate  )
        ie_stream->loadString( input_data );
}

std::string StructDataIEFile::getString()
{
    return ie_stream->getString();
}

bool StructDataIEFile::nextFromFile( std::string& inputjson )
{
    return ie_stream->loadNext( inputjson );
}

// Convert free format json to schema defined
bool StructDataIEFile::readBlock(  const std::string& inputjson, std::string& schemajson, std::string& key, int ndx )
{
    // read data from block to readedData
    external_data.clear();
    if( !read_block_from_json( inputjson, key, ndx ) )
        return false;

    // reset default structure
    dom_node.clear();
    // write data to structNode
    set_readed_data( true );
    run_block_script(block.script, dom_node );

    // return data to json string
    schemajson = dom_node.dump();
    return true;
}

bool StructDataIEFile::writeBlockToFile( const key_values_table_t& )
{
     JSONIO_THROW( "StructDataIEFile", 11, " could not generate json from " );
     // Could be default json + update values by path
}


// Read data from one block in file
bool StructDataIEFile::extract_values( const std::string &key, const JsonFree& foreign_node, values_t& values )
{
    auto fld = foreign_node.field( key );
    if( fld && !fld->isNull() ) {
        if( fld->isArray() )  {
            auto impexOrganization = impex_field_organization( key );
            if( impexOrganization=="table" ) {
                values.clear();
                for (auto it = fld->cbegin(); it != fld->cend(); ++it) {
                    values_t vals;
                    if(  (*it)->get_to_list( vals ) ) {
                        values.insert(values.end(), vals.begin(), vals.end());
                    }
                }
                return true;
            }
            else if( impexOrganization=="string" ) {
                values.clear();
                values.push_back( fld->dump(true));
                return true;
            }
            else if(  fld->get_to_list( values ) ) {
                return true;
            }
        }
        else  {
            std::string  value;
            if( fld->get_to( value  ) )  {
                values = {value};
                return true;
            }
        }
    }
    return false;
}


// Read data from one block in file
bool StructDataIEFile::read_block_from_file()
{
    std::string inputjson;
    if( !nextFromFile( inputjson ) )
        return false;

    auto foreign_node = json::loads( inputjson );
    // set data to readedData
    std::string key;
    std::vector<std::string> values;
    for ( const auto& itr: block.matches )
    {
        key = itr.first;
        if( extract_values( key, foreign_node, values ) )
            external_data[key] = values;
    }
    impex_logger->debug("StructDataIEFile::read_block_from_file:\n {}", json::dump(external_data));
    return true;
}

// Read data one block from json data
bool StructDataIEFile::read_block_from_json( const std::string& inputjson, std::string& secondKey, int ndx )
{
    if( inputjson.empty() )
        return false;

    secondKey = "";
    auto foreign_node = json::loads(inputjson);

    // set data to readedData
    std::string key, key2;
    std::vector<std::string> values;
    for ( const auto& itr: block.matches )
    {
        key2 = key = itr.first;
        if( ndx >= 0)
            key2 = regexp_replace( key, "%", std::to_string(ndx) );

        if( extract_values( key2, foreign_node, values ) )
        {
            external_data[key] = values;
            if( itr.second.field == "@key" )
            {
                // convert key or get substring
                lua_run.runFunc( itr.second.script, values[0] );
                secondKey += values[0];
            }
        }
        else
        {
            if( key != key2 )
                return false;
        }
    }
    return true;
}


} // namespace jsonio17

