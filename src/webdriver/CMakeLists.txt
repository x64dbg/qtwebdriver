# WebDriver Core Library

set(WEBDRIVER_CORE_SOURCES
    commands/alert_commands.cc
    commands/appcache_status_command.cc
    commands/command.cc
    commands/cookie_commands.cc
    commands/create_session.cc
    commands/execute_async_script_command.cc
    commands/execute_command.cc
    commands/find_element_commands.cc
    commands/html5_location_commands.cc
    commands/html5_storage_commands.cc
    commands/keys_command.cc
    commands/log_command.cc
    commands/mouse_commands.cc
    commands/navigate_commands.cc
    commands/response.cc
    commands/orientation_command.cc
    commands/screenshot_command.cc
    commands/sessions.cc
    commands/session_with_id.cc
    commands/set_timeout_commands.cc
    commands/source_command.cc
    commands/target_locator_commands.cc
    commands/title_command.cc
    commands/touch_commands.cc
    commands/url_command.cc
    commands/webdriver_command.cc
    commands/element_commands.cc
    commands/visualizer_commands.cc
    commands/window_commands.cc
    commands/non_session_commands.cc
    commands/xdrpc_command.cc
    commands/cisco_player_commands.cc
    commands/browser_connection_command.cc
    webdriver_route_patterns.cc
    frame_path.cc
    http_response.cc
    value_conversion_traits.cc
    webdriver_access.cc
    webdriver_basic_types.cc
    webdriver_capabilities_parser.cc
    webdriver_element_id.cc
    webdriver_view_id.cc
    webdriver_error.cc
    webdriver_logging.cc
    webdriver_server.cc
    webdriver_route_table.cc
    webdriver_view_enumerator.cc
    webdriver_view_factory.cc
    webdriver_view_executor.cc
    webdriver_session.cc
    webdriver_session_manager.cc
    webdriver_switches.cc
    webdriver_util.cc
    webdriver_view_runner.cc
    webdriver_view_transitions.cc
    url_command_wrapper.cc
    versioninfo.cc
    webdriver_version.cc
    ${CMAKE_SOURCE_DIR}/src/third_party/pugixml/pugixml.cpp
)

# Static library
add_library(WebDriver_core STATIC ${WEBDRIVER_CORE_SOURCES})

# Add mongoose if enabled
if(WD_BUILD_MONGOOSE)
    target_sources(WebDriver_core PRIVATE
        ${CMAKE_SOURCE_DIR}/src/third_party/mongoose/mongoose.c
    )
endif()

target_include_directories(WebDriver_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/inc
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/third_party/mongoose
)

target_link_libraries(WebDriver_core PUBLIC chromium_base)

# Shared library (Linux only)
if(LINUX)
    add_library(WebDriver_core_shared SHARED)
    target_link_libraries(WebDriver_core_shared PUBLIC WebDriver_core)
    set_target_properties(WebDriver_core_shared PROPERTIES OUTPUT_NAME WebDriver_core)
endif()

# Installation
install(TARGETS WebDriver_core ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
if(LINUX AND TARGET WebDriver_core_shared)
    install(TARGETS WebDriver_core_shared LIBRARY DESTINATION lib)
endif()
