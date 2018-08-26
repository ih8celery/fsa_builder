cmake_minimum_required (VERSION 3.5)

project (FSABuilder LANGUAGES CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set (HOME ${CMAKE_CURRENT_LIST_DIR})
set (PROJECT_HEADERS "${HOME}/lib/include")

#enable_testing ()

include_directories (${PROJECT_HEADERS})

add_subdirectory(lib/src)

install (FILES "${PROJECT_HEADERS}/fsa_builder.h"
         DESTINATION include)
