#pragma once

#include <fstream>
#include "thrift_ie_base.h"

namespace jsonio17 {

/// Class TableIEFile - export/import of data records to/from text file with key-value pair data.
/// Using a generated JSON schema of that input file format
class  TableIEFile: public AbstractIEFile
{
public:

    TableIEFile( ImpexFormatFile::ImpexMode amode,const std::string& data_schema_name,
                 const std::string& lua_lib_path, const FormatTableFile& format_description_data );

    ~TableIEFile() {}

    /// Open file to import
    void openFile( const std::string& file_path ) override;
    /// Close internal file
    void closeFile() override;

    /// Load string to import from ( or export to stringstream )
    void loadString( const std::string& input_data ) override;
    /// Get string export to ( if export to stringstream )
    std::string getString() override;


    /// Convert free format json to schema defined ( FormatStructDataFile only )
    virtual bool readBlock( const std::string&, std::string&, std::string&, int ) override
    {
        return false;
    }

    /// Read next json object  ( FormatStructDataFile only )
    virtual bool nextFromFile( std::string& ) override
    {
        return false;
    }

    // export part -----------------------------------------------------------------

    /// Write data from one block to file
    virtual bool writeBlockToFile( const key_values_table_t& data_to_write ) override;

protected:

    bool use_string_stream = false;
    /// File stream to input/output
    std::fstream file_stream;
    /// String stream to input/output
    std::stringstream string_stream;

    /// Definition of key-value pair (line) in file
    FormatTable format;
    /// Rendering syntax for the foreign key-value file "GEMS3K" | "BIB" | "RIS" | ...
    std::string renderer;
    // Obsolute Definition of value, line, row, block, comment, end-of-data separators
    //Separators separs;

    ///  Internal data to match regular expr
    std::string next_row_data;

    std::istream& get_istream()
    {
        if( use_string_stream )
            return string_stream;
        else
            return file_stream;
    }
    std::ostream& get_ostream()
    {
        if( use_string_stream )
            return string_stream;
        else
            return file_stream;
    }

    std::string read_next_data();

    bool read_block_from_file() override;
    bool read_header_from_file();
    void skip_comments( std::string& row );
    std::string read_next_row();
    void add_value( size_t col, const std::string& value, bool toheader );
    void split_fixed_size( const std::string& row, bool toheader=false );
    void split_regexpr( const std::string& row, bool toheader=false );
    size_t extract_quoted( const std::string& row, std::string& value );
    void split_delimiter( const std::string& row, bool toheader=false );

};

TableIEFile* connectFormatTableFile( int amode, const JsonBase& object, const std::string& lua_lib_path );
// ImpexFormatFile::formats["FormatTableFile"] = connectFormatTableFile;
// ImpexFormatFile::addFormat("FormatTableFile", connectFormatTableFile);

} // namespace jsonio17

