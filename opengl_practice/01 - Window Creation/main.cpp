#include <glad/glad.h>
#include <glad/glad.c>
#include <GLFW/glfw3.h>
#include <iostream>

#include "header/functionDefines.h"

int main(){
	//glfw initialisation
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(900, 700, "Window #1", NULL, NULL); //window creation

	if(window == NULL){ //if creation failed
		std::cout << "Window creation failed." << std::endl;
		glfwTerminate(); //end
		return -1; //ended in failure
	}

	//otherwise we can continue processing
	glfwMakeContextCurrent(window); 
	//above makes the context of the window (so all of the states associated with the instance of OpenGL), current on the thread it was called from

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ //initialises glad, and if the initialiser returns false, exit
		std::cout << "Failed to initialise GLAD." << std::endl;
		return -1; //ended in failue
	}

	glViewport(0, 0, 900, 700); //this is where everything is rendered inside of, params are: top x, top y, width, height respectively

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //call the function specified, when the specified window is resized

	//the main loop
	while(!glfwWindowShouldClose(window)){
		processInput(window); //check if the escape key has been pressed

		glClearColor(0.2f,0.2f,0.2f,1.0f); //makes the entire screen this colour
		glClear(GL_COLOR_BUFFER_BIT); //clears the colour buffer, to allow the colour from the above function to be displayed

		glfwSwapBuffers(window); //uses the double buffer thing, where the back buffer is drawn to and then swapped with the front one to prevent flickering
		glfwPollEvents(); //checks for events and allows things such as the framebuffer_size_callback functions to be called once an event has been detected
	}

	//exit properly, first destroys all remaining windows, frees any allocated resources, and sets library to uninitialised state, before actually returning 0
	glfwTerminate();
	return 0;
}