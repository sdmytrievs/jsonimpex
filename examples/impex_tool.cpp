#include <iostream>
#include "jsonio17/io_settings.h"
#include "jsonimpex17/impex_generator.h"
#include "jsonimpex17/yaml_xml2file.h"
#include "jsonio17/dbschemadoc.h"

static int documentsInCollection =  10;

struct ImpexData
{
    /// Type of mode: import, update or export
    jsonio17::ImpexFormatFile::ImpexMode use_mode = jsonio17::ImpexFormatFile::modeImport;

    /// Path to impex format file
    std::string impex_format_file;
    /// Impex format key
    std::string impex_format_key;
    /// Impex format type
    std::string impex_format_type;

    /// Path to other format file
    std::string foreign_format_file;

    /// Internal schema format file (by default save changes to database)
    std::string schema_format_file;
    /// Internal schema format collection
    std::string schema_collection;
    /// Overwrite existing records
    bool overwrite = true;

};

void show_usage( const std::string &name );
int extract_args( int argc, char* argv[], ImpexData& export_data );
void make_simple_schema_file();

// -e  -c Resources/files/phreeqc_el_export.FormatTableFile.json -sf Resources/files/phreeqc.VertexElement.json -o phreeqc_export_table.txt

// -e  -c Resources/files/export.SimpleSchemaTest.FormatTableFile.json -sf Resources/files/simple_schema_input.json -o simple_export_table.txt
// -e  -c Resources/files/export.SimpleSchemaTest.FormatKeyValueFile.json -sf Resources/files/simple_schema_input.json -o simple_export_keyvalue.txt

// -i  -c Resources/files/import.SimpleSchemaTest.FormatTableFile.json -sf simple_schema_import_table.json -o Resources/files/simple_export_table.txt
// -i  -c Resources/files/import.SimpleSchemaTest.FormatKeyValueFile.json -sf simple_schema_import_kv.json -o Resources/files/simple_export_keyvalue.txt
// -i  -c Resources/files/import.SimpleSchemaTest.FormatStructDataFile.json -sf simple_schema_import_yaml.json -o Resources/files/simple_schema_other.yaml

// -i -c is230/old_script/Element.GEMS3k.old.FormatStructDataFile.json -sf is230/elements.old.json -o is230/old_script/source/IComp.Kaolinite.json
//-i -c is230/import_script/Substance.GEMS3k.FormatStructDataFile.json -sf is230/import_script/substances.json -o is230/import_script/source/DComp.Kaolinite.json
//-i -c is230/old_script/Substance.GEMS3k.old.FormatStructDataFile.json -sf is230/old_script/substances.json -o is230/old_script/source/DComp.Kaolinite.json
//-i -c is230/import_script/ReactionSubstance.GEMS3k.FormatStructDataFile.json -sf is230/import_script/reac_substances.json -o is230/import_script/source/ReacDC.Kaolinite.json
//-i -c is230/old_script/ReactionSubstance.GEMS3k.old.FormatStructDataFile.json -sf is230/old_script/reac_substances.json -o is230/old_script/source/ReacDC.Kaolinite.json
//-i -c is230/import_script/Reaction.GEMS3k.FormatStructDataFile.json -sf is230/import_script/reaction.json -o is230/import_script/source/ReacDC.Kaolinite.json
//-i -c is230/old_script/Reaction.GEMS3k.old.FormatStructDataFile.json -sf is230/old_script/reaction.json -o is230/old_script/source/ReacDC.Kaolinite.json

//The simplest case: data exchange using disk files only
int main( int argc, char* argv[] )
{
    jsonio17::JsonioSettings::settingsFileName = "jsonimpex17-config.json";

    try{
        // build input data
        // make_simple_schema_file();

        ImpexData export_data;

        // ??? To do (1): Check connection from configuration or nullptr
        std::shared_ptr<jsonio17::DataBase> database(nullptr); // no real connection

        if(extract_args( argc, argv, export_data ))
            return 1;

        jsonio17::FormatImpexGenerator worker(database, export_data.overwrite);
        switch(export_data.use_mode){
        case jsonio17::ImpexFormatFile::modeImport:
            if(!export_data.schema_format_file.empty()) {
               worker.importFileToFile(export_data.impex_format_type, export_data.impex_format_file,
                                       export_data.foreign_format_file,export_data.schema_format_file);
            }
            else {
               worker.importFileToDatabase(export_data.impex_format_type, export_data.impex_format_file,
                                           export_data.foreign_format_file, export_data.schema_collection);
            }
            break;
        case jsonio17::ImpexFormatFile::modeExport:
            if(!export_data.schema_format_file.empty()) {
               worker.exportFileFromFile(export_data.impex_format_type, export_data.impex_format_file,
                                         export_data.foreign_format_file,export_data.schema_format_file);
            }
            else {
                // ??? To do (3): need select keys by query or all
                std::vector<std::string> keys_to_unload = {};
                worker.exportFileFromDatabase( keys_to_unload,export_data.impex_format_type, export_data.impex_format_file,
                                               export_data.foreign_format_file, export_data.schema_collection);
            }
            break;
        case jsonio17::ImpexFormatFile::modeUpdate:
            worker.updateFileToDatabase(export_data.impex_format_type, export_data.impex_format_file,
                                        export_data.foreign_format_file, export_data.schema_collection);
            break;
        }
        return 0;
    }
    catch(jsonio17::jsonio_exception& e)
    {
        std::cout  << e.what() <<  std::endl;
    }
    catch(std::exception& e)
    {
        std::cout  << "std::exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout  << "unknown exception" << std::endl;
    }
    return -1;
}

