# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make test executable
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp)
add_executable(popl_test ${TEST_SOURCES})
configure_file(test.conf ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
target_link_libraries(popl_test Catch)
