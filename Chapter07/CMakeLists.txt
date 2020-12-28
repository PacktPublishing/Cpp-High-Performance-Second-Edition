cmake_minimum_required(VERSION 3.12)

project(Chapter07-Memory_Management)

file(GLOB_RECURSE CHAPTER_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(${PROJECT_NAME} ${CHAPTER_SRC_FILES})
target_link_libraries(${PROJECT_NAME} GTest::gtest)
