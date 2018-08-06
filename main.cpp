#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <SFML/OpenGL.hpp>

#include "common/loadShaders.hpp"
#include "common/structs.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main()
{
	const unsigned int width = 1920;
	const unsigned int height = 1080;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8; //anti aliasting level 8


	sf::RenderWindow window(sf::VideoMode(width, height), "OpenGL Window", sf::Style::Fullscreen, settings);
	window.setVerticalSyncEnabled(true);

	//activate window
	window.setActive(true);


	glewInit(); //initialises OpenGL for use

	//Making the vectors and triangle shape
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID); //returns (1) vertex array object name
	glBindVertexArray(VertexArrayID); //binds the vertex array object to the vertexarrayid

	
	/*
	/////Shaders
	*/
	GLuint programID = LoadShaders("shaders/vertexshader.glsl", "shaders/fragmentshader.glsl"); //loads shaders

	bool running = true;


	for (int v = 0; v < 36; v++) {
		g_color_buffer_data[v * 3 + 0] = 0.5;
		g_color_buffer_data[v * 3 + 1] = 0.4;
		g_color_buffer_data[v * 3 + 2] = 0.6;
	}


	/**BEGIN ROTATE**/

	glm::mat4 transRotate(1.0f);

	/**END ROTATE**/


	while (running)
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears frame


		transRotate = glm::rotate(transRotate, glm::radians(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));


		sf::Event event;

		if (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				running = false; //closes program if window closed
			}
			else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		/**MVP BEGIN**/
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

		//glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordinates, ortho basically doesn't have depth

		glm::mat4 View = glm::lookAt(
			glm::vec3(4, 2, 5), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		glm::mat4 Model = glm::mat4(1.0f) * transRotate;

		glm::mat4 mvp = Projection * View * Model;

		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		GLuint MatrixID = glGetUniformLocation(programID, "MVP");

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);


		/**MVP END**/

		// This will identify our vertex buffer
		GLuint vertexbuffer;
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		//will bind vertexbuffer to the target GL_ARRAY_BUFFER, basically telling the buffer we're going to use the object as vertex attribute data (description of verticies)

		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
		//will make a new data store for the object bound to the target (GL_ARRAY_BUFFER), and the final param is saying what it's going to be for (usage)

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);//when enabled will be used for rendering methods like glDrawArrays

		glVertexAttribPointer( //specifies what index the vertexattrib array is at for use while rendering
			0,                  // the index
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride (i.e how tightly packed are the values in the array, byte offset between consecutive generic vertex attributes)
			(void*)0            // array buffer offset, pointer to first generic vertex attribute in the array
		);


		/******COLOUR FOR FRAGMENT SHADER START*******/

		//color buffer

		randomStructColourGen();

		GLuint colorbuffer;
		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


		// 2nd attribute buffer : colors

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		/******COLOUR FOR FRAGMENT SHADER END*******/

		//use the shader, notes in shader.hpp
		glUseProgram(programID);

		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		//1st param -> what kind of primitives to be constructed, and how they are constructed
		//2nd param -> starting index in the enabled arrays
		//3rd param -> the number of defined verticies in the point you want to be rendered (starting from the first)

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		window.display();
	}

	return 0;
}