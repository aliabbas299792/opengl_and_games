#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../header/classDefines.h"

Platforms::Platforms(Shader* shader) {
	this->shader = shader;
	this->platform = new Model("models/platform2/platform2.obj");
}

void Platforms::liveUpdate() {
	for (int i = 0; i < platform->meshes.size(); i++) {
		for (int j = 0; j < platform->meshes[i].vertices.size(); j++) {
			std::cout << platform->meshes[i].vertices[j].Pos.r << " -- " << platform->meshes[i].vertices[j].Pos.g << " -- " << platform->meshes[i].vertices[j].Pos.b << std::endl;
		}
	}
}

void Platforms::Draw() {
	platform->Draw(shader);
}