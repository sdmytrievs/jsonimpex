message(STATUS "Building test suite: ${BuildTests}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/lua" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

include(CTest)
enable_testing()
find_package(Threads)
add_subdirectory(googletest REQUIRED)

# -------------------------------------
# Build test program
# -------------------------------------

add_executable(test_main  main.cpp )

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
      target_link_libraries( test_main
            PUBLIC gtest
            PUBLIC jsonimpex17-static
            PUBLIC jsonio17-static
            PUBLIC jsonarango-static
            PRIVATE velocypack
            PRIVATE libcurl
            #PRIVATE thriftmd
            PRIVATE lua
            PRIVATE yaml-cpp
            PRIVATE pugixml
            )
else()
      target_link_libraries( test_main
           PUBLIC gtest
           PUBLIC stdc++fs
           PUBLIC jsonimpex17
           PUBLIC jsonio17
           PUBLIC jsonarango
           PUBLIC velocypack
           PUBLIC curl
           #PRIVATE thrift
           PRIVATE lua
           PRIVATE yaml-cpp
           PRIVATE pugixml
           PUBLIC pthread )
endif()

target_include_directories(test_main PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/../src
)

add_test(NAME test_main COMMAND ./tests/test_main)

