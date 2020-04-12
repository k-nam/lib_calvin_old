cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project (Container)

include_directories ("${PROJECT_SOURCE_DIR}/bench/")
include_directories ("${PROJECT_SOURCE_DIR}/container/")
include_directories ("${PROJECT_SOURCE_DIR}/graph/")
include_directories ("${PROJECT_SOURCE_DIR}/matrix/")
include_directories ("${PROJECT_SOURCE_DIR}/lexer/")
include_directories ("${PROJECT_SOURCE_DIR}/sorting/")
include_directories ("${PROJECT_SOURCE_DIR}/string/")
include_directories ("${PROJECT_SOURCE_DIR}/util/")

include_directories ("${PROJECT_SOURCE_DIR}/../lib/")
include_directories ("${PROJECT_SOURCE_DIR}/../lib/boost_1_67_0")


add_library (AdtTest container/adt_test)
add_library (Adt container/adt)
add_library (BTreeTest container/b_tree_test)
add_library (ContainerTestTypes container/container_test_types)
add_library (ContainerTest container/container_test)
add_library (MapTest container/map_test)
add_library (SetTest container/set_test)
add_library (VectorTest container/vector_test)
add_library (Random util/random)

add_library (Bench benchmark/bench)
add_library (ContainerBench benchmark/container_bench)
add_library (StringBench benchmark/string_bench)
add_library (SortingBench benchmark/sorting_bench)
add_library (NeuralNetworkBench benchmark/neural_network_bench)
add_library (MatrixBench benchmark/matrix_bench)
add_library (GraphBench benchmark/graph_bench)
add_library (EtcBench benchmark/etc_bench)


add_executable (Container.exe container/container_main)
target_link_libraries (Container.exe ContainerTest SetTest MapTest VectorTest AdtTest BTreeTest Adt ContainerTestTypes Random)

add_definitions(-Wfatal-errors)



project (Benchmark)

add_executable (BenchMain.exe benchmark/bench_main)

target_link_libraries (BenchMain.exe  ContainerBench StringBench StringBench Adt SortingBench NeuralNetworkBench MatrixBench GraphBench EtcBench ContainerTestTypes Random Bench)
