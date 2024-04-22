
#ifndef physicalDevice_hpp
#define physicalDevice_hpp

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>   // To report and propagate errors
#include <stdexcept> // To report and propagate errors
#include <cstdlib> //  provides the EXIT_SUCCESS and EXIT_FAILURE macros.
#include <vector>


class PhysicalDeviceHandler {
    
    //  The graphics card that will be selected will be stored in this handle
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    
public:
    
    void pickPhysicalDevice() {
        std::cout << "Header Working" << std::endl;
    }
    
};


#endif
