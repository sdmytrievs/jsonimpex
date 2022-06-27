#pragma once

#include "jsonio17/txt2file.h"

namespace jsonio17 {


/// Class for read/write json/yaml/xml files
class JsonYamlXMLFile : public JsonFile
{

public:

    /// Default file type
    static FileTypes default_type;

    /// Constructor
    JsonYamlXMLFile( const std::string& name, const std::string& ext, const std::string& dir=""):
        JsonFile( name, ext, dir )
    {
        set_type( type_from_extension(file_ext) );
    }

    /// Constructor from path
    JsonYamlXMLFile( const std::string& path): JsonFile( path )
    {
        set_type( type_from_extension(file_ext) );
    }

    /// Load data from file to json object
    void load( JsonBase& object ) const override;

    /// Save data from json object to file
    void save( const JsonBase& object ) const override;

    /// Setup file type
    void set_type( FileTypes ftype );

    /// Get file type from extension
    static FileTypes type_from_extension( const std::string& ext );


protected:

    /// Load data from yaml file to bson object
    void loadYaml( JsonBase& object ) const;

    /// Save data from bson object to yaml file
    void saveYaml( const JsonBase& object ) const;

    /// Load data from xml file to bson object
    void loadXml(  JsonBase& object ) const;

    /// Save data from bson object to xml file
    void saveXml(  const JsonBase& object ) const;

};


/// Class for read/write json/yaml/xml arrays files.
class JsonYamlXMLArrayFile : public JsonArrayFile
{

public:

    /// Constructor
    JsonYamlXMLArrayFile( const std::string& name, const std::string& ext, const std::string& dir="" ):
        JsonArrayFile( name, ext, dir )
    {
        set_type( JsonYamlXMLFile::type_from_extension(file_ext) );
    }
    /// Constructor from path
    explicit JsonYamlXMLArrayFile( const std::string& path ):
        JsonArrayFile( path )
    {
        set_type( JsonYamlXMLFile::type_from_extension(file_ext) );
    }

    /// Destructor
    ~JsonYamlXMLArrayFile()
    {
        if( is_opened )
            Close();
    }

    void Close() override;
    void Open( OpenModeTypes amode ) override;

    /// Load array from string ( do not need open file )
    void loadString( const std::string& input_data ) override;
    /// Export internal data to string
    std::string getString() override;

    /// Setup file type
    void set_type( FileTypes ftype );

protected:

    void read_from_string( const std::string &data_string );
    void save_to_stream( std::ostream &os );

};


} // namespace jsonio17

