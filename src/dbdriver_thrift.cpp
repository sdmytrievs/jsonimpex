#include <cstring>
#include <iostream>

#include "jsonio17/dbquerybase.h"
#include "jsonio17/io_settings.h"
#include "jsonimpex17/dbdriver_thrift.h"

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TProtocolException.h>

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

#include "DBServer.h"
using namespace  dbserver;


namespace jsonio17 {


/// Definition private of graph database chain used Thrift server
class ThriftClientAPI
{
    ::std::shared_ptr<apache::thrift::transport::TTransport> trans;
    dbserver::DBServerClient* dbClient = nullptr;

    query::ConditionData toCondition(const DBQueryBase& query );

public:

    ///  Constructor
    ThriftClientAPI( const std::string& theHost, int thePort )
    {
        resetDBServerClient( theHost, thePort );
    }

    ///  Destructor
    ~ThriftClientAPI()
    {
        delete dbClient;
    }

    dbserver::DBServerClient* resetDBServerClient( const std::string& theHost, int thePort );

    /// Create collection if no exist
    void createCollection( const std::string& collname, const std::string& ctype );
    /// Collect collection names in current database ( or only Edges/Vertex collections )
    std::set<std::string> getCollectionNames( AbstractDBDriver::CollTypes ctype );

    /// Create new record from a JSON representation of a single document
    std::string CreateRecord( const std::string& collname, const std::string& jsonrec );
    /// Read record by rid to JSON representation of a single document
    bool ReadRecord( const std::string& collname, const std::string& rid, std::string& jsonrec );
    /// Update existing document (a  JSON representation of a document update as an object)
    std::string UpdateRecord( const std::string& collname, const std::string& rid, const std::string& jsonrec );
    /// Removes the document identified by document-handle
    bool DeleteRecord( const std::string& collname, const std::string& rid );

    /// Run query
    ///  \param collname - collection name
    ///  \param query - query  condition (where)
    ///  \param setfnc - function for set up readed data
    void selectQuery( const std::string& collname, const DBQueryBase& query,  SetReaded_f setfnc );

    /// Execute function to multiple documents by their ids
    ///  \param collname - collection name
    ///  \param ids - list of _ids
    ///  \param setfnc - function for set up readed data
    void lookupByIds( const std::string& /*collname*/,  const std::vector<std::string>& /*ids*/,  SetReaded_f /*setfnc*/ ){}


    /// Run query for read Linked records list
    ///  \param collname - collection name
    ///  \param queryFields - list of fileds selection
    ///  \param setfnc - function for set up Linked records list
    void allQuery( const std::string& collname, const std::set<std::string>& queryFields,  SetReadedKey_f setfnc );

    /// Run delete edges connected to vertex record
    ///  \param collname - collection name
    ///  \param vertexid - vertex record id
    void deleteEdges(const std::string& collname, const std::string& vertexid );

    /// Removes multiple documents by their ids
    ///  \param collname - collection name
    ///  \param ids - list of _ids
    void removeByIds( const std::string& /*collname*/,  const std::vector<std::string>& /*ids*/  ){}

    ///  Provides 'distinct' operation over collection
    ///  \param fpath Field path to collect distinct values from.
    ///  \param collname - collection name
    ///  \param return values by specified fpath and collname
    void fpathCollect( const std::string& collname, const std::string& fpath, std::vector<std::string>& values );

