CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

project("Sandbox")

add_executable(Sandbox "main.cpp")

target_link_libraries(Sandbox PUBLIC RuamCore Vendor)

set_property(TARGET Sandbox PROPERTY CXX_STANDARD 20)
