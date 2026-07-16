cmake_minimum_required(VERSION 3.25)

project(GauzyUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    src/Test.cpp
    src/Util.cpp
    src/Window.cpp
    src/comp/ComponentGroup.cpp
    src/comp/Component.cpp
    src/event/EventContext.cpp
    src/graphic/Brush.cpp
    src/graphic/Renderer.cpp
    src/graphic/TextFormat.cpp
    src/type/BasicTypes.cpp
    src/type/Color.cpp
    src/type/WindowHandle.cpp
    include/Util.hpp
    include/Window.hpp
    include/comp/ComponentGroup.hpp
    include/comp/Component.hpp
    include/event/Event.hpp
    include/event/EventContext.hpp
    include/graphic/Brush.hpp
    include/graphic/Renderer.hpp
    include/graphic/TextFormat.hpp
    include/type/BasicTypes.hpp
    include/type/Color.hpp
    include/type/WindowHandle.hpp
    include/comp/Label.hpp
    include/graphic/Theme.hpp
    src/comp/Label.cpp
    include/event/ComponentEventContext.hpp
    include/event/WindowEventContext.hpp
    include/event/WindowMessageDispatcher.hpp
    src/event/WindowMessageDispatcher.cpp
    include/comp/Interactive.hpp
    include/type/InputTypes.hpp
    src/type/InputTypes.cpp
)

find_package(Eigen3 CONFIG REQUIRED)
find_package(wil CONFIG REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE comctl32.lib d2d1.lib dwrite.lib Eigen3::Eigen WIL::WIL)

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
add_compile_options("/source-charset:utf-8" "/EHsc")
