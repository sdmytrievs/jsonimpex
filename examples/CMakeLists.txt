# options
message(STATUS "Building jsonimpex examples: ${BuildExamples}")
file(COPY "${CMAKE_SOURCE_DIR}/Resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
file(COPY "${CMAKE_SOURCE_DIR}/Resources/jsonimpex-config.json" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")


include_directories(${CMAKE_SOURCE_DIR})
find_package(Threads REQUIRED)

set(Examples
        yaml_example
        xml_example
        impex_tool )

foreach (exampleName ${Examples})
  add_executable(${exampleName} ${exampleName}.cpp)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
     target_link_libraries(${exampleName} jsonimpex::jsonimpex-static Threads::Threads )
  else()
     target_link_libraries(${exampleName} jsonimpex::jsonimpex Threads::Threads)
  endif()
endforeach()

