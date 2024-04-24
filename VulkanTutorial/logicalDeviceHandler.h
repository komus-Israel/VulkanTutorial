#ifndef logicalDeviceHandler_h
#define logicalDeviceHandler_h
#include "queueFamiliesHandler.h"

class LogicalDeviceHandler {
    
    //  After selecting a physical device to use,
    //  a logical device needs to be setup to interface with it
    
    
    
    //  QueueFamilies handler
    QueueFamiliesHandler queueFamiliesHandler;
    
    
public:
    
    //  To store the logical device
    VkDevice device;
    
    //  The queues are automatically created along with the logical device
    VkQueue graphicsQueue;
    
    void createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface){
        
        //  The creation involves specifying a bunch of details in structs
        //  The first one will be `VkDeviceQueueCreateInfo`
        //  This structure describes the number of queues we want for a single queue family
        
        QueueFamiliesHandler::QueueFamilyIndices indices = queueFamiliesHandler.findQueueFamilies(physicalDevice, surface);
        
        VkDeviceQueueCreateInfo queueCreateInfo{};
        
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        
        //  Vulkan allows to assign priorities to queues to influence the scheduling of
        //  command buffer execution using floating point numbers between 0.0 and 1.0
        //  This is required even if there is a single queue
        
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        
        //  Specifying the device features that will be used
        
        VkPhysicalDeviceFeatures deviceFeatures{};
        
        //  Creating the logical device
        //  With the previous two structures in place, the main VkDeviceCreateInfo can now be filled
        VkDeviceCreateInfo createInfo{};
        
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        
        // First add pointers to the queue creation info and device features structs
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        
        // Instantiate the logical device
        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            
            throw std::runtime_error("Failed to create logical device!");
        }
        
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        
    }
    
};

#endif /* logicalDeviceHandler_h */
