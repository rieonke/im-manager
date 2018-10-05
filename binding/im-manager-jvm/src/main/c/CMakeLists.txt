cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME im_manager_jvm)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(IMM_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../)

project(${PROJECT_NAME})
find_package(PkgConfig)

find_package(JNI REQUIRED)
include_directories(${JAVA_INCLUDE_PATH})
include_directories(${JAVA_INCLUDE_PATH2})

include_directories(${IMM_SOURCE_PATH}/include)

project(${PROJECT_NAME})

find_package(PkgConfig)
include_directories(include)

enable_testing()

if (APPLE)

    #======================macos======================
    set(LIB_OSX_PROJECT_NAME im_manager_jvm_osx)
    add_definitions(-DENABLE_OSX)
    # deps
    #lib
    add_library(${LIB_OSX_PROJECT_NAME} SHARED im_manager_jvm.h im_manager_jvm.c ${IMM_SOURCE_PATH}/src/common.h ${IMM_SOURCE_PATH}/include/im_manager.h ${IMM_SOURCE_PATH}/src/im_manager_osx.c)
    target_link_libraries(${LIB_OSX_PROJECT_NAME} "-framework Carbon")
    install(TARGETS ${LIB_OSX_PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../resources/lib)

elseif (UNIX)

    pkg_check_modules(IBUS ibus-1.0)
    pkg_check_modules(FCITX fcitx)

    if (FCITX_FOUND)

        #======================fcitx======================
        set(LIB_FCITX_PROJECT_NAME im_manager_jvm_fcitx)
        add_definitions(-DENABLE_FCITX)
        # deps
        pkg_check_modules(GLIB2 REQUIRED glib-2.0)
        pkg_check_modules(FCITX REQUIRED fcitx)
        pkg_check_modules(FCITX_GCLIENT REQUIRED fcitx-gclient)
        include_directories(${GLIB2_INCLUDE_DIRS} ${FCITX_INCLUDE_DIRS} ${FCITX_GCLIENT_INCLUDE_DIRS})
        # lib
        add_library(${LIB_FCITX_PROJECT_NAME} SHARED im_manager_jvm.h im_manager_jvm.c ${IMM_SOURCE_PATH}/src/common.h ${IMM_SOURCE_PATH}/include/im_manager.h ${IMM_SOURCE_PATH}/src/im_manager_fcitx.c)
        target_link_libraries(${LIB_FCITX_PROJECT_NAME} ${GLIB2_LIBRARIES} ${FCITX_LIBRARIES} ${FCITX_GCLIENT_LIBRARIES})
        install(TARGETS ${LIB_FCITX_PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../resources/lib)

    endif ()

    if (IBUS_FOUND)

        #======================ibus======================
        set(LIB_IBUS_PROJECT_NAME im_manager_jvm_ibus)
        add_definitions(-DENABLE_IBUS)
        # deps
        pkg_check_modules(GLIB2 REQUIRED glib-2.0)
        pkg_check_modules(IBUS REQUIRED ibus-1.0)
        include_directories(${GLIB2_INCLUDE_DIRS} ${IBUS_INCLUDE_DIRS})
        # lib
        add_library(${LIB_IBUS_PROJECT_NAME} SHARED im_manager_jvm.h im_manager_jvm.c ${IMM_SOURCE_PATH}/src/common.h ${IMM_SOURCE_PATH}/include/im_manager.h ${IMM_SOURCE_PATH}/src/im_manager_ibus.c)
        target_link_libraries(${LIB_IBUS_PROJECT_NAME} ${GLIB2_LIBRARIES} ${IBUS_LIBRARIES})
        install(TARGETS ${LIB_IBUS_PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../resources/lib)

    endif ()

elseif (WIN32)

    set(LIB_WIN_PROJECT_NAME im_manager_jvm_win)
    add_definitions(-DENABLE_WIN)

endif ()
