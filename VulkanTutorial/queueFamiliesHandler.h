#ifndef queueFamiliesHandler_h
#define queueFamiliesHandler_h

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>   // To report and propagate errors
#include <stdexcept> // To report and propagate errors
#include <cstdlib> //  provides the EXIT_SUCCESS and EXIT_FAILURE macros.
#include <vector>
#include <cstring> // for strcmp
#include <optional> // to query if a variable contains a value


class QueueFamiliesHandler {
    
public:
    
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        
        bool isComplete() {
            return graphicsFamily.has_value();
        }
    };
    
    //  Look for the queue that supports graphics command
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice) {
        QueueFamilyIndices indices;
        
        //  Logic to find queue family indices to populate struct with
        //  Assign index to queue families that could be found
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        
        //  Log the number of queue families
        std::cout << "The number of queue families detected are: " << queueFamilyCount << std::endl;
        
        //  A vector of queue families
        //  VkQueueFamilyProperties struct contains some details about the queue
        //  family, including the type of operations that are supported
        //  and the number of queues that can be created based on that family
        std::vector<VkQueueFamilyProperties> queueFamily(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamily.data());
        
        //  find at least one queue that supports VK_QUEUE_GRAPHICS_BIT
        int index = 0;
        for (const auto& queueFamily : queueFamily) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = index;
            
            //  Break if queue family has been found
            if (indices.isComplete()){
                break;
            }
        }
        
        index++;
        
        return indices;
    }
    
};

#endif /* queueFamiliesHandler_h */
