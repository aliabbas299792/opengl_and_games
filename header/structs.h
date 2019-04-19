#ifndef STRUCTS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vector>

//vertex struct
struct Vertex {
	glm::vec3 Pos;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

//texture struct
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

#endif // !STRUCTS