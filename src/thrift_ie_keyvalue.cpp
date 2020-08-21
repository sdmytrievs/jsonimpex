#include "thrift_ie_keyvalue.h"
#include <regex>
namespace rgx = std;

namespace jsonio17 {

KeyValueIEFile* connectFormatKeyValueFile( int amode, const JsonBase& object, const std::string& lua_lib_path )
{
    FormatKeyValueFile fformatdata;
    readDataFromJsonSchema( object, "FormatKeyValueFile",  &fformatdata );
    //Create input structure
    KeyValueIEFile*  inputIEFile=  new  KeyValueIEFile( static_cast<ImpexFormatFile::ImpexMode>(amode), fformatdata.label, lua_lib_path, fformatdata );
    return inputIEFile;
}

//--------------------------------------------------------------------------
// Definition of text file with key-value pair data

KeyValueIEFile::KeyValueIEFile( ImpexFormatFile::ImpexMode amode, const std::string& data_schema_name, const std::string& alualibPath,
           const FormatKeyValueFile& defData ):
           AbstractIEFile( amode, data_schema_name, alualibPath),
           format(defData.format), renderer(defData.renderer)
{
   // set up data from  FormatKeyValueFile !!!
    label = defData.label;
    comment = defData.comment;
    file_name = defData.fname;
    block = defData.block;
    Nblocks = defData.Nblocks;
    Nlines = defData.Nlines;
    structNode = JsonDomSchema::newSchemaObject( label );
    str_buf = "";
    test_start = str_buf.begin();
    test_end = str_buf.end();

}

// Open file to import
void KeyValueIEFile::openFile( const std::string& inputFname )
{
    use_string_stream = false;

    if( !inputFname.empty() )
        file_name = inputFname;

    // open  file
    if (_fstream.is_open())
        _fstream.close();

    if( _mode == modeImport )
        _fstream.open(file_name,  std::fstream::in );
    else
        _fstream.open(file_name,  std::fstream::out|std::fstream::app );

    jsonioErrIf( !_fstream.good() , file_name, "Fileopen error...");
}

// Close internal file
void KeyValueIEFile::closeFile()
{
    // close file
    if (_fstream.is_open())
        _fstream.close();
}

void KeyValueIEFile::loadString(const std::string &inputData)
{
    use_string_stream = true;

    if (_fstream.is_open())
        _fstream.close();

    if( _mode == modeImport )
        _stringstream.str(inputData );
    else
        _stringstream.str("");
    jsonioErrIf( !_stringstream.good(), "loadString", "Loading String error...");

}


/// Read next block of data from file
bool  KeyValueIEFile::readNextBlockfromFile()
{
    // clear analyzed block
    str_buf = std::string(test_start, test_end);
    // delete spaces
    if( !str_buf.empty() )
    { std::string::size_type pos1 = str_buf.find_first_not_of(" \t\r\n");
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
    memset(buffer,'\0', 2049 );
    // read next block
    get_istream().read( buffer, 2048 );
    str_buf += std::string(buffer);
    test_start = str_buf.begin();
    test_end = str_buf.end();
    //cout << endl << "!!!!" << _str_buf.c_str() << "\n<<<" << endl;

    return true;
}

/// Skip comments and blanks
void KeyValueIEFile::skipComments()
{
    if( str_buf.empty() )
      return;

    std::string::size_type posb;
    std::vector<std::string> mach_str;

    do{
        if( !mach_str.empty() )
          insertKeyValue( "comment", mach_str );

        posb = str_buf.find_first_not_of(" \t\r\n",test_start-str_buf.begin() );
        if( posb != std::string::npos )
          test_start = str_buf.begin()+posb;
        else
          test_start = test_end;

        if( test_start == test_end )
              break;
    } while( !format.comment_regexp.empty() && readRegexp( format.comment_regexp, mach_str ) );

}

///  Test/read next regexp
///  \param  re_str - string with Regular Expression
///  \param  mach_str - list of matched strings
///  \return true if matched Expression
bool KeyValueIEFile::readRegexp( const std::string& re_str, std::vector<std::string>& mach_str )
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

   while(rgx::regex_search(test_start, test_end, what, expression, flags))
   {
      // match only partial
      if( !what[0].matched  )
      {
          if( !readNextBlockfromFile() )
            return false; // end of file
      }
      else
      { // what[0] contains the whole string
        // what[>0] contains the tokens.

        for( uint ii=1; ii < what.size(); ii++ )
        { //cout <<  what[ii] << endl;
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
      jsonioErr("Error in Regular Expression", e.what(), re_str);
    }
  return false;
}

///  Only Test next regexp
///  \param  re_str - string with Regular Expression
///  \return true if matched Expression
bool KeyValueIEFile::testRegexp( const std::string& re_str )
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
        if( !readNextBlockfromFile())
            return false; // end of file
      }
      else
        return true;
   }
  }
   catch (rgx::regex_error &e)
    {
      std::cout << "what: " << e.what() << "; code: " << parseCode(e.code()) << std::endl;
      jsonioErr("Error in Regular Expression", e.what(), re_str);
    }
  return false;
}

