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

include_directories ("${PROJECT_SOURCE_DIR}/util/")

add_library (AdtTest container/adt_test)
add_library (Adt container/adt)
add_library (BTreeTest container/b_tree_test)
add_library (ContainerTestTypes container/container_test_types)
add_library (ContainerTest container/container_test)
add_library (MapTest container/map_test)
add_library (SetTest container/set_test)
add_library (VectorTest container/vector_test)

add_library (Random util/random)


add_executable (Container container/container_main)


target_link_libraries (Container ContainerTest SetTest MapTest VectorTest AdtTest BTreeTest Adt ContainerTestTypes Random)

add_definitions(
    -Wfatal-errors
    )