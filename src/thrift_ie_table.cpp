#include "thrift_ie_table.h"
#include <regex>
namespace rgx = std;

namespace jsonio17 {


TableIEFile* connectFormatTableFile( int amode, const JsonBase& object, const std::string& lua_lib_path )
{
    FormatTableFile fformatdata;
    readDataFromJsonSchema( object, "FormatTableFile",  &fformatdata );
    //Create input structure
    TableIEFile*  inputIEFile=  new  TableIEFile( static_cast<ImpexFormatFile::ImpexMode>(amode),
                                                  fformatdata.label, lua_lib_path, fformatdata );
    return inputIEFile;
}

TableIEFile::TableIEFile( ImpexFormatFile::ImpexMode amode,const std::string& data_schema_name,
                          const std::string& lua_lib_path, const FormatTableFile& format_description_data ):
    AbstractIEFile( amode, data_schema_name, lua_lib_path ),
    format( format_description_data.format ), renderer( format_description_data.renderer )
{
    // set up data from  FormatKeyValueFile !!!
    //label = format_description_data.label;
    comment = format_description_data.comment;
    file_name = format_description_data.fname;
    block = format_description_data.block;
    Nblocks = format_description_data.Nblocks;
    Nlines = format_description_data.Nlines;
}


// Open file to import
void TableIEFile::openFile( const std::string& file_path )
{
    use_string_stream = false;
    
    if( !file_path.empty() )
        file_name = file_path;

    // open  file
    if (file_stream.is_open())
        file_stream.close();

    if( mode_use != ImpexFormatFile::modeExport )
        file_stream.open( file_name,  std::fstream::in );
    else
        file_stream.open( file_name,  std::fstream::out|std::fstream::app );

    JSONIO_THROW_IF( !file_stream.good(), "TableIEFile", 11, " could not open file to import/export from " + file_name);
    
    if( mode_use != ImpexFormatFile::modeExport )
        read_header_from_file();
    ///  else
    ///    write_header_to_file();
    
}

// Close internal file
void TableIEFile::closeFile()
{
    if( file_stream.is_open() )
        file_stream.close();
}

void TableIEFile::loadString( const std::string &input_data )
{
    use_string_stream = true;
    
    if( file_stream.is_open() )
        file_stream.close();

    if( mode_use != ImpexFormatFile::modeExport )
        string_stream.str( input_data );
    else
        string_stream.str( "" );

    JSONIO_THROW_IF( !file_stream.good(), "TableIEFile", 12, " loading string error" );
    
    if( mode_use != ImpexFormatFile::modeExport  )
        read_header_from_file();
    ///  else
    ///    write_header_to_file();
    
}

std::string TableIEFile::getString()
{
    if( use_string_stream)
        return  string_stream.str();
    JSONIO_THROW( "TableIEFile", 13, " try get string when working with file stream" );
}


/// Read data from one block in file
bool TableIEFile::read_block_from_file( )
{
    std::string valuesRow = read_next_row();
    if( valuesRow.empty() )
        return false;
    
    if( !format.colsizes.empty() )
        split_fixed_size( valuesRow );
    else
        if( !format.value_regexp.empty() )
            split_regexpr( valuesRow );
        else
            split_delimiter( valuesRow );
    
    impex_logger->debug("TableIEFile::read_block_from_file:\n {}", json::dump(external_data));
    return true;
}

/// Read data from one block in file
bool TableIEFile::read_header_from_file()
{
    if( format.Nhrows <= 0)  // no headers
        return  true;
    
    std:: string valuesRow = read_next_row();
    if( valuesRow.empty() )
        return false;
    
    if( !format.colsizes.empty() )
        split_fixed_size( valuesRow, true );
    else
        //if( !format.value_regexp.empty() )
        //    split_regexpr( valuesRow, true );
        //else
        split_delimiter( valuesRow, true );
    
    if( format.Nhcols == 0 )
        format.Nhcols = format.headers.size();
    
    // skip rows
    for( int ii=1; ii<format.Nhrows; ii++)
        valuesRow = read_next_row();
    
    return true;
}

void TableIEFile::skip_comments( std::string& row )
{
    try{
        rgx::regex re( format.comment_regexp );
        row = regex_replace(row, re, "");
    }
    catch (rgx::regex_error &e)
    {
        impex_logger->error("what: {}; code: {}", e.what(), parseCode(e.code()));
        JSONIO_THROW( "TableIEFile", 14, std::string(" Regular Expression parse error: ") + e.what() + "  " + format.comment_regexp  );
    }
}

std::string TableIEFile::read_next_data( )
{
    char buffer[2049];
    memset( buffer, '\0', 2049 );
    get_istream().getline( buffer, 2048, format.rowend[0] );
    std::string nextrow = std::string( buffer );
    skip_comments( nextrow );
    if( format.row_size > 0 && !nextrow.empty() )
        nextrow.resize( format.row_size, ' ' );
    return nextrow;
}


std::string TableIEFile::read_next_row( )
{
    std::string next_all_row, nextrow;
    
    if( !format.row_header_regexp.empty() )
    {
        next_all_row += std::move( next_row_data );
        next_row_data = "";
        while( !get_istream().eof() && get_istream().good()  )
        {
            nextrow = read_next_data();
            if( nextrow.empty())
                continue;
            
            if( regexp_test( nextrow, format.row_header_regexp ) && !next_all_row.empty() ) {
                next_row_data = nextrow+format.rowend[0];
                break;
            }
            else {
                next_all_row += nextrow+format.rowend[0];
            }
        }
    }
    else
    {
        int ii= std::max( format.rows_one_block, 1);
        while(  ii > 0 )
        {
            if( get_istream().eof() || !get_istream().good()  )
                break;
            // read next block
            nextrow = read_next_data();
            if( !nextrow.empty() ) // skip empty or comment rows
            {
                next_all_row += nextrow;
                ii--;
            }
        }
    }
    return next_all_row;
}

void TableIEFile::add_value( size_t col, const std::string& value, bool toheader )
{
    if( toheader )
    {
        if( format.headers.size() < col+1 )
            format.headers.resize(col+1);
        format.headers[col] = value;
        return;
    }
    
    if( value.empty() ) // no add empty
        return;
    
    std::vector<std::string> vec;
    vec.push_back( value );
    
    std::string key;
    if( col < format.headers.size() )
        key = format.headers[col];
    if( key.empty())
        key = "clm"+std::to_string(col);
    
    external_data[key] = vec;
}

void TableIEFile::split_fixed_size( const std::string& readedrow, bool toheader )
{
    std::string value, row = readedrow;
    auto colsizes = format.colsizes;
    while( static_cast<int>( colsizes.size()) < format.Nhcols )
        colsizes.push_back( colsizes.back() );
    
    for( size_t ii=0; ii<colsizes.size(); ii++)
    {
        if( row.empty())
            break;
        value = row.substr( 0, colsizes[ii] );
        row = row.substr( std::min<size_t>( colsizes[ii], row.size() ));
        trim( value );
        add_value( ii, value, toheader );
    }
}

void TableIEFile::split_regexpr( const std::string& readedrow, bool toheader )
{
    size_t ii=0;
    std::string value, row = readedrow;
    auto rgex = format.value_regexp;
    while( static_cast<int>( rgex.size() ) < format.Nhcols )
        rgex.push_back(rgex[0]);
    
    try{
        for( ii=0; ii<rgex.size(); ii++)
        {
            rgx::regex re(rgex[ii]);
            rgx::smatch sm;
            if(rgx::regex_search(row, sm, re))
            {
                for (rgx::smatch::iterator it = sm.begin(); it!=sm.end(); ++it)
                {
                    value = *it;
                }
                //value = sm.str();
                row = sm.suffix();
                add_value( ii, value, toheader);
            }
            else
                add_value( ii, "", toheader);
        }
    }
    catch (rgx::regex_error &e)
    {
        impex_logger->error("what: {}; code: {}", e.what(), parseCode(e.code()));
        JSONIO_THROW( "TableIEFile", 15, std::string(" Regular Expression parse error: ") + e.what() + "  " + rgex[ii] );
    }
    
}

size_t TableIEFile::extract_quoted( const std::string& row, std::string& value )
{
    rgx::regex re("\"([^\"]+)\"");
    rgx::smatch sm;
    if( rgx::regex_search(row, sm, re ))
    {
        value = sm.str();
        return value.length()+2;
    }
    return 0;
}

void TableIEFile::split_delimiter( const std::string& readedrow, bool toheader )
{
    size_t ii=0;
    std::string value, row = readedrow;
    
    std::string::size_type start = 0;
    auto pos = row.find_first_of(format.colends, start);
    while(pos != std::string::npos)
    {
        if( !format.usemore || pos != start) // ignore empty tokens
        {
            if(format.usequotes && row[start]== '\"' )
            {
                auto dlt = extract_quoted( std::string( row, start), value );
                if( dlt )
                    pos = start + dlt;
                else
                    value = std::string(row, start, pos - start);
            }
            else
                value = std::string(row, start, pos - start);
            
            trim( value );
            add_value( ii++, value, toheader );
        }
        start = pos + 1;
        pos = row.find_first_of( format.colends, start );
    }
    if( start < row.length() ) // ignore trailing delimiter
    {
        value = std::string( row, start, row.length() - start );
        trim( value );
        add_value( ii, value, toheader );
    }
}

bool TableIEFile::writeBlockToFile( const key_values_table_t& data_to_write )
{
    std::vector<std::string> values;
    auto colsizes = format.colsizes;
    auto rgex = format.value_regexp;
    if( !format.colsizes.empty() )
    {
        while( colsizes.size() < format.headers.size() )
            colsizes.push_back(colsizes[0]);
    }
    else
        if( !format.value_regexp.empty() )
        {
            while( rgex.size() < format.headers.size() )
                rgex.push_back(rgex[0]);
        }
    
    size_t ii=0;
    for( auto const& colhead: format.headers )
    {
        values = extract_value( colhead, data_to_write );
        if( values.empty() )
            values.push_back(" ");
        
        if( !colsizes.empty() )
        {
            values[0].resize(colsizes[ii++], ' ');
            get_ostream() << values[0];
        }
        else
            if( !rgex.empty() )
                get_ostream() << regexp_replace( rgex[ii++], "%value", values[0] );
            else
                get_ostream() << values[0] << format.colends;
    }
    get_ostream() << format.rowend;
    return true;
}


} // namespace jsonio17
