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
    
    //  Look for the queue that supports graphics command
    void findQueueFamilies() {
        std::cout << "Queue families header working" << std::endl;
    }
    
};

#endif /* queueFamiliesHandler_h */
