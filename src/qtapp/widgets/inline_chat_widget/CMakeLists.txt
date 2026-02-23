add_library(inline_chat_widget STATIC
    inline_chat_widget.h
    inline_chat_widget.cpp
)

target_link_libraries(inline_chat_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(inline_chat_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
