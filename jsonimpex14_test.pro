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

# Define the directory where jsonimpex14 source code is located
JSONIMPEX14_DIR =  $$PWD/src
JSONIMPEX14_HEADERS_DIR =  $$PWD/include
TESTS_DIR =  $$PWD/tests

DEPENDPATH   += $$JSONIMPEX14_DIR
DEPENDPATH   += $$JSONIMPEX14_HEADERS_DIR
DEPENDPATH   += $$TESTS_DIR

INCLUDEPATH   += $$JSONIMPEX14_DIR
INCLUDEPATH   += $$JSONIMPEX14_HEADERS_DIR
INCLUDEPATH   += $$TESTS_DIR

LIBS +=  -lstdc++fs
LIBS +=  -ljsonio14 -ljsonarango -lcurl  -lvelocypack
LIBS += -lyaml-cpp  -lpugixml -lthrift
macx-clang:LIBS += -llua
!macx-clang:LIBS += -llua5.3

OBJECTS_DIR   = obj

include($$TESTS_DIR/gtest_dependency.pri)
include($$JSONIMPEX14_DIR/jsonimpex14.pri)

HEADERS += \
        $$TESTS_DIR/tst_jsonimpex.h \

SOURCES += \
        $$TESTS_DIR/main.cpp

DISTFILES += \
    Resources/docs/source.md
