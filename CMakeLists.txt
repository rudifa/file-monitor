cmake_minimum_required(VERSION 3.16)
project(file-monitor VERSION 2.1.0 LANGUAGES CXX)

# Generate and create a relative symlink for compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt specific CMake features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set Qt6 directory and prefix path
set(Qt6_DIR "/Users/rudifarkas/Qt/6.8.2/macos/lib/cmake/Qt6")
set(CMAKE_PREFIX_PATH "${Qt6_DIR}")

# Find required Qt packages
find_package(Qt6 6.8.2 COMPONENTS Core Gui Widgets Svg SvgWidgets WebEngineWidgets Xml REQUIRED)

# Set include directories
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_BINARY_DIR})

# Define source files
set(PROJECT_SOURCES
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

set(PROJECT_HEADERS
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

set(PROJECT_UI_FILES
    source/mainwindow.ui
)

# Add executable
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_UI_FILES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Svg
    Qt6::SvgWidgets
    Qt6::WebEngineWidgets
    Qt6::Xml
)

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Install
if(APPLE)
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION /Applications
        RUNTIME DESTINATION /Applications
    )
else()
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()
