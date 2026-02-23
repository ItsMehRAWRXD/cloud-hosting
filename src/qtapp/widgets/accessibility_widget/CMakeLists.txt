add_library(accessibility_widget STATIC
    accessibility_widget.h
    accessibility_widget.cpp
)

target_link_libraries(accessibility_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(accessibility_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
