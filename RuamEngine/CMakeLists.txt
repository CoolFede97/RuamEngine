# CMakeList.txt : CMake project for RuamEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8...4.1.0)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Requerido por Tomy para poder usar lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project ("RuamEngine")

# Add source to this project's executable.
add_executable (RuamEngine "RuamEngine.cpp" "RuamEngine.h")

include(FetchContent)
include(FindOpenGL)

add_subdirectory(CoolFede97)
add_subdirectory(Santinorua)
add_subdirectory(Skuerjuan)
add_subdirectory(Tomyatana)

# -----------------------------------------------------------------------------------------------------
#######################################################################################################
#######################################################################################################
# -----------------------------------------------------------------------------------------------------


# Libraries paths depending on the configuration (x86 or x64 Debug)

set(VENDOR_PATH "${CMAKE_SOURCE_DIR}/vendor")
# Not used
#if (CMAKE_SIZEOF_VOID_P EQUAL 8)
#else()
#endif()


# -----------------------------------------------------------------------------------------------------
#######################################################################################################
#######################################################################################################
# -----------------------------------------------------------------------------------------------------

# Json

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)

# -----------------------------------------------------------------------------------------------------
#######################################################################################################
#######################################################################################################
# -----------------------------------------------------------------------------------------------------

# Vendor

add_library(Vendor INTERFACE)

target_include_directories(Vendor INTERFACE 
	${CMAKE_SOURCE_DIR}/vendor/GLM
	${CMAKE_SOURCE_DIR}/vendor/stb_image
#   ${CMAKE_SOURCE_DIR}/vendor/IMGUI
)

target_compile_definitions(Vendor INTERFACE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(Vendor INTERFACE GLEW_STATIC)

# ///////////////////////////////////////////////////////////////////////////////////////////////

# GLEW

FetchContent_Populate(
  glew
  URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip
)
add_library(glew_s STATIC ${glew_SOURCE_DIR}/src/glew.c
                          ${glew_SOURCE_DIR}/src/glewinfo.c)
target_include_directories(glew_s PUBLIC ${glew_SOURCE_DIR}/include)
target_compile_definitions(
  glew_s
  PRIVATE GLEW_BUILD
  PUBLIC GLEW_STATIC)
target_link_libraries(glew_s PUBLIC OpenGL::GL)

# ///////////////////////////////////////////////////////////////////////////////////////////////

# GLFW

#add_library(GLFW INTERFACE)
#target_link_directories(GLFW INTERFACE ${GLFW_SRC_PATH})
#target_include_directories(GLFW INTERFACE ${GLFW_INCLUDE_PATH})

#target_link_directories(Vendor INTERFACE
#    ${GLEW_SRC_PATH}
#)

set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8)
FetchContent_MakeAvailable(glfw)

set(BUILD_SHARED_LIBS ON)

# ///////////////////////////////////////////////////////////////////////////////////////////////

# GLM

FetchContent_Populate(
  glm 
  URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${glm_SOURCE_DIR})
add_library(glm::glm ALIAS glm)

# ///////////////////////////////////////////////////////////////////////////////////////////////

# dr_libs

FetchContent_Populate(
  dr_libs
  URL		https://github.com/mackron/dr_libs/archive/refs/heads/master.zip
)
add_library(dr_libs INTERFACE)
target_include_directories(dr_libs INTERFACE ${dr_libs_SOURCE_DIR})
add_library(dr_libs::dr_libs ALIAS dr_libs)

# ///////////////////////////////////////////////////////////////////////////////////////////////

# IMGUI

FetchContent_Declare(
    imgui
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         v1.92.3-docking
)
FetchContent_MakeAvailable(imgui)

message("${imgui_SOURCE_DIR}")

