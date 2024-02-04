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
