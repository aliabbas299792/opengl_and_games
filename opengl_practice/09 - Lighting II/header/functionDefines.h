#ifndef FUNCTION_DEFINES
#define FUNCTION_DEFINES
#include <GLFW/glfw3.h> //the framebuffer_size_callback needs GLFWwindow in the prototype
#include <glm/glm.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //prototype function for resizing function
void processInput(GLFWwindow* window); //for processing input, such as the escape key, to close the window
//and for moving the camera
unsigned int loadTexture(const char* file); //for loading textures

//the callback functions, which are defined in main.cpp
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback_zoom(GLFWwindow* window, double xOffset, double yOffset);

#endif