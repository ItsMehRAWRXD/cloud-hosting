add_library(update_checker_widget STATIC
    update_checker_widget.h
    update_checker_widget.cpp
)

target_link_libraries(update_checker_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(update_checker_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
