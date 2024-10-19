#pragma once
#ifndef MODEL

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../external/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <iostream>
#include <vector>

#include "../header/classDefines.h"

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

class Mesh {
public:
	//variables for rendering
	unsigned int VAO, VBO, EBO;

	void setupMesh();

	//the structures holding the data for the mesh
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//the mesh methods
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader* shader);
};

//the actual model, which may contain many meshes
class Model {
public:
	Model(std::string path);
	void Draw(Shader* shader);

	std::vector<Mesh> meshes;
	std::string directory;

	//so there would be a scene, and the scene would havve nodes, and the nodes may have meshes
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName); //loaded materials stored here
	std::vector<Texture> textures_loaded; //loaded textures stored here
};

#endif // !MODEL
