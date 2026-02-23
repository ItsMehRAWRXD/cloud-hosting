add_library(code_minimap STATIC
    code_minimap.h
    code_minimap.cpp
)

target_link_libraries(code_minimap
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(code_minimap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
