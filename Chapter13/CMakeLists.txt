cmake_minimum_required(VERSION 3.12)

project(Chapter13-Asynchronous_Programming_With_Coroutines)

add_definitions(-DBOOST_ASIO_DISABLE_CONCEPTS)
add_definitions(-DBOOST_DATE_TIME_NO_LIB)
add_definitions(-DBOOST_REGEX_NO_LIB)
include_directories(${Boost_INCLUDE_DIR})

# Build test
file(GLOB CHAPTER_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
list(FILTER CHAPTER_SRC_FILES EXCLUDE REGEX ".*asio_server.cpp")
add_executable(${PROJECT_NAME} ${CHAPTER_SRC_FILES})
target_link_libraries(${PROJECT_NAME} GTest::gtest)

# Boost 1.72 only supports awaitable when using MSVC or Clang
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # Build asio server as a standalone program
  file(GLOB SERVER_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/asio_server.cpp")
  add_executable("asio_server" ${SERVER_SRC_FILES})
endif()
