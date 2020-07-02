#include <fstream>
#include "jsonio14/jsondump.h"
#include "jsonimpex14/yaml_xml2file.h"
#include "jsonimpex14/yamldump.h"
#include "jsonimpex14/xmldump.h"


namespace jsonio14 {

//  JsonYamlXMLFile   --------------------------------------------------------------

TxtFile::FileTypes JsonYamlXMLFile::default_type = TxtFile::Json;

TxtFile::FileTypes JsonYamlXMLFile::type_from_extension(const std::string &ext)
{
    FileTypes type =  default_type;
    if( ext == "json" )
        type = Json;
    else  if( ext == "yaml" )
        type = Yaml;
    else if( ext == "xml" )
        type = XML;
    return type;
}

void JsonYamlXMLFile::set_type(TxtFile::FileTypes ftype)
{
    if( !( ftype == Json ||  ftype == Yaml || ftype == XML ) )
        JSONIO_THROW( "JsonYamlXMLFile", 10, " illegal file type." );
    file_type =  ftype;
}


void JsonYamlXMLFile::loadYaml( JsonBase &object ) const
{
    auto data_txt = TxtFile::load_all();
    yaml::loads( data_txt,  object );
}

void JsonYamlXMLFile::saveYaml( const JsonBase &object ) const
{
    std::fstream fout(file_path, std::ios::out );
    JSONIO_THROW_IF( !fout.good(), "JsonYamlXMLFile", 11, "file save error...  " + file_path );
    yaml::dump( fout, object );
}

void JsonYamlXMLFile::loadXml( JsonBase &object ) const
{
    auto data_txt = TxtFile::load_all();
    XML::loads( data_txt,  object );
}

void JsonYamlXMLFile::saveXml( const JsonBase &object ) const
{
    std::fstream fout(file_path, std::ios::out );
    JSONIO_THROW_IF( !fout.good(), "JsonYamlXMLFile", 12, "file save error...  " + file_path );
    XML::dump( fout, object );
}

void JsonYamlXMLFile::load( JsonBase& object ) const
{
    switch( file_type )
    {
    case FileTypes::Json:
        loadJson( object );
        break;
    case FileTypes::Yaml:
        loadYaml( object );
        break;
    case FileTypes::XML:
        loadXml( object );
        break;
    default:
        break;
    }
}

void JsonYamlXMLFile::save(const JsonBase& object) const
{
    switch( file_type )
    {
      case FileTypes::Json:
          saveJson(  object );
           break;
      case FileTypes::Yaml:
           saveYaml(  object );
           break;
      case FileTypes::XML:
           saveXml(  object );
           break;
      default:
           break;
    }
}


//  JsonYamlXMLArrayFile --------------------------------------------------------------------------


void JsonYamlXMLArrayFile::Close()
{
    if( open_mode == WriteOnly )
    {
        std::fstream fout(file_path, std::ios::out );
        JSONIO_THROW_IF( !fout.good(), "JsonYamlXMLFile", 15, " file save error...  " + file_path );

        switch( file_type )
        {
        case FileTypes::Json:
            json::dump( fout, arr_object );
            break;
        case FileTypes::Yaml:
            yaml::dump( fout, arr_object );
            break;
        case FileTypes::XML:
            XML::dump( fout, arr_object );
            break;
        default:
            break;
        }
    }

    is_opened = false;
}

void JsonYamlXMLArrayFile::Open( TxtFile::OpenModeTypes amode )
{
    if( isOpened() )
    {
        JSONIO_THROW_IF( open_mode!=amode, "JsonYamlXMLFile", 16, "file was opened in different mode  " + file_path );
        return;
    }

    // clear settings
    open_mode  = amode;
    loaded_ndx = 0;
    arr_object.clear();

    if( open_mode == ReadOnly )
    {
        auto data_txt = TxtFile::load_all();

        switch( file_type )
        {
        case FileTypes::Json:
            json::loads( data_txt, arr_object );
            break;
        case FileTypes::Yaml:
            yaml::loads( data_txt, arr_object );
            break;
        case FileTypes::XML:
            XML::loads( data_txt, arr_object );
            break;
        default:
            break;
        }
    }
    else if( open_mode != WriteOnly )
    {
        JSONIO_THROW( "JsonYamlXMLFile", 17, " illegal file open mode." );
    }

    is_opened = true;
}

void JsonYamlXMLArrayFile::set_type( TxtFile::FileTypes ftype )
{
    if( !( ftype == Json ||  ftype == Yaml || ftype == XML ) )
        JSONIO_THROW( "JsonYamlXMLFile", 11, " illegal file type." );
    file_type =  ftype;
}


} // namespace jsonio14
