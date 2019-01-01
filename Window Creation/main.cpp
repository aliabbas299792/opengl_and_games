#include <glad/glad.h>
#include <glad/glad.c>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <vector>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //for resize function
void processInput(GLFWwindow *window);

int main() {
	//////////////////////////////////////////////////////
	//////////--------------------------------////////////
	//////////------Window initialisation-----////////////
	//////////--------------------------------////////////
	//////////////////////////////////////////////////////

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create window." << std::endl;

		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		system("PAUSE");
		return -1;
		//if glad does initialise
	}


	//////////////////////////////////////////////////////
	//////////--------------------------------////////////
	//////////------Actual OpenGL Stuff-------////////////
	//////////--------------------------------////////////
	//////////////////////////////////////////////////////

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	unsigned int VBO; //vertex buffer object id
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{

		processInput(window);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
