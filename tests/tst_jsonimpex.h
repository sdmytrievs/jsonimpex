#pragma once

#include <gtest/gtest.h>
//#include <gmock/gmock-matchers.h>
#include "jsonio17/jsondump.h"
#include "lua-run.h"
//https://doc.qt.io/qtcreator/creator-autotest.html
// https://stackoverflow.com/questions/39574360/google-testing-framework-and-qt
// https://stackoverflow.com/questions/49756620/including-google-tests-in-qt-project


using namespace testing;
using namespace jsonio17;

TEST(JSONIMPEX, tests)
{
    EXPECT_EQ(1, 1);
}

TEST(IMPEX, luaFunc)
{
    LuaRun lua_wrk( "lua/" );

    double dval = 2.3;
    int ival = 10;
    EXPECT_EQ( lua_wrk.runFunc( "field = field+20", ival ), true );
    EXPECT_EQ( ival, 30 );
    EXPECT_EQ( lua_wrk.runFunc( "field = field+2.4", dval ), true );
    EXPECT_DOUBLE_EQ( dval, 4.7 );

    std::vector<double> dvals = { 1.1, 2.2, 0. };
    std::vector<int> ivals = { 11, 12, 13, 14 };
    EXPECT_EQ( lua_wrk.runFunc( "field[3] = field[1]+field[2]", dvals ), true );
    EXPECT_EQ( json::dump(dvals),  "[ 1.1, 2.2, 3.3 ]" );
    EXPECT_EQ( lua_wrk.runFunc( "field[1] = field[1]+5; field[3] = field[3]+7", ivals ), true );
    EXPECT_EQ( ivals, std::vector<int>({ 16, 12, 20, 14 }) );


    std::string json_data = "{\"vbool\":true,\"vdouble\":5.2,\"vint\":-100,\"vstring\":\"Test string\"}";
    EXPECT_EQ( lua_wrk.run( " ", json_data ), json_data );
    std::string lua_script  = " rintable.vbool=false; rintable.vint = 55";
    EXPECT_EQ( lua_wrk.run( lua_script, json_data ), "{\"vbool\":false,\"vdouble\":5.2,\"vint\":55,\"vstring\":\"Test string\"}" );
}
