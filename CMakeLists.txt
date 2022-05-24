cmake_minimum_required(VERSION 3.21)
project(MyWord)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# qt5 prefix path
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@5/5.15.3")

# 头文件
include_directories(
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src/Headers"
)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(MyWord src/main.cpp src/myword.cpp src/Headers/myword.h src/Forms/myword.ui src/MyChild.cpp src/Headers/MyChild.h)
target_link_libraries(MyWord
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

