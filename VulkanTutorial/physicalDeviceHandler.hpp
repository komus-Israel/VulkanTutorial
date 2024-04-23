
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
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    
    void pickPhysicalDevice(VkInstance instance) {
        
        
        
        
        //  Query the number of physical devices
        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
        
        //  validate the gpu count
        if (physicalDeviceCount == 0) {
            std::runtime_error("Failed to Locate GPU with Vulkan support");
        }
        
        //  Declare a vector to hold all physical devices
        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
        
        
        //  log out the number of detected gpus with vulkan support
        std::cout << "Number of discovered GPUs: " << physicalDeviceCount << std::endl;
        
        // If any of the GPUs meet the requirement, that GPU is selected
        for (const auto& device : physicalDevices) {
            
            if (isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
            
        }
        
        //  Check if the physical device handler is still null
        if (physicalDevice == VK_NULL_HANDLE){
            std::runtime_error("Failed to find a suitable GPU");
        } else {
            std::cout << "Suitable GPU found" << std::endl;
        }
        
    }
    
    bool isDeviceSuitable(VkPhysicalDevice physicalDevice) {
        return true;
    }
    
};


#endif
