#include <glad/glad.c>
#include <glad/glad.h>
#include <iostream>
#include <math.h>

//the glm headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//for printing vec3 and stuff
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "header/functionDefines.h"
#include "header/classDefines.h"
#include "header/modelStuff.h"

//camera setup - needed outside of the int main() function because I need to pass the data from the callback functions to my camera object
//need to set the window width/height first
const int width = 1000; 
const int height = 1000;
//then create my camera object
Camera *camera = new Camera(width, height, 45.0f); //the first 2 params are obviously window width and height, the third is the initial fov

int main(){
	//*************************************//
	//***********GLFW STUFF****************//
	//*************************************//

	//glfw initialisation
	glfwInit();
	//below 2 are saying requires OpenGL 3.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//below says the core profile is needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//below is a method to enable 8x MSAA in the application
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow* window = glfwCreateWindow(width, height, "Window #1", NULL, NULL); //window creation

	camera->window = window; //here we give the camera object a pointer to the window object, for it to use internally later

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

	glViewport(0, 0, width, height); //this is where everything is rendered inside of, params are: top x, top y, width, height respectively

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //call the function specified, when the specified window is resized

	glfwSetCursorPosCallback(window, mouse_callback); //sets this function whenever a mouse movement even occurs
	glfwSetScrollCallback(window, scroll_callback_zoom);

	Model test("models/nanosuit/nanosuit.obj");

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //ensures the cursor doesn't leave the window, when the window is in focus, and that it is hidden

	//*************************************//
	//***********OPENGL STUFF**************//
	//*************************************//

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f); //empty transformation matrix
	
	vec = trans * vec; //apply the transformation above to this vec

	//light VAO - the cube shaped lamp
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 10.0f); //the position of the light, we will transform one of the cubes and draw them separately using this

	Shader *progShader = new Shader("glsl/vertexShader.glsl", "glsl/fragmentShader.glsl"); //constructing the shader object	

	progShader->use(); //sets the program object as the current active shader object
	
	//uniforms
	progShader->setFloat("offsetX", 0.5f); //sets the uniform for the current active shader program object
	progShader->setInt("material.diffuse", 0); //sets the uniform of the specified name, for the sampler2D in the material struct in the fragment shader, which will have a texture bound using GL_TEXTURE1
	progShader->setInt("material.specular", 1); //sets the specular map uniform
	progShader->setInt("material.emission", 2); //sets the emission map uniform
	//basically a value of 0 up there corresponds to GL_TEXTURE1, a value of 1 corresponds to GL_TEXTURE2 etc

	//progShader->set3Float("objectColour", 1.0f, 1.0f, 1.0f); //can give the object(s) a certain tint, when it's used anyway
	progShader->set3Float("lightColour", 1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST); //basically makes sure that you can't see through objects, by rendering them in the correct order

	//the main loop
	while(!glfwWindowShouldClose(window)){
		processInput(window); //check if the escape key has been pressed

		progShader->use(); //sets the program object as the current active shader object
		camera->liveUpdate(); //does all the stuff that needs to be done regularly in the main loop, in this function

		progShader->setFloat("time", glfwGetTime()); //sends the time to the fragment shader, so I can cause the emission map to move

		progShader->setMatrix4("view", camera->view); //gives the vertex shader the view matrix to transform the points appropriately
		progShader->setMatrix4("projection", camera->projection); //obviously the projection matrix is also used, along with the model matrix, all in one

		//for spotlights, it is different, in this case I will demonstrate with a flashlight
		progShader->setVec3_v2("torch.position", camera->cameraPos);
		progShader->setVec3_v2("torch.direction", camera->cameraFront);
		progShader->setFloat("torch.cutOff", cos(glm::radians(10.0f))); //will pas the cos value of 15 degrees to the shader, rather than passing
		//the actual value of 15 degrees, as, the dot product of the light-direction vector, and the incident light ray vector would return the cos of some value,
		//and to get the angle you need to do inverse cos of that, which is an expensive operation, so we instead pas the cos of the angle we're comparing

		progShader->set3Float("torch.ambient",  0.2f, 0.2f, 0.2f);
		progShader->set3Float("torch.diffuse",  0.5f, 0.5f, 0.5f);
		progShader->set3Float("torch.specular", 2.0f, 2.0f, 2.0f);
		progShader->setFloat("torch.constant", 1.0f);
		progShader->setFloat("torch.linear", 0.07f);
		progShader->setFloat("torch.quadratic", 0.017f);

		progShader->setVec3("cameraPos", camera->cameraPos); //need to pass it to the shader constantly

		glClearColor(0.0f,0.0f,0.0f,1.0f); //makes the entire screen this colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears the colour buffer, to allow the colour from the above function to be displayed, and depth buffer

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		progShader->setMatrix4("model", model);
		test.Draw(progShader);

		glfwSwapBuffers(window); //uses the double buffer thing, where the back buffer is drawn to and then swapped with the front one to prevent flickering
		glfwPollEvents(); //checks for events and allows things such as the framebuffer_size_callback functions to be called once an event has been detected
	}

	//exit properly, first destroys all remaining windows, frees any allocated resources, and sets library to uninitialised state, before actually returning 0
	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos){ //the callback function for the mouse events, only needed to pass on variables to my object
    camera->mouse_callback(xPos, yPos);
}  

void scroll_callback_zoom(GLFWwindow* window, double xOffset, double yOffset){ //the callback function for the mouse scroll events, only needed to pass on variables to my object
  camera->scroll_callback_zoom(xOffset, yOffset);
}