#ifndef surfaceHandler_h
#define surfaceHandler_h

#include "queueFamiliesHandler.h"

class SurfaceHandler {
    
    QueueFamiliesHandler queueFamiliesHandler;
    
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
