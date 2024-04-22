
#ifndef physicalDevice_hpp
#define physicalDevice_hpp

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>   // To report and propagate errors
#include <stdexcept> // To report and propagate errors
#include <cstdlib> //  provides the EXIT_SUCCESS and EXIT_FAILURE macros.
#include <vector>
#include <cstring> // for strcmp
#include <optional> // to query if a variable contains a value

class PhysicalDeviceHandler {
    
private:
    uint32_t physicalDeviceCount = 0;
    
public:
    
    
    //  The graphics card that will be selected will be stored in this handle
    static constexpr VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    
    void pickPhysicalDevice(VkInstance instance) {
        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
        std::cout << "Header Working" << std::endl;
    }
    
};


#endif
