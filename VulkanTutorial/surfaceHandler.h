#ifndef surfaceHandler_h
#define surfaceHandler_h

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>   // To report and propagate errors
#include <stdexcept> // To report and propagate errors

class SurfaceHandler {
    
    
    
public:
    VkSurfaceKHR surface;
    
    
    void createSurface(VkInstance instance, GLFWwindow* window) {
        
        //  the glfwCreateWindowSurface function performs the surface creation very well 
        //  with different implementation for each platform
        
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface!");
        }
    }
    
};
#endif /* surfaceHandler_h */
