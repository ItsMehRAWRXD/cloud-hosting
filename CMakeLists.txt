cmake_minimum_required(VERSION 3.20)
project(RawrXD-ModelLoader VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

# Options
option(ENABLE_TESTING "Enable unit tests" OFF)
option(ENABLE_GTEST "Enable Google Test framework" OFF)
option(ENABLE_QT_TESTS "Enable Qt Test framework" OFF)
option(ENABLE_VULKAN "Enable Vulkan support" OFF)
option(ENABLE_BENCHMARKING "Enable benchmark targets" OFF)
option(ENABLE_THREAD_SAFETY_TESTS "Enable thread safety tests" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

# Required Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Concurrent Test)

# Optional Qt6 packages
find_package(Qt6 QUIET COMPONENTS Charts Multimedia)
if(Qt6Charts_FOUND)
    message(STATUS "Qt6::Charts found")
endif()
if(Qt6Multimedia_FOUND)
    message(STATUS "Qt6::Multimedia found")
endif()

# Optional dependencies
if(ENABLE_VULKAN)
    find_package(Vulkan QUIET)
    if(Vulkan_FOUND)
        message(STATUS "Vulkan found: ${Vulkan_LIBRARIES}")
    else()
        message(WARNING "Vulkan requested but not found")
    endif()
endif()

find_package(OpenSSL QUIET)
if(OpenSSL_FOUND)
    message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
endif()

# ASAN support
if(ENABLE_ASAN)
    if(MSVC)
        add_compile_options(/fsanitize=address)
    else()
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    endif()
    message(STATUS "AddressSanitizer enabled")
endif()

# Coverage support
if(ENABLE_COVERAGE)
    if(NOT MSVC)
        add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
        add_link_options(--coverage)
        message(STATUS "Code coverage enabled")
    else()
        message(WARNING "Code coverage not supported with MSVC")
    endif()
endif()

# Main executable sources
set(MAIN_SOURCES
    src/main.cpp
    src/orchestration/agent_coordinator.cpp
    src/components/voice_processor.cpp
    src/components/ai_merge_resolver.cpp
    src/components/semantic_diff_analyzer.cpp
    src/components/zero_retention_manager.cpp
    src/components/sandboxed_terminal.cpp
    src/components/model_registry.cpp
    src/qtapp/model_memory_hotpatch.cpp
    src/qtapp/byte_level_hotpatcher.cpp
    src/qtapp/gguf_server_hotpatch.cpp
    src/qtapp/unified_hotpatch_manager.cpp
    src/qtapp/proxy_hotpatcher.cpp
    src/qtapp/MainWindow.cpp
    src/qtapp/main_qt.cpp
    src/agent/agentic_failure_detector.cpp
    src/agent/agentic_puppeteer.cpp
)

add_executable(RawrXD-AgenticIDE ${MAIN_SOURCES})

target_include_directories(RawrXD-AgenticIDE PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(RawrXD-AgenticIDE PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Concurrent
)

if(Qt6Charts_FOUND)
    target_link_libraries(RawrXD-AgenticIDE PRIVATE Qt6::Charts)
    target_compile_definitions(RawrXD-AgenticIDE PRIVATE HAS_QT_CHARTS)
endif()

if(Qt6Multimedia_FOUND)
    target_link_libraries(RawrXD-AgenticIDE PRIVATE Qt6::Multimedia)
    target_compile_definitions(RawrXD-AgenticIDE PRIVATE HAS_QT_MULTIMEDIA)
endif()

if(Vulkan_FOUND)
    target_link_libraries(RawrXD-AgenticIDE PRIVATE Vulkan::Vulkan)
    target_compile_definitions(RawrXD-AgenticIDE PRIVATE HAS_VULKAN)
endif()

if(OpenSSL_FOUND)
    target_link_libraries(RawrXD-AgenticIDE PRIVATE OpenSSL::SSL OpenSSL::Crypto)
    target_compile_definitions(RawrXD-AgenticIDE PRIVATE HAS_OPENSSL)
endif()

# Testing
if(ENABLE_TESTING)
    enable_testing()

    if(ENABLE_GTEST)
        find_package(GTest REQUIRED)
    endif()

    set(TEST_COMPONENTS
        voice_processor
        ai_merge_resolver
        semantic_diff_analyzer
        zero_retention_manager
        sandboxed_terminal
        model_registry
    )

    foreach(component ${TEST_COMPONENTS})
        set(test_target ${component}_test)
        set(test_source tests/${component}_test.cpp)

        if(EXISTS ${CMAKE_SOURCE_DIR}/${test_source})
            add_executable(${test_target} ${test_source}
                src/components/${component}.cpp
            )

            target_include_directories(${test_target} PRIVATE
                ${CMAKE_SOURCE_DIR}/src
                ${CMAKE_SOURCE_DIR}/include
            )

            target_link_libraries(${test_target} PRIVATE
                Qt6::Core
                Qt6::Network
                Qt6::Concurrent
            )

            if(ENABLE_GTEST AND GTest_FOUND)
                target_link_libraries(${test_target} PRIVATE GTest::gtest GTest::gtest_main)
            endif()

            if(ENABLE_QT_TESTS)
                target_link_libraries(${test_target} PRIVATE Qt6::Test)
            endif()

            add_test(NAME ${test_target} COMMAND ${test_target})
        else()
            message(STATUS "Test source not found, skipping: ${test_source}")
        endif()
    endforeach()

    if(ENABLE_THREAD_SAFETY_TESTS AND NOT MSVC)
        if(ENABLE_ASAN)
            message(WARNING "ThreadSanitizer is incompatible with AddressSanitizer; skipping TSAN")
        else()
            add_compile_options(-fsanitize=thread)
            add_link_options(-fsanitize=thread)
            message(STATUS "Thread safety tests enabled")
        endif()
    endif()
endif()

# Benchmarking
if(ENABLE_BENCHMARKING)
    find_package(benchmark QUIET)

    set(BENCH_COMPONENTS
        voice_processor
        ai_merge_resolver
        semantic_diff_analyzer
        model_registry
    )

    foreach(component ${BENCH_COMPONENTS})
        set(bench_target ${component}_benchmark)
        set(bench_source tests/benchmarks/${component}_benchmark.cpp)

        if(EXISTS ${CMAKE_SOURCE_DIR}/${bench_source})
            add_executable(${bench_target} ${bench_source}
                src/components/${component}.cpp
            )

            target_include_directories(${bench_target} PRIVATE
                ${CMAKE_SOURCE_DIR}/src
                ${CMAKE_SOURCE_DIR}/include
            )

            target_link_libraries(${bench_target} PRIVATE
                Qt6::Core
                Qt6::Network
                Qt6::Concurrent
            )

            if(benchmark_FOUND)
                target_link_libraries(${bench_target} PRIVATE benchmark::benchmark)
            endif()
        else()
            message(STATUS "Benchmark source not found, skipping: ${bench_source}")
        endif()
    endforeach()
endif()

message(STATUS "")
message(STATUS "=== RawrXD-AgenticIDE Build Configuration ===")
message(STATUS "  C++ Standard:      ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "  Testing:           ${ENABLE_TESTING}")
message(STATUS "  Google Test:       ${ENABLE_GTEST}")
message(STATUS "  Qt Tests:          ${ENABLE_QT_TESTS}")
message(STATUS "  Vulkan:            ${ENABLE_VULKAN}")
message(STATUS "  Benchmarking:      ${ENABLE_BENCHMARKING}")
message(STATUS "  ASAN:              ${ENABLE_ASAN}")
message(STATUS "  Coverage:          ${ENABLE_COVERAGE}")
message(STATUS "==============================================")
