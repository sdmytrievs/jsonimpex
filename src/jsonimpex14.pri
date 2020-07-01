
HEADERS += \
    $$JSONIMPEX14_DIR/lua-run.h \
    $$JSONIMPEX14_HEADERS_DIR/xmldump.h \
    $$JSONIMPEX14_HEADERS_DIR/yamldump.h \
    $$PWD/../include/jsonimpex14/dbdriver_thrift.h


SOURCES += \
    $$JSONIMPEX14_DIR/lua-run.cpp \
    $$JSONIMPEX14_DIR/xmldump.cpp \
    $$JSONIMPEX14_DIR/yamldump.cpp \
    $$PWD/dbdriver_thrift.cpp



