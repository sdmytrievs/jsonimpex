#pragma once

#include <memory>
#include "jsonimpex17/thrift_impex.h"

namespace jsonio17 {

/// Store import result function
using StoringImportResults_f = std::function< void(const std::string& json_data, bool overwrite_existing) >;
/// Load export data function
using loadingExportData_f = std::function< bool(std::string& json_data) >;

jsonio17::DBSchemaDocument* newDBImpexClient(const DataBase& dbconnect,
                                             const jsonio17::DBQueryBase& query= DBQueryBase::emptyQuery());

/// \class FormatImpexGenerator - class to work with Foreign Import/Export File Format
class FormatImpexGenerator
{

public:

    /// Possible formats sources.
    enum FormatSourceTypes {
        File = 0,
        Database = 1,
        String = 2
    };

    static std::string resources_database_name;
    static std::string impex_collection_name;

    /// Set the path to the resource directory to update/reread the schema descriptions.
    /// The resource directory must contain "schemas" and "lua" subdirectories.
    static void setResourcesDirectory(const std::string &path);

    /// The constructor
    /// @param db_connection arangodb database
    /// @param overwrite_existing overwrite if the present existing records
    /// @param formats_from read impex format from "resources" database, from file, or string
    explicit FormatImpexGenerator(std::shared_ptr<jsonio17::DataBase> db_connection,
                                  bool overwrite_existing=true,
                                  FormatSourceTypes formats_from=File):
        work_database(db_connection),
        overwrite(overwrite_existing),
        format_source(formats_from)
    {}

    /// The constructor
    /// Set db connection data as parameters
    /// @param db_url database server URL
    /// @param db_user the name of the database user
    /// @param user_passwd the user password
    /// @param db_name database name
    /// @param overwrite_existing overwrite if the present existing records
    /// @param formats_from read impex format from "resources" database, from file, or string
    explicit FormatImpexGenerator(const std::string& db_url, const std::string& db_user,
                                  const std::string& user_passwd, const std::string& db_name,
                                  bool overwrite_existing=true, FormatSourceTypes formats_from=File);

    /// The constructor
    /// Set db connection data from configuration file
    /// @param db_connection arangodb database
    /// @param overwrite_existing overwrite if the present existing records
    /// @param formats_from read impex format from "resources" database, from file, or string
    explicit FormatImpexGenerator(bool overwrite_existing=true, FormatSourceTypes formats_from=File);

    virtual ~FormatImpexGenerator()
    {}

    bool getOverwrite() const
    {
        return overwrite;
    }
    void setOverwrite(bool new_overwrite)
    {
        overwrite = new_overwrite;
    }

    FormatSourceTypes getFormatSource() const
    {
        return format_source;
    }
    void setFormatSource(FormatSourceTypes new_source)
    {
        format_source = new_source;
    }

    /// Import from other format files
    /// @param format_type import schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the import rules file or database key with rules
    /// @param foreign_format_file the path the  to be imported foreign format file
    /// @param store_funct  function for collecting imported data
    void importFileTo(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_file, StoringImportResults_f store_funct);

    /// Import from other format file to file
    /// @param format_type import schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the import rules file or database key with rules
    /// @param foreign_format_file the path the  to be imported foreign format file
    /// @param schema_format_file  the path the  to the result file
    void importFileToFile(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_file, const std::string &schema_format_file);

    /// Import from other format file to string
    /// @param format_type import schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the import rules file or database key with rules
    /// @param foreign_format_file the path the  to be imported foreign format file
    /// @param schema_format_string  json string with imported data
    void importFileToString(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_file, std::string &schema_format_string);

    /// Import from other format file to database document
    /// @param format_type import schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the import rules file or database key with rules
    /// @param foreign_format_file the path the  to be imported foreign format file
    /// @param dbdocument  database document to store data
    std::vector<std::string> importFileToDatabase(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_file, DBSchemaDocument* dbdocument);

    /// Import from other format file to database document
    /// @param format_type import schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the import rules file or database key with rules
    /// @param foreign_format_file the path the  to be imported foreign format file
    /// @param collection_name  database collection to be collected records
    std::vector<std::string> importFileToDatabase(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_file, const std::string &collection_name);

    /// Export to other format files
    /// @param format_type export schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the export rules file or database key with rules
    /// @param foreign_format_file the path the  to be exported foreign format file
    /// @param load_funct  function for collecting exporting data
    void exportFileFrom(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_file, loadingExportData_f load_funct);

    /// Export to other format files
    /// @param format_type export schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the export rules file or database key with rules
    /// @param foreign_format_file the path the  to be exported foreign format file
    /// @param schema_format_file  path to be load data
    void exportFileFromFile(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_file, const std::string &schema_format_file);

    /// Export to other format files
    /// @param format_type export schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the export rules file or database key with rules
    /// @param foreign_format_file the path the  to be exported foreign format file
    /// @param schema_format_string  string with collecting exporting data
    void exportFileFromString(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_file, const std::string &schema_format_string);

    /// Export to other format files
    /// @param format_type export schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the export rules file or database key with rules
    /// @param foreign_format_file the path the  to be exported foreign format file
    /// @param dbdocument  database document to load data
    void exportFileFromDatabase( const std::vector<std::string>& keys_to_unload,
                                 const std::string &format_type, const std::string &format_script,
                                 const std::string &foreign_format_file, DBSchemaDocument* dbdocument);

