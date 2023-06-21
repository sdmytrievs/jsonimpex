#include "jsonimpex17/impex_generator.h"
#include "jsonimpex17/yaml_xml2file.h"

namespace jsonio17 {

//std::string FormatImpexWidget::impex_collection_name = "impexdefs";

//jsonio17::DBSchemaDocument* newDBImpexClient(  const jsonio17::DBQueryBase& query  )
//{
//    return  jsonio17::DBSchemaDocument::newSchemaDocumentQuery(  uiSettings().resources_database(),  "ImpexFormat",
//                                                                 FormatImpexWidget::impex_collection_name,  query );
//}

std::shared_ptr<jsonio17::ImpexFormatFile> FormatImpexGenerator::read_impex_from_file(const std::string& format_file, const std::string& impex_format_name)
{
    auto impex_schema = impex_format_name;
    auto  formatdata = jsonio17::JsonFree::object();
    jsonio17::JsonYamlXMLFile file(format_file);
    file.load(formatdata);
    if(impex_schema.empty()) {
        auto pos = format_file.find_last_of(".");
        impex_schema = format_file.substr(0,pos);
        pos = impex_schema.find_last_of(".");
        if(pos != std::string::npos) {
            impex_schema = impex_schema.substr(pos+1);
        }
    }
    return std::make_shared<jsonio17::ImpexFormatFile>(use_mode, impex_schema, formatdata);
}

std::shared_ptr<jsonio17::ImpexFormatFile> FormatImpexGenerator::read_impex_from_database(const std::string& format_key, const std::string& impex_format_name)
{
    check_dbimpex_connect();
    auto impex_schema = impex_format_name;
    auto  formatdata = jsonio17::JsonFree::object();
    dbimpex->readDocument( format_key );
    std::string impexstr;
    dbimpex->getValueViaPath<std::string>("impex", impexstr, "");
    formatdata.loads(impexstr);
    if(impex_schema.empty()) {
        dbimpex->getValueViaPath<std::string>("impexschema", impex_schema, "");
    }
    return std::make_shared<jsonio17::ImpexFormatFile>( use_mode, impex_schema, formatdata );
}

void  FormatImpexGenerator::file_import(const std::string& foreign_format_file, const std::string& schema_format_file )
{
    check_impex_set();
    std::string record_json;
    jsonio17::JsonYamlXMLArrayFile file(schema_format_file);

    impex_configuration->openFile(foreign_format_file);
    file.Open( jsonio17::TxtFile::WriteOnly );
    while( impex_configuration->readBlock(record_json) ) {
        file.saveNext(record_json);
    }
    file.Close();
    impex_configuration->closeFile();
}

void  FormatImpexGenerator::file_export( const std::string& foreign_format_file, const std::string& schema_format_file )
{
    check_impex_set();
    std::string record_json;
    jsonio17::JsonYamlXMLArrayFile file(schema_format_file);
    impex_configuration->openFile(foreign_format_file);

    file.Open( jsonio17::TxtFile::ReadOnly );
    while( file.loadNext(record_json) ) {
       impex_configuration->writeBlock(record_json);
    }
    impex_configuration->closeFile();
    file.Close();
}


// Read json records from foreign format files according to import configuration
void FormatImpexGenerator::database_import( const std::string& foreign_format_file, bool overwrite)
{
    check_impex_set();
    check_db_connect();
    std::string record_json;
    impex_configuration->openFile(foreign_format_file);

    while( impex_configuration->readBlock(record_json) ) {
        dbdocument->updateFromJson( record_json, overwrite );
    }
    impex_configuration->closeFile();
}

// Update json records from foreign format files according to import configuration
void FormatImpexGenerator::database_update(const std::string& foreign_format_file)
{
    check_impex_set();
    check_db_connect();
    std::string record_json;
    impex_configuration->openFile(foreign_format_file);
    while( impex_configuration->updateBlock( dbdocument.get(), record_json ) )
    {}
    impex_configuration->closeFile();
}

// Export json records to foreign format files according to export configuration
void FormatImpexGenerator::database_export(const std::string& foreign_format_file, const std::vector<std::string>& keys_document)
{
    check_impex_set();
    check_db_connect();
    if(!keys_document.empty()) {
        impex_configuration->openFile(foreign_format_file);
        for( const auto& doc_key: keys_document ) {
            dbdocument->readDocument( doc_key );
            impex_configuration->writeBlock( dbdocument->getJson() );
        }
        impex_configuration->closeFile();
    }
}

void FormatImpexGenerator::check_dbimpex_connect()
{
    if(!dbimpex) {
      JSONIO_THROW( "FormatImpexGenerator", 1, " no connection was set to the Impex database" );
    }
}

void FormatImpexGenerator::check_db_connect()
{
    if(!dbdocument) {
      JSONIO_THROW( "FormatImpexGenerator", 2, " no connection was set to the database" );
    }
}

void FormatImpexGenerator::check_impex_set()
{
    if(!impex_configuration) {
      JSONIO_THROW( "FormatImpexGenerator", 3, " undefined impex configuration" );
    }
}


} // namespace jsonio17

