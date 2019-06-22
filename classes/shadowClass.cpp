#include <glad/glad.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../header/classDefines.h"

shadows::shadows(Player* player, Shader* shader) : player(player), shader(shader) {
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	//shader->setInt("depthMap", 1);

	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void shadows::shadowLive() {
	shadowTransforms.push_back(shadowProj * glm::lookAt(player->pos + glm::vec3(0, 4.0f, 4.0f), player->pos + glm::vec3(0, 4.0f, 4.0f) + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(player->pos + glm::vec3(0, 4.0f, 4.0f), player->pos + glm::vec3(0, 4.0f, 4.0f) + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(player->pos + glm::vec3(0, 4.0f, 4.0f), player->pos + glm::vec3(0, 4.0f, 4.0f) + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(player->pos + glm::vec3(0, 4.0f, 4.0f), player->pos + glm::vec3(0, 4.0f, 4.0f) + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(player->pos + glm::vec3(0, 4.0f, 4.0f), player->pos + glm::vec3(0, 4.0f, 4.0f) + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(player->pos + glm::vec3(0, 4.0f, 4.0f), player->pos + glm::vec3(0, 4.0f, 4.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	for (unsigned int i = 0; i < 6; ++i)
		shader->setMatrix4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	shader->setFloat("far_plane", far_plane);
	shader->setVec3_v2("lightPos", player->pos + glm::vec3(0,4.0f,4.0f));
}