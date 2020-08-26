#pragma once

#include "jsonimpex17/yaml_xml2file.h"
#include "thrift_ie_base.h"


namespace jsonio17 {


/// Class StructDataIEFile - export/import of data records to/from text file in legacy JSON/YAML/XML (foreign keywords).
/// Using a generated JSON schema of that input file format
class  StructDataIEFile: public AbstractIEFile
{
public:

    /// The constructor
    explicit StructDataIEFile( ImpexFormatFile::ImpexMode amode, const std::string& data_schema_name,
                               const std::string& lua_lib_path, const FormatStructDataFile& format_description_data );

    /// Destructor
    ~StructDataIEFile() {}

    /// Load file to import/export from/to
    void openFile( const std::string& file_path ) override;
    /// Close internal file
    void closeFile() override;

    /// Load string to import from ( or export to stringstream )
    void loadString( const std::string& input_data ) override;
    /// Get string export to ( if export to stringstream )
    std::string getString() override;


    // Import part -----------------------------------------------------------------

    /// Read next json object
    bool nextFromFile( std::string& inputjson ) override;

    /// Convert free format json to schema defined
    bool readBlock(  const std::string& inputjson, std::string& schemajson, std::string& key, int ndx=-1 ) override;


    // export part -----------------------------------------------------------------

    /// Write data from one block to file
    virtual bool writeBlockToFile( const key_values_table_t& data_to_write ) override;

protected:

    /// Rendering syntax for the foreign file "JSON" | "YAML" | "XML" | ...
    std::string renderer = "JSON";

    /// Input (other ) data type "JSON" | "YAML" | "XML" ...
    JsonYamlXMLFile::FileTypes foreign_data_type;

    /// Stream to input/output foreign data
    std::shared_ptr<JsonYamlXMLArrayFile> ie_stream;


    /// Read data from one block in file
    bool read_block_from_file() override;

    /// Read data one block from json data
    bool read_block_from_json( const std::string& inputjson, std::string& key, int ndx=-1 );

    bool extract_values(const std::string &key, const JsonFree &foreign_node, values_t &values) const;
};

StructDataIEFile* connectFormatStructDataFile( int amode, const JsonBase& object, const std::string& lua_lib_path );
// ImpexFormatFile::formats["FormatStructDataFile"] = connectFormatStructDataFile;
// ImpexFormatFile::addFormat("FormatStructDataFile", connectFormatStructDataFile);


} // namespace jsonio17

