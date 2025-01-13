
# Recursively collect all header files from the current directory
file(GLOB_RECURSE HEADER_FILES_TOP  ${CMAKE_SOURCE_DIR}/include/jsonimpex/*.h )

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

  add_library(gencpp STATIC ${gencpp_SOURCES})
  target_link_libraries(gencpp PRIVATE thrift)
  install( TARGETS gencpp
       EXPORT jsonimpexTargets DESTINATION "lib" COMPONENT libraries)

  # Create the shared library using the collected source files
  #add_library(JSONIMPEX_OBJECTS OBJECT ${HEADER_FILES_TOP} ${HEADER_FILES} ${SOURCE_FILES} )
  #add_dependencies(JSONIMPEX_OBJECTS gencpp)

# Check if a shared library is to be built
if(BUILD_IMPEX_SHARED_LIBS)
  # Enable automatic creation of a module definition (.def) file for a SHARED library on Windows.
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

  add_library(jsonimpex SHARED ${HEADER_FILES_TOP} ${HEADER_FILES} ${SOURCE_FILES})
  add_library(jsonimpex::jsonimpex ALIAS jsonimpex)
  target_link_libraries(jsonimpex
                        PUBLIC jsonio::jsonio
                        PRIVATE gencpp
                        PRIVATE lua
                        PRIVATE yaml-cpp
                        PRIVATE pugixml )

  install( TARGETS jsonimpex
       EXPORT jsonimpexTargets DESTINATION "lib" COMPONENT libraries)

  # Install debug symbols
  if(MSVC)
     install(
           FILES $<TARGET_PDB_FILE:jsonimpex>
           DESTINATION ${CMAKE_INSTALL_BINDIR}
           COMPONENT libraries
           OPTIONAL)
  endif()
endif()

# Check if a static library is to be built
if(BUILD_IMPEX_STATIC_LIBS)
  add_library(jsonimpex-static STATIC ${HEADER_FILES_TOP} ${HEADER_FILES} ${SOURCE_FILES})
  add_library(jsonimpex::jsonimpex-static ALIAS jsonimpex-static)
  target_link_libraries(jsonimpex-static
                        PUBLIC jsonio::jsonio
                        PRIVATE gencpp
                        PRIVATE lua
                        PRIVATE yaml-cpp
                        PRIVATE pugixml)

  install( TARGETS jsonimpex-static
          EXPORT jsonimpexTargets DESTINATION "lib" COMPONENT libraries)
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

else()  # without thrift make

  # Recursively collect all header files from the current directory
  file(GLOB_RECURSE HEADER_FILES  *.h )

  # Recursively collect all source files from the current directory
  file(GLOB_RECURSE SOURCE_FILES  *.cpp )

  # Create the shared library using the collected source files
  #add_library(JSONIMPEX_OBJECTS OBJECT ${HEADER_FILES_TOP} ${HEADER_FILES} ${SOURCE_FILES} )

  # Check if a shared library is to be built
  if(BUILD_IMPEX_SHARED_LIBS)
    # Enable automatic creation of a module definition (.def) file for a SHARED library on Windows.
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

    add_library(jsonimpex SHARED ${HEADER_FILES_TOP} ${HEADER_FILES} ${SOURCE_FILES})
    add_library(jsonimpex::jsonimpex ALIAS jsonimpex)
    target_link_libraries(jsonimpex
                          PUBLIC jsonio::jsonio
                          PRIVATE lua
                          PRIVATE yaml-cpp
                          PRIVATE pugixml )

    install( TARGETS jsonimpex
         EXPORT jsonimpexTargets DESTINATION "lib" COMPONENT libraries)

    # Install debug symbols
    if(MSVC)
       install(
             FILES $<TARGET_PDB_FILE:jsonimpex>
             DESTINATION ${CMAKE_INSTALL_BINDIR}
             COMPONENT libraries
             OPTIONAL)
    endif()
  endif()

  # Check if a static library is to be built
  if(BUILD_IMPEX_STATIC_LIBS)
    add_library(jsonimpex-static STATIC ${HEADER_FILES_TOP} ${HEADER_FILES} ${SOURCE_FILES})
    add_library(jsonimpex::jsonimpex-static ALIAS jsonimpex-static)
    target_link_libraries(jsonimpex-static
                          PUBLIC jsonio::jsonio-static
                          PRIVATE lua
                          PRIVATE yaml-cpp
                          PRIVATE pugixml)

    install( TARGETS jsonimpex-static
            EXPORT jsonimpexTargets DESTINATION "lib" COMPONENT libraries)
  endif()

endif()


install(
    DIRECTORY   "${JSONIMPEX_HEADER_DIR}/jsonimpex"
    DESTINATION include
)
                  
