cmake_minimum_required(VERSION 3.8)

project(GauzyUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE H_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.hpp")
target_sources(${PROJECT_NAME} PUBLIC ${CPP_FILES} ${H_FILES})

find_package(Eigen3 CONFIG REQUIRED)
find_package(wil CONFIG REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE comctl32.lib d2d1.lib dwrite.lib Eigen3::Eigen WIL::WIL)

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
add_compile_options("/source-charset:utf-8")