/// Read header of block
bool KeyValueIEFile::readStartBlock()
{
  // read next block of data from file
  readNextBlockfromFile();

  // skip comments and blanks
  skipComments();

  // nothing to test
  if( str_buf.empty() )
    return false;

  // no head condition
  if( format.head_regexp.empty() )
    return true;

  // read header of block
  std::vector<std::string> mach_str;
  bool ret = readRegexp( format.head_regexp, mach_str );
  if( ret )
    for(uint ii=0; ii<mach_str.size(); ii++ )
    {
      std::string _key = "head"+std::to_string(ii);
      addKeyValue( _key, mach_str[ii] );
    }

  return ret;
}

/// Read&test end block
bool KeyValueIEFile::isEndBlock()
{
    bool ret;

    // skip comments and blanks
    skipComments();

    // no end condition (test next head condition or end of file )
    if( format.end_regexp.empty() )
    {
        if( test_start == test_end )
         ret = true;
        else
         ret = testRegexp(  format.head_regexp );
    } else
      { // read end of block
        std::vector<std::string> mach_str;
        ret = readRegexp(  format.end_regexp, mach_str );
        if( ret )
          for(uint ii=0; ii<mach_str.size(); ii++ )
          {
            std::string _key = "end"+std::to_string(ii);
            addKeyValue( _key, mach_str[ii] );
          }
    }
   return ret;
}

/// Read value untill next key or end of block
std::string KeyValueIEFile::readAllValue()
{
  std::string value = "";
  size_t posb;

  auto pos = test_start;
  while( pos != test_end )
  {
      // test next key;
      if( testRegexp(  format.key_regexp ) )
       break;

      // test next end of block
      if( format.end_regexp.empty() )
      {   if( testRegexp(  format.head_regexp ) )
            break;
      } else
          if( testRegexp(  format.end_regexp ) )
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
      {  if( !readNextBlockfromFile() )
           break;
         pos = test_start;
      }
  }
  posb = value.rfind(format.value_next);
  return value.substr(0,posb );
}

/// Read key value pair
bool KeyValueIEFile::readKeyValuePair()
{
    std::string key, value="";
    std::vector<std::string> mach_str;

    // skip comments and blanks
    skipComments();

    // get key
    if( !readRegexp(  format.key_regexp, mach_str ))
      return false;
    key =mach_str[0];

    // skip comments and blanks
    skipComments();

    // get value
    if( !format.value_regexp.empty() )
    {
        if( readRegexp(  format.value_regexp, mach_str ))
           value =mach_str[0];
    }
    else  // value untill end of block or key
    {
       value = readAllValue();
    }

    strip_all(value);
    addKeyValue( key, value );
    return true;
}

/// Inser key value paies to readed list
void KeyValueIEFile::insertKeyValue( const std::string& _key, const std::vector<std::string>& vec )
{
    auto itr = external_data.find(_key);
    if( itr != external_data.end() )
    {    for( uint ii=0; ii<vec.size(); ii++ )
           itr->second.push_back(vec[ii]);
    }
    else
     external_data.insert( std::pair<std::string, std::vector<std::string>>(_key, vec) );

}

