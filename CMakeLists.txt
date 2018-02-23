project (test)

cmake_minimum_required(VERSION 2.8.11)

file(GLOB HDRS	"*.h")
file(GLOB SRCS	"*.cpp")

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
add_executable(test ${SRCS} ${HDRS})

target_link_libraries(test ${OpenCL_LIBRARIES})