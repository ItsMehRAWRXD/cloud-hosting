add_library(pomodoro_widget STATIC
    pomodoro_widget.h
    pomodoro_widget.cpp
)

target_link_libraries(pomodoro_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(pomodoro_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
