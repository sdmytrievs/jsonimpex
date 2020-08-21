#pragma once

#include "jsonimpex17/yaml_xml2file.h"
#include "thrift_ie_base.h"


namespace jsonio17 {


//--------------------------------------------------------------------------
// Export/import of data records to/from text file in legacy JSON/YAML/XML (foreign keywords)
// Maybe using a generated JSON schema of that input file format?

/// Definition of foreign structured data JSON/YAML/XML text file
class  StructDataIEFile: public AbstractIEFile
{
   std::shared_ptr<JsonYamlXMLArrayFile> _stream;   ///< Stream to input/output

   /** Rendering syntax for the foreign file "JSON" | "YAML" | "XML" | ... */
   std::string renderer = "JSON";

  // internal values
   char _ftype;    ///< Input data type "JSON" | "YAML" | "XML" ...

  // internal functions

  /// Read data from one block in file
  bool readBlockFromFile();
  /// Read data one block from json data
  bool readBlockFromBson( const std::string& inputjson, std::string& key, int ndx=-1 );

 public:

   StructDataIEFile( ImpexFormatFile::ImpexMode amode, const std::string& data_schema_name, const std::string& alualibPath,
                     const FormatStructDataFile& defData );

   ~StructDataIEFile() {}

   /// Open file to import
   void openFile( const std::string& inputFname );
   /// Close internal file
   void closeFile();

   /// Read next bson object
   bool nextFromFile( std::string& inputjson );

   /// Convert free format bson to schema defined
   bool readBlock(  const std::string& inputjson, std::string& schemajson, std::string& key, int ndx=-1 );

   // export part -----------------------------------------------------------------

   /// write data from one block to file
   virtual bool writeBlockToFile( const key_values_table_t&  )
   {
       return true;
   }

};

} // namespace jsonio17

