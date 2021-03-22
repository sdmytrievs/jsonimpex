#pragma once

#include "jsonio17/jsonschema.h"
#include "jsonimpex17/thrift_impex.h"
#include "lua-run.h"


#ifdef USE_THRIFT
#include "impex_types.h"
using  impex_struct_t = ::apache::thrift::TBase;
using namespace  ::impex;
#else
#include "no_thrift/impex_types_fix.h"
using  impex_struct_t = ::jsonio17::impex::TBase;
using namespace  ::impex;
#endif

namespace jsonio17 {

/// Class AbstractIEFile - abstract interface of import/export to/from foreign format files.
class AbstractIEFile
{

public:

    /// The constructor
    explicit AbstractIEFile( ImpexFormatFile::ImpexMode amode, const std::string& data_schema_name, const std::string& lua_lib_path );

    virtual ~AbstractIEFile();

    /// Name of data type
    const std::string&  name() const
    {
        return  label;
    }

    /// Load file to import/export from/to
    virtual void openFile( const std::string& file_path ) = 0;
    /// Close internal file
    virtual void closeFile() {}

    /// Load string to import from ( or export to stringstream )
    virtual void loadString( const std::string& input_data ) = 0;
    /// Get string export to ( if export to stringstream )
    virtual std::string getString() = 0;

    // Import part -----------------------------------------------------------------

    /// Read one block to json data
    bool readBlock( std::string& json_block_data )
    {
        // read data from block to readedData
        external_data.clear();
        if( !read_block_from_file() )
            return false;

        // reset default structure
        dom_node.clear();
        // write data to structNode
        set_readed_data();
        // Run script for operation on data values in block
        run_block_script( block.script, dom_node );

        // return data to json string
        json_block_data = dom_node.dump();
        return true;
    }

    /// Convert free format json to schema defined ( FormatStructDataFile only )
    virtual bool readBlock( const std::string& inputjson, std::string& schemajson, std::string& key, int ndx=-1 ) =0;

    /// Read next json object  ( FormatStructDataFile only )
    virtual bool nextFromFile( std::string& inputjson ) = 0;

    // Export part -----------------------------------------------------------------

    /// Write data from one block to file
    virtual bool writeBlockToFile( const key_values_table_t& data_to_write ) = 0;

    /// Extract data from Json to structure
    virtual void extractDataToWrite( const std::string& json_block_data, key_values_table_t& data_to_write );

    /// Write one block to extern format file
    bool writeBlock( const std::string& json_block_data )
    {
        external_data.clear();
        extractDataToWrite( json_block_data, external_data );
        return writeBlockToFile( external_data );
    }

protected:

    /// Import or export
    ImpexFormatFile::ImpexMode mode_use;
    /// LUA-C++ Integration
    LuaRun lua_run;

    /// Label of data type (vertex type), e.g. "datasource", "element"
    std::string label;
    /// Export: the whole comment text; Import: the comment begin markup std::string (to skip until endl)
    std::string comment;
    /// File name or "console" for export
    std::string file_name;
    /// number of data block in file >=1, 0 if unknown
    int Nblocks;
    /// number of text lines in file (>=1), 0 if unknown
    int Nlines;

    /// Text block format in file corresponding to one database document (record)
    FormatBlock block;

    /// Internal schema based JSON object
    JsonSchema dom_node;

    /// Key, Value pairs readed from the block of data in file
    key_values_table_t external_data;


    /// Read data from one block in file
    virtual bool read_block_from_file() = 0;

    /// Save readed data to DOM
    virtual void set_readed_data();

    /// Change number value, using script
    void run_lua_script( const std::string& script, values_t& values, bool isArray );
    /// Change std::string value, using script
    void run_lua_script_string( const std::string& script,  values_t& values, bool isArray );

    /// Change value, using convert
    void run_convert( const std::map<std::string, std::string>& convert, values_t& values );

    /// Run script for operation on data values in block
    void run_block_script( const std::string& script, JsonSchema& object );

    /// Get Organization of object from the imported or exported file
    std::string impex_field_organization( const std::string& impex_field_key );
    /// Get Type of object from the imported or exported file
    std::string impex_field_type( const std::string& impex_field_key );

    /// Inser key value paies to extern data list
    void add_map_key_value( const std::string& key, const  std::vector<std::string>& values );

    std::vector<std::string> extract_value( const std::string& key,
                                           const key_values_table_t& write_data )
    {
        auto itval = write_data.find(key);
        if( itval != write_data.end() && itval->second.size() > 0 )
            return itval->second;
        else
            return {};
    }

    /// Asseble parts of group to one vector
    /// Extract data from different parts of thrift record
    void assemble_groups( key_values_table_t& writeData );

};

void writeDataToJsonSchema( JsonBase& object, const std::string& structName, impex_struct_t* data );
void readDataFromJsonSchema( const JsonBase& object, const std::string& structName,  impex_struct_t* data );
std::string parseCode(/*regex_constants::error_type*/ int etype);

} // namespace jsonio17

