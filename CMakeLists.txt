cmake_minimum_required(VERSION 3.25)

project(GauzyUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME})

set(CPP_FILES
    "${CMAKE_SOURCE_DIR}/src/Test.cpp"
    "${CMAKE_SOURCE_DIR}/src/Util.cpp"
    "${CMAKE_SOURCE_DIR}/src/Window.cpp"
    "${CMAKE_SOURCE_DIR}/src/comp/CompGroup.cpp"
    "${CMAKE_SOURCE_DIR}/src/comp/Component.cpp"
    "${CMAKE_SOURCE_DIR}/src/event/EventContext.cpp"
    "${CMAKE_SOURCE_DIR}/src/graphic/Brush.cpp"
    "${CMAKE_SOURCE_DIR}/src/graphic/Renderer.cpp"
    "${CMAKE_SOURCE_DIR}/src/graphic/TextFormat.cpp"
    "${CMAKE_SOURCE_DIR}/src/type/BasicTypes.cpp"
    "${CMAKE_SOURCE_DIR}/src/type/Color.cpp"
    "${CMAKE_SOURCE_DIR}/src/type/WindowHandle.cpp"
)
set(H_FILES
    "${CMAKE_SOURCE_DIR}/include/Util.hpp"
    "${CMAKE_SOURCE_DIR}/include/Window.hpp"
    "${CMAKE_SOURCE_DIR}/include/comp/CompGroup.hpp"
    "${CMAKE_SOURCE_DIR}/include/comp/Component.hpp"
    "${CMAKE_SOURCE_DIR}/include/event/Event.hpp"
    "${CMAKE_SOURCE_DIR}/include/event/EventContext.hpp"
    "${CMAKE_SOURCE_DIR}/include/graphic/Brush.hpp"
    "${CMAKE_SOURCE_DIR}/include/graphic/Renderer.hpp"
    "${CMAKE_SOURCE_DIR}/include/graphic/TextFormat.hpp"
    "${CMAKE_SOURCE_DIR}/include/type/BasicTypes.hpp"
    "${CMAKE_SOURCE_DIR}/include/type/Color.hpp"
    "${CMAKE_SOURCE_DIR}/include/type/WindowHandle.hpp"
)
target_sources(${PROJECT_NAME} PUBLIC ${CPP_FILES} ${H_FILES})

find_package(Eigen3 CONFIG REQUIRED)
find_package(wil CONFIG REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE comctl32.lib d2d1.lib dwrite.lib Eigen3::Eigen WIL::WIL)

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
add_compile_options("/source-charset:utf-8")
