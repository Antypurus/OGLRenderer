/* date = July 29th 2020 0:21 am */
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <memory>

namespace VKR
{
    
    struct Window
    {
        public:
        std::string name = "";
        uint32_t height;
        uint32_t width;
        GLFWwindow* window_handle = nullptr;
        
        private:
        bool is_open = true;
        
        public:
        Window(uint32_t width, uint32_t height, const std::string& window_name);
        Window(uint32_t width, uint32_t height, std::string&& window_name);
        ~Window();
        bool IsOpen();
        
        private:
        /*Handles creating the GLFW window regardless of what constructor was used*/
        void CreateWindow();
        /*Polls Window Events*/
        void PollEvents();
        
    };
    
}