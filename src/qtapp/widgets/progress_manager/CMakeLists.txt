add_library(progress_manager STATIC
    progress_manager.h
    progress_manager.cpp
)

target_link_libraries(progress_manager
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(progress_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