void make_simple_schema_file() {

    jsonio17::JsonYamlXMLArrayFile file("simple_schema_input.json");
    file.Open( jsonio17::TxtFile::WriteOnly );
    // Insert documents to file
    for( int ii=0; ii<documentsInCollection; ii++ ) {
        auto jsSimple = jsonio17::JsonSchema::object("SimpleSchemaTest");
        jsSimple["vbool"] = static_cast<bool>(ii%2);
        jsSimple["vint"] = ii;
        jsSimple["vdouble"] = 10.01*ii;
        jsSimple["vstring"] = "input-"+std::to_string(ii);
        jsSimple["vlist"] = std::vector<double>({1.*ii,2.*ii,3.*ii});
        file.saveNext(jsSimple);
    }
    file.Close();
}

void show_usage( const std::string &name )
{
    std::cout << "Usage: " << name << " [ option(s) ]  --config-file PATH --foreign-file PATH "
              << "\nImport/export tool for thermodynamic data\n"
              << "Options:\n"
              << "\t-h,\t--help\t\tshow this help message\n\n"

              << "\t-i,\t--import        \timport thermodynamic data from the foreign format file (default) \n"
              << "\t-e,\t--export        \texport thermodynamic data to the foreign format file \n"
              << "\t-u,\t--update        \tupdate thermodynamic data from another format file \n\n"

              << "\t-c,\t--config-file PATH \timpex configuration file \n"
              // to be implement connections to database
              //<< "\t-k,\t--config-key  KEY  \timpex configuration record id\n"
              << "\t-t,\t--format-type NAME  \timpex format (FormatStructDataFile, FormatKeyValueFile,FormatTableFile )\n\n"

              << "\t-o,\t--foreign-file PATH  \tthe foreign format file \n\n"
              << "\t-sf,\t--schema-file  PATH  \t\tinternal format  file (by default save to the database)\n"
              << "\t-sc,\t--schema-collection  NAME  \tthe database collection name\n\n"
              << std::endl;
}


int extract_args( int argc, char* argv[], ImpexData& export_data )
{
    int i=0;
    for( i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage( "impex_tool" );
            return 1;
        }
        else if ((arg == "-i") || (arg == "--import")) {
            export_data.use_mode = jsonio17::ImpexFormatFile::modeImport;
        }
        else if ((arg == "-e") || (arg == "--export"))  {
            export_data.use_mode = jsonio17::ImpexFormatFile::modeExport;
        }
        else if ((arg == "-u") || (arg == "--update"))  {
            export_data.use_mode = jsonio17::ImpexFormatFile::modeUpdate;
        }
        else if ((arg == "-c") || (arg == "--config-file")) {
            if (i + 1 < argc) {
                export_data.impex_format_file = argv[++i];
            } else {
                std::cerr << "--config-file option requires one argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-k") || (arg == "--config-key")) {
            if (i + 1 < argc) {
                export_data.impex_format_key = argv[++i];
            } else {
                std::cerr << "--config-key option requires one argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-t") || (arg == "--format-type"))  {
            if (i + 1 < argc) {
                export_data.impex_format_type = argv[++i];
            } else {
                std::cerr << "--format-type option requires one argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-o") || (arg == "--foreign-file")) {
            if (i + 1 < argc) {
                export_data.foreign_format_file = argv[++i];
            } else {
                std::cerr << "--foreign-file option requires one argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-sf") || (arg == "--schema-file")) {
            if (i + 1 < argc) {
                export_data.schema_format_file = argv[++i];
            } else {
                std::cerr << "--schema-file option requires one argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-sc") || (arg == "--schema-collection")) {
            if (i + 1 < argc) {
                export_data.schema_collection = argv[++i];
            } else {
                std::cerr << "--schema-collection option requires one argument." << std::endl;
                return 1;
            }
        }
    }
    if( export_data.foreign_format_file.empty() ) {
        std::cerr << "undefined foreign file path" << std::endl;
        return 1;
    }
    if( export_data.impex_format_file.empty() && export_data.impex_format_key.empty() ) {
        std::cerr << "undefined impex format" << std::endl;
        return 1;
    }
    if( export_data.schema_format_file.empty() && export_data.schema_collection.empty() ) {
        std::cerr << "undefined destination" << std::endl;
        return 1;
    }
    return 0;
}

