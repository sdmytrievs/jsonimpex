#include <fstream>
#include "jsonio17/dbdriverarango.h"
#include "jsonio17/dbschemadoc.h"
#include "jsonio17/jsondump.h"
#include "jsonimpex17/impex_generator.h"
#include "jsonimpex17/yaml_xml2file.h"
#include "lua-run.h"

namespace jsonio17 {

std::string FormatImpexGenerator::resources_database_name = "resources";
std::string FormatImpexGenerator::impex_collection_name = "impexdefs";
bool FormatImpexGenerator::create_local_database_if_no_exist = false;

jsonio17::DBSchemaDocument* newDBImpexClient(const DataBase& dbconnect, const jsonio17::DBQueryBase& query)
{
    return jsonio17::DBSchemaDocument::newSchemaDocumentQuery(dbconnect,  "ImpexFormat",
                                                              FormatImpexGenerator::impex_collection_name, query );
}

FormatImpexGenerator::FormatImpexGenerator(const std::string &db_url,
                                           const std::string &db_user,
                                           const std::string &user_passwd,
                                           const std::string &db_name,
                                           bool overwrite_existing,
                                           FormatSourceTypes formats_from):
    overwrite(overwrite_existing),
    format_source(formats_from)
{
    if(create_local_database_if_no_exist) {
        create_ArangoDB_local_database_if_no_exist(db_url, db_user, user_passwd, db_name);
    }
    //arangocpp::ArangoDBConnection data(db_url, db_user, user_passwd, db_name);
    //std::shared_ptr<jsonio17::AbstractDBDriver> db_driver( new jsonio17::ArangoDBClient(data) );
    work_database = std::make_shared<jsonio17::DataBase>(db_url, db_user, user_passwd, db_name);
}

FormatImpexGenerator::FormatImpexGenerator(bool overwrite_existing, FormatSourceTypes formats_from):
    work_database(std::make_shared<jsonio17::DataBase>()),
    overwrite(overwrite_existing),
    format_source(formats_from)
{}

void FormatImpexGenerator::setResourcesDirectory(const std::string &path)
{
    ioSettings().setValue( jsonio17::common_section( "ResourcesDirectory" ), path );
    ioSettings().setValue( jsonio17::common_section( "SchemasDirectory" ), path+"/data/schemas/");
    ioSettings().updateSchemaDir();
}

void FormatImpexGenerator::importFileTo(const std::string &format_type,
                                        const std::string &format_script,
                                        const std::string &foreign_format_file,
                                        StoringImportResults_f store_funct)
{
    std::string record_json;
    auto impex_configuration = read_impex_format(jsonio17::ImpexFormatFile::modeImport, format_type, format_script);

    impex_configuration->openFile(foreign_format_file);
    while(impex_configuration->readBlock(record_json))  {
        store_funct(record_json, overwrite);
    }
    impex_configuration->closeFile();
}

void FormatImpexGenerator::importFileToFile(const std::string &format_type,
                                            const std::string &format_script,
                                            const std::string &foreign_format_file,
                                            const std::string &schema_format_file)
{
    clear_export_file(schema_format_file);
    jsonio17::JsonYamlXMLArrayFile file(schema_format_file);

    StoringImportResults_f file_funct = [&file](const std::string& json_data, bool) {
        impex_logger->trace("Import record: {}", json_data);
        file.saveNext(json_data);
    };

    file.Open(jsonio17::TxtFile::WriteOnly);
    importFileTo(format_type, format_script, foreign_format_file, file_funct);
    file.Close();
}

void FormatImpexGenerator::importFileToString(const std::string &format_type,
                                              const std::string &format_script,
                                              const std::string &foreign_format_file,
                                              std::string &schema_format_string)
{
    auto arr_object = jsonio17::JsonFree::array();

    StoringImportResults_f file_funct = [&](const std::string& json_data, bool) {
        impex_logger->trace("Import record: {}", json_data);
        arr_object[arr_object.size()].loads(json_data);
    };

    importFileTo(format_type, format_script, foreign_format_file, file_funct);
    schema_format_string = arr_object.dump(true);
}

std::vector<std::string> FormatImpexGenerator::importFileToDatabase(const std::string &format_type,
                                                                    const std::string &format_script,
                                                                    const std::string &foreign_format_file,
                                                                    DBSchemaDocument *dbdocument)
{
    std::vector<std::string> new_ids;
    if(dbdocument) {

        StoringImportResults_f file_funct = [&](const std::string& json_data, bool overwrite_data) {
            impex_logger->trace("Import record: {}", json_data);
            new_ids.push_back(dbdocument->createFromJson(json_data, overwrite_data));
        };

        importFileTo(format_type, format_script, foreign_format_file, file_funct);
    }
    return new_ids;
}

std::vector<std::string> FormatImpexGenerator::importFileToDatabase(const std::string &format_type,
                                                                    const std::string &format_script,
                                                                    const std::string &foreign_format_file,
                                                                    const std::string &collection_name)
{
    // get schema from format
    auto impex_configuration = read_impex_format(jsonio17::ImpexFormatFile::modeImport, format_type, format_script);
    auto dbdocument  = documentConnect(impex_configuration->dataName(), collection_name);
    return importFileToDatabase(format_type, format_script, foreign_format_file, dbdocument.get());
}

void FormatImpexGenerator::exportFileFrom(const std::string &format_type,
                                          const std::string &format_script,
                                          const std::string &foreign_format_file,
                                          loadingExportData_f load_funct)
{
    std::string record_json;
    auto impex_configuration = read_impex_format(jsonio17::ImpexFormatFile::modeExport, format_type, format_script);

    impex_configuration->openFile(foreign_format_file);
    while( load_funct(record_json) ) {
        impex_configuration->writeBlock(record_json);
    }
    impex_configuration->closeFile();
}

void FormatImpexGenerator::exportFileFromFile(const std::string &format_type,
                                              const std::string &format_script,
                                              const std::string &foreign_format_file,
                                              const std::string &schema_format_file)
{
    jsonio17::JsonYamlXMLArrayFile file(schema_format_file);

    loadingExportData_f file_funct = [&file](std::string& json_data) {
        return file.loadNext(json_data);
    };
    file.Open(jsonio17::TxtFile::ReadOnly);
    exportFileFrom(format_type, format_script, foreign_format_file, file_funct);
    file.Close();
}

void FormatImpexGenerator::exportFileFromString(const std::string &format_type,
                                                const std::string &format_script,
                                                const std::string &foreign_format_file,
                                                const std::string &schema_format_string)
{
    size_t ndx_record = 0;
    auto js_free = json::loads(schema_format_string);

    loadingExportData_f file_funct = [&](std::string& json_data) {
        if( ndx_record < js_free.size() ) {
            json_data =  js_free[ndx_record++].dump(true);
            return true;
        }
        return false;
    };
    exportFileFrom(format_type, format_script, foreign_format_file, file_funct);
}

void FormatImpexGenerator::exportFileFromDatabase(const std::vector<std::string> &keys_to_unload,
                                                  const std::string &format_type,
                                                  const std::string &format_script,
                                                  const std::string &foreign_format_file,
                                                  DBSchemaDocument *dbdocument)
{
    if(dbdocument && !keys_to_unload.empty()) {
        size_t ndx_record = 0;

        loadingExportData_f file_funct = [&](std::string& json_data) {
            if( ndx_record < keys_to_unload.size() ) {
                dbdocument->readDocument(keys_to_unload[ndx_record++]);
                json_data = dbdocument->getJson();
                return true;
            }
            return false;
        };
        exportFileFrom(format_type, format_script, foreign_format_file, file_funct);
    }
}

void FormatImpexGenerator::exportFileFromDatabase(const std::vector<std::string> &keys_to_unload,
                                                  const std::string &format_type,
                                                  const std::string &format_script,
                                                  const std::string &foreign_format_file,
                                                  const std::string &collection_name)
{

    auto impex_configuration = read_impex_format(jsonio17::ImpexFormatFile::modeExport, format_type, format_script);
    auto dbdocument  = documentConnect(impex_configuration->dataName(), collection_name);
    exportFileFromDatabase(keys_to_unload, format_type, format_script, foreign_format_file, dbdocument.get() );
}

// Update json records from foreign format files according to import configuration
void FormatImpexGenerator::updateFileToDatabase(const std::string &format_type,
                                                const std::string &format_script,
                                                const std::string &foreign_format_file,
                                                DBSchemaDocument* dbdocument)
{
    if(dbdocument) {
        auto impex_configuration = read_impex_format(jsonio17::ImpexFormatFile::modeUpdate, format_type, format_script);
        std::string record_json;
        impex_configuration->openFile(foreign_format_file);
        while( impex_configuration->updateBlock(dbdocument, record_json) )
        {}
        impex_configuration->closeFile();
    }
}

void FormatImpexGenerator::updateFileToDatabase(const std::string &format_type,
                                                const std::string &format_script,
                                                const std::string &foreign_format_file,
                                                const std::string &collection_name,
                                                const jsonio17::DBQueryBase& query,
                                                const std::vector<std::string>& fields_list)
{
    auto impex_configuration = read_impex_format(jsonio17::ImpexFormatFile::modeUpdate, format_type, format_script);
    auto dbdocument  = documentConnect(impex_configuration->dataName(), collection_name, query, fields_list);
    std::string record_json;
    impex_configuration->openFile(foreign_format_file);
    while( impex_configuration->updateBlock(dbdocument.get(), record_json) )
    {}
    impex_configuration->closeFile();
}

void FormatImpexGenerator::importStringTo(const std::string &format_type,
                                          const std::string &format_script,
                                          const std::string &foreign_format_string,
                                          StoringImportResults_f store_funct)
{
    std::string record_json;
    auto impex_configuration = read_impex_format(jsonio17::ImpexFormatFile::modeImport, format_type, format_script);

    impex_configuration->loadString(foreign_format_string);
    while(impex_configuration->readBlock(record_json))  {
        store_funct(record_json, overwrite);
    }
}

std::vector<std::string> FormatImpexGenerator::importStringToDatabase(const std::string &format_type,
                                                                      const std::string &format_script,
                                                                      const std::string &foreign_format_string,
                                                                      DBSchemaDocument *dbdocument)
{
    std::vector<std::string> new_ids;
    if(dbdocument) {

        StoringImportResults_f file_funct = [&new_ids, dbdocument](const std::string& json_data, bool overwrite_data) {
            impex_logger->trace("Import record: {}", json_data);
            new_ids.push_back(dbdocument->createFromJson(json_data, overwrite_data));
        };

        importStringTo(format_type, format_script, foreign_format_string, file_funct);
    }
    return new_ids;
}

void FormatImpexGenerator::importStringToString(const std::string &format_type,
                                                const std::string &format_script,
                                                const std::string &foreign_format_string,
                                                std::string &schema_format_string)
{
    auto arr_object = jsonio17::JsonFree::array();

    StoringImportResults_f file_funct = [&](const std::string& json_data, bool) {
        impex_logger->trace("Import record: {}", json_data);
        arr_object[arr_object.size()].loads(json_data);
    };

    importStringTo(format_type, format_script, foreign_format_string, file_funct);
    schema_format_string = arr_object.dump(true);
}


void FormatImpexGenerator::exportStringFrom(const std::string &format_type,
                                            const std::string &format_script,
                                            std::string &foreign_format_string,
                                            loadingExportData_f load_funct)
{
    std::string record_json;
    auto impex_configuration = read_impex_format(jsonio17::ImpexFormatFile::modeExport, format_type, format_script);

    impex_configuration->loadString(foreign_format_string);
    while( load_funct(record_json) ) {
        impex_configuration->writeBlock(record_json);
    }
    foreign_format_string = impex_configuration->getString();
}

void FormatImpexGenerator::exportStringFromDatabase(const std::vector<std::string> &keys_to_unload,
                                                    const std::string &format_type,
                                                    const std::string &format_script,
                                                    std::string &foreign_format_string,
                                                    DBSchemaDocument *dbdocument)
{
    if(dbdocument && !keys_to_unload.empty()) {
        size_t ndx_record = 0;

        loadingExportData_f file_funct = [&](std::string& json_data) {
            if( ndx_record < keys_to_unload.size() ) {
                dbdocument->readDocument(keys_to_unload[ndx_record++]);
                json_data = dbdocument->getJson();
                return true;
            }
            return false;
        };
        exportStringFrom(format_type, format_script, foreign_format_string, file_funct);
    }
}

void FormatImpexGenerator::exportStringFromString(const std::string &format_type,
                                                  const std::string &format_script,
                                                  std::string &foreign_format_string,
                                                  const std::string &schema_format_string)
{
    size_t ndx_record = 0;
    auto js_free = json::loads(schema_format_string);

    loadingExportData_f file_funct = [&](std::string& json_data) {
        if( ndx_record < js_free.size() ) {
            json_data =  js_free[ndx_record++].dump(true);
            return true;
        }
        return false;
    };
    exportStringFrom(format_type, format_script, foreign_format_string, file_funct);
}


//--------------------------------------------------------------------------

void FormatImpexGenerator::clear_export_file(const std::string &file_name)
{
    std::fstream astream(file_name,  std::fstream::out);
    jsonio17::JSONIO_THROW_IF(!astream.good() , "FormatImpexGenerator", 4, file_name+" fileopen error...");
}

std::shared_ptr<jsonio17::ImpexFormatFile>
FormatImpexGenerator::read_impex_from_file(ImpexFormatFile::ImpexMode mode,
                                           const std::string& format_file,
                                           const std::string& impex_format_name) const
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

std::shared_ptr<jsonio17::ImpexFormatFile>
FormatImpexGenerator::read_impex_from_database(ImpexFormatFile::ImpexMode mode,
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

std::shared_ptr<jsonio17::ImpexFormatFile>
FormatImpexGenerator::read_impex_from_string(ImpexFormatFile::ImpexMode mode,
                                             const std::string& format_string,
                                             const std::string& impex_format_name) const
{
    auto  formatdata = jsonio17::JsonFree::object();
    formatdata.loads(format_string);
    return std::make_shared<jsonio17::ImpexFormatFile>(mode, impex_format_name, formatdata);
}

std::shared_ptr<ImpexFormatFile> FormatImpexGenerator::read_impex_format(ImpexFormatFile::ImpexMode mode,
                                                                         const std::string &format_type,
                                                                         const std::string &format_script)
{
    switch(format_source){
    case jsonio17::FormatImpexGenerator::File:
        return read_impex_from_file(mode, format_script, format_type);
        break;
    case jsonio17::FormatImpexGenerator::Database:
        return read_impex_from_database(mode, format_script, format_type);
        break;
    case jsonio17::FormatImpexGenerator::String:
        return read_impex_from_string(mode, format_script, format_type);
        break;
    }
    JSONIO_THROW( "FormatImpexGenerator", 3, " undefined impex configuration ");
}


std::shared_ptr<jsonio17::DBSchemaDocument> FormatImpexGenerator::documentConnect(const std::string &schema_name,
                                                                                  std::string collection_name,
                                                                                  const DBQueryBase& query,
                                                                                  const std::vector<std::string>& fields_list)
{
    std::shared_ptr<jsonio17::DBSchemaDocument> dbdocument;
    // try connect from configuration
    if(!work_database) {
        // Connect to Arangodb ( load settings from "config.json" file )
        work_database.reset(new jsonio17::DataBase());
    }
    if(work_database) {
        if(collection_name.empty()) {
            collection_name = collectionNameFromSchema(schema_name);
        }
        impex_logger->debug("Connect to document: {} {}", schema_name, collection_name);
        dbdocument.reset(DBSchemaDocument::newSchemaDocumentQuery(*work_database.get(), schema_name, collection_name, query, fields_list));
    }
    if(!dbdocument) {
        JSONIO_THROW("FormatImpexGenerator", 2, " no connection was set to the database");
    }
    return dbdocument;
}

void FormatImpexGenerator::check_dbimpex_connect()
{
    if(!dbimpex && work_database) {
        resourse_database = work_database->clone(resources_database_name);
        dbimpex.reset(jsonio17::newDBImpexClient(*resourse_database.get()));
    }
    if(!dbimpex) {
        JSONIO_THROW( "FormatImpexGenerator", 1, " no connection was set to the resource database" );
    }
}

} // namespace jsonio17

