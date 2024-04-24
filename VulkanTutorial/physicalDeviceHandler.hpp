
#ifndef physicalDevice_hpp
#define physicalDevice_hpp

#include "queueFamiliesHandler.h"


class PhysicalDeviceHandler {
    
private:
    uint32_t physicalDeviceCount = 0;
    
public:
    
    //  The graphics card that will be selected will be stored in this handle
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    QueueFamiliesHandler queueFamiliesHandler;
    
    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
        
        
        
        
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
            
            if (isDeviceSuitable(device, surface)) {
                physicalDevice = device;
                break;
            }
            
        }
        
        //  Check if the physical device handler is still null
        if (physicalDevice == VK_NULL_HANDLE){
            throw std::runtime_error("Failed to find a suitable GPU");
        } else {
            std::cout << "Suitable GPU found" << std::endl;
        }
        
    }
    
    //  Find the queue families for the device
    //  Checks if the device supports the Graphics family queue
    bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        QueueFamiliesHandler::QueueFamilyIndices indices = queueFamiliesHandler.findQueueFamilies(physicalDevice, surface);
        return indices.isComplete();
    }
    
};


#endif
