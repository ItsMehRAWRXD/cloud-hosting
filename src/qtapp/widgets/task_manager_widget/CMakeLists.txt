add_library(task_manager_widget STATIC
    task_manager_widget.h
    task_manager_widget.cpp
)

target_link_libraries(task_manager_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(task_manager_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
