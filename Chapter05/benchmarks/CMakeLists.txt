cmake_minimum_required(VERSION 3.12)

project(Chapter05-Algorithms_Benchmarks)

file(GLOB BM_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(CPP_FILE_PATH ${BM_SRC_FILES})
  get_filename_component(CPP_FILE ${CPP_FILE_PATH} NAME)
  string(REPLACE ".cpp" "" EXE_NAME ${CPP_FILE})
  add_executable(${EXE_NAME} ${CPP_FILE})
  target_link_libraries(${EXE_NAME} PRIVATE benchmark::benchmark)
endforeach(CPP_FILE_PATH ${BM_SRC_FILES})
