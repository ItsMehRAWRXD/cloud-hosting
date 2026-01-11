add_library(plugin_manager_widget STATIC
    plugin_manager_widget.h
    plugin_manager_widget.cpp
)

target_link_libraries(plugin_manager_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(plugin_manager_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
