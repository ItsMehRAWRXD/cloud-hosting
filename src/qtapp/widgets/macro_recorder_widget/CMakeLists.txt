add_library(macro_recorder_widget STATIC
    macro_recorder_widget.h
    macro_recorder_widget.cpp
)

target_link_libraries(macro_recorder_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(macro_recorder_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
