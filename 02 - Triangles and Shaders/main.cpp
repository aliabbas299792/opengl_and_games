#include <glad/glad.h>
#include <iostream>

#include "header/functionDefines.h"

int main(){
	//*************************************//
	//***********GLFW STUFF****************//
	//*************************************//

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

	//*************************************//
	//***********OPENGL STUFF**************//
	//*************************************//

	//3 vertices for a triangle
	float vertices[] = {
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	//setup VAO, VBO and vertex atrrib stuff
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO); //generate 1 vertex array object
	glGenBuffers(1, &VBO); //generate 1 buffer object

	glBindVertexArray(VAO); //bind the vertex array object
	//the fact that it is currently bound is why you don't need to reference it again in functions later, until you unbind it
	//the VAO stores vertex attribute configurations via glVertexAttribPointer, calls to glEnableVertexAttribArray or glDisableVertexAttribArray
	     //->> and vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //this will bind the buffer type to this buffer object, making it a vertex buffer object
	//as the type of buffer of a vertex buffer object is always GL_VERTEX_ARRAY
	//the same as for the VAO applies for the binding stuff

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//1st param specifies the type of buffer object which is currently bound
	//2nd param is the size of the data you want to put in there
	//3rd param is the actual data you want to put in there
	//4th param is how we want the graphics processing unit to process the data, GL_STATIC_DRAW is for data that will not change that much
	      //->> GL_DYNAMIC_DRAW is for data that changes a lot, GL_STREAM_DRAW is for data that will change every time it is drawn

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//1st param is the index of the attribute in the vertexShader GLSL, must match layout in shader
	//2nd param is the number of components per vertex attribute
	//3rd param is the data type of each component in the array
	//4th param is whether or not the data should be normalised, i.e clamped to the -1 to +1 range, and 0 to 1 for unsigned values
	//5th param is the 'stride', which is the byte offset between consecutive verticies in the array, assuming they are tightly packed (directly consecutive), so 0 normally
	//6th param is the 'pointer', which is the offset of the first component of the array
	glEnableVertexAttribArray(0); //enables a generic vertex attribute, for index 0, as in the above function, can be disabled via glDisableVertexAttribArray(0)
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); //calls the correct VBO using GL_ARRAY_BUFFER, and safely unbinds it
	
	glBindVertexArray(0); //unbind the VAO safely
	
	//shaders
	unsigned int programID = loadShaders("glsl/vertexShader.glsl", "glsl/fragmentShader.glsl");

	//the main loop
	while(!glfwWindowShouldClose(window)){
		processInput(window); //check if the escape key has been pressed

		glClearColor(0.2f,0.2f,0.2f,1.0f); //makes the entire screen this colour
		glClear(GL_COLOR_BUFFER_BIT); //clears the colour buffer, to allow the colour from the above function to be displayed

		glUseProgram(programID); //sets the program object as the current active shader object
		glBindVertexArray(VAO); //need this as it contains the VBO configuration
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float)); //draws the vertices found in the currently bound VBO using the pattern in the first enum, 2nd param is starting pos
			//->> 3rd param is the size of the VBO/number of vertices

		glfwSwapBuffers(window); //uses the double buffer thing, where the back buffer is drawn to and then swapped with the front one to prevent flickering
		glfwPollEvents(); //checks for events and allows things such as the framebuffer_size_callback functions to be called once an event has been detected
	}

	//exit properly, first destroys all remaining windows, frees any allocated resources, and sets library to uninitialised state, before actually returning 0
	glfwTerminate();
	return 0;
}