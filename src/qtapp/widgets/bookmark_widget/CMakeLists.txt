add_library(bookmark_widget STATIC
    bookmark_widget.h
    bookmark_widget.cpp
)

target_link_libraries(bookmark_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(bookmark_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
