# Chromium Base Library

set(CHROMIUM_BASE_SOURCES
    at_exit.cc
    base64.cc
    base_paths.cc
    base_switches.cc
    callback_internal.cc
    command_line.cc
    debug/alias.cc
    debug/debugger.cc
    environment.cc
    file_path.cc
    file_util.cc
    json/json_parser.cc
    json/json_reader.cc
    json/json_writer.cc
    json/string_escape.cc
    lazy_instance.cc
    location.cc
    logging.cc
    memory/ref_counted.cc
    memory/singleton.cc
    memory/weak_ptr.cc
    message_loop.cc
    message_loop_proxy.cc
    message_loop_proxy_impl.cc
    message_pump.cc
    message_pump_default.cc
    metrics/bucket_ranges.cc
    metrics/histogram_base.cc
    metrics/histogram.cc
    metrics/histogram_samples.cc
    metrics/sample_vector.cc
    metrics/statistics_recorder.cc
    path_service.cc
    pending_task.cc
    pickle.cc
    platform_file.cc
    profiler/alternate_timer.cc
    profiler/scoped_profile.cc
    profiler/tracked_time.cc
    rand_util.cc
    run_loop.cc
    scoped_temp_dir.cc
    string_number_conversions.cc
    string_piece.cc
    stringprintf.cc
    string_split.cc
    string_util.cc
    synchronization/lock.cc
    task_runner.cc
    third_party/dmg_fp/dtoa.cc
    third_party/dmg_fp/g_fmt.cc
    third_party/dynamic_annotations/dynamic_annotations.c
    third_party/icu/icu_utf.cc
    third_party/nspr/prtime.cc
    threading/post_task_and_reply_impl.cc
    threading/thread.cc
    threading/thread_checker_impl.cc
    threading/thread_collision_warner.cc
    threading/thread_restrictions.cc
    threading/worker_pool.cc
    thread_task_runner_handle.cc
    time.cc
    timer.cc
    tracked_objects.cc
    tracking_info.cc
    utf_string_conversions.cc
    utf_string_conversion_utils.cc
    values.cc
)

# Network sources
set(CHROMIUM_BASE_NET_SOURCES
    ../net/base/file_stream.cc
    ../net/base/file_stream_metrics.cc
    ../net/base/file_stream_net_log_parameters.cc
    ../net/base/io_buffer.cc
    ../net/base/net_errors.cc
    ../net/base/net_log.cc
)

# Third-party sources
set(CHROMIUM_BASE_THIRD_PARTY_SOURCES
    ../third_party/modp_b64/modp_b64.cc
)

# Platform-specific sources
if(LINUX)
    set(CHROMIUM_BASE_PLATFORM_SOURCES
        string16.cc
        time_posix.cc
        base_paths_posix.cc
        debug/debugger_posix.cc
        file_util_posix.cc
        nix/xdg_util.cc
        platform_file_posix.cc
        rand_util_posix.cc
        safe_strerror_posix.cc
        synchronization/condition_variable_posix.cc
        synchronization/lock_impl_posix.cc
        synchronization/waitable_event_posix.cc
        sys_info_posix.cc
        sys_string_conversions_posix.cc
        threading/platform_thread_posix.cc
        threading/thread_local_posix.cc
        threading/thread_local_storage_posix.cc
        threading/worker_pool_posix.cc
        sys_info_linux.cc
        third_party/xdg_user_dirs/xdg_user_dir_lookup.cc
    )
    set(CHROMIUM_BASE_NET_PLATFORM_SOURCES
        ../net/base/file_stream_metrics_posix.cc
        ../net/base/file_stream_posix.cc
        ../net/base/net_errors_posix.cc
    )
elseif(WIN32)
    set(CHROMIUM_BASE_PLATFORM_SOURCES
        cpu.cc
        time_win.cc
        base_paths_win.cc
        debug/debugger_win.cc
        file_util_win.cc
        platform_file_win.cc
        rand_util_win.cc
        synchronization/condition_variable_win.cc
        synchronization/lock_impl_win.cc
        synchronization/waitable_event_win.cc
        sys_info_win.cc
        threading/platform_thread_win.cc
        threading/thread_local_win.cc
        threading/thread_local_storage_win.cc
        threading/worker_pool_win.cc
        win/windows_version.cc
        win/registry.cc
        win/scoped_handle.cc
        win/wrapped_window_proc.cc
        message_pump_win.cc
        process_util.cc
        process_util_win.cc
    )
    set(CHROMIUM_BASE_NET_PLATFORM_SOURCES
        ../net/base/file_stream_win.cc
        ../net/base/file_stream_metrics_win.cc
        ../net/base/net_errors_win.cc
    )
elseif(APPLE)
    set(CHROMIUM_BASE_PLATFORM_SOURCES
        string16.cc
        time_posix.cc
        base_paths_posix.cc
        debug/debugger_posix.cc
        platform_file_posix.cc
        rand_util_posix.cc
        safe_strerror_posix.cc
        synchronization/condition_variable_posix.cc
        synchronization/lock_impl_posix.cc
        synchronization/waitable_event_posix.cc
        sys_info_posix.cc
        sys_string_conversions_mac.mm
        threading/platform_thread_posix.cc
        threading/thread_local_posix.cc
        threading/thread_local_storage_posix.cc
        threading/worker_pool_posix.cc
        base_paths_mac.mm
        file_util_posix.cc
        file_util_mac.mm
        message_pump_mac.mm
        scoped_nsautorelease_pool.mm
        mac/foundation_util.mm
        mac/bundle_locations.mm
        mac/mac_logging.cc
        mac/mach_logging.cc
        mac/scoped_mach_port.cc
        threading/platform_thread_mac.mm
        time_mac.cc
        sys_info_mac.cc
    )
    set(CHROMIUM_BASE_NET_PLATFORM_SOURCES
        ../net/base/file_stream_metrics_posix.cc
        ../net/base/file_stream_posix.cc
        ../net/base/net_errors_posix.cc
    )
endif()

add_library(chromium_base STATIC
    ${CHROMIUM_BASE_SOURCES}
    ${CHROMIUM_BASE_NET_SOURCES}
    ${CHROMIUM_BASE_THIRD_PARTY_SOURCES}
    ${CHROMIUM_BASE_PLATFORM_SOURCES}
    ${CHROMIUM_BASE_NET_PLATFORM_SOURCES}
)

target_include_directories(chromium_base
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc
        ${PROJECT_SOURCE_DIR}/src
)

# Suppress Clang warnings for unavoidable Win32 WndProc function-pointer patterns
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(chromium_base PRIVATE
        -Wno-microsoft-cast
        -Wno-int-to-pointer-cast
    )
endif()

if(WIN32)
    target_link_libraries(chromium_base PUBLIC
        user32 ws2_32 shell32 advapi32 psapi kernel32 userenv
    )
elseif(APPLE)
    find_library(FOUNDATION Foundation)
    find_library(COREFOUNDATION CoreFoundation)
    find_library(APPSERVICES ApplicationServices)
    find_library(SECURITY Security)
    target_link_libraries(chromium_base PUBLIC
        ${FOUNDATION} ${COREFOUNDATION} ${APPSERVICES} ${SECURITY}
    )
elseif(LINUX)
    target_link_libraries(chromium_base PUBLIC pthread rt dl)
endif()

# Installation
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    install(TARGETS chromium_base ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
endif()
