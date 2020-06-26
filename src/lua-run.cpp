//  This is JSONIO library+API (https://bitbucket.org/gems4/jsonio)
//
/// \file LuaRun.cpp
/// LuaRun LUA-C++ Integration Class.
//
// JSONIO is a C++ library and API aimed at implementing the interfaces
// for exchanging the structured data between NoSQL database backends,
// JSON/YAML/XML files, and client-server RPC (remote procedure calls).
//
// Copyright (c) 2015-2016 Svetlana Dmytriieva (svd@ciklum.com) and
//   Dmitrii Kulik (dmitrii.kulik@psi.ch)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU (Lesser) General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// JSONIO depends on the following open-source software products:
// Apache Thrift (https://thrift.apache.org); Pugixml (http://pugixml.org);
// YAML-CPP (https://github.com/jbeder/yaml-cpp); EJDB (http://ejdb.org);
// Lua5.2 (https://www.lua.org)
//

#include "lua-run.h"
using namespace std;

namespace jsonio {

/// Initialization
LuaRun::LuaRun(const string& dlualib): _lualibPath(dlualib)
{
  // cout << "_lualibPath : " << _lualibPath << endl;
  // Init Lua
  _L = luaL_newstate();
   // Make standard libraries available in the Lua state
  luaL_openlibs(_L);
}

/// Free Lua resources
LuaRun::~LuaRun()
{
  lua_close(_L);
}

bool LuaRun::runFileScript(const string& fluascript)
{
    lua_settop(_L,0); //empty the lua stack
    if(luaL_dofile(_L, fluascript.c_str()) )
    {
      fprintf(stderr, "error: %s\n", lua_tostring(_L,-1));
      lua_pop(_L,1);
      return false;
    }
    return true;
}

bool LuaRun::runStringScript(const string& sluascript)
{
    lua_settop(_L,0); //empty the lua stack

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

    string luascr = "JSON =( loadfile \"";
           luascr+=  _lualibPath+"JSON.lua\")()\n";
           luascr+= "-- Now JSON decode the json string\n"
                    "   rintable = JSON:decode( ijsdata );\n";
           luascr += sluascript;
           luascr += "-- Encodes a table\n"
                     "   ojsdata = JSON:encode(rintable);\n";

    if(luaL_dostring(_L, luascr.c_str()) )
    {
      fprintf(stderr, "error: %s\n", lua_tostring(_L,-1));
      lua_pop(_L,1);
      return false;
    }
    return true;
}


string LuaRun::run(const string& luascript, const string& ijsdata, bool isFileName )
{
  //  Make a insert a global var into Lua from C++"
    //--lua_pushstring( _L, _lualibPath.c_str());
    //--lua_setglobal( _L, "lib_path");
    lua_pushstring( _L, ijsdata.c_str());
    lua_setglobal( _L, "ijsdata");


   // Run lua script
    bool ret;
    if( isFileName )
      ret = runFileScript(luascript);
    else
      ret = runStringScript(luascript);

    if( !ret)
      return "";

   // Read a global var from Lua into C++"
    lua_getglobal(_L, "ojsdata");
    string luanew = lua_tostring(_L,-1);
    lua_pop(_L,1);

   // test output
   // cout << luanew.c_str() << endl;
    return luanew;
}

// Set string vector to lua
template <>
bool LuaRun::setVector(const string& valname, const vector<string>& v)
{
    lua_createtable(_L, static_cast<int>(v.size()), 0);
    int itable = lua_gettop(_L);
    int index = 1;
    auto iter = v.begin();
    while(iter != v.end())
    {
        lua_pushstring(_L, (*iter++).c_str());
        lua_rawseti(_L, itable, index++);
    }
    lua_setglobal( _L, valname.c_str() );
    return true;
}

// Read string vector from lua
template <>
 bool LuaRun::getVector(const string& valname, vector<string>& v)
 {
    v.clear();
    lua_getglobal(_L, valname.c_str());
    if(lua_isnil(_L, -1))
      return false;
    lua_pushnil(_L);
    while(lua_next(_L, -2))
    {
      v.push_back( string(lua_tostring(_L, -1)) );
      lua_pop(_L, 1);
    }
    clean();
    return true;
}

 // Run lua script to change value
 // Script must look like "field = field+20"
 template <>
   bool LuaRun::runFunc( const string& luascript, string& value )
    {
       //  Make a insert a global var into Lua from C++"
       lua_pushstring( _L, value.c_str() );
       lua_setglobal( _L, "field");

       // run script
       lua_settop(_L,0); //empty the lua stack
       if(luaL_dostring(_L, luascript.c_str()) )
       {
         cout <<  "error: " << lua_tostring(_L,-1) << endl;
         lua_pop(_L,1);
         return false;
       }

       // get result
       // Read a global var from Lua into C++"
       lua_getglobal(_L, "field");
       value = lua_tostring(_L,-1);
       lua_pop(_L,1);

       return true;
   }

} // namespace jsonio
