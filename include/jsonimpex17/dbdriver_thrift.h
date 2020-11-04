#pragma once

#include "jsonio17/dbdriverbase.h"


namespace jsonio17 {

class ThriftClientAPI;

/// Default host that the socket is connected to
const std::string defHost = "localhost";

/// Default port that the socket is connected to
const int defPort = 9090;

const std::string jsonimpex_section_name = "jsonimpex";

/// Implementation of Database Driver using Low-Level C++ Driver for ArangoDB.
class ThriftClient: public AbstractDBDriver
{

public:

    /// Constructor
    ThriftClient();

    /// Constructor
    explicit ThriftClient( const std::string& theHost, int thePort ):
        AbstractDBDriver()
    {
        reset_db_connection( theHost, thePort );
    }

    ///  Destructor
    ~ThriftClient()
    {}

    std::string host() const
    {
        return thrift_host;
    }
    int port() const
    {
        return thrift_port;
    }

    // Collections API

    /// Create collection if no exist
    /// \param colname - name of collection
    /// \param type - type of collection ( "undef", "schema", "vertex", "edge" )
    void create_collection(const std::string& collname, const std::string& ctype) override;

    /// Returns all collections names of the given database.
    /// \param ctype - types of collection to select.
    std::set<std::string> get_collections_names( CollTypes ctype ) override;

    std::string get_server_key( const std::string& second ) const override
    {
        return second;
    }

    void set_server_key( std::string& second, const std::string& key ) override;

    // CRUD API

    /// Creates a new document in the collection from the given data or
    /// replaces an existing document described by the selector.
    /// \param collname - collection name
    /// \param jsonrec - json object with data
    /// \return the document-handle.
    std::string create_record( const std::string& collname, std::string& second, const JsonBase& recdata ) override;

    /// Returns the document described by the selector.
    /// \param collname - collection name
    /// \param it -  pair: key -> selector
    /// \param jsonrec - object to receive data
    bool read_record( const std::string& collname, keysmap_t::iterator& it, JsonBase& recdata ) override;

    /// Update an existing document described by the selector.
    /// \param collname - collection name
    /// \param it -  pair: key -> selector
    /// \param jsonrec - json object with data
    std::string update_record( const std::string& collname, keysmap_t::iterator& it, const JsonBase& recdata ) override;

    /// Removes a document described by the selector.
    /// \param collname - collection name
    /// \param it -  pair: key -> selector
    bool delete_record(const std::string& collname, keysmap_t::iterator& it ) override;

    // Query API

    /// Fetches all documents from a collection that match the specified condition.
    ///  \param collname - collection name
    ///  \param query -    selection condition
    ///  \param setfnc -   callback function fetching document data
    void select_query( const std::string& collname, const DBQueryBase& query, SetReaded_f setfnc ) override;

    /// Looks up the documents in the specified collection using the array of ids provided.
    ///  \param collname - collection name
    ///  \param ids -      array of _ids
    ///  \param setfnc -   callback function fetching document data
    void lookup_by_ids( const std::string& collname,  const std::vector<std::string>& ids,  SetReaded_f setfnc ) override;

    /// Fetches all documents from a collection.
    ///  \param collname -    collection name
    ///  \param query_fields - list of fields to selection
    ///  \param setfnc -     callback function fetching document data
    void all_query( const std::string& collname, const std::set<std::string>& query_fields,  SetReadedKey_f setfnc ) override;

    ///  Provides 'distinct' operation over collection
    ///  \param collname - collection name
    ///  \param fpath    - field path to collect distinct values from
    ///  \param  values  - return values by specified fpath and collname
    void fpath_collect( const std::string& collname, const std::string& fpath, std::vector<std::string>& values ) override;

    /// Delete all edges linked to vertex record.
    ///  \param collname - collection name
    ///  \param vertexid - vertex record id
    void delete_edges(const std::string& collname, const std::string& vertexid ) override;

    /// Removes all documents from the collection whose keys are contained in the keys array.
    ///  \param collname - collection name
    ///  \param ids -      array of keys
    void remove_by_ids( const std::string& collname,  const std::vector<std::string>& ids  ) override;

    /// Check the document-handle example in to contain only
    /// characters officially allowed by ArangoDB.
    /// \return  a document-handle that contain only only allowed characters.
    std::string sanitization( const std::string& documentHandle ) override;

protected:

    /// The host that the socket is connected to
    std::string thrift_host;
    /// The port that the socket is connected to
    int thrift_port;

    /// ArangoDB connection data
    std::shared_ptr<ThriftClientAPI> pdata = nullptr;

    /// Reset connections to ArangoDB server
    void reset_db_connection( const std::string& theHost, int thePort );
};


} // namespace jsonio17


