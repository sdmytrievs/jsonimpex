TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

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

# Define the directory where jsonimpex17 source code is located
JSONIMPEX17_DIR =  $$PWD/src
JSONIMPEX17_HEADERS_DIR =  $$PWD/include
THRIFT_DIR    = $$PWD/thrift
THRIFT_HEADERS_DIR =  $$THRIFT_DIR/gen-cpp
TESTS_DIR =  $$PWD/tests

DEPENDPATH   += $$JSONIMPEX17_DIR
DEPENDPATH   += $$JSONIMPEX17_HEADERS_DIR
DEPENDPATH   += $$THRIFT_HEADERS_DIR
DEPENDPATH   += $$TESTS_DIR

INCLUDEPATH   += $$JSONIMPEX17_DIR
INCLUDEPATH   += $$JSONIMPEX17_HEADERS_DIR
INCLUDEPATH   += $$THRIFT_HEADERS_DIR
INCLUDEPATH   += $$TESTS_DIR

LIBS +=  -lstdc++fs
LIBS +=  -ljsonio17 -ljsonarango -lcurl  -lvelocypack
LIBS += -lyaml-cpp  -lpugixml -lthrift
macx-clang:LIBS += -llua
!macx-clang:LIBS += -llua5.3

OBJECTS_DIR   = obj

include($$TESTS_DIR/gtest_dependency.pri)
include($$THRIFT_DIR/thrift.pri)
include($$JSONIMPEX17_DIR/jsonimpex17.pri)

HEADERS += \
        $$TESTS_DIR/tst_jsonimpex.h \
        $$TESTS_DIR/tst_yaml_xml.h \
        tests/example_schema.h

SOURCES += \
        $$TESTS_DIR/main.cpp

DISTFILES += \
    Resources/docs/source.md

#thrift -r -v --gen cpp
