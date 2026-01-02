add_library(common STATIC 
    Graph.cpp
    Node.cpp
)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
