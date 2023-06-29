#pragma once

#include <memory>
#include "jsonimpex17/thrift_impex.h"


namespace jsonio17 {


/// \class FormatImpexGenerator - class to work with Foreign Import/Export File Format
class FormatImpexGenerator
{

public:

    explicit FormatImpexGenerator(ImpexFormatFile::ImpexMode mode, const std::string& collection_name=""):
        use_mode(mode), document_collection_name(collection_name)
    {}
    virtual ~FormatImpexGenerator()
    {}

    void link_to_database(std::shared_ptr<jsonio17::DBSchemaDocument> document)
    {
       dbdocument = document;
    }

    void link_to_resourse_database(std::shared_ptr<jsonio17::DBSchemaDocument> document)
    {
       dbimpex = document;
    }

    void set_impex_rules(std::shared_ptr<jsonio17::ImpexFormatFile> configuration)
    {
       impex_configuration = configuration;
    }
    void set_impex_from_database(const std::string &format_key, const std::string &impex_format_name)
    {
        impex_configuration = read_impex_from_database(format_key, impex_format_name);
    }
    void set_impex_from_file(const std::string &format_file, const std::string &impex_format_name)
    {
        impex_configuration = read_impex_from_file(format_file, impex_format_name);
    }

    void file_import(const std::string &foreign_format_file, const std::string &schema_format_file);
    void file_export(const std::string &foreign_format_file, const std::string &schema_format_file);
    void database_import(const std::string &foreign_format_file, bool overwrite);
    void database_update(const std::string &foreign_format_file);
    void database_export(const std::string &foreign_format_file, const std::vector<std::string> &keys_document);

    void string_import(const std::string &foreign_format_string, std::string &schema_format_string);
    void string_export(std::string &foreign_format_string, const std::string &schema_format_string);
private:

    /// Type of mode: import or export
    ImpexFormatFile::ImpexMode use_mode = ImpexFormatFile::modeImport;
    /// The rules of generation
    std::shared_ptr<jsonio17::ImpexFormatFile> impex_configuration;

    /// Current schema name
    std::string document_collection_name;
    /// Try connect to datatbase if dbdocument undefined
    std::shared_ptr<jsonio17::DataBase> work_database;
    /// Current link to Arango Database
    std::shared_ptr<jsonio17::DBSchemaDocument> dbdocument;

    /// Current link to impex Database
    std::shared_ptr<jsonio17::DBSchemaDocument> dbimpex;

    void check_dbimpex_connect();
    void check_db_connect();
    void check_impex_set();

    std::shared_ptr<jsonio17::ImpexFormatFile> read_impex_from_file(const std::string &format_file, const std::string &impex_format_name);
    std::shared_ptr<jsonio17::ImpexFormatFile> read_impex_from_database(const std::string &format_key, const std::string &impex_format_name);
};

} // namespace jsonio17

