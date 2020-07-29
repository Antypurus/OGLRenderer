cmake_minimum_required(VERSION 3.14)

project(VK_Renderer VERSION 0.0.1)

find_package(Vulkan REQUIRED)
add_subdirectory(ThirdParty/glfw)
add_subdirectory(ThirdParty/glm)

# MSVC Configurations
set(warnings "/W4 /WX /EHsc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd26812")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /nologo /Oi")

file(GLOB_RECURSE  SRCS
        "Renderer/*.cpp"
        "Renderer/*.hpp"
	    "Renderer/*.h")

add_compile_definitions(WIN32_LEAN_AND_MEAN)

add_executable(${CMAKE_PROJECT_NAME} Renderer/main.cpp ${SRCS})


set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} glfw)