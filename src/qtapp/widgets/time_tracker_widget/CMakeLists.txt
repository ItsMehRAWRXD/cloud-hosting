add_library(time_tracker_widget STATIC
    time_tracker_widget.h
    time_tracker_widget.cpp
)

target_link_libraries(time_tracker_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(time_tracker_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
