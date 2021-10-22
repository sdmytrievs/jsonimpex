
#pragma once

#include <memory>
#include <functional>
#include "jsonio17/jsonbase.h"
#include "jsonio17/dbvertexdoc.h"

namespace jsonio17 {

class AbstractIEFile;

/// Factory method creating import/export format implementation from a format name
using  IEFileFactory_f = std::function< AbstractIEFile*( int amode, const JsonBase& object, const std::string& lua_lib_path ) >;


/// Class ImpexFormatFile - implementation of import/export to/from foreign format files.
/// Prepared an "impex.thrift" file describing 3 file formats for import/export of data
/// to internal store schema-based JSON object.
class ImpexFormatFile
{

public:

    /// Type of mode: import or export
    enum ImpexMode {
        modeImport = 0, modeExport = 1, modeUpdate = 2
    };

    /// Registering new import/export format and Factory Method where instances are actually created
    static void addFormat( const std::string& impex_format_name, IEFileFactory_f format_method )
    {
        formats[impex_format_name] = format_method;
    }
    /// Registering default import/export formats and Factory Method where instances are actually created
    static void initDefaultFormats();

    /// The constructor
    explicit ImpexFormatFile( ImpexMode mode_of_use, const std::string& impex_format_name, const JsonBase& object );

    virtual ~ImpexFormatFile();

    /// Name of the current data schema name
    const std::string&  dataName() const;

    /// Load file to import/export from/to
    virtual void openFile( const std::string& file_path );
    /// Close internal file
    virtual void closeFile();

    /// Load string to import from ( or export to stringstream )
    virtual void loadString( const std::string& input_data );
    /// Get string export to ( if export to stringstream )
    virtual std::string getString();


    // Import part -----------------------------------------------------------------

    /// Read one block to json data
    bool readBlock( std::string& json_block_data );
    /// Convert free format json to schema defined ( FormatStructDataFile only )
    bool readBlock(  const std::string& input_json, std::string& schema_json, std::string& key, int ndx=-1 );
    /// Read next json object  ( FormatStructDataFile only )
    bool nextFromFile( std::string& input_json );

    // Update part -----------------------------------------------------------------

    /// Function to update documents, either value in existing fields, or add new fields while leaving the rest of the record unchanged.
    bool updateBlock( DBVertexDocument* dbdocument, std::string& json_block_data );

    // Export part -----------------------------------------------------------------

    /// Write data from one block to file
    bool writeBlockToFile( const key_values_table_t& data_to_write );
    /// Extract data from Json to structure
    void extractDataToWrite( const std::string& json_block_data, key_values_table_t& data_to_write );
    /// Write one block to extern format file
    bool writeBlock( const std::string& json_block_data );

protected:

    /// Dictionary of import/export formats  for Factory Method
    static std::map<std::string, IEFileFactory_f> formats;

    /// Implementation of the current import/export format
    std::shared_ptr<AbstractIEFile> impex_format;

    AbstractIEFile* create_format_data( ImpexMode amode, const std::string& impex_format_name, const JsonBase& object );

};


} // namespace jsonio17

