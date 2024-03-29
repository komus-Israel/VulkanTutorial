#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h> ///  To provide functions, structures and enumerations

#include <iostream> /// To report and propagate errors
#include <stdexcept> /// To report and propagate errors
#include <cstdlib> ///  provides the EXIT_SUCCESS and EXIT_FAILURE macros.

class HelloTriangleApplication {
    
    
public:
    
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    
    
    
    void run (){
        initWindow();
        initVulkan();
        mainLoop();
        cleanUp();
    }
    
private:
    
    GLFWwindow* window;
    VkInstance instance;
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    
    
    
    void initWindow(){
        
        /// The very first call in initWindow should be glfwInit() which initializes the GLFW library.
        glfwInit();
        
        /// Because GLFW was originally designed to create an OpenGL context, we need to tell it to not create an OpenGL context with a subsequent call
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        /// Because handling resized windows takes special care that we'll look into later, disable it for now with another window hint call
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        /// All that's left now is creating the actual window. Add a GLFWwindow* window; private class member to store a reference to it and initialize the window
        /// The first three parameters specify the width, height and title of the window. The fourth parameter allows you to optionally specify a monitor to open the window on and the last parameter is only relevant to OpenGL
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Window Creation", nullptr, nullptr);
        
        
    
    }
    
    /// initialize vulkan object
    void initVulkan(){
        createInstance();
    }
    
    void createInstance(){
        
        /// Optional struct: Information about the application
        VkApplicationInfo appInfo{};
        
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        
        /// Struct is not optional and tells the Vulkan driver which global extensions and validation layers we want to use
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;
        
        /// create instance
        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        
        std::vector<const char*> requiredExtensions;

        for(uint32_t i = 0; i < glfwExtensionCount; i++) {
            requiredExtensions.emplace_back(glfwExtensions[i]);
        }

        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create instance");
        }
        
    }
    
    /// render frames
    void mainLoop(){
        
        /// To keep the application running until either an error occurs or the window is closed, we need to add an event loop to the mainLoop function as follows:
        
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
        
    }
    
    /// deallocate memory
    void cleanUp(){
        
        /// Once the window is closed, we need to clean up resources by destroying it and terminating GLFW itself
        glfwDestroyWindow(window);
        glfwTerminate();
        
    }
    
};


int main(){
    HelloTriangleApplication app;
    
    try{
        app.run();
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
