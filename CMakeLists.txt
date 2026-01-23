cmake_minimum_required(VERSION 3.20)
project(RawrXD-ModelLoader VERSION 1.0.0 LANGUAGES CXX)

# ============================================================
# Build Configuration
# ============================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Build output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================
# Platform Detection
# ============================================================
if(WIN32)
    set(PLATFORM_WINDOWS TRUE)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX TRUE)
    add_definitions(-DPLATFORM_LINUX)
elseif(APPLE)
    set(PLATFORM_MACOS TRUE)
    add_definitions(-DPLATFORM_MACOS)
endif()

# ============================================================
# Options
# ============================================================
option(ENABLE_VULKAN "Enable Vulkan support (uses internal stubs)" OFF)
option(ENABLE_ROCM "Enable ROCm support (uses internal stubs)" OFF)
option(BUILD_GUI "Build Qt GUI IDE" ON)
option(BUILD_CLI "Build CLI interface" ON)
option(BUILD_TESTS "Build test suite" ON)

# ============================================================
# Find Qt6
# ============================================================
if(BUILD_GUI)
    find_package(Qt6 COMPONENTS Core Widgets Gui QUIET)
    
    if(NOT Qt6_FOUND)
        message(STATUS "Qt6 not found, disabling GUI build")
        set(BUILD_GUI OFF)
    else()
        message(STATUS "Qt6 found: ${Qt6_DIR}")
        message(STATUS "Qt6 version: ${Qt6_VERSION}")
    endif()
endif()

# ============================================================
# Core Library (shared between CLI and GUI)
# ============================================================
set(CORE_SOURCES
    src/core/patch_result.hpp
    src/core/platform_utils.hpp
    src/core/platform_utils.cpp
)

add_library(rawrxd_core STATIC ${CORE_SOURCES})
target_include_directories(rawrxd_core PUBLIC src)

# ============================================================
# Internal GPU Stubs (Vulkan/ROCm replacements)
# ============================================================
set(GPU_STUB_SOURCES
    src/core/gpu_stubs.hpp
    src/core/gpu_stubs.cpp
)

add_library(gpu_stubs STATIC ${GPU_STUB_SOURCES})
target_include_directories(gpu_stubs PUBLIC src)
target_link_libraries(gpu_stubs rawrxd_core)

if(ENABLE_VULKAN)
    target_compile_definitions(gpu_stubs PUBLIC ENABLE_VULKAN_STUBS)
endif()

if(ENABLE_ROCM)
    target_compile_definitions(gpu_stubs PUBLIC ENABLE_ROCM_STUBS)
endif()

# ============================================================
# Hotpatch Engine Library
# ============================================================
set(HOTPATCH_SOURCES
    src/qtapp/model_memory_hotpatch.hpp
    src/qtapp/model_memory_hotpatch.cpp
    src/qtapp/byte_level_hotpatcher.hpp
    src/qtapp/byte_level_hotpatcher.cpp
    src/qtapp/gguf_server_hotpatch.hpp
    src/qtapp/gguf_server_hotpatch.cpp
    src/qtapp/unified_hotpatch_manager.hpp
    src/qtapp/unified_hotpatch_manager.cpp
    src/qtapp/proxy_hotpatcher.hpp
    src/qtapp/proxy_hotpatcher.cpp
)

if(BUILD_GUI)
    add_library(hotpatch_engine STATIC ${HOTPATCH_SOURCES})
    target_link_libraries(hotpatch_engine 
        PUBLIC 
            Qt6::Core
            rawrxd_core
            gpu_stubs
    )
    target_include_directories(hotpatch_engine PUBLIC src)
else()
    # CLI-only version without Qt dependencies
    set(HOTPATCH_CLI_SOURCES
        src/qtapp/model_memory_hotpatch.hpp
        src/qtapp/model_memory_hotpatch.cpp
    )
    add_library(hotpatch_engine STATIC ${HOTPATCH_CLI_SOURCES})
    target_link_libraries(hotpatch_engine PUBLIC rawrxd_core gpu_stubs)
    target_include_directories(hotpatch_engine PUBLIC src)
    target_compile_definitions(hotpatch_engine PUBLIC NO_QT_SUPPORT)
