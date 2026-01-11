add_library(ai_completion_cache STATIC
    ai_completion_cache.h
    ai_completion_cache.cpp
)

target_link_libraries(ai_completion_cache
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(ai_completion_cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
