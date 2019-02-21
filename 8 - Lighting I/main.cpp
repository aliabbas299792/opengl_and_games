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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //ensures the cursor doesn't leave the window, when the window is in focus, and that it is hidden

	//*************************************//
	//***********OPENGL STUFF**************//
	//*************************************//

	//4 vertices for a rectangle, i.e 2 triangles
	//not using an EBO for now, as I need to use the normals provided in this model
	float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f); //empty transformation matrix
	
	vec = trans * vec; //apply the transformation above to this vec

	//setup VAO, VBO and vertex atrrib stuff
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO); //generate 1 vertex array object
	glGenBuffers(1, &VBO); //generate 1 buffer object

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //enables a generic vertex attribute, for index 0, as in the above function, can be disabled via glDisableVertexAttribArray(0)

	//normal vector attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //calls the correct VBO using GL_ARRAY_BUFFER, and safely unbinds it
	
	glBindVertexArray(0); //unbind the VAO safely

	//light VAO - the cube shaped lamp
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f); //the position of the light, we will transform one of the cubes and draw them separately using this

	//textures
	//the below use the texture loading function to load a texture and return its id
	unsigned int textureID_1 = loadTexture("images/1.png"); 
	//shaders
	Shader *progShader = new Shader("glsl/vertexShader.glsl", "glsl/fragmentShader.glsl"); //constructing the shader object	

	Shader *lightShader = new Shader("glsl/vertexShader.glsl", "glsl/lightFragShader.glsl"); //the shader object specifically for the lamp

	progShader->use(); //sets the program object as the current active shader object
	
	//uniforms
	progShader->setFloat("offsetX", 0.5f); //sets the uniform for the current active shader program object
	progShader->setInt("rectTexture", 0); //sets the uniform of the specified name, for the sampler2D, which will have a texture bound using GL_TEXTURE1
	//basically a value of 0 up there corresponds to GL_TEXTURE1, a value of 1 corresponds to GL_TEXTURE2 etc

	progShader->set3Float("objectColour", 1.0f, 0.5f, 0.31f);
	progShader->set3Float("lightColour", 1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST); //basically makes sure that you can't see through objects, by rendering them in the correct order

	//the main loop
	while(!glfwWindowShouldClose(window)){
		processInput(window); //check if the escape key has been pressed

		//for making the light loop around the object
		float camX = sin(glfwGetTime()) * 10;
		float camZ = cos(glfwGetTime()) * 10;

		progShader->use(); //sets the program object as the current active shader object
		camera->liveUpdate(); //does all the stuff that needs to be done regularly in the main loop, in this function

		progShader->setMatrix4("view", camera->view); //gives the vertex shader the view matrix to transform the points appropriately
		progShader->setMatrix4("projection", camera->projection); //obviously the projection matrix is also used, along with the model matrix, all in one

		glm::vec3 temp = lightPos + glm::vec3(camX, 0.0f, camZ); //the transformation for the light itself (round in a circle)
		progShader->setVec3("lightPos", temp); //sending the transformation as a vector

		progShader->setVec3("cameraPos", camera->cameraPos); //need to pass it to the shader constantly

		glClearColor(0.2f,0.2f,0.25f,1.0f); //makes the entire screen this colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears the colour buffer, to allow the colour from the above function to be displayed, and depth buffer

		//by the way GL_TEXTURE1 is equal to GL_TEXTURE0 + 1, GL_TEXTURE2 is euqal to GL_TEXTURE0 + 2 etc
		glBindTexture(GL_TEXTURE_2D, textureID_1); //binds the texture to the sampler2D with the name rextTexture (first one (0) as specified in uniforms section)
		glActiveTexture(GL_TEXTURE1); //sets the 2nd texture location as this

		glBindVertexArray(VAO); //need this as it contains the VBO configuration

		glm::mat4 model = glm::mat4(1.0f);
		progShader->setMatrix4("model", model);

		//the current matrix transform would be applied here
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float)); //draws the vertices found in the currently bound VBO

		lightShader->use();

		lightShader->setMatrix4("view", camera->view); //gives the vertex shader the view matrix to transform the points appropriately
		lightShader->setMatrix4("projection", camera->projection); //obviously the projection matrix is also used, along with the model matrix, all in one

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f));

		model = glm::translate(model, lightPos + glm::vec3(camX, 0.0f, camZ));

		lightShader->setMatrix4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float)); //draws the vertices found in the currently bound VBO

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