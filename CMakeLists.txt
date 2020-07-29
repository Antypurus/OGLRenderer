cmake_minimum_required(VERSION 3.14)

project(OGL_Renderer VERSION 0.0.1)


if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # MSVC/Windows Configurations
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
    set(warnings "/W4 /WX /EHsc") # Warning levels
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd26812") # Disables the warning from the usage of C-Style enums
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /nologo /Oi") # Multithreaded compiling, intrinsic generation, and not msvc logo on compile
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    # using Intel C++
endif()

add_subdirectory(ThirdParty/glfw)
add_subdirectory(ThirdParty/glm)

file(GLOB_RECURSE  SRCS
            "Renderer/*.cpp"
            "Renderer/*.hpp"
	        "Renderer/*.h")

add_executable(${CMAKE_PROJECT_NAME} Renderer/main.cpp ${SRCS})

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories( ${OPENGL_INCLUDE_DIRS})
    target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES})
endif()

include_directories("ThirdParty/glew/include")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
    target_link_libraries(${CMAKE_PROJECT_NAME} "ThirdParty/glew/lib/release/x64/glew32s")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
    target_link_libraries(${CMAKE_PROJECT_NAME} "ThirdParty/glew/lib/release/Win32/glew32s")
endif()

# GLEW_STATIC must be defined due to the usage of static linking with the GLEW library
add_compile_definitions(GLEW_STATIC)

target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} glfw)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20) # Enable C++20 Support