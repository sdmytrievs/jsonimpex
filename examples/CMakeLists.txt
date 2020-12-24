# options
message(STATUS "Building jsonimpex17 examples: ${BuildExamples}")

include_directories(${CMAKE_SOURCE_DIR})

set(Examples
        yaml_example
        xml_example )

foreach (exampleName ${Examples})

    add_executable(${exampleName} ${exampleName}.cpp)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
          target_link_libraries(${exampleName}
            PUBLIC jsonimpex17-static
            PUBLIC jsonio17-static
            PUBLIC jsonarango-static
            PRIVATE velocypack
            PRIVATE libcurl
            PRIVATE thriftmd
#            PRIVATE lua5.3
            PRIVATE lua
            PRIVATE yaml-cpp
            PRIVATE pugixml
            PUBLIC pthread )
    else()
          target_link_libraries( ${exampleName}
            PUBLIC stdc++fs
            PUBLIC jsonimpex17
            PUBLIC jsonio17
            PUBLIC jsonarango
            PUBLIC velocypack
            PUBLIC curl
            PRIVATE thrift
#            PRIVATE lua5.3
            PRIVATE lua
            PRIVATE yaml-cpp
            PRIVATE pugixml
            PUBLIC pthread )
    endif()

endforeach()


