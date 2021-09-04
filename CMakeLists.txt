cmake_minimum_required(VERSION 3.0.0)
project(MPG VERSION 0.0.1)

include(CTest)
enable_testing()

add_library(MPG src/MPG.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
