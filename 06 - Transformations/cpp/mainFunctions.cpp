#include "../header/functionDefines.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height); //resizes the window
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ //checks if the escape key has been pressed for the specified window
        glfwSetWindowShouldClose(window, true); //sets the should close thing to true, for the specified window
    }
}