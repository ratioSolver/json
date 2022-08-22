add_executable(json_lib_tests test_json.cpp)
target_link_libraries(json_lib_tests PRIVATE json)

add_test(NAME JSON_LibTest COMMAND json_lib_tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR})