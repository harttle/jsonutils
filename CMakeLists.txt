cmake_minimum_required(VERSION 3.15)
project(jsonutils VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compile options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
include(FetchContent)

# Add nlohmann/json
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Add CLI11
FetchContent_Declare(cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
)
FetchContent_MakeAvailable(cli11)

# Find jq using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(JQ REQUIRED IMPORTED_TARGET libjq)

# Add GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Common library
add_library(jsonutils_lib
    src/jq_utils.cpp
)
target_include_directories(jsonutils_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${JQ_INCLUDE_DIRS}
)
target_link_libraries(jsonutils_lib 
    PUBLIC 
        nlohmann_json::nlohmann_json
        CLI11::CLI11
    PRIVATE
        PkgConfig::JQ
        CLI11::CLI11
)

# Commands
add_executable(jwhere src/commands/jwhere.cpp)
target_link_libraries(jwhere PRIVATE jsonutils_lib)

add_executable(jjoin src/commands/jjoin.cpp)
target_link_libraries(jjoin PRIVATE jsonutils_lib)

add_executable(jmap src/commands/jmap.cpp)
target_link_libraries(jmap PRIVATE jsonutils_lib)

# Tests
enable_testing()
add_executable(jsonutils_tests 
    tests/jwhere_test.cpp
    tests/jmap_test.cpp
    tests/jjoin_test.cpp
)
target_link_libraries(jsonutils_tests PRIVATE 
    jsonutils_lib 
    GTest::gtest_main
)
add_test(NAME jsonutils_tests COMMAND jsonutils_tests)

# Installation rules
install(TARGETS jwhere jjoin jmap jsonutils_lib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

# Package configuration
include(CPack)
set(CPACK_PACKAGE_NAME "jsonutils")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "JSON command line utilities")
set(CPACK_PACKAGE_VENDOR "Yang Jun") 