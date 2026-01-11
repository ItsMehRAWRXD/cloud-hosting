add_library(audio_call_widget STATIC
    audio_call_widget.h
    audio_call_widget.cpp
)

target_link_libraries(audio_call_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(audio_call_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
