cmake_minimum_required(VERSION 3.25)

# Which type of build?
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # This is the top-level project
    project(SimpleLogger)

else ()
    # Export the include directory
    set(SIMPLE_LOGGER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

endif ()

# Options for the project
if (NOT DEFINED SIMPLE_LOGGER_MIN_LOG_LEVEL)
    set(SIMPLE_LOGGER_MIN_LOG_LEVEL 4)
endif ()

option(BUILD_LOGGER_EXAMPLE "Build the logger example" OFF)

# Required C++ version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------- SimpleLogger Library ---------------------
add_library(SimpleLogger STATIC
        # Headers
        include/simplelogger.hpp
        include/loggerloc.hpp
        include/logexception.hpp

        # Sources
        src/simplelogger.cpp
        src/loggerloc.cpp
)

target_include_directories(SimpleLogger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (DEFINED ENABLE_STD_FORMAT)
    target_compile_definitions(SimpleLogger PUBLIC SL_ENABLE_STD_FORMAT=${ENABLE_STD_FORMAT})
endif ()

# Debugging
if (CMAKE_BUILD_TYPE STREQUAL "Debug")

    message(STATUS "Building SimpleLogger with a log level of ${SIMPLE_LOGGER_MIN_LOG_LEVEL}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSF_MIN_LOG_LEVEL=${SIMPLE_LOGGER_MIN_LOG_LEVEL}")

else ()
    # Release build
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")

endif ()

if (BUILD_LOGGER_EXAMPLE)
    # Build the example
    add_executable(LoggerExample
            example/logger_example.cpp
    )
    target_link_libraries(LoggerExample SimpleLogger)
endif ()