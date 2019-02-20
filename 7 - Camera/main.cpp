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

int main(){
	//*************************************//
	//***********GLFW STUFF****************//
	//*************************************//

	const int width = 900;
	const int height = 900;

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

	//*************************************//
	//***********OPENGL STUFF**************//
	//*************************************//

	//camera stuff
	/*	This is the manual way of doing the stuff, use glm::lookAt() instead  */
	/*
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //camera position
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //the camera target
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); //this makes the vector of cameraPos to cameraTarget into a direction vector (normalises it)
	//we are giving the direction of the target to the camera, so pointing towards the camera
	//as we are going to move everything in the scene relative to the camera
	//the vector will actually point towards the positive z axis, in other words, towards the viewer - out of the screen
	
	//to get the positive x axis of the camera, you need use cross product (as there are 2 possible positive x axis, left and right)
	//so make a vector for up, and do cross product with the one pointing towards positive z axis
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); //the up vector for world space
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight); //crosses the two direction vectors to get the positive y direction vector for the camera

	//using these variables, and a piece of code towards the beginning of the while loop (I'll label it #333), I will make the camera rotate around a point (x and z axis)
	float radius = 10.0f; //the circle path it will take will be of radius 10
	float camX = 0;
	float camZ = 0;

	glm::mat4 view; //the actual view matrix that will be changed during the while loop
	*/

	glm::mat4 view = glm::mat4(1.0f);

	glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

	//4 vertices for a rectangle, i.e 2 triangles
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
		0.5f,  0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   //---------- top right
		0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	
	//element indices array
	unsigned int indices[] = {
		3, 2, 1, //first triangle
		3, 0, 1, //second triangle
		0, 4, 5,
		1, 0, 5,
		7, 6, 5,
		7, 4, 5,
		0, 4, 7,
		0, 3, 7,
		1, 5, 6,
		1, 2, 6,
		3, 2, 6,
		3, 7, 6

	};

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f); //empty transformation matrix
	
	vec = trans * vec; //apply the transformation above to this vec

	unsigned int EBO; //EBO id
	glGenBuffers(1, &EBO); //generate EBO buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the buffer array data type to the EBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //load the indices into the EBO buffer object

	//setup VAO, VBO and vertex atrrib stuff
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO); //generate 1 vertex array object
	glGenBuffers(1, &VBO); //generate 1 buffer object

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //enables a generic vertex attribute, for index 0, as in the above function, can be disabled via glDisableVertexAttribArray(0)

	//colour attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float))); //there are now 6 floats before it, so specify and all that
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //calls the correct VBO using GL_ARRAY_BUFFER, and safely unbinds it
	
	glBindVertexArray(0); //unbind the VAO safely

	//textures
	//the below use the texture loading function to load a texture and return its id
	unsigned int textureID_1 = loadTexture("images/1.png"); 

	glm::vec3 cubePositions[] = { //positions for scattered cubes
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};
	
	//shaders
	Shader *progShader = new Shader("glsl/vertexShader.glsl", "glsl/fragmentShader.glsl"); //constructing the shader object	
	progShader->use(); //sets the program object as the current active shader object

	//uniforms
	progShader->setFloat("offsetX", 0.5f); //sets the uniform for the current active shader program object
	progShader->setInt("rectTexture", 0); //sets the uniform of the specified name, for the sampler2D, which will have a texture bound using GL_TEXTURE1
	//basically a value of 0 up there corresponds to GL_TEXTURE1, a value of 1 corresponds to GL_TEXTURE2 etc
	
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 100.0f);
	//projection matrix, i.e transforms all of the coords to clip space and applies the perspective division
	//the first param: the FoV, 2nd: aspect ratio, 3rd: distance of near plane, anything closer than this is not drawn, 4th: far plane, anything past it is not drawn

	progShader->setMatrix4("projection", projection);

	glEnable(GL_DEPTH_TEST);


	//the main loop
	while(!glfwWindowShouldClose(window)){
		processInput(window, cameraPos, cameraFront, cameraUp); //check if the escape key has been pressed

		//the piece of code labelled #333
		/*
		//sin and cos are 90 deg, or 1/2 pi radians apart, so the peak of the cos occurs when sin is at 0, so as they oscillate on x and z in sync, they make a circle path
		camX = sin(glfwGetTime()) * radius;
		camZ = cos(glfwGetTime()) * radius;

		//give the view matrix the camZ and camX
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), 
						glm::vec3(0.0, 0.0, 0.0), 
						glm::vec3(0.0, 1.0, 0.0f)
		);
		*/
		view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
		//the first param is the position of the camera, and the second is the target, the third is just the up vector
		//the 2nd param has the cameraPos vector added to it, as we want the target to be constantly in front of the camera, so cameraPos + cameraFront

		progShader->setMatrix4("view", view); //gives the vertex shader the view matrix to transform the points appropriately
		progShader->setMatrix4("projection", projection);
		//obviously the projection matrix is also used, along with the model matrix, all in one

		glClearColor(0.2f,0.2f,0.25f,1.0f); //makes the entire screen this colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears the colour buffer, to allow the colour from the above function to be displayed, and depth buffer

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the EBO object to use the indices

		//by the way GL_TEXTURE1 is equal to GL_TEXTURE0 + 1, GL_TEXTURE2 is euqal to GL_TEXTURE0 + 2 etc
		glBindTexture(GL_TEXTURE_2D, textureID_1); //binds the texture to the sampler2D with the name rextTexture (first one (0) as specified in uniforms section)
		glActiveTexture(GL_TEXTURE1); //sets the 2nd texture location as this

		glBindVertexArray(VAO); //need this as it contains the VBO configuration

		float scaleFactor = (sin(glfwGetTime())*0.3) + 0.5; //repeatedly scales
		//float scaleFactor = glfwGetTime(); //just gets bigger and bigger with time

		for(int i = 0;i < 10; i++){ //draws 10 cubes, all with slightly different positions, and the same transformations applied to them
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			progShader->setMatrix4("model", model);

			//the current matrix transform would be applied here
			glDrawElements(GL_TRIANGLES, sizeof(vertices)/sizeof(float), GL_UNSIGNED_INT, (void*)0); //used for indexed drawing, i.e using an EBO
		}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //this essentially allows viewing in wireframe mode

		glfwSwapBuffers(window); //uses the double buffer thing, where the back buffer is drawn to and then swapped with the front one to prevent flickering
		glfwPollEvents(); //checks for events and allows things such as the framebuffer_size_callback functions to be called once an event has been detected
	}

	//exit properly, first destroys all remaining windows, frees any allocated resources, and sets library to uninitialised state, before actually returning 0
	glfwTerminate();
	return 0;
}