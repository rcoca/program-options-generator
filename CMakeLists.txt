project(NodeConfGenerated)
cmake_minimum_required(VERSION 2.8)
find_package( Boost REQUIRED COMPONENTS program_options regex system thread)
find_package(Threads)

aux_source_directory(. SRC_LIST)

aux_source_directory(ConfigCode SRC_LIST)
aux_source_directory(ConfigCode/common/config SRC_LIST)
aux_source_directory(ConfigCode/NodeOptions SRC_LIST)
aux_source_directory(ConfigCode/RequiredOptions SRC_LIST)
aux_source_directory(ConfigCode/SimpleOptions SRC_LIST)

INCLUDE_DIRECTORIES(ConfigCode/common/config)
INCLUDE_DIRECTORIES(ConfigCode/NodeOptions)
INCLUDE_DIRECTORIES(ConfigCode/RequiredOptions)
INCLUDE_DIRECTORIES(ConfigCode/SimpleOptions)

MESSAGE(INFO  " sources compiled: ${SRC_LIST}")

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME}  ${Boost_LIBRARIES} ${PTHREAD_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
