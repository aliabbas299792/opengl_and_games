#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>

#include "../header/modelHeader.h"
#include "../header/classDefines.h"
#include "../header/functionDefines.h"

//assimp headers
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	//sets the classes public variables, and then sets the mesh up
	setupMesh();
}

void Mesh::setupMesh() { //sets up the EBO, VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO); //binds this one, so this is acted on from now on
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //binds this buffer to use from now on

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); //loads vertices into the VBO, and says that they will modified once and used many time

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); //binds the EBO and gives the index data to it

	//vertex positions
	glEnableVertexAttribArray(0); //first attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //passes the first part of the Vertex as parts of the first attribute (the position)
	//vertex normals
	glEnableVertexAttribArray(1); //second attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0); //safely unbind vertex array
}

void Mesh::Draw(Shader* shader) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); //activate the useful texture unit before using

		//then have the correct prefix for easier use in the shader
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++); //store the number as a string  for use in sending the name
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); //store the number as a string  for use in sending the name

		shader->setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Model::Model(std::string path) {
	loadModel(path);
}

void Model::Draw(Shader* shader) { //loops through the meshes and calls the draw functions for all of those
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import; //makes an importer
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); //converts the model into entirely triangles if not already like that, and flips the texture coordinates, as OpenGL normally flipped images on the y axis (mostly)

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) //if returned data is incomplete or didn't load at all print an error and return
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/')); //set the directory variable to the directory the model has been retrieved from

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if there are any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene); //so basically it recursively looks through each node, and checks if it has any children, to find all the meshses
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) //loop through the vertices of the mesh
	{
		Vertex vertex; //make a Vertex struct to hold the data we will extract, and eventually add this to the vertices vector

		// process vertex positions, normals and texture coordinates
		glm::vec3 vector; //you can use the same vector over and over for adding the properties

		//position
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Pos = vector;

		//normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		//textures
		if (mesh->mTextureCoords[0]) { //check if it contains texture coordinates
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		//finally push it to the vertices vector
		vertices.push_back(vertex);
	}
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) { //each face basically contains the indices you need, so each face is iterated through, and all of the indices of the face are added to the indices vector
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0) { //checks if the mesh contains a material
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; //gets the material, using the index from the current mesh (as the materials are all stored in one place rather than separately for each mesh)

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse"); //diffuse texture structs are retrieved
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end()); //and stored at the end of the textures vector

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular"); //specular texture structs are retrieved
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end()); //and stored at the end of the textures vector
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) { //to load the material's textures
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) { //textures looped through, based on the type that they are
		aiString str; //special string to store the properties
		mat->GetTexture(type, i, &str); //gets the texture

		bool skip = false; //bool to indicate whether or not a texture should be loaded (false means that it would be)
		
		for (unsigned int j = 0; j < textures_loaded.size(); j++) { //loops through already loaded textures, so would use an identical loaded one rather than load another one
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) { //compares the loaded texture's path and this one's
				textures.push_back(textures_loaded[j]); //if they are the same then just push the loaded texture to the textures vector, rather than loading a new one
				skip = true; //and skip loading it of course
				break;
			}
		}

		if (!skip) { // if texture hasn't been loaded already, load it
			Texture texture; //struct setup to store the retrieved values

			std::string temp = directory + "/" + str.C_Str(); //the full location of the texture (including directory), for use in loading the texture (the next bit)

			texture.id = loadTexture(temp.c_str()); //texture is loaded, and the returned id is stored here (this is the function I made in imageLoad.cpp, linked through functionDefines.h)
			texture.type = typeName; //texture type
			texture.path = str.C_Str(); //texture path

			textures.push_back(texture); //add to textures vector
			textures_loaded.push_back(texture); //add to loaded textures vector
		}
	}
	return textures;
}