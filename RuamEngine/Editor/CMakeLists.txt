file(GLOB EDITOR_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(Editor ${EDITOR_SRC})
target_link_libraries(Editor PRIVATE RuamCore)
