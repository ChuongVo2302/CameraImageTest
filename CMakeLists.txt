cmake_minimum_required(VERSION 3.15)
project(CameraModuleTestSoftware CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR "C:/opencv/build")
find_package(OpenCV REQUIRED)

add_executable(CameraTest
    src/main.cpp
    src/ImageAnalyzer.cpp
    src/Logger.cpp
    include/ImageAnalyzer.h
    include/Logger.h
    include/TestTypes.h
)

target_include_directories(CameraTest 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(CameraTest 
    PRIVATE 
        ${OpenCV_LIBS}
)
