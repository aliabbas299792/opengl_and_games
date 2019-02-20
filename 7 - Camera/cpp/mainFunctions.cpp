#include "../header/functionDefines.h"
#include <iostream>

//for printing vec3 and stuff
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height); //resizes the window
}

void processInput(GLFWwindow* window, glm::vec3 &cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ //checks if the escape key has been pressed for the specified window
        glfwSetWindowShouldClose(window, true); //sets the should close thing to true, for the specified window
    }

    //using static floats means that it is initialised once and then not reinitialised every time, instead it stays the same, hence static
    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;
    
    float currentFrame = glfwGetTime(); //current time

    deltaTime = currentFrame - lastFrame; //gets the difference between the past 2 frames
    lastFrame = currentFrame; //sets the time of the last frame

    float cameraSpeed = 2.5f * deltaTime; //deltaTime, being the difference between the time of 2 frames, essentially scales the speed to be the same all the time
    //so it ends up moving the same amount over a given time period, regardless of the frame rate
    //so it doesn't let faster computers allow it to move faster etc

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cameraPos += cameraSpeed * cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cameraPos -= cameraSpeed * cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; //does the cross product of front and up to get right
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; //does the cross product of front and up to get right
    }
}