/*
 * Defines abstract DB chain interface.
*/

/** Uses query.ConditionData, query.QueryStyle (enum) */
include "query.thrift"

namespace cpp dbserver

enum CollectionType
{
    CDocument = 0,
    CVertex,
    CEdge,
    CAll
}

enum ResponseCode 
{
    Success = 0,
    Error,
    NameNotFound,  // Undefined name of collection
    NameExists,
    RecordNotFound,
    RecordExists
}

/** Struct for data record
*   A record is a string key / json string value pair.
*/
struct DBRecord
{
    1:required string key,
    2:required string value
}

/** Structure with query response
 list of pair key - value record or brief information from query
*/
struct DBRecordListResponse
{
    1: required ResponseCode responseCode,
    2: list<DBRecord> records
    3: string errormsg
}

/** Struct value (json string) response */
struct ValueResponse
{
    1: required ResponseCode responseCode,
    2: string value
    3: string errormsg
}

/** Struct key response */
struct KeyResponse
{
    1: required ResponseCode responseCode,
    2: string key
    3: string errormsg
}

/** Struct keys list response */
struct StringListResponse 
{
    1: required ResponseCode responseCode,
    2: list<string> names
    3: string errormsg
}


/**
 * Note about collection name:
 * Thrift string type translates to std::string in C++ and String in 
 * Java. Thrift does not validate whether the string is in utf8 in C++,
 * but it does in Java. If you are using this class in C++, you need to
 * make sure the map name is in utf8. Otherwise requests will fail.
 */
service DBServer
{
    /**
     * Pings DBServer.
     *
     * @return Success - if ping was successful.
     *         Error -   if ping failed.
     */
    ResponseCode ping(),


    /**
     * Stop DBServer.
     *
     */
    oneway void stop(),

    /**
     * Add a new collection of records.
     *
     * A record is a string key / json string value pair.
     * A key uniquely identifies a record in a collection.
     * 
     * @param  colName collection name
     * @param  ctype   collection type  (Vertex or Edge)
     * @return Success - on success.
     *         NameExists - collection already exists.
     *         Error - on any other errors.
     */
    ResponseCode addCollection( 1:string colName, 2:CollectionType ctype ),

    /**
     * Drops a collection of records.
     *
     * @param colName collection name
     * @return Ok - on success.
     *         NameNotFound - map doesn't exist.
     *         Error - on any other errors.
     */
    ResponseCode dropCollection(1:string colName),

    /**
     * List the collections.
     *
     * @param  ctype   collection type  (Vertex or Edge or All )
     * @returns StringListResponse
     *              responseCode Success  or
     *                           Error - on error.
     *              values - list of collection names.
     */
    StringListResponse listCollections( 1:CollectionType ctype ),

    /**
     * Add new record into the collection
     *
     * @param colName       collection name
     * @param value         record value to save
     * @returns
     *          responseCode - Success
     *                         NameNotFound collection doesn't exist.
     *                         RecordExists
     *                         Error
     *              value -   oid of the record.
     */
    KeyResponse CreateRecord(1:string colName, 3:string value),

    /**
     * Retrive one record from the collection
     *
     * @param colName       collection name
     * @param key           records key to retrive.
     * @returns BinaryResponse
     *              responseCode - Success
     *                             NameNotFound collection doesn't exist.
     *                             RecordNotFound record doesn't exist.
     *                             Error on any other errors.
     *              value - value of the record.
     */
    ValueResponse ReadRecord(1:string colName, 2:string key),

    /**
    * Update record in the collection
    *
    * @param colName       collection name
    * @param key           records key to update.
    * @param value         record value to update
    * @returns Success
    *          NameNotFound collection doesn't exist.
    *          RecordNotFound
    *          Error
    */
    ResponseCode UpdateRecord(1:string colName, 2:string key, 3:string value),

    /**
     * Removes record from the collection
     *
     * @param colName       collection name
     * @param key           records key to delete.
     * @returns Success
     *          NameNotFound collection doesn't exist.
     *          RecordNotFound
     *          Error
     */
    ResponseCode DeleteRecord(1:string colName, 2:string key),

    /**
     * Returns list of records in a collection.
     *
     * @param colName       collection name
     * @param fields - list of fileds selection
     * @param recordQuery   json query string for collection
     * @return RecordListResponse
     *             responseCode - Success
     *                          - NameNotFound collection doesn't exist.
     *                          - Error on any other errors
     *             records - list of records.
     */
    DBRecordListResponse SearchRecords( 1:string colName, 2:set<string> fields,
                      3:query.ConditionData recordQuery  ),

    /**
     * Delete edges connected to vertex record
     *
     * @param colName       vertex collection name
     * @param vertexkey     vertex key
     * @returns Success
     *          NameNotFound collection doesn't exist.
     *          Error
     */
    ResponseCode DeleteEdges( 1:string colName, 2:string vertexkey  ),



    /**
     * Provides 'distinct' operation over collection
     *
     * @param colName       collection name
     * @param fpath         Field path to collect distinct values from.
     * @return StringListResponse
     *             responseCode - Success
     *                          - NameNotFound collection doesn't exist.
     *                          - Error on any other errors
     *             names - list of distinct values filed fpath.
     */
    StringListResponse CollectValues( 1:string colName, 2:string fpath  ),


    /**
     *  Generate new unique oid or other key string
     *
     * @param colName       collection name
     * @returns new key string
     */
    string GenerateId( 1:string colName, 2:string _keytemplate );

}
