cmake_minimum_required(VERSION 3.21)

add_library(miniaudio INTERFACE)
target_include_directories(miniaudio INTERFACE INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)