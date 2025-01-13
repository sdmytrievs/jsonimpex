TEMPLATE = app
CONFIG += console c++2a
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

#DEFINES += USE_THRIFT

!win32 {
  DEFINES += __unix
}

macx-g++ {
  DEFINES += __APPLE__
}

macx-clang {
  DEFINES += __APPLE__
  INCLUDEPATH   += "/usr/local/include"
  DEPENDPATH   += "/usr/local/include"
  LIBPATH += "/usr/local/lib/"
}

win32 {
  INCLUDEPATH   += "C:\usr\local\include"
  DEPENDPATH   += "C:\usr\local\include"
  LIBPATH += "C:\usr\local\lib"
}

# Define the directory where jsonimpex source code is located
JSONIMPEX_DIR =  $$PWD/src
JSONIMPEX_HEADERS_DIR =  $$PWD/include
TESTS_DIR =  $$PWD/tests

DEPENDPATH   += $$JSONIMPEX_DIR
DEPENDPATH   += $$JSONIMPEX_HEADERS_DIR
DEPENDPATH   += $$TESTS_DIR

INCLUDEPATH   += $$JSONIMPEX_DIR
INCLUDEPATH   += $$JSONIMPEX_HEADERS_DIR
INCLUDEPATH   += $$TESTS_DIR

!win32::LIBS += -ljsonio -lyaml-cpp  -lpugixml
win32::LIBS += -ljsonio-static -lyaml-cppd  -lpugixml
unix:!macx-clang:LIBS += -llua -ldl
macx-clang:LIBS += -llua
win32:LIBS += -llua

#macx-clang:LIBS += -llua
#!macx-clang:LIBS += -llua5.3
#INCLUDEPATH   += "/usr/include/lua5.3"
#DEPENDPATH   += "/usr/include/lua5.3"

OBJECTS_DIR   = obj

contains(DEFINES, USE_THRIFT) {

THRIFT_DIR    = $$PWD/thrift
THRIFT_HEADERS_DIR =  $$THRIFT_DIR/gen-cpp
DEPENDPATH   += $$THRIFT_HEADERS_DIR
INCLUDEPATH   += $$THRIFT_HEADERS_DIR
LIBS += -lyaml-cpp  -lpugixml -lthrift

include($$THRIFT_DIR/thrift.pri)
}

include($$TESTS_DIR/gtest_dependency.pri)
include($$JSONIMPEX_DIR/jsonimpex.pri)

HEADERS += \
        $$TESTS_DIR/tst_jsonimpex.h \
        $$TESTS_DIR/tst_yaml_xml.h \
        $$TESTS_DIR/tst_impex.h \
        $$TESTS_DIR/example_schema.h \
        $$TESTS_DIR/impex_schema.h \
        $$TESTS_DIR/impex_schema_example.h

SOURCES += \
        $$TESTS_DIR/main.cpp

DISTFILES += \
    Resources/docs/source.md

#thrift -r -v --gen cpp
