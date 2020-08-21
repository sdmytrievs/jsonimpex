#include "thrift_ie_table.h"
#include <regex>
namespace rgx = std;

namespace jsonio17 {


TableIEFile* connectFormatTableFile( int amode, const JsonBase& object, const std::string& lua_lib_path )
{
    FormatTableFile fformatdata;
    readDataFromJsonSchema( object, "FormatTableFile",  &fformatdata );
    //Create input structure
    TableIEFile*  inputIEFile=  new  TableIEFile( static_cast<ImpexFormatFile::ImpexMode>(amode), fformatdata.label, lua_lib_path, fformatdata );
    return inputIEFile;
}

//-----------------------------------------------------------------------

TableIEFile::TableIEFile( ImpexFormatFile::ImpexMode amode,const std::string& data_schema_name, const std::string& alualibPath,
                          const FormatTableFile& defData ):
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
}

// Open file to import
void TableIEFile::openFile( const std::string& inputFname )
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

    if( _mode == modeImport )
        readHeaderFromFile();
    ///  else
    ///    writeHeaderToFile();

}

void TableIEFile::loadString(const std::string &inputData)
{
    use_string_stream = true;

    // close  file
    if (_fstream.is_open())
        _fstream.close();

    if( _mode == modeImport )
        _stringstream.str(inputData );
    else
        _stringstream.str("");
    jsonioErrIf( !_stringstream.good(), "loadString", "Loading String error...");

    if( _mode == modeImport )
        readHeaderFromFile();
    ///  else
    ///    writeHeaderToFile();

}


// Close internal file
void TableIEFile::closeFile()
{
    if (_fstream.is_open())
    _fstream.close();
}

/// Read data from one block in file
bool TableIEFile::readBlockFromFile( )
{
  std::string valuesRow = readNextRow();
  if( valuesRow.empty() )
      return false;

 if( !format.colsizes.empty() )
      splitFixedSize( valuesRow );
  else
      if( !format.value_regexp.empty() )
          splitRegExpr( valuesRow );
      else
          splitDelimiter( valuesRow );

  /* test output
  auto it = externData.begin();
  while( it != externData.end() )
  {
   cout << " key " << it->first << ":" ;
   for(uint ii=0; ii< it->second.size(); ii++ )
       cout << " \""  << it->second[ii] << "\" ";
   it++;
  }
  cout << "------------\n";
  */
  return true;
}


/// Read data from one block in file
bool TableIEFile::readHeaderFromFile( )
{
  if( format.Nhrows <= 0)  // no headers
    return  true;

 std:: string valuesRow = readNextRow();
  if( valuesRow.empty() )
      return false;

  if( !format.colsizes.empty() )
      splitFixedSize( valuesRow, true );
  else
      //if( !format.value_regexp.empty() )
      //    splitRegExpr( valuesRow, true );
      //else
          splitDelimiter( valuesRow, true );

  if( format.Nhcols == 0 )
      format.Nhcols = format.headers.size();

  // skip rows
  for( int ii=1; ii<format.Nhrows; ii++)
    valuesRow = readNextRow();

  return true;
}

void TableIEFile::skipComments( std::string& row )
{
  try{
    rgx::regex re(format.comment_regexp);
    row = regex_replace(row, re, "");
    }
     catch (rgx::regex_error &e)
      {
        std::cout << "what: " << e.what() << "; code: " << parseCode(e.code()) << std::endl;
        jsonioErr("Error in Regular Expression", e.what(), format.comment_regexp);
      }
}

std::string TableIEFile::read_next_data( )
{
    char buffer[2049];
    memset(buffer,'\0', 2049 );
    get_istream().getline( buffer, 2048, format.rowend[0] );
    std::string nextrow = std::string(buffer);
    skipComments( nextrow );
    if( format.row_size > 0 && !nextrow.empty() )
        nextrow.resize(format.row_size,' ');
    return nextrow;
}


std::string TableIEFile::readNextRow( )
{
    std::string next_all_row, nextrow;

    if( !format.row_header_regexp.empty() )
    {
        next_all_row += std::move(next_row_data);
        next_row_data = "";
        while( !get_istream().eof() && get_istream().good()  )
        {
            nextrow = read_next_data();
            if( nextrow.empty())
                continue;

            if( regexp_test(nextrow, format.row_header_regexp) && !next_all_row.empty() ) {
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

void TableIEFile::addValue( size_t col, const std::string& _value, bool toheader )
{
    if( toheader )
    {
        if( format.headers.size() < col+1 )
            format.headers.resize(col+1);
        format.headers[col] = _value;
        return;
    }

    if( _value.empty() ) // no add empty
      return;

    std::vector<std::string> vec;
    vec.push_back(_value);

    std::string key;
    if( col < format.headers.size() )
        key = format.headers[col];
    if( key.empty())
        key = "clm"+std::to_string(col);

    externData.insert( std::pair<std::string, std::vector<std::string>>(key, vec) );
}

void TableIEFile::splitFixedSize( const std::string& readedrow, bool toheader )
{
  std::string value, row = readedrow;
  auto colsizes = format.colsizes;
  while( static_cast<int>(colsizes.size()) < format.Nhcols )
    colsizes.push_back(colsizes.back());

  for( size_t ii=0; ii<colsizes.size(); ii++)
  {
      if( row.empty())
        break;
      value = row.substr(0, colsizes[ii]);
      row = row.substr( std::min<size_t>( colsizes[ii], row.size()));
      strip_all(value);
      addValue( ii, value, toheader );
  }
}

void TableIEFile::splitRegExpr( const std::string& readedrow, bool toheader )
{
  uint ii=0;
  std::string value, row = readedrow;
  auto rgex = format.value_regexp;
  while( static_cast<int>(rgex.size()) < format.Nhcols )
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
        addValue( ii, value, toheader);
      }
      else
       addValue( ii, "", toheader);
    }
  }
   catch (rgx::regex_error &e)
    {
      std::cout << "what: " << e.what() << "; code: " << parseCode(e.code()) << std::endl;
      jsonioErr("Error in Regular Expression", e.what(), rgex[ii] );
    }

}

uint TableIEFile::extractQuoted( const std::string& row, std::string& value )
{
     rgx::regex re("\"([^\"]+)\"");
     rgx::smatch sm;
     if(rgx::regex_search(row, sm, re))
     {  value = sm.str();
        return value.length()+2;
     }
     return 0;
}

void TableIEFile::splitDelimiter( const std::string& readedrow, bool toheader )
{
  uint ii=0;
  std::string value, row = readedrow;

  std::string::size_type start = 0;
  auto pos = row.find_first_of(format.colends, start);
  while(pos != std::string::npos)
  {
      if( !format.usemore || pos != start) // ignore empty tokens
      {
        if(format.usequotes && row[start]== '\"' )
         {
           auto dlt = extractQuoted( std::string( row, start), value );
           if( dlt )
            pos = start + dlt;
           else
            value = std::string(row, start, pos - start);
         }
         else
           value = std::string(row, start, pos - start);

        strip(value);
        addValue( ii++, value, toheader);
      }
      start = pos + 1;
      pos = row.find_first_of(format.colends, start);
  }
  if(start < row.length()) // ignore trailing delimiter
  {
      value =std::string (row, start, row.length() - start);
      strip(value);
      addValue( ii, value, toheader);
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
   for( auto colhead: format.headers )
   {
       values = extractValue( colhead, data_to_write );
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
