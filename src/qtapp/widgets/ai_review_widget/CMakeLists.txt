add_library(ai_review_widget STATIC
    ai_review_widget.h
    ai_review_widget.cpp
)

target_link_libraries(ai_review_widget
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(ai_review_widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
