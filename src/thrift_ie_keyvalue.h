#pragma once

#include <fstream>
#include "thrift_ie_base.h"

namespace jsonio17 {



// http://eax.me/cpp-regex/
// https://regex101.com/

// bib
// "head_regexp" :   "^\\s*@article\\s*\\{\\s*(\\w+)\\s*,",
// "key_regexp" :   "^\\s*(\\w+)\\s*=\\s*",
// "value_regexp" :   "^\"([^\"]*)\"\\s*,{0,1}",
// "end_regexp" :   "^\\s*\\}",

// ris
// "head_regexp" :   "",
// "end_regexp" :   "^\\s*ER\\s*-\\s*",
// "key_regexp" :   "^\\s*([A-Z0-9]{2})\\s*-\\s*",
// "value_regexp" :   "",
// "value_next" :   "\r\n",

// Match a "quoted std::string"  : ^"(?:[^\\"]|\\.)*"$


//--------------------------------------------------------------------------
// Definition of text file with key-value pair data
class  KeyValueIEFile: public AbstractIEFile
{
public:

    KeyValueIEFile( ImpexFormatFile::ImpexMode amode, const std::string& data_schema_name, const std::string& lua_lib_path,
                    const FormatKeyValueFile& format_description_data );

    ~KeyValueIEFile() {}

    /// Open file to import
    void openFile( const std::string& inputFname ) override;
    /// Close internal file
    void closeFile() override;

    /// Load string to import from ( or export to stringstream )
    void loadString( const std::string& inputData ) override;

    /// Convert readed key-value data to schema defined json
    bool initBlock(  std::string& blockdata, const key_values_table_t& readed_data );

    const key_values_table_t& readedData() const
    {
        return external_data;
    }

    // export part -----------------------------------------------------------------

    /// Write data from one block to file
    bool writeBlockToFile( const key_values_table_t& data_to_write ) override;

protected:

    bool use_string_stream = false;
    std::fstream _fstream;      ///< Stream to input/output
    std::stringstream _stringstream;      ///< Stream to input/output
    std::istream& get_istream()
    {
        if( use_string_stream )
            return _stringstream;
        else
            return _fstream;
    }
    std::ostream& get_ostream()
    {
        if( use_string_stream )
            return _stringstream;
        else
            return _fstream;
    }

    /** Definition of key-value pair (line) in file */
    //FormatKeyValuePair lineFormat;
    /** Definition of key-value pair (line) in file */
    FormatKeyValue format;
    /** Rendering syntax for the foreign key-value file "GEMS3K" | "BIB" | "RIS" | ... */
    std::string renderer;
    /** Obsolute Definition of value, line, row, block, comment, end-of-data separators */
    Separators separs;

    // internal data
    std::string str_buf;  ///< Internal data to match regular expr
    std::string::const_iterator test_start;
    std::string::const_iterator test_end;

    // internal functions

    bool readBlockFromFile();
    bool readNextBlockfromFile();
    bool readRegexp( const std::string& re_str, std::vector<std::string>& mach_str );
    bool testRegexp( const std::string& re_str );
    bool readStartBlock();
    bool isEndBlock();
    std::string readAllValue();
    bool readKeyValuePair();
    void addKeyValue( const std::string& _key, const std::string& _value );
    void insertKeyValue( const std::string& _key, const std::vector<std::string>& vec );
    void skipComments();


};


} // namespace jsonio17


