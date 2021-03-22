
# Recursively collect all header files from the current directory
file(GLOB_RECURSE HEADER_FILES_TOP  ${CMAKE_SOURCE_DIR}/include/jsonimpex17/*.h )

if(JSONIMPEX_USE_THRIFT MATCHES ON)

  add_definitions( -DUSE_THRIFT )

# Collect all header files from the current directory
file(GLOB HEADER_FILES  *.h )

# Collect all source files from the current directory
file(GLOB SOURCE_FILES  *.cpp )

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
add_library(JSONIMPEX17_OBJECTS OBJECT ${HEADER_FILES_TOP} ${HEADER_FILES} ${SOURCE_FILES} )

add_dependencies(JSONIMPEX17_OBJECTS gencpp)

# Check if a shared library is to be built
if( BUILD_IMPEX_SHARED_LIBS )
    add_library(JSONIMPEX17_SHARED SHARED $<TARGET_OBJECTS:JSONIMPEX17_OBJECTS>)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
      target_link_libraries(JSONIMPEX17_SHARED
            PUBLIC gencpp
            PUBLIC jsonio17-static
            PUBLIC jsonarango-static
            PRIVATE velocypack
            PRIVATE libcurl
            PRIVATE thriftmd
            PRIVATE lua
            PRIVATE yaml-cpp
            PRIVATE pugixml )
else()
      target_link_libraries( JSONIMPEX17_SHARED
           PUBLIC gencpp
           PUBLIC stdc++fs
           PUBLIC jsonio17
           PUBLIC jsonarango
           PUBLIC velocypack
           PUBLIC curl
           PRIVATE thrift
           PRIVATE lua
           PRIVATE yaml-cpp
           PRIVATE pugixml
           PUBLIC pthread )
endif()

    set_target_properties(JSONIMPEX17_SHARED PROPERTIES OUTPUT_NAME jsonimpex17)
    install(TARGETS JSONIMPEX17_SHARED DESTINATION lib)
endif()

# Check if a static library is to be built
if( BUILD_IMPEX_STATIC_LIBS )
    add_library(JSONIMPEX17_STATIC STATIC $<TARGET_OBJECTS:JSONIMPEX17_OBJECTS>)
    target_link_libraries(JSONIMPEX17_STATIC gencpp )
    set_target_properties(JSONIMPEX17_STATIC PROPERTIES OUTPUT_NAME jsonimpex17-static)
    install(TARGETS JSONIMPEX17_STATIC DESTINATION lib)
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

else()
# without thrift make

# Recursively collect all header files from the current directory
file(GLOB_RECURSE HEADER_FILES  *.h )

# Recursively collect all source files from the current directory
file(GLOB_RECURSE SOURCE_FILES  *.cpp )

# Create the shared library using the collected source files
add_library(JSONIMPEX17_OBJECTS OBJECT ${HEADER_FILES_TOP} ${HEADER_FILES} ${SOURCE_FILES} )

# Check if a shared library is to be built
if( BUILD_IMPEX_SHARED_LIBS )
    add_library(JSONIMPEX17_SHARED SHARED $<TARGET_OBJECTS:JSONIMPEX17_OBJECTS>)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
      target_link_libraries(JSONIMPEX17_SHARED
            PUBLIC jsonio17-static
            PUBLIC jsonarango-static
            PRIVATE velocypack
            PRIVATE libcurl
            PRIVATE lua
            PRIVATE yaml-cppd
            PRIVATE pugixml )
else()
      target_link_libraries( JSONIMPEX17_SHARED
           PUBLIC stdc++fs
           PUBLIC jsonio17
           PUBLIC jsonarango
           PUBLIC velocypack
           PUBLIC curl
           PRIVATE lua
           PRIVATE yaml-cpp
           PRIVATE pugixml
           PUBLIC pthread )
endif()

    set_target_properties(JSONIMPEX17_SHARED PROPERTIES OUTPUT_NAME jsonimpex17)
    install(TARGETS JSONIMPEX17_SHARED DESTINATION lib)
endif()

# Check if a static library is to be built
if( BUILD_IMPEX_STATIC_LIBS )
    add_library(JSONIMPEX17_STATIC STATIC $<TARGET_OBJECTS:JSONIMPEX17_OBJECTS>)
    set_target_properties(JSONIMPEX17_STATIC PROPERTIES OUTPUT_NAME jsonimpex17-static)
    install(TARGETS JSONIMPEX17_STATIC DESTINATION lib)
endif()

endif()

install(
    DIRECTORY   "${JSONIMPEX17_HEADER_DIR}/jsonimpex17"
    DESTINATION include
)
                  
