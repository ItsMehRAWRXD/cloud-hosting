add_library(todo_widget STATIC
    todo_widget.h
    todo_widget.cpp
)

target_link_libraries(todo_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(todo_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
