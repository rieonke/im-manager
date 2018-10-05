cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME im_manager)
set(CMAKE_C_STANDARD 11)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(${PROJECT_NAME})

find_package(PkgConfig)
include_directories(include)

enable_testing()

if (APPLE)
    #======================macos======================
    set(LIB_OSX_PROJECT_NAME im_manager_osx)
    set(STATIC_LIB_OSX_PROJECT_NAME im_manager_osx_static)
    set(TEST_OSX_PROJECT_NAME im_manager_osx_test)
    add_definitions(-DENABLE_OSX)
    # deps
    #lib
    add_library(${LIB_OSX_PROJECT_NAME} SHARED include/im_manager.h src/im_manager_osx.c src/common.h)
    add_library(${STATIC_LIB_OSX_PROJECT_NAME} STATIC include/im_manager.h src/im_manager_osx.c src/common.h)
    target_link_libraries(${LIB_OSX_PROJECT_NAME} "-framework Carbon")
    install(TARGETS ${LIB_OSX_PROJECT_NAME} LIBRARY DESTINATION lib)
    install(FILES include/im_manager.h DESTINATION include)
    # test
    add_executable(${TEST_OSX_PROJECT_NAME} src/test/test.c src/common.h)
    target_link_libraries(${TEST_OSX_PROJECT_NAME} ${LIB_OSX_PROJECT_NAME} "-framework Carbon")
    add_test(TEST_OSX ${TEST_OSX_PROJECT_NAME})
elseif (UNIX)

    pkg_check_modules(IBUS ibus-1.0)
    pkg_check_modules(FCITX fcitx)

    if (FCITX_FOUND)

        #======================fcitx======================
        set(LIB_FCITX_PROJECT_NAME im_manager_fcitx)
        set(STATIC_LIB_FCITX_PROJECT_NAME im_manager_fcitx_static)
        set(TEST_FCITX_PROJECT_NAME im_manager_fcitx_test)
        add_definitions(-DENABLE_FCITX)
        # deps
        pkg_check_modules(GLIB2 REQUIRED glib-2.0)
        pkg_check_modules(FCITX REQUIRED fcitx)
        pkg_check_modules(FCITX_GCLIENT REQUIRED fcitx-gclient)
        include_directories(${GLIB2_INCLUDE_DIRS} ${FCITX_INCLUDE_DIRS} ${FCITX_GCLIENT_INCLUDE_DIRS})
        # lib
        add_library(${LIB_FCITX_PROJECT_NAME} SHARED include/im_manager.h src/im_manager_fcitx.c src/common.h)
        add_library(${STATIC_LIB_FCITX_PROJECT_NAME} STATIC include/im_manager.h src/im_manager_fcitx.c src/common.h)
        target_link_libraries(${LIB_FCITX_PROJECT_NAME} ${GLIB2_LIBRARIES} ${FCITX_LIBRARIES} ${FCITX_GCLIENT_LIBRARIES})
        # test
        add_executable(${TEST_FCITX_PROJECT_NAME} src/test/test.c src/common.h)
        target_link_libraries(${TEST_FCITX_PROJECT_NAME} ${LIB_FCITX_PROJECT_NAME} ${GLIB2_LIBRARIES} ${FCITX_LIBRARIES})
        add_test(TEST_FCITX ${TEST_FCITX_PROJECT_NAME})

    endif ()

    if (IBUS_FOUND)

        #======================ibus======================
        set(LIB_IBUS_PROJECT_NAME im_manager_ibus)
        set(STATIC_LIB_IBUS_PROJECT_NAME im_manager_ibus_static)
        set(TEST_IBUS_PROJECT_NAME im_manager_ibus_test)
        add_definitions(-DENABLE_IBUS)
        # deps
        pkg_check_modules(GLIB2 REQUIRED glib-2.0)
        pkg_check_modules(IBUS REQUIRED ibus-1.0)
        include_directories(${GLIB2_INCLUDE_DIRS} ${IBUS_INCLUDE_DIRS})
        # lib
        add_library(${LIB_IBUS_PROJECT_NAME} SHARED include/im_manager.h src/im_manager_ibus.c src/common.h)
        add_library(${STATIC_LIB_IBUS_PROJECT_NAME} STATIC include/im_manager.h src/im_manager_ibus.c src/common.h)
        target_link_libraries(${LIB_IBUS_PROJECT_NAME} ${GLIB2_LIBRARIES} ${IBUS_LIBRARIES})
        # test
        add_executable(${TEST_IBUS_PROJECT_NAME} src/test/test.c src/common.h)
        target_link_libraries(${TEST_IBUS_PROJECT_NAME} ${LIB_IBUS_PROJECT_NAME} ${GLIB2_LIBRARIES} ${IBUS_LIBRARIES})
        add_test(TEST_IBUS ${TEST_IBUS_PROJECT_NAME})

    endif ()

elseif (WIN32)

    set(LIB_WIN_PROJECT_NAME im_manager_win)
    set(TEST_WIN_PROJECT_NAME im_manager_win_test)
    add_definitions(-DENABLE_WIN)

endif ()

