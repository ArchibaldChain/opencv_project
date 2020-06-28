cmake_minimum_required(VERSION 3.16)
project(opencv_project)

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(
        /usr/include
        /usr/include/opencv2
        /usr/include/opencv
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}
)


set(flppybird
        flppybird/Flppybird.cpp
        flppybird/Flppybird.h
        )

link_directories(/usr/lib ${CMAKE_BINARY_DIR}/bin)

set(code
        code/cvui.h
        code/gesture.h
        code/hci_control.h
        code/main.cpp
        code/traject_cap.h
        code/traject_face.h
        code/flppybird/Flppybird.h
        )
add_executable(code ${code})
target_link_libraries(code  ${OpenCV_LIBS})