    /// Export to other format files
    /// @param format_type export schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the export rules file or database key with rules
    /// @param foreign_format_file the path the  to be exported foreign format file
    /// @param collection_name  database collection to be load records
    void exportFileFromDatabase( const std::vector<std::string>& keys_to_unload,
                                 const std::string& format_type, const std::string& format_script,
                                 const std::string& foreign_format_file, const std::string& collection_name);

    /// Update from other format file to database document
    /// @param format_type import schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the import rules file or database key with rules
    /// @param foreign_format_file the path the  to be imported foreign format file
    /// @param dbdocument  database document to store data
    void updateFileToDatabase(const std::string &format_type, const std::string &format_script,
                              const std::string &foreign_format_file, DBSchemaDocument *dbdocument);

    /// Update from other format file to database document
    /// @param format_type import schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the import rules file or database key with rules
    /// @param foreign_format_file the path the  to be imported foreign format file
    /// @param collection_name  database collection to be load records
    /// @param query is required to select records to be updated (by default all)
    /// @param fields_list the list of fields to used in comparing (if empty used default list)
    void updateFileToDatabase(const std::string &format_type, const std::string &format_script,
                              const std::string &foreign_format_file, const std::string& collection_name,
                              const DBQueryBase& query = DBQueryBase::emptyQuery(),
                              const std::vector<std::string>& fields_list = {} );

    /// Import from other format string
    /// @param format_type import schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the import rules file or database key with rules
    /// @param foreign_format_string foreign format data
    /// @param store_funct  function for collecting imported data
    void importStringTo(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_string, StoringImportResults_f store_funct);

    /// Import from other format string
    /// @param format_type import schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the import rules file or database key with rules
    /// @param foreign_format_string foreign format data
    /// @param schema_format_string  string with collecting exporting data
    void importStringToString(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_string, std::string &schema_format_string);

    /// Import from other format string to database document
    /// @param format_type import schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the import rules file or database key with rules
    /// @param foreign_format_string the path the  to be imported foreign format file
    /// @param dbdocument  database document to store data
    std::vector<std::string> importStringToDatabase(
            const std::string &format_type, const std::string &format_script,
            const std::string &foreign_format_string, DBSchemaDocument* dbdocument);

    /// Export to other format string bufer
    /// @param format_type export schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the export rules file or database key with rules
    /// @param foreign_format_string foreign format data
    /// @param load_funct  function for collecting exporting data
    void exportStringFrom(
            const std::string &format_type, const std::string &format_script,
            std::string &foreign_format_string, loadingExportData_f load_funct);

    /// Export to other format string bufer
    /// @param format_type export schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the export rules file or database key with rules
    /// @param foreign_format_string foreign format data
    /// @param schema_format_string  string with collecting exporting data
    void exportStringFromString(
            const std::string &format_type, const std::string &format_script,
            std::string &foreign_format_string, const std::string &schema_format_string);

    /// Export to other format string bufer
    /// @param format_type export schema format name ("FormatStructDataFile", "FormatKeyValueFile", "FormatTableFile")
    /// @param format_script path to the export rules file or database key with rules
    /// @param foreign_format_string result string
    /// @param dbdocument  database document to load data
    void exportStringFromDatabase( const std::vector<std::string>& keys_to_unload,
                                   const std::string &format_type, const std::string &format_script,
                                   std::string &foreign_format_string, DBSchemaDocument* dbdocument);

    /// Link to external resource database to read impex rules script
    /// (by default settings to be got from work_database)
    void linkToResourseDatabase(std::shared_ptr<jsonio17::DataBase> resourse_db)
    {
        resourse_database = resourse_db;
        dbimpex.reset(newDBImpexClient(*resourse_database.get()));
    }

protected:

    /// Database connection
    std::shared_ptr<jsonio17::DataBase> work_database;
    /// Database connection
    std::shared_ptr<jsonio17::DataBase> resourse_database;
    /// Link to impex collection in resource database
    std::shared_ptr<jsonio17::DBSchemaDocument> dbimpex;

    /// Overwrite if the present existing records
    bool overwrite = true;

    /// Reads impex scripts from "resources" database, from file, or string
    FormatSourceTypes format_source;

    std::shared_ptr<jsonio17::ImpexFormatFile> read_impex_from_file(ImpexFormatFile::ImpexMode mode,
                                                                    const std::string &format_file,
                                                                    const std::string &impex_format_name) const;
    std::shared_ptr<jsonio17::ImpexFormatFile> read_impex_from_database(ImpexFormatFile::ImpexMode mode,
                                                                        const std::string &format_key,
                                                                        const std::string &impex_format_name);
    std::shared_ptr<jsonio17::ImpexFormatFile> read_impex_from_string(ImpexFormatFile::ImpexMode mode,
                                                                      const std::string &format_string,
                                                                      const std::string &impex_format_name) const;
    std::shared_ptr<jsonio17::ImpexFormatFile> read_impex_format( ImpexFormatFile::ImpexMode mode,
                                                                  const std::string &impex_format_name,
                                                                  const std::string &impex_format_source);
    // Clear and test file to export data
    void clear_export_file(const std::string &file_name);

    virtual std::shared_ptr<jsonio17::DBSchemaDocument> documentConnect(const std::string &schema_name, std::string collection,
                                                                        const DBQueryBase& query = DBQueryBase::emptyQuery(),
                                                                        const std::vector<std::string>& fields_list = {});

    void resourse_database_from_work();
    void check_dbimpex_connect();

};

} // namespace jsonio17