endif()

# ============================================================
# Agentic Systems Library
# ============================================================
if(BUILD_GUI)
    set(AGENT_SOURCES
        src/agent/agentic_failure_detector.hpp
        src/agent/agentic_failure_detector.cpp
        src/agent/agentic_puppeteer.hpp
        src/agent/agentic_puppeteer.cpp
        src/agent/auto_bootstrap.cpp
        src/agent/self_patch.cpp
    )

    add_library(agentic_systems STATIC ${AGENT_SOURCES})
    target_link_libraries(agentic_systems 
        PUBLIC 
            Qt6::Core
            hotpatch_engine
    )
    target_include_directories(agentic_systems PUBLIC src)
endif()

# ============================================================
# GUI Application (Qt IDE)
# ============================================================
if(BUILD_GUI)
    set(GUI_SOURCES
        src/qtapp/main_qt.cpp
        src/qtapp/MainWindow.hpp
        src/qtapp/MainWindow.cpp
        src/qtapp/CodeEditor.hpp
        src/qtapp/CodeEditor.cpp
        src/qtapp/MiniMap.hpp
        src/qtapp/MiniMap.cpp
    )

    add_executable(RawrXD-AgenticIDE ${GUI_SOURCES})
    
    target_link_libraries(RawrXD-AgenticIDE 
        PRIVATE
            Qt6::Core
            Qt6::Widgets
            Qt6::Gui
            hotpatch_engine
            agentic_systems
            rawrxd_core
            gpu_stubs
    )
    
    target_include_directories(RawrXD-AgenticIDE PRIVATE src)
    
    # Set executable properties
    if(WIN32)
        set_target_properties(RawrXD-AgenticIDE PROPERTIES
            WIN32_EXECUTABLE TRUE
            OUTPUT_NAME "RawrXD-AgenticIDE"
        )
    endif()
    
    message(STATUS "GUI target: RawrXD-AgenticIDE")
endif()

# ============================================================
# CLI Application
# ============================================================
if(BUILD_CLI)
    set(CLI_SOURCES
        src/cli/main_cli.cpp
        src/cli/cli_commands.hpp
        src/cli/cli_commands.cpp
        src/cli/model_loader.hpp
        src/cli/model_loader.cpp
    )

    add_executable(rawrxd-cli ${CLI_SOURCES})
    
    target_link_libraries(rawrxd-cli 
        PRIVATE
            hotpatch_engine
            rawrxd_core
            gpu_stubs
    )
    
    target_include_directories(rawrxd-cli PRIVATE src)
    
    message(STATUS "CLI target: rawrxd-cli")
endif()

# ============================================================
# Testing
# ============================================================
if(BUILD_TESTS)
    enable_testing()
    
    add_executable(self_test_gate
        tests/self_test_gate.cpp
    )
    
    target_link_libraries(self_test_gate 
        PRIVATE
            rawrxd_core
            hotpatch_engine
            gpu_stubs
    )
    
    add_test(NAME SelfTestGate COMMAND self_test_gate)
endif()

# ============================================================
# Installation
# ============================================================
if(BUILD_GUI)
    install(TARGETS RawrXD-AgenticIDE
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()

if(BUILD_CLI)
    install(TARGETS rawrxd-cli
        RUNTIME DESTINATION bin
    )
endif()

# ============================================================
# Build Summary
# ============================================================
message(STATUS "")
message(STATUS "==================== RawrXD Build Configuration ====================")
message(STATUS "Build type:            ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:          C++${CMAKE_CXX_STANDARD}")
message(STATUS "Platform:              ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler:              ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "")
message(STATUS "Build Options:")
message(STATUS "  GUI (Qt IDE):        ${BUILD_GUI}")
message(STATUS "  CLI:                 ${BUILD_CLI}")
message(STATUS "  Tests:               ${BUILD_TESTS}")
message(STATUS "  Vulkan Stubs:        ${ENABLE_VULKAN}")
message(STATUS "  ROCm Stubs:          ${ENABLE_ROCM}")
message(STATUS "")
message(STATUS "Output directory:      ${CMAKE_BINARY_DIR}/bin")
message(STATUS "====================================================================")
message(STATUS "")