/// Add key value data to Readed Key, Value pairs
void KeyValueIEFile::addKeyValue( const std::string& _key, const std::string& _value )
{
    std::vector<std::string> vec;
    vec.push_back(_value);

    auto  organization = impexFieldOrganization( _key );

    //  here we test if key is array type in  block.pairs type
    //  we split _value to vector use format.value_part_regexp
    if( organization == "list" ||   organization == "set" ||
        organization == "map" ||     organization == "group"  )
      vec = regexp_split(_value, format.value_token_regexp );

    if( organization == "group"  )
    {
          for( uint ii=0; ii<vec.size(); ii++)
           insertKeyValue( _key+std::to_string(ii), { vec[ii] } );
          return;
    }
    insertKeyValue( _key, vec );
 }


/// Read data from one block in file
bool KeyValueIEFile::readBlockFromFile( )
{
  if( !readStartBlock() )
      return false;

  int ii=0;
  while( !isEndBlock() )
  {
      if( format.Ndata>0 && ii > format.Ndata )
        return true;
      if( !readKeyValuePair() )
         jsonioErr("ReadKeyValuePair -error ",
                   std::string(test_start, min(test_start+100, test_end)));
      ii++;
  }
  /* test output
  auto it = externData.begin();
  while( it != externData.end() )
  {
   cout << " key " << it->first ;
   for(uint ii=0; ii< it->second.size(); ii++ )
    cout << "         "  << it->second[ii].c_str() << endl;
   it++;
  }*/
  return true;
}


// Convert free format data to schema defined
bool KeyValueIEFile::initBlock(   std::string& blockdata,  const ValuesMap& areadedData )
{
  // read data from block to readedData
    external_data = areadedData;
  // reset default structure
    structNode->clearField();
  // write data to structNode
     setReadedData();
  // Run script for operation on data values in block
     run_block_script(block.script, structNode );

  // return data to json string
     printNodeToJson( blockdata, structNode );
     return true;
}

// export

bool KeyValueIEFile::writeBlockToFile( const ValuesMap& writeData_ )
{
    externData = writeData_;
    assembleGroups( externData  );
    std::vector<std::string> impexValues;
    std::string impexOrganization, impexType;
    uint impexSize;

    // write head
    std::vector<std::string> tokens = regexp_extract( format.head_regexp, "%head\\d+");
    std::string headstr = format.head_regexp;
    for( auto token: tokens )
    {
        impexValues = extractValue( token.substr(1), externData );
        if( impexValues.size() > 0 )
             headstr = regexp_replace( headstr, token, impexValues[0] );
    }
    get_ostream() << headstr;

    //build order list
    auto orderFields = format.key_order;
    if( orderFields.empty() )
      for( auto item: external_data )
      {
        auto impexKey = item.first;
        if( impexKey.find("head") == std::string::npos && impexKey.find("end") == std::string::npos)
         orderFields.push_back(impexKey);
      }

    // write matches (use orderFields )
    for( auto impexKey: orderFields )
    {
      auto idom = external_data.find(impexKey);
      if( idom == external_data.end() ) //not exist
         continue;
      impexValues = idom->second;
      if( impexValues.empty() )
         continue;

      get_ostream() << regexp_replace( format.key_regexp, "%key", impexKey );

      impexOrganization = impexFieldOrganization( impexKey );
      if( impexOrganization=="list" || impexOrganization == "set" ||
              impexOrganization == "group" )
       impexSize = impexValues.size();
      else
       impexSize = 1;

      impexType = impexFieldType( impexKey );
      for( uint ii=0; ii< impexSize; ii++  )
       if( !format.strvalue_exp.empty()  && impexType == "string" )
         get_ostream() << regexp_replace( format.strvalue_exp, "%value", impexValues[ii] ) << format.value_token_regexp;
       else
         get_ostream() << regexp_replace( format.value_regexp, "%value", impexValues[ii] ) << format.value_token_regexp;

      get_ostream() << format.value_next;
    }

    // write end of block
    tokens = regexp_extract( format.end_regexp, "%end\\d+");
    std::string endstr = format.end_regexp;
    for( auto token: tokens )
    {
        impexValues = extractValue( token.substr(1), external_data );
        if( impexValues.size() > 0 )
             endstr = regexp_replace( endstr, token, impexValues[0] );
    }
    get_ostream() << endstr;
   return true;
}

} // namespace jsonio17
