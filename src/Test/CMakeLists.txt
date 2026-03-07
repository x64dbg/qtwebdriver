# WebDriver Test Executables

# Helper function for Qt moc
function(qt_wrap_moc output_var)
    set(moc_files "")
    foreach(header ${ARGN})
        get_filename_component(basename ${header} NAME_WE)
        set(moc_output ${CMAKE_CURRENT_BINARY_DIR}/moc_${basename}.cc)
        add_custom_command(
            OUTPUT ${moc_output}
            COMMAND ${QT_MOC_EXECUTABLE} ${header} -o ${moc_output}
            DEPENDS ${header}
            COMMENT "Running moc on ${header}"
        )
        list(APPEND moc_files ${moc_output})
    endforeach()
    set(${output_var} ${moc_files} PARENT_SCOPE)
endfunction()

# ============================================================================
# test_widgets library
# ============================================================================

set(TEST_WIDGETS_SOURCES
    Headers.h
    Samples.h
    ClickTest.cc
    ClickTest.h
    ClickScrollingTest.cc
    ClickScrollingTest.h
    ClickScrollingTest.ui
    WindowTest.cc
    WindowTest.h
    FindingTest.cc
    FindingTest.h
    CoordinatesTest.cc
    CoordinatesTest.h
    ElementAttributeTest.cc
    ElementAttributeTest.h
    ElementSelectingTest.cc
    ElementSelectingTest.h
    ElementSelectingTest.ui
    TypingTest.cc
    TypingTest.h
    BasicKeyboardInterfaceTest.cc
    BasicKeyboardInterfaceTest.h
    TextHandlingTest.cc
    TextHandlingTest.h
    FormHandlingTest.cc
    FormHandlingTest.h
    img.xpm
    XPathElementFindingTest.cc
    XPathElementFindingTest.h
    StaleElementReferenceTest.cc
    StaleElementReferenceTest.h
    VisibilityTest.cc
    VisibilityTest.h
    RestyledLabel.cc
    RestyledLabel.h
    DragableWidget.cc
    DragableWidget.h
    BasicMouseInterfaceTest.cc
    BasicMouseInterfaceTest.h
    TouchTest.cc
    TouchTest.h
    MenuTest.cc
    MenuTest.h
)

set(TEST_WIDGETS_MOC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/ClickTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ClickScrollingTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/WindowTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FindingTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CoordinatesTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ElementAttributeTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ElementSelectingTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TypingTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BasicKeyboardInterfaceTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TextHandlingTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FormHandlingTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/XPathElementFindingTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/StaleElementReferenceTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/VisibilityTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RestyledLabel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DragableWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BasicMouseInterfaceTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MenuTest.h
)

# Video test (Qt5 + player support)
if(QT5 AND WD_CONFIG_PLAYER)
    list(APPEND TEST_WIDGETS_SOURCES VideoTest.h VideoTest.cc)
    list(APPEND TEST_WIDGETS_MOC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/VideoTest.h)
endif()

qt_wrap_moc(TEST_WIDGETS_MOC_SOURCES ${TEST_WIDGETS_MOC_HEADERS})

add_library(test_widgets STATIC
    ${TEST_WIDGETS_SOURCES}
    ${TEST_WIDGETS_MOC_SOURCES}
)

# Disable AUTOMOC since we manually run moc via qt_wrap_moc
set_target_properties(test_widgets PROPERTIES AUTOMOC OFF)

