cmake_minimum_required(VERSION 3.16)

project(QtWebDriver VERSION 1.0.0 LANGUAGES CXX C)

# Options for build configuration
option(WD_CONFIG_QWIDGET_BASE "Enable QWidget support" ON)
option(WD_CONFIG_WEBKIT "Enable WebKit support" OFF)
option(WD_CONFIG_QUICK "Enable Qt Quick support" ON)
option(WD_CONFIG_PLAYER "Enable player support" OFF)
option(WD_CONFIG_ONE_KEYRELEASE "Enable one key release" OFF)
option(WD_BUILD_MONGOOSE "Build with mongoose" ON)

# Default BUILD_TESTS ON for standalone, OFF as subdirectory
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    option(BUILD_TESTS "Build test executables" ON)
else()
    option(BUILD_TESTS "Build test executables" OFF)
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform detection
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Qt version selection (default to Qt5)
set(QT_VERSION "5" CACHE STRING "Qt version to use (5 or 6)")
set_property(CACHE QT_VERSION PROPERTY STRINGS "5" "6")

if(QT_VERSION EQUAL 6)
    set(QT6 TRUE)
    set(QT5 FALSE)
elseif(QT_VERSION EQUAL 5)
    set(QT5 TRUE)
    set(QT6 FALSE)
else()
    message(FATAL_ERROR "Unsupported Qt version: ${QT_VERSION}. Use 5 or 6.")
endif()

# Top-level-only settings
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # Build type
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
    endif()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")

    # Output directories
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

    # Help find Qt - check common installation locations if not specified
    if(NOT DEFINED CMAKE_PREFIX_PATH OR CMAKE_PREFIX_PATH STREQUAL "")
        set(QT_SEARCH_PATHS
            # Windows
            "C:/Qt/Qt${QT_VERSION}.*/*/msvc*"
            "C:/Qt/${QT_VERSION}.*/msvc*"
            # Linux
            "$ENV{HOME}/Qt/${QT_VERSION}.*/gcc_64"
            "/opt/qt/${QT_VERSION}.*/gcc_64"
            "/opt/Qt/${QT_VERSION}.*/gcc_64"
            "/usr/local/Qt-${QT_VERSION}.*"
            # macOS
            "/usr/local/opt/qt@${QT_VERSION}"
            "/usr/local/opt/qt${QT_VERSION}"
        )

        foreach(search_path ${QT_SEARCH_PATHS})
            file(GLOB qt_paths ${search_path})
            if(qt_paths)
                list(GET qt_paths 0 qt_path)
                if(EXISTS "${qt_path}")
                    message(STATUS "Found Qt at: ${qt_path}")
                    set(CMAKE_PREFIX_PATH ${qt_path})
                    break()
                endif()
            endif()
        endforeach()
    endif()
endif()

# Find Qt
if(QT6)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Xml)

    if(WD_CONFIG_QUICK)
        find_package(Qt6 REQUIRED COMPONENTS Quick Qml)
    endif()
    if(WD_CONFIG_WEBKIT)
        find_package(Qt6 COMPONENTS WebEngineWidgets)
    endif()

    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    get_filename_component(QT_INSTALL_PREFIX "${QT_BIN_DIR}/.." ABSOLUTE)
    set(QT_MOC_EXECUTABLE "${QT_BIN_DIR}/moc")
    set(QT_UIC_EXECUTABLE "${QT_BIN_DIR}/uic")
    set(QT_RCC_EXECUTABLE "${QT_BIN_DIR}/rcc")

    # Qt6 may put tools in libexec
    if(EXISTS "${QT_INSTALL_PREFIX}/libexec/moc")
        set(QT_MOC_EXECUTABLE "${QT_INSTALL_PREFIX}/libexec/moc")
        set(QT_UIC_EXECUTABLE "${QT_INSTALL_PREFIX}/libexec/uic")
        set(QT_RCC_EXECUTABLE "${QT_INSTALL_PREFIX}/libexec/rcc")
    endif()

    set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::Xml)
    if(WD_CONFIG_QUICK)
        list(APPEND QT_LIBRARIES Qt6::Quick Qt6::Qml)
    endif()
elseif(QT5)
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network Xml)

    if(WD_CONFIG_QUICK)
        find_package(Qt5 REQUIRED COMPONENTS Quick Qml)
    endif()
    if(WD_CONFIG_WEBKIT)
        find_package(Qt5 COMPONENTS WebKit WebKitWidgets)
    endif()

    get_target_property(QT_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    set(QT_MOC_EXECUTABLE "${QT_BIN_DIR}/moc")
    set(QT_UIC_EXECUTABLE "${QT_BIN_DIR}/uic")
    set(QT_RCC_EXECUTABLE "${QT_BIN_DIR}/rcc")

    set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Xml)
    if(WD_CONFIG_QUICK)
        list(APPEND QT_LIBRARIES Qt5::Quick Qt5::Qml)
    endif()
endif()

# MSVC-specific: handle .exe extension for moc
if(WIN32)
    if(NOT QT_MOC_EXECUTABLE MATCHES "\\.exe$")
        set(QT_MOC_EXECUTABLE "${QT_MOC_EXECUTABLE}.exe")
        set(QT_UIC_EXECUTABLE "${QT_UIC_EXECUTABLE}.exe")
        set(QT_RCC_EXECUTABLE "${QT_RCC_EXECUTABLE}.exe")
    endif()
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Common definitions (directory-scoped, won't leak to parent)
add_compile_definitions(
    QT_NO_DEBUG
    QT_GUI_LIB
    QT_CORE_LIB
    QT_SHARED
    QT_NO_OPENGL
)

if(QT5)
    add_compile_definitions(QT5=1)
endif()

# Platform-specific settings
if(LINUX)
    add_compile_definitions(
        __STDC_FORMAT_MACROS
        OS_POSIX
    )
    add_compile_options(-fPIC -Wall -W -Wno-unused-parameter)
endif()

if(WIN32)
    add_compile_definitions(
        _WIN32
        WIN32
        OS_WIN
        NOMINMAX
        UNICODE
        _UNICODE
        _WINSOCKAPI_
        _CRT_RAND_S
    )
endif()

if(APPLE)
    add_compile_definitions(OS_POSIX)
endif()

# Conditional defines
if(WD_CONFIG_PLAYER)
    add_compile_definitions(WD_ENABLE_PLAYER=1)
endif()
if(WD_CONFIG_ONE_KEYRELEASE)
    add_compile_definitions(WD_ENABLE_ONE_KEYRELEASE=1)
endif()

# Subdirectories
add_subdirectory(src/base)
add_subdirectory(src/webdriver)
add_subdirectory(src/webdriver/extension_qt)

if(BUILD_TESTS)
    add_subdirectory(src/Test)
endif()

# Installation (only when built standalone)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    install(DIRECTORY inc/ DESTINATION include/qtwebdriver)
endif()
