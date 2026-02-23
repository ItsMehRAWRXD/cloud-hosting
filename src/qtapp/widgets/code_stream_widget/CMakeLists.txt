add_library(code_stream_widget STATIC
    code_stream_widget.h
    code_stream_widget.cpp
)

target_link_libraries(code_stream_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(code_stream_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
