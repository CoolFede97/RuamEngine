
project("Skuerjuan")
set(CMAKE_CXX_STANDARD 17)

add_executable(Skuerjuan "src/main.cpp"
        src/TestComponent.cpp)

target_link_libraries(Skuerjuan PRIVATE RuamCore)