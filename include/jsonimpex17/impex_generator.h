#pragma once

#include <memory>
#include "jsonimpex17/thrift_impex.h"


namespace jsonio17 {


/// \class FormatImpexGenerator - class to work with Foreign Import/Export File Format
class FormatImpexGenerator
{

public:

    /// @param db_connection arangodb database
    /// @param overwrite_existing overwrite if the present existing records
    /// @param formats_from_db read impex format from "resources" database if true, otherwise from file
    explicit FormatImpexGenerator(std::shared_ptr<jsonio17::DataBase> db_connection,
                                  bool overwrite_existing=true,
                                  bool formats_from_db=false):
        work_database(db_connection),
        overwrite(overwrite_existing),
        use_db(formats_from_db)
    {}
    virtual ~FormatImpexGenerator()
    {}

    bool getOverwrite() const;
    void setOverwrite(bool new_overwrite);
    bool getUsedb() const;
    void setUsedb(bool new_use_db);

    void link_to_resourse_database(std::shared_ptr<jsonio17::DBSchemaDocument> document)
    {
       dbimpex = document;
    }

    void set_impex_rules(std::shared_ptr<jsonio17::ImpexFormatFile> configuration)
    {
       impex_configuration = configuration;
    }
    void set_impex_from_database(ImpexFormatFile::ImpexMode mode, const std::string &format_key, const std::string &impex_format_name)
    {
        impex_configuration = read_impex_from_database(mode, format_key, impex_format_name);
    }
    void set_impex_from_file(ImpexFormatFile::ImpexMode mode, const std::string &format_file, const std::string &impex_format_name)
    {
        impex_configuration = read_impex_from_file(mode, format_file, impex_format_name);
    }

    void file_import(const std::string &foreign_format_file, const std::string &schema_format_file);
    void file_export(const std::string &foreign_format_file, const std::string &schema_format_file);
    std::vector<std::string> database_import(const std::string &foreign_format_file, DBSchemaDocument* dbdocument);
    void database_update(const std::string &foreign_format_file, DBSchemaDocument* dbdocument);
    void database_export(const std::string &foreign_format_file, DBSchemaDocument* dbdocument, const std::vector<std::string> &keys_document);

    void string_import(const std::string &foreign_format_string, std::string &schema_format_string);
    void string_export(std::string &foreign_format_string, const std::string &schema_format_string);

    /// Clear and test file to export data
    void clear_export_file(const std::string &file_name);
    std::shared_ptr<jsonio17::DBSchemaDocument> documentConnect(const std::string &collection);

protected:

    /// The rules of generation
    std::shared_ptr<jsonio17::ImpexFormatFile> impex_configuration;

    /// Database connection
    std::shared_ptr<jsonio17::DataBase> work_database;
    /// Link to impex collection in resource database
    std::shared_ptr<jsonio17::DBSchemaDocument> dbimpex;

    /// Overwrite if the present existing records
    bool overwrite = true;
    /// Reads impex format from "resources" database if true, otherwise from file
    bool use_db = false;

    void check_dbimpex_connect() const;
    void check_impex_set() const;

    std::shared_ptr<jsonio17::ImpexFormatFile> read_impex_from_file(ImpexFormatFile::ImpexMode mode,
                                                                    const std::string &format_file,
                                                                    const std::string &impex_format_name) const;
    std::shared_ptr<jsonio17::ImpexFormatFile> read_impex_from_database(ImpexFormatFile::ImpexMode mode,
                                                                        const std::string &format_key,
                                                                        const std::string &impex_format_name) const ;
};

} // namespace jsonio17

