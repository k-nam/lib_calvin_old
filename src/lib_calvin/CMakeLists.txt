cmake_minimum_required (VERSION 2.6)
project (Container)

include_directories ("${PROJECT_SOURCE_DIR}/container/")
include_directories ("${PROJECT_SOURCE_DIR}/graph/")
include_directories ("${PROJECT_SOURCE_DIR}/lexer/")
include_directories ("${PROJECT_SOURCE_DIR}/sorting/")
include_directories ("${PROJECT_SOURCE_DIR}/string/")
include_directories ("${PROJECT_SOURCE_DIR}/util/")

include_directories ("${PROJECT_SOURCE_DIR}/../lib/boost_1_67_0")

include_directories ("${PROJECT_SOURCE_DIR}/../lib/")

add_executable (Container container/container_main)
add_executable (AdtTest container/adt_test)
add_executable (Adt container/adt)
add_executable (BTreeTest container/b_tree_test)
add_executable (ContainerTestTypes container/container_test_types)
add_executable (ContainerTest container/container_test)
add_executable (MapTest container/map_test)
add_executable (SetTest container/set_test)
add_executable (VectorTest container/vector_test)



target_link_libraries (Container  ${EXTRA_LIBS})