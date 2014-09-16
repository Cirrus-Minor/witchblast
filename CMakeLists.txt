cmake_minimum_required(VERSION 2.8.12)

# Projet name
project("Witch_Blast")

# Detect compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(USING_GCC TRUE)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(USING_CLANG TRUE)
endif()

# For GCC and Clang, enable C++11 support and add some other flags
if(USING_GCC OR USING_CLANG)
  add_compile_options(-std=c++11 -pedantic -Wall)
endif()

include_directories(.)

file(
        GLOB_RECURSE
        source_files
        src/*
)

add_executable(
        "Witch_Blast"
        ${source_files}
)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.1 REQUIRED system window graphics audio)
target_link_libraries(Witch_Blast ${SFML_LIBRARIES})

include_directories(${SFML_INCLUDE_DIR})

Message(${SFML_LIBRARIES})
