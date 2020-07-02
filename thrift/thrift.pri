THRIFTSOURCES = \
    $$THRIFT_DIR/dbserver.thrift \
    $$THRIFT_DIR/impex.thrift \
    $$THRIFT_DIR/query.thrift


SOURCES += \
    $$THRIFT_HEADERS_DIR/DBServer.cpp \
    #$$THRIFT_HEADERS_DIR/dbserver_constants.cpp \
    $$THRIFT_HEADERS_DIR/dbserver_types.cpp \
    $$THRIFT_HEADERS_DIR/impex_types.cpp \
    #$$THRIFT_HEADERS_DIR/impex_constants.cpp \
    $$THRIFT_HEADERS_DIR/query_types.cpp
    #$$THRIFT_HEADERS_DIR/query_constants.cpp

HEADERS += \
    $$THRIFT_HEADERS_DIR/DBServer.h \
    #$$THRIFT_HEADERS_DIR/dbserver_constants.h \
    $$THRIFT_HEADERS_DIR/dbserver_types.h \
    $$THRIFT_HEADERS_DIR/impex_types.h \
    #$$THRIFT_HEADERS_DIR/impex_constants.h \
    $$THRIFT_HEADERS_DIR/query_types.h
    #$$THRIFT_HEADERS_DIR/query_constants.h
