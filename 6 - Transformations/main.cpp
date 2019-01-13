#include <glad/glad.c>
#include <glad/glad.h>
#include <iostream>
#include <math.h>

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

	//4 vertices for a rectangle, i.e 2 triangles

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	
	//element indices array
	unsigned int indices[] = {
		3, 2, 1, //first triangle
		3, 0, 1 //second triangle
	};

	/* The commented out code below is for drawing a cubic graph, so long as the glDrawElements()'s first param is GL_POINTS
	float xCoord = -100; //the starting x coord

	float vertices[1700] = { 0 };
	unsigned int indices[200] = { 0 };

	for(int i = 0; i <= 1700; i++){
		vertices[i] = 0;
	}

	for(int i = 0; i < 200; i++){
		indices[i] = 0;
	}

	for(int i = 0;i <= 200;i++){
		vertices[i*8] = xCoord/100;
		vertices[(i*8)+1] = (pow(xCoord,3))/1000000;
		xCoord++;
		
		indices[i] = i;
	
		//std::cout << vertices[i*8] << " - " << vertices[(i*8)+1] << " - " << vertices[(i*8)+2] << " - " << vertices[(i*8)+3] << " - " << vertices[(i*8)+4] << " - " << vertices[(i*8)+5] << " - " << vertices[(i*8)+6] << " - " << vertices[(i*8)+7] << std::endl;
	}
	*/

	unsigned int EBO; //EBO id
	glGenBuffers(1, &EBO); //generate EBO buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the buffer array data type to the EBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //load the indices into the EBO buffer object

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

	//position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//1st param is the index of the attribute in the vertexShader GLSL, must match layout in shader
	//2nd param is the number of components per vertex attribute
	//3rd param is the data type of each component in the array
	//4th param is whether or not the data should be normalised, i.e clamped to the -1 to +1 range, and 0 to 1 for unsigned values
	//5th param is the 'stride', which is the byte offset between consecutive verticies in the array, assuming they are tightly packed 
		//->> (directly consecutive), so 0 normally, however if it is not using more than just vertex position, make it (index) * (sizeof(datatype))
		//if say the indicies are [ x y z r g b ], so 6 elements, the r of element 1 and element 2 is 6 away, if it uses 9 indicies, r and 2nd r are 9 apart etc
		//and we have added colour, so it is now 8 elements
	//6th param is the 'pointer', which is the offset of the first component of the array
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
	unsigned int textureID_2 = loadTexture("images/2.jpg"); 
	unsigned int textureID_3 = loadTexture("images/3.jpg"); 
	unsigned int textureID_4 = loadTexture("images/4.png"); 
	unsigned int textureID_5 = loadTexture("images/5.png"); 
	unsigned int textureID_6 = loadTexture("images/6.jpg"); 
	unsigned int textureID_7 = loadTexture("images/7.jpg"); 
	unsigned int textureID_8 = loadTexture("images/8.jpg"); 
	
	//shaders
	Shader *progShader = new Shader("glsl/vertexShader.glsl", "glsl/fragmentShader.glsl"); //constructing the shader object	
	progShader->use(); //sets the program object as the current active shader object

	//uniforms
	progShader->setFloat("offsetX", 0.5f); //sets the uniform for the current active shader program object
	progShader->setInt("rectTexture", 0); //sets the uniform of the specified name, for the sampler2D, which will have a texture bound using GL_TEXTURE1
	progShader->setInt("rectTexture2", 1); //sets the uniform of the specified name, for the sampler2D, which will have a texture bound using GL_TEXTURE2
	progShader->setInt("rectTexture3", 2); //sets the uniform of the specified name, for the sampler2D, which will have a texture bound using GL_TEXTURE3
	progShader->setInt("rectTexture4", 3); //sets the uniform of the specified name, for the sampler2D, which will have a texture bound using GL_TEXTURE4
	progShader->setInt("rectTexture5", 4); //sets the uniform of the specified name, for the sampler2D, which will have a texture bound using GL_TEXTURE5
	progShader->setInt("rectTexture6", 5); //sets the uniform of the specified name, for the sampler2D, which will have a texture bound using GL_TEXTURE6
	progShader->setInt("rectTexture7", 6); //sets the uniform of the specified name, for the sampler2D, which will have a texture bound using GL_TEXTURE7
	progShader->setInt("rectTexture8", 7); //sets the uniform of the specified name, for the sampler2D, which will have a texture bound using GL_TEXTURE8
	//basically a value of 0 up there corresponds to GL_TEXTURE1, a value of 1 corresponds to GL_TEXTURE2 etc

	float mixStrength = 0.5; //for the uniform mixStrength
	progShader->setInt("mixStrength", mixStrength); //initially sets that uniform to 0.5

	//the main loop
	while(!glfwWindowShouldClose(window)){
		processInput(window); //check if the escape key has been pressed
		
		//ensures the mixStrength uniform is within the boundaries of 0 and 1
		if(mixStrength > 1){
			mixStrength = 1;
		}else if(mixStrength < 0){
			mixStrength = 0;
		}

		glClearColor(0.2f,0.2f,0.25f,1.0f); //makes the entire screen this colour
		glClear(GL_COLOR_BUFFER_BIT); //clears the colour buffer, to allow the colour from the above function to be displayed

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the EBO object to use the indices

		//sets the mixStrength to whatever (increments/decrements of 0.01), when pressing the up/down buttons, respectively
		if(mixStrength >= 0 && mixStrength <= 1){
			if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
				progShader->setFloat("mixStrength", (mixStrength+=0.01));
			}else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
				progShader->setFloat("mixStrength", (mixStrength-=0.01));
			}
		}

		//by the way GL_TEXTURE1 is equal to GL_TEXTURE0 + 1, GL_TEXTURE2 is euqal to GL_TEXTURE0 + 2 etc
		glBindTexture(GL_TEXTURE_2D, textureID_1); //binds the texture to the sampler2D with the name rextTexture (first one (0) as specified in uniforms section)
		glActiveTexture(GL_TEXTURE1); //sets the 2nd texture location as this
		glBindTexture(GL_TEXTURE_2D, textureID_2); //binds the texture to the sampler2D with the name rextTexture2 (second one (1) as specified in uniforms section)
		glActiveTexture(GL_TEXTURE2); //this sets the location in the fragment shader for textures, so this is the first and is activated by default
		glBindTexture(GL_TEXTURE_2D, textureID_3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureID_4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureID_5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, textureID_6);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, textureID_7);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, textureID_8);
		glActiveTexture(GL_TEXTURE0 + 8); //equal to GL_TEXTURE0 + 8

		glBindVertexArray(VAO); //need this as it contains the VBO configuration
		glDrawElements(GL_TRIANGLES, sizeof(vertices)/sizeof(float), GL_UNSIGNED_INT, (void*)0); //used for indexed drawing, i.e using an EBO
		//1st param draw mode, 2nd param number of vertices, 3rd param type of the indices, 4th param is basically the index at which to start from, but can only be 0
		//however note that 2nd param is still number of vertices, because it is technically still drawing just vertices using indices as a 'guide' of sorts

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //this essentially allows viewing in wireframe mode

		glfwSwapBuffers(window); //uses the double buffer thing, where the back buffer is drawn to and then swapped with the front one to prevent flickering
		glfwPollEvents(); //checks for events and allows things such as the framebuffer_size_callback functions to be called once an event has been detected
	}

	//exit properly, first destroys all remaining windows, frees any allocated resources, and sets library to uninitialised state, before actually returning 0
	glfwTerminate();
	return 0;
}