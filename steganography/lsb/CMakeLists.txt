cmake_minimum_required(VERSION 3.9)
project(Steganography)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS true)

set(LIBRARY_NAME liblsb)
set(APP_NAME lsb)

find_package(OpenCV 4.0.1 REQUIRED)

add_library(${LIBRARY_NAME} src/Lsb.cpp)
target_link_libraries(${LIBRARY_NAME} ${OpenCV_LIBS})
set_target_properties(${LIBRARY_NAME} PROPERTIES PREFIX "")

add_executable(${APP_NAME} app/main.cpp)
target_link_libraries(${APP_NAME} ${LIBRARY_NAME})

target_include_directories(${LIBRARY_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)