    std::string sanitization( const std::string& documentHandle )
    {
        return documentHandle;
    }
};

query::ConditionData ThriftClientAPI::toCondition(const DBQueryBase& query )
{
    query::ConditionData condit;
    condit.style = query::QueryStyle::QUndef;
    condit.find = query.queryString();
    condit.bind = query.bindVars();
    condit.fields = query.queryFields();

    switch( query.type() )
    {
    case DBQueryBase::qTemplate:
        condit.style = query::QueryStyle::QTemplate;
        break;
    case DBQueryBase::qAll:
        condit.style = query::QueryStyle::QAll;
        break;
    case DBQueryBase::qEdgesAll:
        condit.style = query::QueryStyle::QEdgesAll;
        break;
    case DBQueryBase::qEdgesFrom:
        condit.style = query::QueryStyle::QEdgesFrom;
        break;
    case DBQueryBase::qEdgesTo:
        condit.style = query::QueryStyle::QEdgesTo;
        break;
    case DBQueryBase::qUndef:
    case DBQueryBase::qAQL:
        condit.style = query::QueryStyle::QAQL;
        break;
    case DBQueryBase::qEJDB:
        condit.style = query::QueryStyle::QEJDB;
        break;
    }
    return condit;
}

DBServerClient* ThriftClientAPI::resetDBServerClient( const std::string& theHost, int thePort )
{
    delete dbClient;

    //Set the Transport
    trans =  ::std::make_shared<TSocket>(theHost.c_str(), thePort );
    trans =  ::std::make_shared<TBufferedTransport>(trans);

    //Set the Protocol
    ::std::shared_ptr<TProtocol> proto;
    proto.reset(new TBinaryProtocol(trans));

    dbClient = new DBServerClient(proto);

    // Connect to service
    return dbClient ;
}

// Retrive one record from the collection
bool ThriftClientAPI::ReadRecord( const std::string& collname, const std::string& rid, std::string& jsonrec )
{
    // Get oid of record
    ValueResponse resp_data;
    std::string errejdb;

    // read from server
    try{
        trans->open();
        dbClient->ReadRecord( resp_data, collname, rid );
        trans->close();
    }
    catch( const TException& te )
    {
        std::cout << "TException: " << te.what() << std::endl;
        JSONIO_THROW( "ThriftClient", 10, te.what() );
    }

    // test responseCode
    switch( resp_data.responseCode )
    {
    case ResponseCode::Success:
        jsonrec = resp_data.value;
        return true;
    case ResponseCode::NameNotFound:
        errejdb = "Collection doesn't exist";
        break;
    case ResponseCode::RecordNotFound:
        errejdb = "Record doesn't exist";
        break;
    case ResponseCode::Error:
        errejdb =  resp_data.errormsg;
        break;
    default: break;
    }

    JSONIO_THROW( "ThriftClient", 11, errejdb );
}

// Removes record from the collection
bool ThriftClientAPI::DeleteRecord( const std::string& collname, const std::string& rid )
{
    ResponseCode::type resp_data;

    // read from server
    try{
        trans->open();
        resp_data = dbClient->DeleteRecord( collname, rid );
        trans->close();
    }
    catch (const TException & te)
    {
        std::cout << "TException: " << te.what() << std::endl;
        JSONIO_THROW( "ThriftClient", 12, te.what() );
    }

    // test responseCode
    std::string errejdb;
    switch( resp_data )
    {
    case ResponseCode::Success:
        return true;
    case ResponseCode::NameNotFound:
        errejdb = "Collection doesn't exist";
        break;
    case ResponseCode::RecordNotFound:
        errejdb = "Record doesn't exist";
        break;
    case ResponseCode::Error:
        errejdb =  "Undefined error";
        break;
    default: break;
    }

    JSONIO_THROW( "ThriftClient", 13, errejdb );
}

// Save/update record in the collection
std::string ThriftClientAPI::CreateRecord( const std::string& collname, const std::string& jsonrec )
{
    ResponseCode::type data_ret;
    std::string errormsg = "Undefined error";
    std::string newid;

    // read from server
    try{
        trans->open();
        KeyResponse key_return;
        dbClient->CreateRecord( key_return, collname, jsonrec );
        data_ret =key_return.responseCode;
        if( data_ret == ResponseCode::Success )
            newid = key_return.key;
        else
            errormsg =  key_return.errormsg;
        trans->close();
    }
    catch (const TException & te)
    {
        std::cout << "TException: " << te.what() << std::endl;
        JSONIO_THROW( "ThriftClient", 14, te.what() );
    }

    // test responseCode
    switch( data_ret )
    {
    case ResponseCode::Success:
        return newid;
    case ResponseCode::NameNotFound:
        errormsg = "Collection doesn't exist";
        break;
    case ResponseCode::Error:
    default: break;
    }

    JSONIO_THROW( "ThriftClient", 15, errormsg );
}

// Save/update record in the collection
std::string ThriftClientAPI::UpdateRecord( const std::string& collname, const std::string& rid, const std::string& jsonrec )
{
    ResponseCode::type data_ret;
    std::string errormsg = "Undefined error";
    std::string newid;

    // read from server
    try{
        trans->open();
        newid = rid;
        data_ret = dbClient->UpdateRecord( collname, rid, jsonrec );
        trans->close();
    }
    catch (const TException & te)
    {
        std::cout << "TException: " << te.what() << std::endl;
        JSONIO_THROW( "ThriftClient", 16, te.what() );
    }

    // test responseCode
    switch( data_ret )
    {
    case ResponseCode::Success:
        return newid;
    case ResponseCode::NameNotFound:
        errormsg = "Collection doesn't exist";
        break;
    case ResponseCode::RecordNotFound:
        errormsg = "Record doesn't exist";
        break;
    case ResponseCode::Error:
    default: break;
    }

    JSONIO_THROW( "ThriftClient", 17, errormsg );
}

// Create collection if no exist
void ThriftClientAPI::createCollection( const std::string& collname, const std::string& ctype )
{
    ResponseCode::type ret_data;
    CollectionType::type thrift_ctype = CollectionType::CDocument;
    if( ctype == "vertex")
        thrift_ctype = CollectionType::CVertex;
    else if( ctype == "edge")
        thrift_ctype = CollectionType::CEdge;

    // read from server
    try{
        trans->open();
        ret_data = dbClient->addCollection( collname, thrift_ctype);
        trans->close();
    }
    catch (const TException & te)
    {
        std::cout << "TException: " << te.what() << std::endl;
        JSONIO_THROW( "ThriftClient", 18, te.what() );
    }

    // test responseCode
    if( ret_data!=ResponseCode::Success  )
        JSONIO_THROW( "ThriftClient", 19, "Error when create collection" );
}

// Run query
//  \param collname - collection name
//  \param query -  query  condition (where)
//  \param setfnc - function for set up readed data
void ThriftClientAPI::selectQuery( const std::string& collname, const DBQueryBase& query,  SetReaded_f setfnc )
{
    // read from server
    DBRecordListResponse list_data;
    try{
        trans->open();
        dbClient->SearchRecords( list_data, collname, {}, toCondition(query) );
        trans->close();
    }
    catch( const TException& te )
    {
        std::cout << "TException: " << te.what() << std::endl;
        JSONIO_THROW( "ThriftClient", 20, te.what() );
    }

    // test responseCode
    switch( list_data.responseCode )
    {
    case ResponseCode::NameNotFound:
        JSONIO_THROW( "ThriftClient", 21, "Collection doesn't exist" );
    case ResponseCode::Error:
        JSONIO_THROW( "ThriftClient", 22, list_data.errormsg );
    default: break;
    }

    //cout << _data.records.size() << " records in collection " << getKeywd() << endl;
    for (uint i = 0; i < list_data.records.size(); ++i)
        setfnc( list_data.records[i].value );

}

// Run query
//  \param collname - collection name
//  \param queryFields - list of fileds selection
//  \param setfnc - function for set up readed data
void ThriftClientAPI::allQuery( const std::string& collname, const std::set<std::string>& queryFields,
                                SetReadedKey_f setfnc )
{
    // read from server
    DBRecordListResponse list_data;
    try{
        trans->open();
        dbClient->SearchRecords( list_data, collname, queryFields, toCondition(DBQueryBase( DBQueryBase::qAll ) ));
        trans->close();
    }
    catch (const TException & te)
    {
        std::cout << "TException: " << te.what() << std::endl;
        JSONIO_THROW( "ThriftClient", 23, te.what() );
    }

    // test responseCode
    switch( list_data.responseCode )
    {
    case ResponseCode::NameNotFound:
        JSONIO_THROW( "ThriftClient", 24, "Collection doesn't exist" );
    case ResponseCode::Error:
        JSONIO_THROW( "ThriftClient", 25, list_data.errormsg );
    default: break;
    }

    //cout << _data.records.size() << " records in collection " << getKeywd() << endl;
    for (uint i = 0; i < list_data.records.size(); ++i)
    {
        //cout << _data.records[i].c_str() << endl;
        setfnc(   list_data.records[i].value, list_data.records[i].key );
    }

}

void ThriftClientAPI::deleteEdges(const std::string& collname, const std::string& vertexid )
{
    ResponseCode::type ret_data;

    // read from server
    try{
        trans->open();
        ret_data = dbClient->DeleteEdges( collname, vertexid );
        trans->close();
    }
    catch (const TException & te)
    {
        std::cout << "TException: " << te.what() << std::endl;
        JSONIO_THROW( "ThriftClient", 26, te.what() );
    }

    // test responseCode
    switch( ret_data )
    {
    case ResponseCode::NameNotFound:
        JSONIO_THROW( "ThriftClient", 27, "Collection doesn't exist" );
    case ResponseCode::Error:
        JSONIO_THROW( "ThriftClient", 28,  "Undefined error" );
    default: break;
    }

}

void ThriftClientAPI::fpathCollect( const std::string& collname, const std::string& fpath,
                                    std::vector<std::string>& values )
{
    // read from server
    StringListResponse list_data;
    try{
        trans->open();
        dbClient->CollectValues( list_data, collname, fpath );
        trans->close();
    }
    catch (const TException & te)
    {
        std::cout << "TException: " << te.what() << std::endl;
        JSONIO_THROW( "ThriftClient", 29, te.what() );
    }

    // test responseCode
    switch( list_data.responseCode )
    {
    case ResponseCode::NameNotFound:
        JSONIO_THROW( "ThriftClient", 30, "Collection doesn't exist" );
    case ResponseCode::Error:
        JSONIO_THROW( "ThriftClient", 31, list_data.errormsg );
    default: break;
    }
    values = list_data.names;
}


// Create collection if no exist
std::set<std::string> ThriftClientAPI::getCollectionNames( AbstractDBDriver::CollTypes ctype )
{
    std::set<std::string> collist;
    StringListResponse list_return;

    CollectionType::type thrift_ctype = CollectionType::CDocument;
    switch( ctype )
    {
    case AbstractDBDriver::clVertex:
        thrift_ctype = CollectionType::CVertex;
        break;
    case AbstractDBDriver::clEdge:
        thrift_ctype = CollectionType::CEdge;
        break;
    case AbstractDBDriver::clAll:
        thrift_ctype = CollectionType::CAll;
        break;
    }

    // read from server
    try{
        trans->open();
        dbClient->listCollections( list_return, thrift_ctype) ;
        trans->close();
    }
    catch( const TException& te )
    {
        std::cout << "TException: " << te.what() << std::endl;
        JSONIO_THROW( "ThriftClient", 32, te.what() );
    }

    // test responseCode
    if( list_return.responseCode != ResponseCode::Success )
        JSONIO_THROW( "ThriftClient", 33, list_return.errormsg );

    collist.insert( list_return.names.begin(), list_return.names.end() );
    return collist;
}

//--------------------------------------------------------------------------------------------------------------


// Default Constructor
ThriftClient::ThriftClient():AbstractDBDriver()
{
    std::string newHost =  ioSettings().value( jsonimpex_section_name+"."+"ThriftDBSocketHost", defHost );
    int newPort = ioSettings().value( jsonimpex_section_name+"."+"ThriftDBSocketPort", defPort );
    reset_db_connection( newHost, newPort );
}

void ThriftClient::create_collection( const std::string& collname, const std::string& ctype )
{
    pdata->createCollection( collname, ctype );
}

// list collection names
std::set<std::string> ThriftClient::get_collections_names( CollTypes ctype )
{
    return pdata->getCollectionNames( ctype );
}

void ThriftClient::set_server_key( std::string &second, const std::string& id_key )
{
    second = id_key;
}

std::string ThriftClient::create_record(const std::string &collname, std::string& second, const JsonBase& recdata)
{
    auto  jsonrec = recdata.dump( true );
    auto new_id =  pdata->CreateRecord( collname, jsonrec);
    set_server_key( second, new_id );
    return new_id;
}

// Save/update record in the collection
std::string ThriftClient::update_record( const std::string& collname, keysmap_t::iterator& it, const JsonBase& recdata )
{
    auto  jsonrec = recdata.dump( true );
    std::string rid = get_server_key( it->second );
    rid = pdata->UpdateRecord(  collname, rid, jsonrec );
    return rid;
}

// Retrive one record from the collection
bool ThriftClient::read_record( const std::string& collname, keysmap_t::iterator&it, JsonBase& recdata )
{
    std::string jsonrec;
    std::string rid = get_server_key( it->second );
    auto ret =  pdata->ReadRecord( collname, rid, jsonrec );
    recdata.loads(jsonrec);
    return ret;
}

// Removes record from the collection
bool ThriftClient::delete_record( const std::string& collname, keysmap_t::iterator& itr  )
{
    std::string rid = get_server_key( itr->second );
    return pdata->DeleteRecord( collname, rid );
}

void ThriftClient::select_query( const std::string& collname, const DBQueryBase& query,  SetReaded_f setfnc )
{
    pdata->selectQuery( collname,  query, setfnc );
}


void ThriftClient::all_query( const std::string& collname, const std::set<std::string>& query_fields,
                              SetReadedKey_f setfnc )
{
    pdata->allQuery( collname, query_fields, setfnc );
}


void ThriftClient::delete_edges(const std::string& collname, const std::string& vertexid )
{
    pdata->deleteEdges( collname, vertexid );
}

void ThriftClient::fpath_collect( const std::string& collname, const std::string& fpath,
                                  std::vector<std::string>& values )
{
    pdata->fpathCollect( collname, fpath, values );
}

void ThriftClient::lookup_by_ids( const std::string& collname,  const std::vector<std::string>& ids,
                                  SetReaded_f setfnc )
{
    pdata->lookupByIds( collname, ids, setfnc );
}

void ThriftClient::remove_by_ids( const std::string& collname,  const std::vector<std::string>& ids )
{
    pdata->removeByIds( collname, ids );
}

std::string ThriftClient::sanitization( const std::string &documentHandle )
{
    return pdata->sanitization( documentHandle );
}

void ThriftClient::reset_db_connection( const std::string &theHost, int thePort )
{
    thrift_host = theHost;
    thrift_port = thePort;
    pdata.reset( new ThriftClientAPI( thrift_host, thrift_port ) );
}


} // namespace jsonio17
