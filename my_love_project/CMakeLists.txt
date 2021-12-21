cmake_minimum_required(VERSION 3.5)

project(my_love_project LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_library(client SHARED ./src/client.cpp)
target_link_libraries(client pthread ${Boost_LIBRARIES} nlohmann_json::nlohmann_json)

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

set(SET_CREATE_NEW_TASK
        src/create_new_task.cpp
        include/create_new_task.h
        src/create_new_task.ui
)

set(EDIT_TASK
        src/edittask.cpp
        include/edittask.h
        src/edittask.ui
)

set(PROJECT_SOURCES
      ${MAIN_FILES}
      ${CAT_FORM_FILES}
      ${REG_FORM_FILES}
      ${PLAN_FILES}
      ${SET_FORM_FILES}
      ${SET_CREATE_NEW_TASK}
      ${EDIT_TASK}
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

target_link_libraries(my_love_project PRIVATE client Qt${QT_VERSION_MAJOR}::Widgets)
