cmake_minimum_required(VERSION 3.16)
project(file-monitor VERSION 2.1.0 LANGUAGES CXX)

# Add Homebrew Qt5 paths
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@5")
link_directories("/opt/homebrew/opt/qt@5/lib")
include_directories("/opt/homebrew/opt/qt@5/include")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt package
find_package(Qt5 COMPONENTS Widgets Svg Xml WebEngineWidgets REQUIRED)

set(SOURCES
    source/main.cpp
    source/mainwindow.cpp
    source/tabwidget.cpp
    source/tabpage.cpp
    source/view.cpp
    source/svgview.cpp
    source/textview.cpp
    source/imageview.cpp
    source/utility.cpp
    source/customgraphicsview.cpp
    source/filesystemwatcher.cpp
    source/zoom.cpp
    source/customtextedit.cpp
    source/contextmenus.cpp
    source/finddialog.cpp
    source/htmlview.cpp
    source/customwebview.cpp
)

set(HEADERS
    source/mainwindow.hpp
    source/tabwidget.hpp
    source/tabpage.hpp
    source/view.hpp
    source/svgview.hpp
    source/textview.hpp
    source/imageview.hpp
    source/utility.hpp
    source/customgraphicsview.hpp
    source/filesystemwatcher.hpp
    source/zoom.hpp
    source/customtextedit.hpp
    source/contextmenus.hpp
    source/finddialog.hpp
)

set(UI_FILES
    source/mainwindow.ui
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Widgets
    Qt5::Svg
    Qt5::Xml
    Qt5::WebEngineWidgets
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
