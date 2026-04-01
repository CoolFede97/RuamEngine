project("Santinorua")
set(CMAKE_CXX_STANDARD 17)

add_executable(Santinorua src/main.cpp)

file (GLOB TESTSANTI_SRC "${CMAKE_SOURCE_DIR}/Santinorua/src/*.cpp")
add_library(TestSanti STATIC ${TESTSANTI_SRC})
target_link_libraries(TestSanti PUBLIC RuamCore)
target_include_directories(Santinorua PRIVATE
        ${CMAKE_SOURCE_DIR}/Santinorua/src
)
target_link_libraries(Santinorua PRIVATE RuamCore TestSanti)