target_include_directories(test_widgets
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/inc
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(test_widgets PUBLIC ${QT_LIBRARIES})

# Additional Qt modules for tests
if(QT6)
    find_package(Qt6 COMPONENTS Multimedia MultimediaWidgets PrintSupport Sql OpenGL)
    if(Qt6Multimedia_FOUND)
        target_link_libraries(test_widgets PUBLIC
            Qt6::Multimedia Qt6::MultimediaWidgets)
    endif()
    if(Qt6PrintSupport_FOUND)
        target_link_libraries(test_widgets PUBLIC Qt6::PrintSupport)
    endif()
    if(Qt6Sql_FOUND)
        target_link_libraries(test_widgets PUBLIC Qt6::Sql)
    endif()
    if(Qt6OpenGL_FOUND)
        target_link_libraries(test_widgets PUBLIC Qt6::OpenGL)
    endif()
elseif(QT5)
    find_package(Qt5 COMPONENTS Multimedia MultimediaWidgets PrintSupport Sql OpenGL Sensors)
    if(Qt5Multimedia_FOUND)
        target_link_libraries(test_widgets PUBLIC
            Qt5::Multimedia Qt5::MultimediaWidgets)
    endif()
    if(Qt5PrintSupport_FOUND)
        target_link_libraries(test_widgets PUBLIC Qt5::PrintSupport)
    endif()
    if(Qt5Sql_FOUND)
        target_link_libraries(test_widgets PUBLIC Qt5::Sql)
    endif()
    if(Qt5OpenGL_FOUND)
        target_link_libraries(test_widgets PUBLIC Qt5::OpenGL)
    endif()
    if(Qt5Sensors_FOUND)
        target_link_libraries(test_widgets PUBLIC Qt5::Sensors)
    endif()
endif()

# ============================================================================
# test_WD_hybrid (with WebKit)
# ============================================================================

if(WD_CONFIG_WEBKIT)
    set(TEST_WD_HYBRID_SOURCES
        main.cc
        WindowWithEmbeddedViewTest.cc
        WindowWithEmbeddedViewTest.h
        WidgetAndWebViewTest.cc
        WidgetAndWebViewTest.h
        GraphicsWebViewTest.cc
        GraphicsWebViewTest.h
    )

    set(TEST_WD_HYBRID_MOC_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/WindowWithEmbeddedViewTest.h
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetAndWebViewTest.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GraphicsWebViewTest.h
    )

    qt_wrap_moc(TEST_WD_HYBRID_MOC_SOURCES ${TEST_WD_HYBRID_MOC_HEADERS})

    add_executable(test_WD_hybrid
        ${TEST_WD_HYBRID_SOURCES}
        ${TEST_WD_HYBRID_MOC_SOURCES}
    )

    set_target_properties(test_WD_hybrid PROPERTIES OUTPUT_NAME WebDriver)
    target_compile_definitions(test_WD_hybrid PRIVATE WD_ENABLE_WEB_VIEW=1)

    target_link_libraries(test_WD_hybrid PRIVATE
        chromium_base WebDriver_core
        WebDriver_extension_qt_base WebDriver_extension_qt_web
        test_widgets
    )

    if(WD_CONFIG_QUICK)
        target_link_libraries(test_WD_hybrid PRIVATE
            WebDriver_extension_qt_quick WebDriver_extension_qt_quick_web)
    else()
        target_compile_definitions(test_WD_hybrid PRIVATE QT_NO_QML)
    endif()

    if(QT5)
        target_link_libraries(test_WD_hybrid PRIVATE Qt5::WebKit Qt5::WebKitWidgets)
    elseif(QT6)
        target_link_libraries(test_WD_hybrid PRIVATE Qt6::WebEngineWidgets)
    endif()

    if(NOT WD_BUILD_MONGOOSE)
        target_sources(test_WD_hybrid PRIVATE
            ${CMAKE_SOURCE_DIR}/src/third_party/mongoose/mongoose.c)
    endif()

    install(TARGETS test_WD_hybrid RUNTIME DESTINATION bin)
endif()

# ============================================================================
# test_WD_hybrid_noWebkit
# ============================================================================

add_executable(test_WD_hybrid_noWebkit main.cc)
set_target_properties(test_WD_hybrid_noWebkit PROPERTIES OUTPUT_NAME WebDriver_noWebkit)

target_link_libraries(test_WD_hybrid_noWebkit PRIVATE
    chromium_base WebDriver_core WebDriver_extension_qt_base test_widgets
)

if(WD_CONFIG_QUICK)
    target_link_libraries(test_WD_hybrid_noWebkit PRIVATE WebDriver_extension_qt_quick)
else()
    target_compile_definitions(test_WD_hybrid_noWebkit PRIVATE QT_NO_QML)
endif()

if(NOT WD_BUILD_MONGOOSE)
    target_sources(test_WD_hybrid_noWebkit PRIVATE
        ${CMAKE_SOURCE_DIR}/src/third_party/mongoose/mongoose.c)
endif()

install(TARGETS test_WD_hybrid_noWebkit RUNTIME DESTINATION bin)

# ============================================================================
# test_WD_hybrid_noWebkit_with_shared_libs (Linux only)
# ============================================================================

if(LINUX)
    add_executable(test_WD_hybrid_noWebkit_with_shared_libs main.cc)
    set_target_properties(test_WD_hybrid_noWebkit_with_shared_libs PROPERTIES
        OUTPUT_NAME WebDriver_noWebkit_sharedLibs)

    target_link_libraries(test_WD_hybrid_noWebkit_with_shared_libs PRIVATE
        chromium_base_shared WebDriver_core_shared
        WebDriver_extension_qt_base_shared test_widgets
    )

    if(WD_CONFIG_QUICK)
        target_link_libraries(test_WD_hybrid_noWebkit_with_shared_libs PRIVATE
            WebDriver_extension_qt_quick)
    else()
        target_compile_definitions(test_WD_hybrid_noWebkit_with_shared_libs PRIVATE QT_NO_QML)
    endif()

    if(NOT WD_BUILD_MONGOOSE)
        target_sources(test_WD_hybrid_noWebkit_with_shared_libs PRIVATE
            ${CMAKE_SOURCE_DIR}/src/third_party/mongoose/mongoose.c)
    endif()

    install(TARGETS test_WD_hybrid_noWebkit_with_shared_libs RUNTIME DESTINATION bin)
endif()
