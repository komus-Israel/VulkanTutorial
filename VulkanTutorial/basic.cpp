#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h> /// GLFW will include its own definitions and automatically load the vulkan header with it

#include <iostream>   /// To report and propagate errors
#include <stdexcept> /// To report and propagate errors
#include <cstdlib> ///  provides the EXIT_SUCCESS and EXIT_FAILURE macros.

class HelloTriangleApplication {
    
public:
    
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    
    void run(){
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }
    
private:
    
    GLFWwindow* window;
    VkInstance instance;
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    
    /// To initialize GLFW
    void initWindow() {
        
        /// 1. Initlalize the GLFW library
        glfwInit();
        
        /// 2.  GLFW was originally designed to create an OPENGL context, we need to tell it not to
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        /// 3. Disable windows resize for now. Takes special care to handle resize
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        /// 4. Create the actual window
        window = glfwCreateWindow(800, 600, "Vulkan Tutorial", nullptr, nullptr);
        
    }
    
    /// create vulkan instance
    /// the instance is the connection between the application and the vulkan library
    void createInstance(){
        
        /// To create an instance, fill in a struct with some information about the application
        /// This struct is optional
        VkApplicationInfo appInfo{};
        
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "NO ENGINE";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        
        
        /// This struct isn't optional
        /// It tells the vulklan driver what global extentions and validation layers we want to use
        /// Global here means that they apply to the entire program and not a specific device
        
        VkInstanceCreateInfo createInfo{};
        
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        
        /// vulkan is a platform agnostic API
        /// meaning, it needs an extension to interface with the window system
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        
        /// these two members of the struct determine the global validation layers to enable
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        
        createInfo.enabledLayerCount = 0;
        
        /// create the instance and store the result in the `result` variable
        /// If everything goes well, the handle to the instance is stored in the VkInstance class member
        /// `NOTE:` nearly all vulkan functions return a value of type VkResult that is either `VK_SUCCESS` or an `error code`
        /// `VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);`
        /// To check if the instance was created successfully, instead of storing the result, the success status can be checked like this instead:
        
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Instance!");
        }
        
    }

    void initVulkan() {
        
    }
    
    /// to render frames
    void mainLoop(){
        
        /// To keep the application running until either an error occurs or the window is closed, add ab event loop
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
        
    }
    
    /// once window is closed and mainLoop returns, resources will be deallocated using this function
    /// terminate window, clean up resources by destroying it and terminating GLFW
    /// VkInstance should be only destroyed right before the program exits. It can be destroyed using the `vkDestroyInstance` function
    void cleanup() {
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
};



int main(){
   
    HelloTriangleApplication app;
    
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
    
}
