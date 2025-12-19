cmake_minimum_required(VERSION 3.21)
project(json VERSION 0.2.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers) 

include(CTest)
enable_testing()

add_library(json src/json.cpp)
add_library(ratio::json ALIAS json)
target_compile_features(json PUBLIC cxx_std_17)
target_include_directories(json PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
if(NOT TARGET utils)
    add_subdirectory(extern/utils)
endif()
add_dependencies(json utils)
target_link_libraries(json PUBLIC utils)
setup_sanitizers(json)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

install(TARGETS json EXPORT json-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT json-targets FILE json-targets.cmake NAMESPACE ratio:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/json)
configure_package_config_file(cmake/json-config.cmake.in json-config.cmake INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/json)
write_basic_package_version_file(json-version.cmake COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/json-config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/json-version.cmake" DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/json)

set(CPACK_PROJECT_NAME json)
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
