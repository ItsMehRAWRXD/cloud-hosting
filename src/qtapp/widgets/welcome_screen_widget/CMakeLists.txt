add_library(welcome_screen_widget STATIC
    welcome_screen_widget.h
    welcome_screen_widget.cpp
)

target_link_libraries(welcome_screen_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(welcome_screen_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
