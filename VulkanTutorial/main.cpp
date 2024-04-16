//  MVK is MoltenVK
//  It is a Vulkan Portability implementation that layers the Vulkan graphics and compute
//  API over Apple's Metal graphics framework
//#define VK_USE_PLATFORM_MACOS_MVK // for macOSX
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h> // GLFW will include its own definitions and automatically load the vulkan header with it
//#define GLFW_EXPOSE_NATIVE_COCOA // for macOSX
//#include <GLFW/glfw3native.h>

#include <iostream>   // To report and propagate errors
#include <stdexcept> // To report and propagate errors
#include <cstdlib> //  provides the EXIT_SUCCESS and EXIT_FAILURE macros.
#include <vector>
#include <cstring> // for strcmp
#include <optional> // to query if a variable contains a value

class HelloTriangleApplication {
    
public:
    
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    
    
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;   // the graphics card will be stored in a `VkPhysicalDevice handle`
    VkDevice device;    // class member to store the logical device handle
    VkQueue graphicsQueue; //   class member to store a handle to the graphics  queue
    
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;  //  presentation queue
        
        /// to make this a little bit more convenient, we will add a generic check to this struct
        bool isComplete(){
            return graphicsFamily.has_value() &&
                   presentFamily.has_value();
        }
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
    VkSurfaceKHR surface;
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
        
        std::cout << "Required Extensions: " << std::endl;
        
        for(uint32_t i = 0; i < glfwExtensionCount; i++) {
            
            std::cout << glfwExtensions[i] << std::endl;
            requiredExtensions.emplace_back(glfwExtensions[i]);
        }
        
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        
        createInfo.enabledExtensionCount = (uint32_t)
        requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        
        /// TO HERE FIXES VK_ERROR_INCOMPATIBLE_DRIVER
        
        //  FROM HERE: Check extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        
        std::cout << "available extensions:\n";
        
        for (const auto& extension : extensions) {
            std::cout << '\t' << extension.extensionName << '\n';
        }
        
        //  TO HERE
    
        
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
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
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
    /// The device should be destroyed before instance termination
    void cleanup() {
        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
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
        
        /// Listing the graphics card is very similar to the listing extensions and starts with querying just the number
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        
        /// If there are 0 devices with vulkan support then there is no point going further
        if (deviceCount == 0) {
            throw std::runtime_error("Failed to find GPUS with vulkan support!");
        }
        
        std::cout << "Number of GPUs: " << deviceCount << std::endl;
        std::cout << "Physical Device: " << physicalDevice << std::endl; //    should be null for now
        
        /// otherwise, we can now allocate an array to hold all the VkPhysicalDevice handles
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        
        /// Now, we need to evaluate each of them and check if they are
        /// suitable for the operations we want to perform because not all graphics cards are created equal
        /// for this, call the function created for this purpose `isDeviceSuitable`
        /// And we will check if any of the physical devices meet the requirements that we will add to the function
        
        for (const auto& device : devices) {
            
            /// log out the devices
            std::cout << "GPU: " << device << std::endl;
            
            
            if (isDeviceSuitable(device)) {
                physicalDevice = device; // assign this device to the device handler
                break; // break once a suitable device is found
            }
        }
        
        //  After the iteration, if the device handler is still none
        //  Then no suitable device was found
        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to find a suitable GPU!");
        }
        
    
        
    }
    
    bool isDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indices = findQueueFamilies(device);
        return indices.isComplete();
    }
    
    /// We are going to look for a queue that supports graphics commands
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        
        QueueFamilyIndices indices;
        uint32_t queueFamilyCount = 0;
        VkBool32 presentSupport = 0;
        
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        
        
        std::cout << "Queue Family Count: " << queueFamilyCount << std::endl;
        
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        
        
        int i = 0;
        
        for (const auto& queueFamily: queueFamilies){
            
            std::cout << "Queue Flag: " << queueFamily.queueFlags << std::endl;
            std::cout << "Queue Count: " << queueFamily.queueCount << std::endl;
            
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.graphicsFamily = i;
            }
            
            // check that it has the capability to presenting to our window surface
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            
            std::cout << "It can present to our window surface: " << presentSupport << std::endl;
            
            if (presentSupport) {
                indices.presentFamily = i;
            }
            
            if (indices.isComplete()) {
                break;
            }
            
            i ++;
        }
        
        // Logic to find graphics queue family
        // Assign index to queue families that could be found
        return indices;
        
        
        
    }
    
        
    /// After selecting a physical device, a `logical device` needs to be setup to interface with it
    /// This creation process is similar to the instance creation process and describes the features we want to use
    /// We also need to specify which queues to create now that we've queried which queue families are available
    void createLogicalDevice(){
        
        /// specify the queues to be created
        /// The creation of a logical device involves specifying a bunch of details in structs again
        /// of which the first one will be `VkDeviceQueueCreateInfo`
        /// This struct describes the number of queues we want for a single queue family
        /// Right now, we're only interested in a queue with graphics capabilities
        
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
        
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        
        /// specifying used device features
        VkPhysicalDeviceFeatures deviceFeatures{};
        
        /// Creating the logical device
        /// with the previous tow structures in place, we can start filling in the main `VkDeviceCreateInfo` structure
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        
        /// first add pointers to the queue creation info and device features structs
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        
        
        /// Previous implementations of Vulkan made a distinction between instance and device specific validation layers, but this is no longer the case. That means that the
        /// enabledLayerCount and ppEnabledLayerNames fields of VkDeviceCreateInfo
        /// are ignored by up-to-date implementations. However, it is still a good idea to
        /// set them anyway to be compatible with older implementations:
    
        createInfo.enabledExtensionCount = 0;
        
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }
        
        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device");
        }
    }
    
    //  the glfwCreateWindowSurface function performs the surface creation very well with different implementation for each platform
    void createSurface() {
        
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface");
        }
        
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
