#include "thrift_ie_keyvalue.h"
#include <regex>
namespace rgx = std;

namespace jsonio17 {

KeyValueIEFile* connectFormatKeyValueFile( int amode, const JsonBase& object, const std::string& lua_lib_path )
{
    FormatKeyValueFile fformatdata;
    readDataFromJsonSchema( object, "FormatKeyValueFile",  &fformatdata );
    //Create input structure
    KeyValueIEFile* inputIEFile = new KeyValueIEFile( static_cast<ImpexFormatFile::ImpexMode>(amode),
                                                      fformatdata.label, lua_lib_path, fformatdata );
    return inputIEFile;
}


// Definition of text file with key-value pair data
KeyValueIEFile::KeyValueIEFile( ImpexFormatFile::ImpexMode amode, const std::string& data_schema_name,
                                const std::string& lua_lib_path,  const FormatKeyValueFile& format_description_data ):
    AbstractIEFile( amode, data_schema_name, lua_lib_path ),
    format( format_description_data.format ), renderer( format_description_data.renderer )
{
    // set up data from  FormatKeyValueFile
    // label = format_description_data.label;
    comment = format_description_data.comment;
    file_name = format_description_data.fname;
    block = format_description_data.block;
    Nblocks = format_description_data.Nblocks;
    Nlines = format_description_data.Nlines;
    str_buf = "";
    test_start = str_buf.begin();
    test_end = str_buf.end();
}

// Open file to import
void KeyValueIEFile::openFile( const std::string& file_path )
{
    use_string_stream = false;

    if( !file_path.empty() )
        file_name = file_path;

    // open  file
    if (file_stream.is_open())
        file_stream.close();

    if( mode_use == ImpexFormatFile::modeImport )
        file_stream.open( file_name,  std::fstream::in );
    else
        file_stream.open( file_name,  std::fstream::out|std::fstream::app );

    JSONIO_THROW_IF( !file_stream.good(), "KeyValueIEFile", 11, " could not open file to import/export from " + file_name);
}

// Close internal file
void KeyValueIEFile::closeFile()
{
    if( file_stream.is_open() )
        file_stream.close();
}

void KeyValueIEFile::loadString(const std::string &input_data)
{
    use_string_stream = true;

    if( file_stream.is_open() )
        file_stream.close();

    if( mode_use == ImpexFormatFile::modeImport )
        string_stream.str( input_data );
    else
        string_stream.str( "" );

    JSONIO_THROW_IF( !file_stream.good(), "KeyValueIEFile", 12, " loading string error" );
}

std::string KeyValueIEFile::getString()
{
    if( use_string_stream)
        return  string_stream.str();
    JSONIO_THROW( "KeyValueIEFile", 13, " try get string when working with file stream" );
}


/// Read next block of data from file
bool  KeyValueIEFile::read_next_block_from_file()
{
    // clear analyzed block
    str_buf = std::string(test_start, test_end);
    // delete spaces
    if( !str_buf.empty() )
    {
        std::string::size_type pos1 = str_buf.find_first_not_of(" \t\r\n");
        if( pos1 != std::string::npos )
            str_buf = str_buf.substr(pos1);
        else
            str_buf = "";
    }
    test_start = str_buf.begin();
    test_end = str_buf.end();

    if( get_istream().eof() || !get_istream().good()  )
        return false;

    //let's say read by 2048 char block
    char buffer[2049];
    memset( buffer,'\0', 2049 );
    // read next block
    get_istream().read( buffer, 2048 );
    str_buf += std::string(buffer);
    test_start = str_buf.begin();
    test_end = str_buf.end();
    //cout << endl << "!!!!" << str_buf.c_str() << "\n<<<" << endl;

    return true;
}

// Skip comments and blanks
void KeyValueIEFile::skip_comments()
{
    if( str_buf.empty() )
        return;

    std::string::size_type posb;
    std::vector<std::string> mach_str;

    do{
        if( !mach_str.empty() )
            insert_key_value( "comment", mach_str );

        posb = str_buf.find_first_not_of( " \t\r\n", test_start-str_buf.begin() );
        if( posb != std::string::npos )
            test_start = str_buf.begin()+posb;
        else
            test_start = test_end;

        if( test_start == test_end )
            break;
    } while( !format.comment_regexp.empty() && read_regexp( format.comment_regexp, mach_str ) );

}

///  Test/read next regexp
///  \param  re_str - string with Regular Expression
///  \param  mach_str - list of matched strings
///  \return true if matched Expression
bool KeyValueIEFile::read_regexp( const std::string& re_str, std::vector<std::string>& mach_str )
{
    try
    {
        mach_str.clear();

        if( test_start == test_end )
            return false;

        rgx::match_results<std::string::const_iterator> what;
        //rgx::match_flag_type flags = rgx::match_continuous| rgx::match_partial;
        rgx::regex_constants::match_flag_type flags = rgx::regex_constants::match_continuous;//| rgx::regex_constants::match_partial;
        const rgx::regex expression( re_str.c_str() );

        while( rgx::regex_search( test_start, test_end, what, expression, flags ) )
        {
            // match only partial
            if( !what[0].matched  )
            {
                if( !read_next_block_from_file() )
                    return false; // end of file
            }
            else
            {
                // what[0] contains the whole string
                // what[>0] contains the tokens.

                for( size_t ii=1; ii < what.size(); ii++ )
                {
                    //cout <<  what[ii] << endl;
                    mach_str.push_back(what[ii]);
                }
                // update search position:
                test_start = what[0].second;
                return true;
            }
        }

    }
    catch (rgx::regex_error &e)
    {
        std::cout << "what: " << e.what() << "; code: " << parseCode(e.code()) << std::endl;
        JSONIO_THROW( "KeyValueIEFile", 14, std::string(" Regular Expression parse error: ") + e.what() + "  " + re_str  );
    }
    return false;
}

///  Only Test next regexp
///  \param  re_str - string with Regular Expression
///  \return true if matched Expression
bool KeyValueIEFile::test_regexp( const std::string& re_str )
{
    try
    {
        //if( test_start == test_end )
        // return false;

        rgx::match_results<std::string::const_iterator> what;
        //rgx::match_flag_type flags = rgx::match_continuous| rgx::match_partial;
        rgx::regex_constants::match_flag_type flags = rgx::regex_constants::match_continuous;//| rgx::regex_constants::match_partial;
        const rgx::regex expression( re_str.c_str() );
        while(rgx::regex_search(test_start, test_end, what, expression, flags))
        {
            // match only partial
            if( !what[0].matched  )
            {
                if( !read_next_block_from_file())
                    return false; // end of file
            }
            else
                return true;
        }
    }
    catch (rgx::regex_error &e)
    {
        std::cout << "what: " << e.what() << "; code: " << parseCode(e.code()) << std::endl;
        JSONIO_THROW( "KeyValueIEFile", 15, std::string(" Regular Expression parse error: ") + e.what() + "  " + re_str  );
    }
    return false;
}

/// Read header of block
bool KeyValueIEFile::read_start_block()
{
    // read next block of data from file
    read_next_block_from_file();

    // skip comments and blanks
    skip_comments();

    // nothing to test
    if( str_buf.empty() )
        return false;

    // no head condition
    if( format.head_regexp.empty() )
        return true;

    // read header of block
    std::vector<std::string> mach_str;
    bool ret = read_regexp( format.head_regexp, mach_str );
    if( ret )
        for( size_t ii=0; ii<mach_str.size(); ii++ )
        {
            std::string akey = "head"+std::to_string(ii);
            add_key_value( akey, mach_str[ii] );
        }

    return ret;
}

/// Read&test end block
bool KeyValueIEFile::is_end_block()
{
    bool ret;

    // skip comments and blanks
    skip_comments();

    // no end condition (test next head condition or end of file )
    if( format.end_regexp.empty() )
    {
        if( test_start == test_end )
            ret = true;
        else
            ret = test_regexp(  format.head_regexp );
    }
    else
    {  // read end of block
        std::vector<std::string> mach_str;
        ret = read_regexp(  format.end_regexp, mach_str );
        if( ret )
            for( size_t ii=0; ii<mach_str.size(); ii++ )
            {
                std::string akey = "end"+std::to_string(ii);
                add_key_value( akey, mach_str[ii] );
            }
    }
    return ret;
}

/// Read value untill next key or end of block
std::string KeyValueIEFile::read_all_value()
{
    std::string value = "";
    size_t posb;

    auto pos = test_start;
    while( pos != test_end )
    {
        // test next key;
        if( test_regexp(  format.key_regexp ) )
            break;

        // test next end of block
        if( format.end_regexp.empty() )
        {   if( test_regexp(  format.head_regexp ) )
                break;
        } else
            if( test_regexp(  format.end_regexp ) )
                break;

        if( format.value_next.empty() )
        {
            pos+=1;   // only next symbol
        }
        else
        {
            posb =  test_start-str_buf.begin();
            posb = str_buf.find( format.value_next, posb );
            if( posb != std::string::npos )
                pos = str_buf.begin()+posb+format.value_next.length();
            else
                pos = test_end;
        }
        value += std::string( test_start, pos );
        test_start = pos;

        if( pos == test_end )
        {
            if( !read_next_block_from_file() )
                break;
            pos = test_start;
        }
    }
    posb = value.rfind( format.value_next );
    return value.substr( 0,posb );
}

/// Read key value pair
bool KeyValueIEFile::read_key_value_pair()
{
    std::string key, value="";
    std::vector<std::string> mach_str;

    // skip comments and blanks
    skip_comments();

    // get key
    if( !read_regexp(  format.key_regexp, mach_str ))
        return false;
    key = mach_str[0];

    // skip comments and blanks
    skip_comments();

    // get value
    if( !format.value_regexp.empty() )
    {
        if( read_regexp(  format.value_regexp, mach_str ))
            value = mach_str[0];
    }
    else  // value untill end of block or key
    {
        value = read_all_value();
    }

    trim( value );
    add_key_value( key, value );
    return true;
}

/// Inser key value paies to readed list
void KeyValueIEFile::insert_key_value( const std::string& key, const std::vector<std::string>& values )
{
    auto itr = external_data.find( key );
    if( itr != external_data.end() )
    {
        for( const auto& val: values )
            itr->second.push_back( val );
    }
    else
        external_data[key] = values;

}

/// Add key value data to Readed Key, Value pairs
void KeyValueIEFile::add_key_value( const std::string& key, const std::string& value )
{
    std::vector<std::string> values;

    auto  organization = impex_field_organization( key );

    //  here we test if key is array type in  block.pairs type
    //  we split _value to vector use format.value_part_regexp
    if( organization == "list" ||   organization == "set" ||
            organization == "map" ||     organization == "group"  )
        values = regexp_split( value, format.value_token_regexp );
    else
        values = { value };

    if( organization == "group"  )
    {
        for( size_t ii=0; ii<values.size(); ii++)
            insert_key_value( key+std::to_string(ii), { values[ii] } );
        return;
    }
    insert_key_value( key, values );
}


/// Read data from one block in file
bool KeyValueIEFile::read_block_from_file( )
{
    if( !read_start_block() )
        return false;

    int ii=0;
    while( !is_end_block() )
    {
        if( format.Ndata>0 && ii > format.Ndata )
            return true;
        if( !read_key_value_pair() )
            JSONIO_THROW( "KeyValueIEFile", 16, std::string(" read key value pair error : ")+std::string( test_start, min( test_start+100, test_end )  ));
        ii++;
    }
    /* test output
    for( const auto& it: external_data )
    {
        std::cout << " key " << it.first ;
        for(size_t ii=0; ii< it.second.size(); ii++ )
            std::cout << "         "  << it.second[ii].c_str() << std::endl;
    }*/
    return true;
}

// export

bool KeyValueIEFile::writeBlockToFile( const key_values_table_t& data_to_write )
{
    external_data = data_to_write;
    assemble_groups( external_data  );
    std::vector<std::string> impexValues;
    std::string impexOrganization, impexType;
    size_t impexSize;

    // write head
    std::vector<std::string> tokens = regexp_extract( format.head_regexp, "%head\\d+");
    std::string headstr = format.head_regexp;
    for( auto const& token: tokens )
    {
        impexValues = extract_value( token.substr(1), external_data );
        if( impexValues.size() > 0 )
            headstr = regexp_replace( headstr, token, impexValues[0] );
    }
    get_ostream() << headstr;

    //build order list
    auto orderFields = format.key_order;
    if( orderFields.empty() )
        for( auto const& item: external_data )
        {
            auto impexKey = item.first;
            if( impexKey.find("head") == std::string::npos && impexKey.find("end") == std::string::npos)
                orderFields.push_back(impexKey);
        }

    // write matches (use orderFields )
    for( auto const& impexKey: orderFields )
    {
        auto idom = external_data.find(impexKey);
        if( idom == external_data.end() ) //not exist
            continue;
        impexValues = idom->second;
        if( impexValues.empty() )
            continue;

        get_ostream() << regexp_replace( format.key_regexp, "%key", impexKey );

        impexOrganization = impex_field_organization( impexKey );
        if( impexOrganization=="list" || impexOrganization == "set" ||
                impexOrganization == "group" )
            impexSize = impexValues.size();
        else
            impexSize = 1;

        impexType = impex_field_type( impexKey );
        for( size_t ii=0; ii< impexSize; ii++  )
            if( !format.strvalue_exp.empty()  && impexType == "string" )
                get_ostream() << regexp_replace( format.strvalue_exp, "%value", impexValues[ii] ) << format.value_token_regexp;
            else
                get_ostream() << regexp_replace( format.value_regexp, "%value", impexValues[ii] ) << format.value_token_regexp;

        get_ostream() << format.value_next;
    }

    // write end of block
    tokens = regexp_extract( format.end_regexp, "%end\\d+");
    std::string endstr = format.end_regexp;
    for( auto const& token: tokens )
    {
        impexValues = extract_value( token.substr(1), external_data );
        if( impexValues.size() > 0 )
            endstr = regexp_replace( endstr, token, impexValues[0] );
    }
    get_ostream() << endstr;
    return true;
}

std::string parseCode(/*regex_constants::error_type*/ int etype)
{
    switch (etype)
    {
    case rgx::regex_constants::error_collate:
        return "error_collate: invalid collating element request";
    case rgx::regex_constants::error_ctype:
        return "error_ctype: invalid character class";
    case rgx::regex_constants::error_escape:
        return "error_escape: invalid escape character or trailing escape";
    case rgx::regex_constants::error_backref:
        return "error_backref: invalid back reference";
    case rgx::regex_constants::error_brack:
        return "error_brack: mismatched bracket([ or ])";
    case rgx::regex_constants::error_paren:
        return "error_paren: mismatched parentheses(( or ))";
    case rgx::regex_constants::error_brace:
        return "error_brace: mismatched brace({ or })";
    case rgx::regex_constants::error_badbrace:
        return "error_badbrace: invalid range inside a { }";
    case rgx::regex_constants::error_range:
        return "erro_range: invalid character range(e.g., [z-a])";
    case rgx::regex_constants::error_space:
        return "error_space: insufficient memory to handle this regular expression";
    case rgx::regex_constants::error_badrepeat:
        return "error_badrepeat: a repetition character (*, ?, +, or {) was not preceded by a valid regular expression";
    case rgx::regex_constants::error_complexity:
        return "error_complexity: the requested match is too complex";
    case rgx::regex_constants::error_stack:
        return "error_stack: insufficient memory to evaluate a match";
    default:
        return "";
    }
}

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


} // namespace jsonio17
