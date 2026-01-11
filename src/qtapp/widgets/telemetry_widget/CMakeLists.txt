add_library(telemetry_widget STATIC
    telemetry_widget.h
    telemetry_widget.cpp
)

target_link_libraries(telemetry_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Charts
)

target_include_directories(telemetry_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
