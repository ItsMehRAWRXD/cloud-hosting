cmake_minimum_required(VERSION 3.20)
project(RawrXD-QtShell VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 configuration
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network Charts)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/qtapp)
include_directories(${CMAKE_SOURCE_DIR}/src/agent)

# Main Qt application sources
set(QTAPP_SOURCES
    src/qtapp/main_qt.cpp
    src/qtapp/MainWindow.cpp
    src/qtapp/MainWindow.h
    src/qtapp/Subsystems.h
)

# Add widget subdirectories
add_subdirectory(src/qtapp/widgets/whiteboard_widget)
add_subdirectory(src/qtapp/widgets/audio_call_widget)
add_subdirectory(src/qtapp/widgets/screen_share_widget)
add_subdirectory(src/qtapp/widgets/code_stream_widget)
add_subdirectory(src/qtapp/widgets/ai_review_widget)
add_subdirectory(src/qtapp/widgets/inline_chat_widget)
add_subdirectory(src/qtapp/widgets/telemetry_widget)
add_subdirectory(src/qtapp/widgets/progress_manager)
add_subdirectory(src/qtapp/widgets/code_minimap)
add_subdirectory(src/qtapp/widgets/breadcrumb_bar)
add_subdirectory(src/qtapp/widgets/time_tracker_widget)
add_subdirectory(src/qtapp/widgets/task_manager_widget)
add_subdirectory(src/qtapp/widgets/pomodoro_widget)
add_subdirectory(src/qtapp/widgets/search_result_widget)
add_subdirectory(src/qtapp/widgets/bookmark_widget)
add_subdirectory(src/qtapp/widgets/todo_widget)
add_subdirectory(src/qtapp/widgets/macro_recorder_widget)
add_subdirectory(src/qtapp/widgets/ai_completion_cache)
add_subdirectory(src/qtapp/widgets/language_client_host)
add_subdirectory(src/qtapp/widgets/plugin_manager_widget)
add_subdirectory(src/qtapp/widgets/accessibility_widget)
add_subdirectory(src/qtapp/widgets/update_checker_widget)
add_subdirectory(src/qtapp/widgets/welcome_screen_widget)

# Main executable
add_executable(RawrXD-QtShell ${QTAPP_SOURCES})

# Link Qt libraries
target_link_libraries(RawrXD-QtShell
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
    Qt6::Charts
    # Widget libraries
    whiteboard_widget
    audio_call_widget
    screen_share_widget
    code_stream_widget
    ai_review_widget
    inline_chat_widget
    telemetry_widget
    progress_manager
    code_minimap
    breadcrumb_bar
    time_tracker_widget
    task_manager_widget
    pomodoro_widget
    search_result_widget
    bookmark_widget
    todo_widget
    macro_recorder_widget
    ai_completion_cache
    language_client_host
    plugin_manager_widget
    accessibility_widget
    update_checker_widget
    welcome_screen_widget
)

# Platform-specific settings
if(WIN32)
    set_target_properties(RawrXD-QtShell PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
