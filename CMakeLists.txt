cmake_minimum_required(VERSION 3.16)
project(ContainersServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Xml)

qt_add_executable(ContainersServer
    main.cpp
    server.cpp
    server.h
    containertablemodel.cpp
    containertablemodel.h
    resources.qrc
)

target_link_libraries(ContainersServer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Xml
)
