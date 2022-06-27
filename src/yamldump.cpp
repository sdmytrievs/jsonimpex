#include <iomanip>
#include <iostream>
#include <cassert>
#include <stack>
#include <memory>
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/eventhandler.h"

#include "jsonio17/jsonfree.h"
#include "jsonio17/jsonschema.h"
#include "jsonio17/jsonbuilder.h"
#include "jsonio17/service.h"
#include "jsonimpex17/yamldump.h"

namespace jsonio17 {

/// Default logger for jsonimpex17 library
extern std::shared_ptr<spdlog::logger> impex_logger;

namespace yaml {

void object_emitter( YAML::Emitter& out, const JsonBase& object );

/// The NodeHandler class provides support for dynamically generating
/// JsonNode data from yaml parser
class NodeHandler: public YAML::EventHandler
{

public:

    std::string to_string()
    {
        return dump( m_object );
    }


    NodeHandler( JsonBase& object ): m_object(object)
    {
        if(  object.isObject() )
            m_objectStack.push( std::make_shared<JsonObjectBuilder>(&m_object) );
        else
            m_objectStack.push( std::make_shared<JsonArrayBuilder>(&m_object) );

    }

    virtual void OnDocumentStart( const YAML::Mark& /*mark*/ ) {}
    virtual void OnDocumentEnd() {}

    virtual void OnNull( const YAML::Mark& /*mark*/, YAML::anchor_t /*anchor*/ )
    {
        assert(m_stateStack.top().state == WaitingForValue );
        m_objectStack.top()->testScalar( m_stateStack.top().key, "null" );
        BeginNode();
    }

    virtual void OnAlias( const YAML::Mark& /*mark*/, YAML::anchor_t /*anchor*/ ) {}
    virtual void OnScalar( const YAML::Mark& mark, const std::string& tag,
                           YAML::anchor_t anchor, const std::string& value );

    virtual void OnSequenceStart( const YAML::Mark& mark, const std::string& tag,
                                  YAML::anchor_t anchor, YAML::EmitterStyle::value style );
    virtual void OnSequenceEnd();

    virtual void OnMapStart( const YAML::Mark& mark, const std::string& tag,
                             YAML::anchor_t anchor, YAML::EmitterStyle::value style );
    virtual void OnMapEnd();

private:

    void BeginNode();
    //void EmitProps(const std::string& tag, YAML::anchor_t anchor);

private:
    JsonBase& m_object;

    enum   STATE_ { WaitingForSequenceEntry, WaitingForKey, WaitingForValue, };

    struct State {
        std::string key;
        int    ndx;
        STATE_ state;

        State( STATE_ st ):
            key(""), ndx(0), state(st)
        {}
    };

    std::stack<State> m_stateStack;
    std::stack< std::shared_ptr<JsonBuilderBase>> m_objectStack;
};

/// The JsonHandler1 class provides support for dynamically generating
/// json data from yaml parser
class JsonHandler1: public YAML::EventHandler
{

    void addHead(const std::string& key );
    void addScalar(const std::string&  key, const std::string& value );
    void shift()
    {
        for(int temp = 0; temp < m_depth; temp++)
            m_os << "     ";
    }

public:

    std::string to_string()
    {
        return m_os.str();
    }

    JsonHandler1(std::stringstream& os_) : m_os(os_)
    { }

    virtual void OnDocumentStart( const YAML::Mark& /*mark*/ ) {}
    virtual void OnDocumentEnd() {}

    virtual void OnNull( const YAML::Mark& mark, YAML::anchor_t anchor );
    virtual void OnAlias( const YAML::Mark& mark, YAML::anchor_t anchor );
    virtual void OnScalar( const YAML::Mark& mark, const std::string& tag,
                           YAML::anchor_t anchor, const std::string& value );

    virtual void OnSequenceStart( const YAML::Mark& mark, const std::string& tag,
                                  YAML::anchor_t anchor, YAML::EmitterStyle::value style );
    virtual void OnSequenceEnd();

    virtual void OnMapStart( const YAML::Mark& mark, const std::string& tag,
                             YAML::anchor_t anchor, YAML::EmitterStyle::value style );
    virtual void OnMapEnd();

private:

    void BeginNode();
    //void EmitProps(const std::string& tag, YAML::anchor_t anchor);

private:

    std::stringstream& m_os;
    int m_depth =0;
    bool m_first= true;

    enum   STATE_ { WaitingForSequenceEntry, WaitingForKey, WaitingForValue, };

    struct State {
        std::string key;
        int    ndx;
        STATE_ state;

        State( STATE_ st):
            key(""),ndx(0),state(st)
        {}
    };

