cmake_minimum_required(VERSION 3.13)
project(SEP7US)

set(CMAKE_CXX_STANDARD 23)

set (VersionResourceFile ${CMAKE_CURRENT_BINARY_DIR}/VersionResource.rc)

add_library(SEP7US SHARED SEP7US.cpp SEP7US.h VersionResource.rc Sorting.cpp Sorting.h )


set_target_properties(SEP7US PROPERTIES PREFIX "")
set_target_properties(SEP7US PROPERTIES OUTPUT_NAME "SEP7US")
set_target_properties(SEP7US PROPERTIES SUFFIX ".dll")

include_directories("C:\\Program Files\\Microsoft\\jdk-17.0.2.8-hotspot\\include" "C:\\Program Files\\Microsoft\\jdk-17.0.2.8-hotspot\\include\\win32")
