cmake_minimum_required(VERSION 3.5)

project(ahs LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Network Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network Sql REQUIRED)

if(ANDROID)
  add_library(ahs SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    loginform.cpp
    loginform.h
    loginform.ui
    network.h
    network.cpp
    config.h
    config.cpp
    tokenizer.h
    tokenizer.cpp
    devicemodel.cpp
    devicemodel.h
    registerdeviceform.cpp
    registerdeviceform.h
    registerdeviceform.ui
    deviceform.cpp
    deviceform.h
    deviceform.ui
    registeruserform.cpp
    registeruserform.h
    registeruserform.ui
    statuslistwidgetitem.cpp
    statuslistwidgetitem.h
  )
else()
  add_executable(ahs
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    loginform.cpp
    loginform.h
    loginform.ui
    network.h
    network.cpp
    config.h
    config.cpp
    tokenizer.h
    tokenizer.cpp
    devicemodel.cpp
    devicemodel.h
    registerdeviceform.cpp
    registerdeviceform.h
    registerdeviceform.ui
    deviceform.cpp
    deviceform.h
    deviceform.ui
    registeruserform.cpp
    registeruserform.h
    registeruserform.ui
    statuslistwidgetitem.cpp
    statuslistwidgetitem.h
  )
endif()

target_link_libraries(ahs PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Sql)
