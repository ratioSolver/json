cmake_minimum_required(VERSION 3.5.0)
project(json VERSION 0.2.0 LANGUAGES CXX)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")

include(CTest)
enable_testing()

add_library(json src/json.cpp)
add_dependencies(json utils)
target_compile_features(json PUBLIC cxx_std_17)
target_include_directories(json PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
if(NOT TARGET utils)
    add_subdirectory(extern/utils)
    add_dependencies(json utils)
endif()
target_link_libraries(json PUBLIC utils)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

setup_sanitizers(json)

set(CPACK_PROJECT_NAME json)
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
