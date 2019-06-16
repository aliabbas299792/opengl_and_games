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
    projection = glm::perspective(glm::radians(z), (float)(windowWidth/windowHeight), 0.1f, 100.0f);
    //projection matrix, i.e transforms all of the coords to clip space and applies the perspective division
	//the first param: the FoV, 2nd: aspect ratio, 3rd: distance of near plane, anything closer than this is not drawn, 4th: far plane, anything past it is not drawn
	bool temp = 0;
	this->mouse_callback(0, 0, temp);

}

void Camera::mouse_callback(double xpos, double ypos, bool &firstMouse){ //the function we pass on the processing for the callback to
	ypos -= 100; //ensure that the ypos is centered on the player at the beginning

	if (firstMouse) { //if this is the first mouse movement, do those things (as it prevents a sudden jump in the view)
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

	// limit mouse movement
	if (pitch > 20.0f)
		pitch = 20.0f;
	if (pitch < -40.0f)
		pitch = -40.0f;
	if (yaw > -50.0f)
		yaw = -50.0f;
	if (yaw < -130.0f)
		yaw = -130.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->cameraFront = glm::normalize(front);
}  

void Camera::scroll_callback_zoom(double xOffset, double yOffset){ //the response to the callback is processed here
    //the yOffset increases/decreases based on the mouse wheel, and that is used to increase/decrease fov, which essentially acts as zooming in/out
    if(fov >= 1.0f && fov <= 90.0f)
        fov -= yOffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 90.0f)
        fov = 90.0f;
}

void Camera::liveUpdate(){
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
	//the first param is the position of the camera, and the second is the target, the third is just the up vector
	//the 2nd param has the cameraPos vector added to it, as we want the target to be constantly in front of the camera, so cameraPos + cameraFront
    projection = glm::perspective(glm::radians(fov), (float)windowWidth/windowHeight, 0.1f, 100.0f);
}