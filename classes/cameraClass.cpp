#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../header/classDefines.h"

Camera::Camera(int x, int y, float z){
    //sets the internal window width and height variables
    windowWidth = x;
    windowHeight = y;

    fov = z; //sets the internal fov variable
    projection = glm::perspective(glm::radians(z), (float)windowWidth/windowHeight, 0.1f, 100.0f);
    //projection matrix, i.e transforms all of the coords to clip space and applies the perspective division
	//the first param: the FoV, 2nd: aspect ratio, 3rd: distance of near plane, anything closer than this is not drawn, 4th: far plane, anything past it is not drawn

}

void Camera::keyboard_movement(){
    static float deltaTime = 0;
    static float lastFrame = 0;
    //the variables are static, so initialised once, and then can use them as if they are not reset on each call - they are static

    float currentFrame = glfwGetTime(); //current time

    deltaTime = currentFrame - lastFrame; //gets the difference between the past 2 frames
    lastFrame = currentFrame; //sets the time of the last frame

    float cameraSpeed = 2.5f * deltaTime; //deltaTime, being the difference between the time of 2 frames, essentially scales the speed to be the same all the time
    //so it ends up moving the same amount over a given time period, regardless of the frame rate
    //so it doesn't let faster computers allow it to move faster etc

    //basic movement in forward x, backwards x, forward z, backwards z

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

void Camera::mouse_callback(double xpos, double ypos, bool &firstMouse){ //the function we pass on the processing for the callback to
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->cameraFront = glm::normalize(front);
}  

void Camera::scroll_callback_zoom(double xOffset, double yOffset){ //the response to the callback is processed here
    //the yOffset increases/decreases based on the mouse wheel, and that is used to increase/decrease fov, which essentially acts as zooming in/out
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yOffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}

void Camera::liveUpdate(){
    this->keyboard_movement(); //references the keyboard_movement() function for this instance of the object
    
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    projection = glm::perspective(glm::radians(fov), (float)windowWidth/windowWidth, 0.1f, 100.0f);
	//the first param is the position of the camera, and the second is the target, the third is just the up vector
	//the 2nd param has the cameraPos vector added to it, as we want the target to be constantly in front of the camera, so cameraPos + cameraFront
}