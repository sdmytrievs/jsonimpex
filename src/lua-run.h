//  This is JSONIO library+API (https://bitbucket.org/gems4/jsonio)
//
/// \file LuaRun.h
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

#ifndef LUARUN_H
#define LUARUN_H

#include <string>
#include <vector>
#include <iostream>
#include <lua5.3/lua.hpp>

namespace jsonio {

/// LUA-C++ Integration Class
class LuaRun
{

  /// Clean lua stack
  void clean()
  {
    int n = lua_gettop(_L);
    lua_pop(_L, n);
   }


  /// Set vector to lua
  template <class T>
  bool setVector(const std::string& valname, const std::vector<T>& v)
  {
      lua_createtable(_L, v.size(), 0);
      int itable = lua_gettop(_L);
      int index = 1;
      auto iter = v.begin();
      while(iter != v.end())
      {
          lua_pushnumber(_L, *iter++);
          lua_rawseti(_L, itable, index++);
      }
      lua_setglobal( _L, valname.c_str() );
      return true;
  }

  /// Read vector from lua
  template <class T>
   bool  getVector(const std::string& valname, std::vector<T>& v)
   {
      v.clear();
      lua_getglobal(_L, valname.c_str());
      if(lua_isnil(_L, -1))
        return false;
      lua_pushnil(_L);
      while(lua_next(_L, -2))
      {
        v.push_back( static_cast<T>(lua_tonumber(_L, -1)));
        lua_pop(_L, 1);
      }
      clean();
      return true;
  }


public:

  LuaRun( const std::string& dlualib );
  ~LuaRun();

  /// Run lua script with data
  /// Params File path or lua script and  Json data std::string
  std::string run(const std::string& luascript, const std::string& ijsdata, bool isFileName = false );

  /// Run lua script to change value
  /// Script must look like "field = field+20"
  template <class T>
    bool runFunc( const std::string& luascript, T& value )
     {
        //  Make a insert a global var into Lua from C++"
        lua_pushnumber( _L, value );
        lua_setglobal( _L, "field");

        // run script
        lua_settop(_L,0); //empty the lua stack
        if(luaL_dostring(_L, luascript.c_str()) )
        {
          std::cout <<  "error: " << lua_tostring(_L,-1) << std::endl;
          lua_pop(_L,1);
          return false;
        }

        // get result
        // Read a global var from Lua into C++"
        lua_getglobal(_L, "field");
        value = lua_tonumber(_L,-1);
        lua_pop(_L,1);

        return true;
    }

    /// Run lua script to change value
    /// Script must look like "field[2] = math.abs(field[1])"
    template <class T>
      bool runFunc( const std::string& luascript, std::vector<T>& value )
       {
          std::string tabName = "field";
          //  Make a insert a global var into Lua from C++"
          setVector( tabName, value );

          // run script
          lua_settop(_L,0); //empty the lua stack
          if(luaL_dostring(_L, luascript.c_str()) )
          {
            std::cout <<  "error: " << lua_tostring(_L,-1) << std::endl;
            lua_pop(_L,1);
            return false;
          }

          // Read a global var from Lua into C++
          getVector( tabName, value );
          return true;
      }

private:

  /// An opaque structure that points to a thread and indirectly (through the thread)
  ///  to the whole state of a Lua interpreter.
  lua_State *_L;
  /// Path for lua modules library
  std::string _lualibPath;

  bool runFileScript(const std::string& fluascript);
  bool runStringScript(const std::string& sluascript);

};


template <> bool LuaRun::setVector(const std::string& valname, const std::vector<std::string>& v);
template <> bool LuaRun::getVector(const std::string& valname, std::vector<std::string>& v);
template <> bool LuaRun::runFunc( const std::string& luascript, std::string& value );

} // namespace jsonio

#endif // LUARUN_H
