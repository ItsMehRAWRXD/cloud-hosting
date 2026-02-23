add_library(language_client_host STATIC
    language_client_host.h
    language_client_host.cpp
)

target_link_libraries(language_client_host
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

target_include_directories(language_client_host PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
