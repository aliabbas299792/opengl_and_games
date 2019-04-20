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

//camera setup - needed outside of the int main() function because I need to pass the data from the callback functions to my camera object
//need to set the window width/height first
const int width = 1000; 
const int height = 1000;

bool firstMouse = true; //basically needed so that the view doesn't randomly jump
//then create my camera object
Camera *camera = new Camera(width, height, 45.0f); //the first 2 params are obviously window width and height, the third is the initial fov

Player* player = new Player(camera); //the player is initialised

bool Platforms::onPlatform = false;
Model* Platforms::platform = NULL;

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
	glfwSetScrollCallback(window, scroll_callback_zoom); //scroll callback
	glfwSetKeyCallback(window, key_callback); //key callback

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //ensures the cursor doesn't leave the window, when the window is in focus, and that it is hidden

	//*************************************//
	//***********OPENGL STUFF**************//
	//*************************************//

	Shader *progShader = new Shader("glsl/vertexShader.glsl", "glsl/fragmentShader.glsl"); //constructing the shader object

	progShader->use(); //sets the program object as the current active shader object

	player->shader = progShader;
	player->window = window;

	player->player = new Model("models/sphere/sphere.obj");

	chunksHolder* megaChunk = new chunksHolder(glm::vec3(-30, -30, -30), progShader, player);

	Platforms::platform = new Model("models/platform2/platform2.obj");

	glEnable(GL_DEPTH_TEST); //basically makes sure that you can't see through objects, by rendering them in the correct order

	float timePrev = glfwGetTime();

	//the main loop
	while(!glfwWindowShouldClose(window)){
		//manually capping the framerate to 100fps, but it's irrespective of time as delta time is implemented
		if (glfwGetTime() >= timePrev + 0.01) {
			timePrev = glfwGetTime();
		} else {
			continue;
		}

		processInput(window); //check if the escape key has been pressed

		progShader->use(); //sets the program object as the current active shader object
		camera->liveUpdate(); //does all the stuff that needs to be done regularly in the main loop, in this function

		progShader->setMatrix4("view", camera->view); //gives the vertex shader the view matrix to transform the points appropriately
		progShader->setMatrix4("projection", camera->projection); //obviously the projection matrix is also used, along with the model matrix, all in one

		//for spotlights, it is different, in this case I will demonstrate with a flashlight
		progShader->setVec3_v2("light.position", glm::vec3(2,2,2));

		progShader->set3Float("light.ambient",  0.1f, 0.05f, 0.1f);
		progShader->set3Float("light.diffuse",  0.7f, 0.7f, 0.7f);
		progShader->set3Float("light.specular", 2.0f, 2.0f, 2.0f);
		progShader->setFloat("light.constant", 1.0f);
		progShader->setFloat("light.linear", 0.09f);
		progShader->setFloat("light.quadratic", 0.032f);

		glClearColor(0.0f,0.0f,0.0f,1.0f); //makes the entire screen this colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears the colour buffer, to allow the colour from the above function to be displayed, and depth buffer
		
		Platforms::onPlatform = false;

		megaChunk->liveChunks();

		player->liveUpdate(progShader);
		player->onPlatform = Platforms::onPlatform;

		glfwSwapBuffers(window); //uses the double buffer thing, where the back buffer is drawn to and then swapped with the front one to prevent flickering
		glfwPollEvents(); //checks for events and allows things such as the framebuffer_size_callback functions to be called once an event has been detected
	}

	//exit properly, first destroys all remaining windows, frees any allocated resources, and sets library to uninitialised state, before actually returning 0
	//and call destructors for objects
	player->~Player();
	progShader->~Shader();
	camera->~Camera();

	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos){ //the callback function for the mouse events, only needed to pass on variables to my object
	camera->mouse_callback(xPos, yPos, firstMouse);
}  

void scroll_callback_zoom(GLFWwindow* window, double xOffset, double yOffset){ //the callback function for the mouse scroll events, only needed to pass on variables to my object
	camera->scroll_callback_zoom(xOffset, yOffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) //callback needed to send the states of keys to the player class more accurately
{
	glm::vec4 keyStates(0);
	keyStates.x = glfwGetKey(window, GLFW_KEY_W);
	keyStates.y = glfwGetKey(window, GLFW_KEY_A);
	keyStates.z = glfwGetKey(window, GLFW_KEY_S);
	keyStates.w = glfwGetKey(window, GLFW_KEY_D);

	player->playerMovement(keyStates);
}