# In order to use a library which has cpp and h files, you need to create a library (add_library) and
# give it a source path (with the cpp files). 
file (GLOB IMGUI_SRC "${imgui_SOURCE_DIR}/*.cpp" "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp" "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")
add_library(ImGui STATIC ${IMGUI_SRC})
# In this case, the library has #include <AnotherLibrary> in its cpp files, like GLFW or GLEW, so 
# we need to indicate where those glfw.h and glew.h files are.
target_link_libraries(ImGui PUBLIC glfw glew_s)
target_include_directories(ImGui PUBLIC "${imgui_SOURCE_DIR}")
# Lastly, add this new library to the function target_link_directories and also provice the path to 
# the h files to the target_include_directories function

# ///////////////////////////////////////////////////////////////////////////////////////////////

# OpenAl-soft
FetchContent_Declare(
    openal-soft
    GIT_REPOSITORY https://github.com/kcat/openal-soft.git
    GIT_TAG         dc7d7054a5b4f3bec1dc23a42fd616a0847af948
    FIND_PACKAGE_ARGS NAMES OpenAl
)

set(ALSOFT_EXAMPLES         OFF)
set(ALSOFT_UTILS            OFF)
set(ALSOFT_NO_CONFIG_UTIL   ON)
FetchContent_MakeAvailable(openal-soft)


# ///////////////////////////////////////////////////////////////////////////////////////////////

if(LINUX)
    # easy_profiler
    FetchContent_Declare(
            easy_profiler
            GIT_REPOSITORY https://github.com/yse/easy_profiler.git
            GIT_TAG v2.1.0
    )

    set(PREV_MIN_VERSION ${CMAKE_POLICY_VERSION_MINIMUM})

    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

    FetchContent_MakeAvailable(easy_profiler)

    set(CMAKE_POLICY_VERSION_MINIMUM ${PREV_MIN_VERSION})
endif ()


# ///////////////////////////////////////////////////////////////////////////////////////////////

# Linking
#[==========[
Not used

if (UNIX)
    target_link_libraries(Vendor INTERFACE
        glfw glew_s glm ImGui OpenGL::GL
    )
endif()
if (WIN32)
    target_link_libraries(Vendor INTERFACE
        glfw glew_s glm ImGui OpenGL::GL 
    )
endif()
]==========]

