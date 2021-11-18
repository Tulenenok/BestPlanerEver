cmake_minimum_required(VERSION 3.5)

project(my_love_project LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(include)

set(MAIN_FILES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        src/mainwindow.ui
)

set(CAT_FORM_FILES
        src/catform.cpp
        include/catform.h
        src/catform.ui
)

set(REG_FORM_FILES
        src/regform.cpp
        include/regform.h
        src/regform.ui
)

set(PLAN_FILES
        src/planwindow.cpp
        include/planwindow.h
        src/planwindow.ui
)

set(SET_FORM_FILES
        src/settingsform.cpp
        include/settingsform.h
        src/settingsform.ui
)


set(PROJECT_SOURCES
      ${MAIN_FILES}
      ${CAT_FORM_FILES}
      ${REG_FORM_FILES}
      ${PLAN_FILES}
      ${SET_FORM_FILES}
)

set(RESOURCES
        sources/resources.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(my_love_project
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(my_love_project SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(my_love_project
            ${PROJECT_SOURCES} ${RESOURCES}
        )
    endif()
endif()

target_link_libraries(my_love_project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
