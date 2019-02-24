#ifndef FUNCTION_DEFINES
#define FUNCTION_DEFINES
#include <GLFW/glfw3.h> //the framebuffer_size_callback needs GLFWwindow in the prototype

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //prototype function for resizing function
void processInput(GLFWwindow* window); //for processing input, such as the escape key, to close the window

unsigned int loadShaders(const char* v_file_path, const char* f_file_path); //loading the vertex and fragment shaders respectively
#endif