target_compile_definitions(Vendor INTERFACE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(Vendor INTERFACE BUILD_WITH_EASY_PROFILER)

target_link_libraries(Vendor INTERFACE
        glfw glew_s glm ImGui OpenGL::GL OpenAL::OpenAL nlohmann_json::nlohmann_json dr_libs
)
if (LINUX)
    target_link_libraries(Vendor INTERFACE easy_profiler)
endif ()

target_include_directories(Vendor INTERFACE 
    ${VENDOR_PATH}/stb_image
)

target_compile_definitions(Vendor INTERFACE GLEW_STATIC)

# -----------------------------------------------------------------------------------------------------
#######################################################################################################
#######################################################################################################
# -----------------------------------------------------------------------------------------------------


add_library(RuamCore INTERFACE)


# -----------------------------------------------------------------------------------------------------

# Rendering

file (GLOB RENDERING_SRC "${CMAKE_SOURCE_DIR}/RuamCore/Rendering/*.cpp")
add_library(Rendering STATIC ${RENDERING_SRC}
        RuamCore/Rendering/VertexBufferLayout.cpp)
target_link_libraries(Rendering PUBLIC Vendor Files Maths RuamUtils)
target_include_directories(Rendering PUBLIC ${CMAKE_SOURCE_DIR}/RuamCore/Rendering)

# ///////////////////////////////////////////////////////////////////////////////////////////////

# Maths

# -----------------------------------------------------------------------------------------------------

# Vec

file (GLOB VEC_SRC "${CMAKE_SOURCE_DIR}/RuamCore/Maths/Vec/*.cpp")
add_library(Vec STATIC ${VEC_SRC})
target_include_directories(Vec PUBLIC ${CMAKE_SOURCE_DIR}/RuamCore/Maths/Vec)
target_link_libraries(Vec PUBLIC Vendor)

# -----------------------------------------------------------------------------------------------------

add_library(Maths INTERFACE)
target_link_libraries(Maths INTERFACE Vec)

# ///////////////////////////////////////////////////////////////////////////////////////////////

# Time

file (GLOB TIME_SRC "${CMAKE_SOURCE_DIR}/RuamCore/Time/*.cpp")
add_library(Time STATIC ${TIME_SRC})
target_link_libraries(Time PUBLIC glfw)
target_include_directories(Time INTERFACE ${CMAKE_SOURCE_DIR}/RuamCore/Time)


# ///////////////////////////////////////////////////////////////////////////////////////////////

# Files

file (GLOB FILES_SRC "${CMAKE_SOURCE_DIR}/RuamCore/Files/*.cpp")
add_library(Files STATIC ${FILES_SRC})
target_compile_definitions(Files PUBLIC PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")
target_include_directories(Files INTERFACE ${CMAKE_SOURCE_DIR}/RuamCore/Files)


# ///////////////////////////////////////////////////////////////////////////////////////////////

# Input

file (GLOB INPUT_SRC "${CMAKE_SOURCE_DIR}/RuamCore/Input/*.cpp")
add_library(Input STATIC ${INPUT_SRC})
target_link_libraries(Input PUBLIC Rendering Maths)
target_include_directories(Input INTERFACE ${CMAKE_SOURCE_DIR}/RuamCore/Input)


# /////////////////////////////////////////////////////////////////////////////////////////////// 

# Utils

add_library(RuamUtils INTERFACE)
target_include_directories(RuamUtils INTERFACE ${CMAKE_SOURCE_DIR}/RuamCore/RuamUtils)


# /////////////////////////////////////////////////////////////////////////////////////////////// 

# Audio

file (GLOB AUDIO_SRC "${CMAKE_SOURCE_DIR}/RuamCore/Audio/*.cpp")
add_library(Audio STATIC ${AUDIO_SRC})
target_include_directories(Audio PUBLIC "${CMAKE_SOURCE_DIR}/RuamCore/Audio")
target_link_libraries(Audio PUBLIC glm dr_libs Vendor RuamUtils)


# ///////////////////////////////////////////////////////////////////////////////////////////////

# Objects

file (GLOB OBJECTS_SRC "${CMAKE_SOURCE_DIR}/RuamCore/Objects/*.cpp")
file (GLOB COMPONENTS_SRC "${CMAKE_SOURCE_DIR}/RuamCore/Components/*.cpp")
add_library(Objects STATIC ${OBJECTS_SRC} ${COMPONENTS_SRC})
target_include_directories(Objects PUBLIC "${CMAKE_SOURCE_DIR}/RuamCore/Objects" "${CMAKE_SOURCE_DIR}/RuamCore/Components")
target_link_libraries(Objects PUBLIC Audio RuamUtils)


# ///////////////////////////////////////////////////////////////////////////////////////////////

# Event System

file (GLOB EVENTSYSTEM_SRC "${CMAKE_SOURCE_DIR}/RuamCore/EventSystem/*.cpp")
add_library(EventSystem STATIC ${EVENTSYSTEM_SRC})
target_link_libraries(EventSystem PUBLIC Rendering Maths)
target_include_directories(EventSystem INTERFACE ${CMAKE_SOURCE_DIR}/RuamCore/EventSystem)


# ///////////////////////////////////////////////////////////////////////////////////////////////

# Linking

target_link_libraries(RuamCore INTERFACE
    Rendering Maths Time Files Input Objects RuamUtils Audio EventSystem
)

# -----------------------------------------------------------------------------------------------------
#######################################################################################################
#######################################################################################################
# -----------------------------------------------------------------------------------------------------

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RuamEngine PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(RuamEngine PUBLIC RuamCore)

# TODO: Add tests and install targets if needed.
