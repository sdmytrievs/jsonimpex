
# Recursively collect all header files from the current directory
file(GLOB_RECURSE HEADER_FILES_TOP  ${CMAKE_SOURCE_DIR}/include/jsonimpex14/*.h )

# Recursively collect all header files from the current directory
file(GLOB_RECURSE HEADER_FILES  *.h )

# Recursively collect all source files from the current directory
file(GLOB_RECURSE SOURCE_FILES  *.cpp )

# Create the thrift C++ test library
set( gencpp_SOURCES
    gen-cpp/impex_types.cpp
    gen-cpp/impex_types.h
    gen-cpp/query_types.cpp
    gen-cpp/query_types.h
    gen-cpp/dbserver_types.cpp
    gen-cpp/dbserver_types.h
    gen-cpp/DBServer.cpp
    gen-cpp/DBServer.h
)

set( gencpp_HEADER
    ${CMAKE_CURRENT_BINARY_DIR}/gen-cpp/impex_types.h
    ${CMAKE_CURRENT_BINARY_DIR}/gen-cpp/query_types.h
    ${CMAKE_CURRENT_BINARY_DIR}/gen-cpp/dbserver_types.h
    ${CMAKE_CURRENT_BINARY_DIR}/gen-cpp/DBServer.h
)


add_library( gencpp STATIC ${gencpp_SOURCES} )

# Create the shared library using the collected source files
add_library(JSONIMPEX14_OBJECTS OBJECT ${HEADER_FILES_TOP} ${HEADER_FILES} ${SOURCE_FILES} )

add_dependencies(JSONIMPEX14_OBJECTS gencpp)

# Check if a shared library is to be built
if( BUILD_IMPEX_SHARED_LIBS )
    add_library(JSONIMPEX14_SHARED SHARED $<TARGET_OBJECTS:JSONIMPEX14_OBJECTS>)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
      target_link_libraries(JSONIMPEX14_SHARED
            PUBLIC gencpp
            PUBLIC stdc++fs
            PUBLIC jsonio14-static
            PUBLIC jsonarango-static
            PRIVATE velocypack
            PRIVATE libcurl
            PRIVATE thrift
            PRIVATE lua5.3
            PRIVATE yaml-cpp
            PRIVATE pugixml
            PUBLIC pthread )
else()
      target_link_libraries( JSONIMPEX14_SHARED
           PUBLIC gencpp
           PUBLIC stdc++fs
           PUBLIC jsonio14
           PUBLIC jsonarango
           PUBLIC velocypack
           PUBLIC curl
           PRIVATE thrift
           PRIVATE lua5.3
           PRIVATE yaml-cpp
           PRIVATE pugixml
           PUBLIC pthread )
endif()

    set_target_properties(JSONIMPEX14_SHARED PROPERTIES OUTPUT_NAME jsonimpex14)
    install(TARGETS JSONIMPEX14_SHARED DESTINATION lib)
endif()

# Check if a static library is to be built
if( BUILD_IMPEX_STATIC_LIBS )
    add_library(JSONIMPEX14_STATIC STATIC $<TARGET_OBJECTS:JSONIMPEX14_OBJECTS>)
    target_link_libraries(JSONIMPEX14_STATIC gencpp )
    set_target_properties(JSONIMPEX14_STATIC PROPERTIES OUTPUT_NAME jsonimpex14-static)
    install(TARGETS JSONIMPEX14_STATIC DESTINATION lib)
endif()

add_custom_command( OUTPUT  gen-cpp/impex_types.h gen-cpp/impex_types.cpp
                    PRE_BUILD
                    COMMAND echo touching impex
                    COMMAND thrift -r -v --gen cpp ${CMAKE_SOURCE_DIR}/thrift/impex.thrift
                  )

add_custom_command( OUTPUT  gen-cpp/query_types.h gen-cpp/query_types.cpp
                    PRE_BUILD
                    COMMAND echo touching query
                    COMMAND thrift -r -v --gen cpp ${CMAKE_SOURCE_DIR}/thrift/query.thrift
                  )

add_custom_command( OUTPUT  gen-cpp/dbserver_types.h gen-cpp/DBServer.h
                            gen-cpp/dbserver_types.cpp gen-cpp/DBServer.cpp
                    PRE_BUILD
                    COMMAND echo touching dbserver
                    COMMAND thrift -r -v --gen cpp ${CMAKE_SOURCE_DIR}/thrift/dbserver.thrift
                  )

install(
    DIRECTORY   "${JSONIMPEX14_HEADER_DIR}/jsonimpex14"
    DESTINATION include
)
                  
