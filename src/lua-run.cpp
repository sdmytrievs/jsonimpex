#include "lua-run.h"
#include "jsonio17/service.h"
#include "jsonio17/jsonfree.h"
#include "jsonio17/jsondump.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace jsonio17 {

// Thread-safe logger to stdout with colors
std::shared_ptr<spdlog::logger> impex_logger = spdlog::stdout_color_mt("jsonimpex17");

// Initialization
LuaRun::LuaRun( const std::string& lib_path ): lua_lib_path( lib_path )
{
    impex_logger->debug(" LUA lib path: {}", lua_lib_path);
    // Init Lua
    lua_internal = luaL_newstate();
    // Make standard libraries available in the Lua state
    luaL_openlibs( lua_internal );
}

// Free Lua resources
LuaRun::~LuaRun()
{
    lua_close( lua_internal );
}

bool LuaRun::run_file_script(const std::string& file_with_lua_script )
{
    lua_settop( lua_internal, 0 ); //empty the lua stack
    if( luaL_dofile( lua_internal, file_with_lua_script.c_str() ) )
    {
        impex_logger->error(" LUA error: {}", lua_tostring( lua_internal, -1 ));
        lua_pop( lua_internal, 1 );
        return false;
    }
    return true;
}

bool LuaRun::run_string_script(const std::string& string_with_lua_script )
{
    lua_settop( lua_internal, 0 ); //empty the lua stack

    // add main part to script
    /* --
    string luascr = "-- load  JSON4Lua module\n"
                    "   dofile(lib_path..'json.lua');\n"
                    "   json = require('json');\n\n"
                    "-- Now JSON decode the json string\n"
                    "   rintable = json.decode( ijsdata );\n";
           luascr += sluascript;
           luascr += "-- Encodes a table\n"
                     "   ojsdata = json.encode(rintable);\n";
    --*/

//    std::string lua_dump =
//            "function dump(o) "
//            "  if type(o) == 'table' then "
//            "     local s = '{ ' "
//            "     for k,v in pairs(o) do "
//            "         if type(k) ~= 'number' then k = '\"'..k..'\"' end "
//            "         s = s .. '['..k..'] = ' .. dump(v) .. ',' "
//            "      end "
//            "      return s .. '} ' "
//            "  else "
//            "      return tostring(o) "
//            "  end  "
//            "end \n";

    std::string luascr = "JSON =( loadfile \"";
    luascr+=  lua_lib_path+"JSON.lua\")()\n";
    luascr+= "-- Now JSON decode the json string\n"
             "   rintable = JSON:decode(ijsdata);\n";
//             "-- print( dump(rintable) ) \n";
    luascr += string_with_lua_script;
    luascr += "-- Encodes a table\n"
              "   NullPlaceholder = \"---\"   \n"
              "   encode_options = { null = NullPlaceholder }"
              "   ojsdata = JSON:encode(rintable, nil, encode_options);\n";
//    luascr = lua_dump + luascr;

    if( luaL_dostring( lua_internal, luascr.c_str() ) )
    {
        impex_logger->error(" LUA error: {}", lua_tostring( lua_internal, -1 ));
        lua_pop( lua_internal, 1 );
        return false;
    }
    return true;
}


std::string LuaRun::run(const std::string& lua_script, const std::string& json_data_1, bool is_file )
{
    //  Make a insert a global var into Lua from C++"
    //--lua_pushstring( _L, _lualibPath.c_str());
    //--lua_setglobal( _L, "lib_path");

    // null in lua lost
    // the solution is to use a placeholder instead of null
    // and serializing the table structure to a json string using designated encode options
    auto jsFree = json::loads( json_data_1 );
    jsFree.placehold_null();
    //auto json_data = regexp_replace(json_data_1, "null", "\"---\"");

    impex_logger->debug(" LUA before: {}", jsFree.dump(true).c_str());
    lua_pushstring( lua_internal, jsFree.dump(true).c_str() );
    lua_setglobal( lua_internal, "ijsdata" );

    // Run lua script
    bool ret;
    if( is_file )
        ret = run_file_script( lua_script );
    else
        ret = run_string_script( lua_script );

    if( !ret)
        return "";

    // Read a global var from Lua into C++"
    lua_getglobal( lua_internal, "ojsdata" );
    std::string luanew = lua_tostring( lua_internal, -1 );
    lua_pop( lua_internal, 1 );

    // test output
    impex_logger->debug(" LUA after: {}", luanew.c_str());
    return luanew;
}

// Set string vector to lua
template <>
bool LuaRun::set_vector( const std::string& valname, const std::vector<std::string>& val )
{
    lua_createtable( lua_internal, static_cast<int>( val.size() ), 0);
    int itable = lua_gettop( lua_internal );
    int index = 1;
    auto iter = val.begin();
    while( iter != val.end() )
    {
        lua_pushstring( lua_internal, (*iter++).c_str() );
        lua_rawseti( lua_internal, itable, index++ );
    }
    lua_setglobal( lua_internal, valname.c_str() );
    return true;
}

// Read string vector from lua
template <>
bool LuaRun::get_vector( const std::string& valname, std::vector<std::string>& val )
{
    val.clear();
    lua_getglobal( lua_internal, valname.c_str() );
    if( lua_isnil( lua_internal, -1 ) )
        return false;
    lua_pushnil( lua_internal );
    while( lua_next( lua_internal, -2 ) )
    {
        val.push_back( std::string( lua_tostring( lua_internal, -1 )) );
        lua_pop( lua_internal, 1 );
    }
    clean();
    return true;
}

// Run lua script to change value
// Script must look like "field = field+20"
template <>
bool LuaRun::runFunc( const std::string& lua_script, std::string& value )
{
    //  Make a insert a global var into Lua from C++"
    lua_pushstring( lua_internal, value.c_str() );
    lua_setglobal( lua_internal, "field");

    // run script
    lua_settop( lua_internal, 0 ); //empty the lua stack
    if(luaL_dostring( lua_internal, lua_script.c_str() ) )
    {
        impex_logger->error(" LUA error: {}", lua_tostring( lua_internal, -1 ));
        lua_pop( lua_internal, 1 );
        return false;
    }

    // get result
    // Read a global var from Lua into C++"
    lua_getglobal( lua_internal, "field" );
    value = lua_tostring( lua_internal, -1 );
    lua_pop( lua_internal, 1 );

    return true;
}

} // namespace jsonio17
