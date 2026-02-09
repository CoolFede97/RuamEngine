CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

project("RuamEditor")

add_executable(RuamEditor "src/main.cpp" "src/RuamEditor.cpp")

target_link_libraries(RuamEditor PUBLIC RuamCore Vendor)

set_property(TARGET RuamEditor PROPERTY CXX_STANDARD 20)
