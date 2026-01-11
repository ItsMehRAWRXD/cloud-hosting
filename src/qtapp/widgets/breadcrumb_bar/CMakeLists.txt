add_library(breadcrumb_bar STATIC
    breadcrumb_bar.h
    breadcrumb_bar.cpp
)

target_link_libraries(breadcrumb_bar
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(breadcrumb_bar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
