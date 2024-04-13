#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h> /// GLFW will include its own definitions and automatically load the vulkan header with it

#include <iostream>   /// To report and propagate errors
#include <stdexcept> /// To report and propagate errors
#include <cstdlib> ///  provides the EXIT_SUCCESS and EXIT_FAILURE macros.
#include <vector>
#include <cstring> /// for strcmp

class HelloTriangleApplication {
    
public:
    
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
/// `NDEBUG` macro is a c++ standard. It means `not debug`
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    
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
    uint32_t deviceCount = 0;
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
        
        /// validation layer check
        if (enableValidationLayers && !checkValidationLayerSupport()){
            throw std::runtime_error("Validation Layer Requested, but not found");
        }
        
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
        
        /// modity the `VkInstanceCreateInfo` struct instantiation to include the
        /// validation layer names if they are enabled
        
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }
        
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        
        /// vulkan is a platform agnostic API
        /// meaning, it needs an extension to interface with the window system
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        
        /// these two members of the struct determine the global validation layers to enable
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        
        createInfo.enabledLayerCount = 0;
        
        /// FROM HERE
        
        std::vector<const char*> requiredExtensions;
        
        for(uint32_t i = 0; i < glfwExtensionCount; i++) {
            requiredExtensions.emplace_back(glfwExtensions[i]);
        }
        
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        
        createInfo.enabledExtensionCount = (uint32_t)
        requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        
        /// TO HERE FIXES VK_ERROR_INCOMPATIBLE_DRIVER
    
        
        /// create the instance and store the result in the `result` variable
        /// If everything goes well, the handle to the instance is stored in the VkInstance class member
        /// `NOTE:` nearly all vulkan functions return a value of type VkResult that is either `VK_SUCCESS` or an `error code`
        /// `VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);`
        /// To check if the instance was created successfully, instead of storing the result, the success status can be checked like this instead:
        
        std::cout << &instance << ": instance checking" << std::endl;
        
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            
            throw std::runtime_error("Failed to create Instance!");
        }
        
    }

    void initVulkan() {
        createInstance();
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
    
    
    /// Add a method that checks if all of the requested validation layers are available
    /// this method can be called in the `createInsance` method
    bool checkValidationLayerSupport() {
        
        uint32_t layerCount;
        
        /// why is this initialized with a null pointer
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        
        /// declare vector of type `VkLayerProperties` and size `layerCount`
        std::vector<VkLayerProperties> availableLayers(layerCount);
        
        /// and why is this reinitialized with the vector
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        
        /// next, check if all of  the layers in `validationLayers` exist in the `availableLayers` list
        /// #include <cstring> for strcmp
        
        for (const char* layerName : validationLayers) {
            bool layerFound = false;
            
            /// auto detects the data type
            for (const auto& layerProperties : availableLayers ) {
                
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
                
            }
            
            if (!layerFound){
                return false;
            }
        }
        
        return true;
    }
    
    /// After initializing vulkan lib via the VkInstance, we need to look for and select a graphics card in the system tjat supports the feature we need
    void pickPhysicalDevice(){
        
        /// the graphics card will be stored in a `VkPhysicalDevice handle`
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        
        /// Listing the graphics card is very similar to the listing extensions and starts with querying just the number
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        
        /// If there are 0 devices with vulkan support then there is no point going further
        if (deviceCount == 0) {
            throw std::runtime_error("Failed to find GPUS with vulkan support!");
        }
        
        /// otherwise, we can now allocate an array to hold all the VkPhysicalDevice handles
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        
        /// Now, we need to evaluate each of them and check if they are
        /// suitable for the operations we want to perform because not all graphics cards are created equal
        /// for this, call the function created for this purpose `isDeviceSuitable`
        /// And we will check if any of the physical devices meet the requirements that we will add to the function
        
        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }
        
        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to find a suitable GPU!");
        }
        
    
        
    }
    
    bool isDeviceSuitable(VkPhysicalDevice device) {
        return true;
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



/// `NOTES ON VALIDATION LAYERS`
/// Common operations iin validation layers are:
/// 1.  Checking the values of parameters against the specification to detect misuse
/// 2.  Tracking creation and destruction of objects to find resource leaks
/// 3.  Checking thread safety by tracking the threads that calls originate from
/// 4.  Logging every call and its parameters to the standard output
/// 5.  Tracing vulkan calls for  profiling and replaying

