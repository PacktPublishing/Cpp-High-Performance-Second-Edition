cmake_minimum_required(VERSION 3.12)

project(Chapter10-Proxy_Objects_And_Lazy_Evaluation)

file(GLOB CHAPTER_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${CHAPTER_SRC_FILES})
target_link_libraries(${PROJECT_NAME} GTest::gtest)
