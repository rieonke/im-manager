cmake_minimum_required(VERSION 3.10)

project (ImManager)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(IMM_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)

include_directories(${CMAKE_JS_INC} ${IMM_SOURCE_PATH}/include)

if (APPLE)

    #======================macos======================
    set(LIB_OSX_PROJECT_NAME im_manager_node_osx)
    add_definitions(-DENABLE_OSX)
    #lib
    add_library(${LIB_OSX_PROJECT_NAME} SHARED im_manager_node.h im_manager_node.c ${IMM_SOURCE_PATH}/include/im_manager.h ${IMM_SOURCE_PATH}/src/common.h ${IMM_SOURCE_PATH}/src/im_manager_osx.c)
    target_link_libraries(${LIB_OSX_PROJECT_NAME} ${CMAKE_JS_LIB} "-framework Carbon")
    set_target_properties(${LIB_OSX_PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
    #install(TARGETS ${LIB_OSX_PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../resources/lib)

endif()
