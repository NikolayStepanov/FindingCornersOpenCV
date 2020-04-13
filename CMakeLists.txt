cmake_minimum_required(VERSION 3.5)

project(ImageProcessing LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)

set ( SOURCES
    main.cpp
    mainwindow.cpp
    ImgEffectsWorkerThread.cpp
    )

set ( HEADERS
    mainwindow.h
    ImgConverterQTvsOpenCV.h
    ImgEffectsWorkerThread.h
    )

add_executable(ImageProcessing  ${SOURCES} ${HEADERS})

target_link_libraries(ImageProcessing PRIVATE Qt5::Widgets ${OpenCV_LIBS})
