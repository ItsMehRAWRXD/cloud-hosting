add_library(search_result_widget STATIC
    search_result_widget.h
    search_result_widget.cpp
)

target_link_libraries(search_result_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(search_result_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
