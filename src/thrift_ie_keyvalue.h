#pragma once

#include <fstream>
#include "thrift_ie_base.h"

namespace jsonio17 {


/// Class KeyValueIEFile - export/import of data records to/from text file with key-value pair data.
/// Using a generated JSON schema of that input file format
class  KeyValueIEFile: public AbstractIEFile
{
public:

    KeyValueIEFile( ImpexFormatFile::ImpexMode amode, const std::string& data_schema_name,
                    const std::string& lua_lib_path,  const FormatKeyValueFile& format_description_data );

    ~KeyValueIEFile() {}

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
    bool writeBlockToFile( const key_values_table_t& data_to_write ) override;

protected:

    bool use_string_stream = false;
    /// File stream to input/output
    std::fstream file_stream;
    /// String stream to input/output
    std::stringstream string_stream;

    /// Definition of key-value pair (line) in file
    FormatKeyValue format;
    /// Rendering syntax for the foreign key-value file "GEMS3K" | "BIB" | "RIS" | ...
    std::string renderer;
    // Obsolute Definition of value, line, row, block, comment, end-of-data separators
    //Separators separs;

    /// Internal data to match regular expr
    std::string str_buf;
    std::string::const_iterator test_start;
    std::string::const_iterator test_end;

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

    bool read_block_from_file() override;
    bool read_next_block_from_file();
    bool read_regexp( const std::string& re_str, std::vector<std::string>& mach_str );
    bool test_regexp( const std::string& re_str );
    bool read_start_block();
    bool is_end_block();
    std::string read_all_value();
    bool read_key_value_pair();
    void add_key_value( const std::string& key, const std::string& value );
    void insert_key_value( const std::string& key, const std::vector<std::string>& values );
    void skip_comments();
};

KeyValueIEFile* connectFormatKeyValueFile( int amode, const JsonBase& object, const std::string& lua_lib_path );
// ImpexFormatFile::formats["FormatKeyValueFile"] = connectFormatKeyValueFile;
// ImpexFormatFile::addFormat("FormatKeyValueFile", connectFormatKeyValueFile);


} // namespace jsonio17


