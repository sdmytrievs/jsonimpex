#pragma once

#include <string>
#include <vector>
#include <spdlog/spdlog.h>
#ifdef __APPLE__
#include <lua5.4/lua.hpp>
#else
#include <lua.hpp>
#endif

namespace jsonio {

/// Default logger for jsonimpex library
extern std::shared_ptr<spdlog::logger> impex_logger;

/// LUA-C++ Integration Class
class LuaRun
{

public:

    /// Constructor
    /// \param lib_path - path to internal lua scripts
    explicit LuaRun( const std::string& lib_path );

    /// Destructor. Free Lua resources.
    ~LuaRun();

    /// Run Lua script with data in JSON format.
    /// \param lua_script - file or string with the lua script
    /// \param json_data - inital data in JSON format
    /// \param if is_file true lua_script is  file  with the lua script
    std::string run(const std::string& lua_script, const std::string& json_data, bool is_file = false );

    /// Run lua script to change value
    /// Script must look like "field = field+20"
    template <class T>
    bool runFunc( const std::string& lua_script, T& value )
    {
        //  Make a insert a global var into Lua from C++"
        lua_pushnumber( lua_internal, value );
        lua_setglobal( lua_internal, "field" );

        // run script
        lua_settop( lua_internal,0 ); //empty the lua stack
        if( luaL_dostring( lua_internal, lua_script.c_str() ) )
        {
            impex_logger->error(" LUA error: {}", lua_tostring( lua_internal, -1 ));
            lua_pop( lua_internal,1 );
            return false;
        }

        // get result
        // Read a global var from Lua into C++"
        lua_getglobal( lua_internal, "field" );
        value = lua_tonumber( lua_internal, -1 );
        lua_pop( lua_internal, 1 );

        return true;
    }

    /// Run lua script to change values
    /// Script must look like "field[2] = math.abs(field[1])"
    template <class T>
    bool runFunc( const std::string& lua_script, std::vector<T>& values )
    {
        std::string tabName = "field";
        //  Make a insert a global var into Lua from C++"
        set_vector( tabName, values );

        // run script
        lua_settop( lua_internal, 0 ); //empty the lua stack
        if( luaL_dostring( lua_internal, lua_script.c_str() ) )
        {
            impex_logger->error(" LUA error: {}", lua_tostring( lua_internal, -1 ));
            lua_pop( lua_internal, 1 );
            return false;
        }

        // Read a global var from Lua into C++
        get_vector( tabName, values );
        return true;
    }

private:

    /// An opaque structure that points to a thread and indirectly (through the thread)
    ///  to the whole state of a Lua interpreter.
    lua_State *lua_internal;
    /// Path for lua modules library
    std::string lua_lib_path;

    bool run_file_script(const std::string& file_with_lua_script);
    bool run_string_script(const std::string& string_with_lua_script);

    /// Clean lua stack
    void clean()
    {
        int n = lua_gettop( lua_internal );
        lua_pop( lua_internal, n );
    }


    /// Set vector to lua
    template <class T>
    bool set_vector(const std::string& valname, const std::vector<T>& v)
    {
        lua_createtable( lua_internal, v.size(), 0 );
        int itable = lua_gettop( lua_internal );
        int index = 1;

        auto iter = v.begin();
        while(iter != v.end())
        {
            lua_pushnumber( lua_internal, *iter++ );
            lua_rawseti( lua_internal, itable, index++ );
        }
        lua_setglobal( lua_internal, valname.c_str() );
        return true;
    }

    /// Read vector from lua
    template <class T>
    bool  get_vector(const std::string& valname, std::vector<T>& v)
    {
        v.clear();
        lua_getglobal( lua_internal, valname.c_str());
        if(lua_isnil( lua_internal, -1))
            return false;

        lua_pushnil( lua_internal );
        while( lua_next( lua_internal, -2 ))
        {
            v.push_back( static_cast<T>(lua_tonumber( lua_internal, -1 )));
            lua_pop( lua_internal, 1);
        }
        clean();
        return true;
    }

};


template <> bool LuaRun::set_vector(const std::string& valname, const std::vector<std::string>& v);
template <> bool LuaRun::get_vector(const std::string& valname, std::vector<std::string>& v);
template <> bool LuaRun::runFunc( const std::string& luascript, std::string& value );

} // namespace jsonio


