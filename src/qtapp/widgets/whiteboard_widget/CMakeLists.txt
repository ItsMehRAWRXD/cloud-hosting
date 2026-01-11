add_library(whiteboard_widget STATIC
    whiteboard_widget.h
    whiteboard_widget.cpp
)

target_link_libraries(whiteboard_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(whiteboard_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
