#include <fstream>
#include "jsonimpex17/impex_generator.h"
#include "jsonimpex17/yaml_xml2file.h"
#include "jsonio17/dbschemadoc.h"
#include "jsonio17/jsondump.h"
#include "lua-run.h"

namespace jsonio17 {

//std::string FormatImpexWidget::impex_collection_name = "impexdefs";

//jsonio17::DBSchemaDocument* newDBImpexClient(  const jsonio17::DBQueryBase& query  )
//{
//    return  jsonio17::DBSchemaDocument::newSchemaDocumentQuery(  uiSettings().resources_database(),  "ImpexFormat",
//                                                                 FormatImpexWidget::impex_collection_name,  query );
//}

bool FormatImpexGenerator::getOverwrite() const
{
    return overwrite;
}

void FormatImpexGenerator::setOverwrite(bool newOverwrite)
{
    overwrite = newOverwrite;
}

bool FormatImpexGenerator::getUsedb() const
{
    return use_db;
}

void FormatImpexGenerator::setUsedb(bool newUse_db)
{
    use_db = newUse_db;
}

void FormatImpexGenerator::clear_export_file(const std::string &file_name)
{
    std::fstream astream(file_name,  std::fstream::out);
    jsonio17::JSONIO_THROW_IF(!astream.good() , "FormatImpexGenerator", 4, file_name+" fileopen error...");
}

void FormatImpexGenerator::check_dbimpex_connect()
{
    if(!dbimpex) {
        JSONIO_THROW( "FormatImpexGenerator", 1, " no connection was set to the resource database" );
    }
}

void FormatImpexGenerator::check_impex_set()
{
    if(!impex_configuration) {
        JSONIO_THROW( "FormatImpexGenerator", 3, " undefined impex configuration" );
    }
}

std::shared_ptr<jsonio17::ImpexFormatFile> FormatImpexGenerator::read_impex_from_file(ImpexFormatFile::ImpexMode mode,
                                                                                      const std::string& format_file,
                                                                                      const std::string& impex_format_name)
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
    return std::make_shared<jsonio17::ImpexFormatFile>(mode, impex_schema, formatdata);
}

std::shared_ptr<jsonio17::ImpexFormatFile> FormatImpexGenerator::read_impex_from_database(ImpexFormatFile::ImpexMode mode,
                                                                                          const std::string& format_key,
                                                                                          const std::string& impex_format_name)
{
    check_dbimpex_connect();
    auto impex_schema = impex_format_name;
    auto  formatdata = jsonio17::JsonFree::object();
    dbimpex->readDocument(format_key);
    std::string impexstr;
    dbimpex->getValueViaPath<std::string>("impex", impexstr, "");
    formatdata.loads(impexstr);
    if(impex_schema.empty()) {
        dbimpex->getValueViaPath<std::string>("impexschema", impex_schema, "");
    }
    return std::make_shared<jsonio17::ImpexFormatFile>(mode, impex_schema, formatdata);
}

void  FormatImpexGenerator::file_import(const std::string& foreign_format_file, const std::string& schema_format_file)
{
    check_impex_set();
    std::string record_json;
    clear_export_file(schema_format_file);
    jsonio17::JsonYamlXMLArrayFile file(schema_format_file);

    impex_configuration->openFile(foreign_format_file);
    file.Open( jsonio17::TxtFile::WriteOnly );
    while( impex_configuration->readBlock(record_json) ) {
        impex_logger->debug("Import record: {}", record_json);
        file.saveNext(record_json);
    }
    file.Close();
    impex_configuration->closeFile();
}

void  FormatImpexGenerator::file_export(const std::string& foreign_format_file, const std::string& schema_format_file)
{
    check_impex_set();
    std::string record_json;
    clear_export_file(foreign_format_file);
    jsonio17::JsonYamlXMLArrayFile file(schema_format_file);
    impex_configuration->openFile(foreign_format_file);

    file.Open( jsonio17::TxtFile::ReadOnly );
    while( file.loadNext(record_json) ) {
        impex_configuration->writeBlock(record_json);
    }
    impex_configuration->closeFile();
    file.Close();
}

void  FormatImpexGenerator::string_import(const std::string& foreign_format_string, std::string& schema_format_string)
{
    check_impex_set();
    std::string record_json;
    auto arr_object = jsonio17::JsonFree::array();

    impex_configuration->loadString(foreign_format_string);
    while( impex_configuration->readBlock(record_json) ) {
        arr_object[arr_object.size()].loads(record_json);
    }
    schema_format_string= arr_object.dump(true);
}

void  FormatImpexGenerator::string_export(std::string& foreign_format_string, const std::string& schema_format_string)
{
    check_impex_set();
    std::string record_json;
    auto jsFree = json::loads(schema_format_string);
    impex_configuration->loadString(foreign_format_string);

    for( const auto& object: jsFree ) {
        impex_configuration->writeBlock(object->dump(true));
    }
    foreign_format_string= impex_configuration->getString();
}


// Read json records from foreign format files according to import configuration
std::vector<std::string> FormatImpexGenerator::database_import(const std::string& foreign_format_file, DBSchemaDocument* dbdocument)
{
    std::vector<std::string> new_ids;
    if(!dbdocument) {
        return new_ids;
    }
    check_impex_set();
    std::string record_json;
    impex_configuration->openFile(foreign_format_file);

    while( impex_configuration->readBlock(record_json) ) {
        new_ids.push_back(dbdocument->createFromJson(record_json, overwrite));
    }
    impex_configuration->closeFile();
    return new_ids;
}

// Update json records from foreign format files according to import configuration
void FormatImpexGenerator::database_update(const std::string& foreign_format_file, DBSchemaDocument* dbdocument)
{
    if(!dbdocument) {
        return;
    }
    check_impex_set();
    std::string record_json;
    impex_configuration->openFile(foreign_format_file);
    while( impex_configuration->updateBlock( dbdocument, record_json ) )
    {}
    impex_configuration->closeFile();
}

// Export json records to foreign format files according to export configuration
void FormatImpexGenerator::database_export(const std::string& foreign_format_file,
                                           DBSchemaDocument* dbdocument,
                                           const std::vector<std::string>& keys_document)
{
    if(!dbdocument) {
        return;
    }
    check_impex_set();
    if(!keys_document.empty()) {
        impex_configuration->openFile(foreign_format_file);
        for( const auto& doc_key: keys_document ) {
            dbdocument->readDocument( doc_key );
            impex_configuration->writeBlock(dbdocument->getJson());
        }
        impex_configuration->closeFile();
    }
}


std::shared_ptr<jsonio17::DBSchemaDocument> FormatImpexGenerator::documentConnect(const std::string& collection)
{
    std::shared_ptr<jsonio17::DBSchemaDocument> dbdocument;
    std::string collection_name = collection;

    // try connect from configuration
    if(!work_database) {
        // Connect to Arangodb ( load settings from "config.json" file )
        work_database.reset(new jsonio17::DataBase());
    }
    if(work_database && impex_configuration) {
        if(collection_name.empty()) {
            collection_name = collectionNameFromSchema(impex_configuration->dataName());
        }
        dbdocument.reset(DBSchemaDocument::newSchemaDocument(*work_database.get(), impex_configuration->dataName(), collection_name));
    }
    else {
        JSONIO_THROW("FormatImpexGenerator", 2, " no connection was set to the database");
    }
    return dbdocument;
}


} // namespace jsonio17

