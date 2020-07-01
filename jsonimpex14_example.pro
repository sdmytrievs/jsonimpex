TEMPLATE = app
CONFIG += thread console c++17
CONFIG -= app_bundle
CONFIG -= qt

!win32 {
  DEFINES += __unix
QMAKE_CFLAGS += pedantic -Wall -Wextra -Wwrite-strings -Werror
#QMAKE_CXXFLAGS += -ansi -pedantic -Wall -Wextra -Weffc++
QMAKE_CXXFLAGS += -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith \
 -Wmissing-declarations -Winline -Wundef \ #-Weffc++ \
 -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter \
 -Wfloat-equal -pedantic -ansi

#QMAKE_CXXFLAGS += -pg  # gprof information

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
THRIFT_DIR    = $$PWD/thrift
THRIFT_HEADERS_DIR =  $$THRIFT_DIR/gen-cpp

DEPENDPATH   += $$JSONIMPEX14_DIR
DEPENDPATH   += $$JSONIMPEX14_HEADERS_DIR
DEPENDPATH   += $$THRIFT_HEADERS_DIR

INCLUDEPATH   += $$JSONIMPEX14_DIR
INCLUDEPATH   += $$JSONIMPEX14_HEADERS_DIR
INCLUDEPATH   += $$THRIFT_HEADERS_DIR

LIBS +=  -lstdc++fs
LIBS +=  -ljsonio14 -ljsonarango -lcurl  -lvelocypack
LIBS += -lyaml-cpp  -lpugixml -lthrift
macx-clang:LIBS += -llua
!macx-clang:LIBS += -llua5.3

#LIBS += -pg  # gprof information

OBJECTS_DIR   = obj

include($$THRIFT_DIR/thrift.pri)
include($$JSONIMPEX14_DIR/jsonimpex14.pri)

SOURCES += \
       main.cpp

DISTFILES += \
    Resources/docs/source.md \


#thrift -r -v --gen json
