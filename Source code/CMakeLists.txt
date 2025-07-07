cmake_minimum_required(VERSION 3.16)
project(FaceRecognition)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# vcpkg toolchain
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
else()
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/vcpkg-master/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(dlib CONFIG REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB SRC
    "main.cpp"
    "mainwindow.cpp"
    "CameraManager.cpp"
    "FaceDetector.cpp"
    "FaceRecognizer.cpp"
    "FaceDatabase.cpp"
)
file(GLOB HEADERS
    "mainwindow.h"
    "CameraManager.h"
    "FaceDetector.h"
    "FaceRecognizer.h"
    "FaceDatabase.h"
)

add_executable(FaceRecognition ${SRC} ${HEADERS})

target_link_libraries(FaceRecognition
    Qt6::Widgets
    ${OpenCV_LIBS}
    dlib::dlib
)

# 解决Windows下控制台窗口弹出问题
if (WIN32)
    set_target_properties(FaceRecognition PROPERTIES WIN32_EXECUTABLE TRUE)
endif() 