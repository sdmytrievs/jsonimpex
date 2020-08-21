
HEADERS += \
    $$JSONIMPEX17_DIR/lua-run.h \
    $$JSONIMPEX17_HEADERS_DIR/jsonimpex17/xmldump.h \
    $$JSONIMPEX17_HEADERS_DIR/jsonimpex17/yamldump.h \
    $$JSONIMPEX17_HEADERS_DIR/jsonimpex17/yaml_xml2file.h \
    $$JSONIMPEX17_HEADERS_DIR/jsonimpex17/dbdriver_thrift.h \
    $$PWD/../include/jsonimpex17/thrift_impex.h \
    $$PWD/TJSONSerializer.h \
    $$PWD/thrift_ie_base.h \
    $$PWD/thrift_ie_json.h \
    $$PWD/thrift_ie_keyvalue.h \
    $$PWD/thrift_ie_table.h


SOURCES += \
    $$JSONIMPEX17_DIR/lua-run.cpp \
    $$JSONIMPEX17_DIR/xmldump.cpp \
    $$JSONIMPEX17_DIR/yamldump.cpp \
    $$JSONIMPEX17_DIR/yaml_xml2file.cpp \
    $$JSONIMPEX17_DIR/dbdriver_thrift.cpp \
    $$PWD/TJSONSerializer.cpp \
    $$PWD/thrift_ie_base.cpp \
    $$PWD/thrift_ie_json.cpp \
    $$PWD/thrift_ie_keyvalue.cpp \
    $$PWD/thrift_ie_table.cpp \
    $$PWD/thrift_impex.cpp



