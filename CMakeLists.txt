#    CMakeLists.txt for afsm library
#
#    @author zmij
#    @date Nov 30, 2015

cmake_minimum_required(VERSION 2.6)

# Set library name here
set(lib_name metapushkin)
string(TOUPPER ${lib_name} LIB_NAME)

set(_pname ${lib_name})
if (PROJECT_VERSION)
    set(_pversion ${PROJECT_VERSION})
else()
    set(_pversion 0.1.0)
endif()

if (${CMAKE_VERSION} VERSION_GREATER "3.0")
    cmake_policy(SET CMP0048 NEW)
    project(${_pname} VERSION ${_pversion})
else()
    project(${_pname})
    set(PROJECT_VERSION ${_pversion})
endif()

option(META_BUILD_TESTS "Build test programs" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()
message(STATUS "C++ standard ${CMAKE_CXX_STANDARD}")
add_definitions(-Wall -Werror -Wextra -pedantic -Weffc++)

# Add subdirectories here

add_subdirectory(include)
add_subdirectory(cmake)

if (META_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

get_directory_property(has_parent PARENT_DIRECTORY)
if (has_parent)
    set(${LIB_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE
        INTERNAL "Path to metapushkin libaray includes" )
endif()
