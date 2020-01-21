cmake_minimum_required(VERSION 3.5)

project(OpenGLExample LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


list (APPEND CMAKE_PREFIX_PATH "C:\\vcpkg\\installed\\x64-windows")

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" CACHE PATH "toolchain file")
endif()

# set(glfw3_DIR "C:\\vcpkg\\installed\\x64-windows\\share\\glfw3")
# set(GLEW_DIR "C:\\vcpkg\\installed\\x64-windows\\share\\glew")
# set(GLEW_LIBRARY "C:\\vcpkg\\installed\\x64-windows\\lib\\glew32.lib")
# set(GLEW_INCLUDE_DIR "C:\\vcpkg\\installed\\x64-windows\\include\\GL")
# set(glm_DIR "C:\\vcpkg\\installed\\x64-windows\\share\\glm")

# QtCreator supports the following variables for Android, which are identical to qmake  Android variables.
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

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

if(ANDROID)
  add_library(OpenGLExample SHARED
    main.cpp
    openglwidget.cpp
    openglwidget.h
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(OpenGLExample
    main.cpp
    openglwidget.cpp
    openglwidget.h
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
endif()

target_link_libraries(OpenGLExample Qt5::Widgets
    glfw GLEW::GLEW ${OPENGL_LIBRARY} glm)

