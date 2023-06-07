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
  target_link_libraries(test_main gtest_main jsonimpex17::jsonimpex17-static lua Threads::Threads)
else()
  target_link_libraries(test_main gtest jsonimpex17::jsonimpex17 lua Threads::Threads)
endif()

target_include_directories(test_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src )

add_test(NAME test_main COMMAND ./tests/test_main)

