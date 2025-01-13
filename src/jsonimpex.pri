
HEADERS += \
    $$JSONIMPEX_DIR/lua-run.h \
    $$JSONIMPEX_HEADERS_DIR/jsonimpex/xmldump.h \
    $$JSONIMPEX_HEADERS_DIR/jsonimpex/yamldump.h \
    $$JSONIMPEX_HEADERS_DIR/jsonimpex/yaml_xml2file.h \
    $$JSONIMPEX_HEADERS_DIR/jsonimpex/thrift_impex.h \
    $$JSONIMPEX_HEADERS_DIR/jsonimpex/impex_generator.h \
    $$JSONIMPEX_DIR/TJSONSerializer.h \
    $$JSONIMPEX_DIR/thrift_ie_base.h \
    $$JSONIMPEX_DIR/thrift_ie_json.h \
    $$JSONIMPEX_DIR/thrift_ie_keyvalue.h \
    $$JSONIMPEX_DIR/thrift_ie_table.h


SOURCES += \
    $$JSONIMPEX_DIR/lua-run.cpp \
    $$JSONIMPEX_DIR/xmldump.cpp \
    $$JSONIMPEX_DIR/yamldump.cpp \
    $$JSONIMPEX_DIR/yaml_xml2file.cpp \
    $$JSONIMPEX_DIR/TJSONSerializer.cpp \
    $$JSONIMPEX_DIR/thrift_ie_base.cpp \
    $$JSONIMPEX_DIR/thrift_ie_json.cpp \
    $$JSONIMPEX_DIR/thrift_ie_keyvalue.cpp \
    $$JSONIMPEX_DIR/thrift_ie_table.cpp \
    $$JSONIMPEX_DIR/thrift_impex.cpp \
    $$JSONIMPEX_DIR/impex_generator.cpp


!contains(DEFINES, USE_THRIFT) {

HEADERS += \
    $$JSONIMPEX_DIR/no_thrift/impex_base.h \
    $$JSONIMPEX_DIR/no_thrift/impex_types_fix.h

SOURCES += \
    $$JSONIMPEX_DIR/no_thrift/impex_types_fix.cpp

}
