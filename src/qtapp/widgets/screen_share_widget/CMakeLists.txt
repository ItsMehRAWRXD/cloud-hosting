add_library(screen_share_widget STATIC
    screen_share_widget.h
    screen_share_widget.cpp
)

target_link_libraries(screen_share_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(screen_share_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
