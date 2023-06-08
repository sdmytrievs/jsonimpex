# options
message(STATUS "Building jsonimpex17 examples: ${BuildExamples}")

include_directories(${CMAKE_SOURCE_DIR})
find_package(Threads REQUIRED)

set(Examples
        yaml_example
        xml_example )

foreach (exampleName ${Examples})
  add_executable(${exampleName} ${exampleName}.cpp)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
     target_link_libraries(${exampleName} jsonimpex17::jsonimpex17-static Threads::Threads )
  else()
     target_link_libraries(${exampleName} jsonimpex17::jsonimpex17 Threads::Threads)
  endif()
endforeach()

