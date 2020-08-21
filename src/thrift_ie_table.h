#pragma once

#include <fstream>
#include "thrift_ie_base.h"

namespace jsonio17 {

//--------------------------------------------------------------------------
// Definition of text file with table data
class  TableIEFile: public AbstractIEFile
{
public:

    TableIEFile( ImpexFormatFile::ImpexMode amode,const std::string& data_schema_name, const std::string& alualibPath,
                 const FormatTableFile& defData );

    ~TableIEFile() {}

    /// Open file to import
    void openFile( const std::string& inputFname ) override;
    /// Close internal file
    void closeFile() override;

    /// Load string to import from ( or export to stringstream )
    void loadString( const std::string& inputData ) override;


    // export part -----------------------------------------------------------------

    /// Write data from one block to file
    virtual bool writeBlockToFile( const key_values_table_t& data_to_write ) override;

protected:

    bool use_string_stream = false;
    std::fstream _fstream;      ///< Stream to input/output
    std::stringstream _stringstream;      ///< Stream to input/output

    // internal data
    std::string next_row_data;  ///< Internal data to match regular expr


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
    FormatTable format;
    /** Rendering syntax for the foreign key-value file "GEMS3K" | "BIB" | "RIS" | ... */
    std::string renderer;
    /** Obsolute Definition of value, line, row, block, comment, end-of-data separators */
    Separators separs;

    // internal functions

    std::string read_next_data();

    bool readBlockFromFile();
    bool readHeaderFromFile();
    void skipComments( std::string& row );
    std::string readNextRow();
    void addValue( size_t col, const std::string& _value, bool toheader);
    void splitFixedSize( const std::string& row, bool toheader=false);
    void splitRegExpr( const std::string& row, bool toheader=false);
    uint extractQuoted( const std::string& row, std::string& value );
    void splitDelimiter( const std::string& row, bool toheader=false);

};


} // namespace jsonio17