    std::stack<State> m_stateStack;
};


// NodeHandler------------------------------------------------

void NodeHandler::OnScalar(const YAML::Mark&, const std::string& /*tag*/,
                           YAML::anchor_t /*anchor*/, const std::string& value)
{
    switch (m_stateStack.top().state)
    {
    case WaitingForSequenceEntry:
    {
        auto key = std::to_string(m_stateStack.top().ndx++);
        m_objectStack.top()->testScalar( key, value );
        break;
    }
    case WaitingForKey:
        m_stateStack.top().key = value;
        m_stateStack.top().state = WaitingForValue;
        break;
    case WaitingForValue:
        auto key =  m_stateStack.top().key;
        m_objectStack.top()->testScalar( key, value );
        m_stateStack.top().key = "";
        m_stateStack.top().state = WaitingForKey;
        break;
    }
}

void NodeHandler::OnSequenceStart(const YAML::Mark&, const std::string& /*tag*/,
                                  YAML::anchor_t /*anchor*/, YAML::EmitterStyle::value /*style*/)
{
    if (!m_stateStack.empty())
    {
        std::string key;
        if( m_stateStack.top().state == WaitingForSequenceEntry )
            key = std::to_string( m_stateStack.top().ndx++ );
        else
            key =  m_stateStack.top().key;
        m_objectStack.push(  std::make_shared<JsonArrayBuilder>( m_objectStack.top()->addArray( key ) ) );
    }
    else
    {

    }
    BeginNode();
    m_stateStack.push(State(WaitingForSequenceEntry));
}

void NodeHandler::OnSequenceEnd()
{
    assert(m_stateStack.top().state == WaitingForSequenceEntry );
    m_stateStack.pop();
    if (!m_stateStack.empty())
        m_objectStack.pop();
}

void NodeHandler::OnMapStart(const YAML::Mark&, const std::string& /*tag*/,
                             YAML::anchor_t /*anchor*/, YAML::EmitterStyle::value /*style*/)
{
    if (!m_stateStack.empty() )
    {
        std::string key;
        if( m_stateStack.top().state == WaitingForSequenceEntry )
            key = std::to_string(m_stateStack.top().ndx++);
        else
            key =  m_stateStack.top().key;
        m_objectStack.push( std::make_shared<JsonObjectBuilder>( m_objectStack.top()->addObject( key ) ));
    }
    BeginNode();
    m_stateStack.push(State(WaitingForKey));
}

void NodeHandler::OnMapEnd() {
    m_stateStack.pop();
    if (!m_stateStack.empty() )
        m_objectStack.pop();
    //assert(m_stateStack.top().state == WaitingForKey);
}

void NodeHandler::BeginNode()
{
    if (m_stateStack.empty())
        return;

    switch (m_stateStack.top().state) {
    case WaitingForKey:
        m_stateStack.top().state = WaitingForValue;
        break;
    case WaitingForValue:
        m_stateStack.top().state = WaitingForKey;
        break;
    default:
        break;
    }
}

// JsonHandler------------------------------------------------

void JsonHandler1::OnNull(const YAML::Mark&, YAML::anchor_t /*anchor*/)
{
    assert(m_stateStack.top().state == WaitingForValue );
    addScalar(m_stateStack.top().key.c_str(), "null" );
    BeginNode();
}

void JsonHandler1::OnAlias(const YAML::Mark&, YAML::anchor_t /*anchor*/) {}

void JsonHandler1::OnScalar(const YAML::Mark&, const std::string& /*tag*/,
                            YAML::anchor_t /*anchor*/, const std::string& value)
{
    switch (m_stateStack.top().state)
    {
    case WaitingForSequenceEntry:
    { m_stateStack.top().ndx++;
        addScalar( "", value );
        break;
    }
    case WaitingForKey:
        m_stateStack.top().key = value;
        m_stateStack.top().state = WaitingForValue;
        break;
    case WaitingForValue:
        std::string key =  m_stateStack.top().key;
        addScalar( key.c_str(), value );
        m_stateStack.top().key = "";
        m_stateStack.top().state = WaitingForKey;
        break;
    }
}

void JsonHandler1::OnSequenceStart(const YAML::Mark&, const std::string& /*tag*/,
                                   YAML::anchor_t /*anchor*/, YAML::EmitterStyle::value /*style*/)
{
    std::string key="";
    if (!m_stateStack.empty())
    {
        if( m_stateStack.top().state == WaitingForSequenceEntry )
            m_stateStack.top().ndx++;
        else
            key =  m_stateStack.top().key;
    }
    addHead( key );
    m_os << "[\n";
    m_depth++;
    m_first = true;
    BeginNode();
    m_stateStack.push(State(WaitingForSequenceEntry));
}

void JsonHandler1::OnSequenceEnd()
{
    assert(m_stateStack.top().state == WaitingForSequenceEntry );
    m_stateStack.pop();
    m_depth--;
    m_os << "\n";
    shift();
    m_os << "]";
}

void JsonHandler1::OnMapStart(const YAML::Mark&, const std::string& /*tag*/,
                              YAML::anchor_t /*anchor*/, YAML::EmitterStyle::value /*style*/)
{
    std::string key="";
    if (!m_stateStack.empty() )
    {
        if( m_stateStack.top().state == WaitingForSequenceEntry )
            m_stateStack.top().ndx++;
        else
            key =  m_stateStack.top().key;
    }
    addHead( key );
    m_os << "{\n";
    m_depth++;
    m_first = true;
    BeginNode();
    m_stateStack.push(State(WaitingForKey));
}

void JsonHandler1::OnMapEnd() {
    m_stateStack.pop();
    m_depth--;
    m_os << "\n";
    shift();
    m_os << "}";
}

void JsonHandler1::BeginNode()
{
    if (m_stateStack.empty())
        return;

    switch (m_stateStack.top().state) {
    case WaitingForKey:
        m_stateStack.top().state = WaitingForValue;
        break;
    case WaitingForValue:
        m_stateStack.top().state = WaitingForKey;
        break;
    default:
        break;
    }
}

void JsonHandler1::addHead(const std::string& key )
{
    if(!m_first )
        m_os <<  ",\n";
    else
        m_first = false;

    shift();

    if( !key.empty())
        m_os << "\"" << key << "\" :   ";
}

void JsonHandler1::addScalar(const std::string&  key, const std::string& value )
{
    int ival = 0;
    double dval=0.;

    addHead( key );

    if( value == "null" || value == "true" ||  value == "false" )
        m_os << value;
    else
        if( is<int>( ival, value.c_str()) || is<double>( dval, value.c_str()))
            m_os << value.c_str();
        else
            m_os << "\"" << value.c_str() << "\"";
}

//---------------------------------------------------------------------

void object_emitter( YAML::Emitter& out, const JsonBase& object )
{
    auto objtype = object.type();
    auto objsize = object.size();

    for( std::size_t ii=0; ii<objsize; ii++ )
    {
        auto& childobj = object.child( ii );
        switch( objtype )
        {
        case JsonBase::Object:
            out << YAML::Key << childobj.getKey();
            out << YAML::Value;
            break;
        default:
            break;
        }

        switch( childobj.type() )
        {
        // impotant datatypes
        case JsonBase::Null:
            out << YAML::_Null();
            break;
        case JsonBase::Bool:
        case JsonBase::Int:
        case JsonBase::Double:
        case JsonBase::String:
            out <<  childobj.toString();
            break;
            // main constructions
        case JsonBase::Object:
            out << YAML::BeginMap;
            object_emitter( out, childobj );
            out << YAML::EndMap;
            break;
        case JsonBase::Array:
            out << YAML::BeginSeq;
            object_emitter(out, childobj );
            out << YAML::EndSeq;
            break;
        default:
            out  << "can't print type : " << childobj.type();
        }
    }
}

//-------------------------------------------------------------------------------------------------

std::string dump( const JsonBase &object )
{
    YAML::Emitter out;
    auto objtype = object.type();

    if( objtype == JsonBase::Object )
        out << YAML::BeginMap;
    else
        if( objtype == JsonBase::Array )
            out << YAML::BeginSeq;
        else
            return "";

    object_emitter( out, object );

    if( objtype == JsonBase::Object )
        out << YAML::EndMap;
    else
        out << YAML::EndSeq;

    return out.c_str();
}

void dump( std::ostream &os, const JsonBase &object )
{
    os << yaml::dump( object );
}

void loads(const std::string &yaml_str, JsonBase &object)
{
    try{
        std::stringstream stream(yaml_str);
        YAML::Parser parser(stream);
        NodeHandler builder(object);
        parser.HandleNextDocument(builder);
    }
    catch(YAML::Exception& e)
    {
        impex_logger->error(" Yaml2Json error: {}", e.what());
        JSONIO_THROW( "Yaml2Json", 10, e.what() );
    }
}

JsonFree loads(const std::string &yaml_str)
{
    auto object = JsonFree::object();
    yaml::loads( yaml_str, object );
    return object;
}

JsonSchema loads(const std::string& schema_name,const std::string &yaml_str)
{
    auto object = JsonSchema::object( schema_name );
    yaml::loads( yaml_str, object );
    return object;
}

std::string Yaml2Json(const std::string &yaml_str)
{
    std::stringstream jsonstream;

    try{
        std::stringstream stream(yaml_str);
        YAML::Parser parser(stream);
        JsonHandler1 builder(jsonstream);
        parser.HandleNextDocument(builder);
    }
    catch(YAML::Exception& e)
    {
        impex_logger->error(" Yaml2Json error: {}", e.what());
        JSONIO_THROW("Yaml2Json", 15, e.what());
    }
    return jsonstream.str();
}


} // yaml namespace

} // namespace jsonio17

