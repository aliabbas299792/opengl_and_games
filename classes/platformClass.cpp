#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../header/classDefines.h"
#include <glm/ext.hpp>

void Platforms::collisionDetection() {
	//we need collision detection in 2 planes, XY and XZ
	//so we can model those 2 planes as 2x 2D planes and it would work
	//the player ball radius is 1
	//the model is 5 long, 5 wide and 0.25 deep
	if (!destroyed) {
		glm::vec2 playerCenterXZ(player->pos.x, player->pos.z);
		glm::vec2 playerCenterXY(player->pos.x, player->pos.y);

		glm::vec2 platformCenterXZ(pos.x, pos.z);
		glm::vec2 platformCenterXY(pos.x, pos.y);
		glm::vec2 platformCenterZY(pos.z, pos.y);

		glm::vec2 halfExtent(4*scale, 4*scale); //this seems to work best for collision detection, based on trial and error
		glm::vec2 difference = playerCenterXZ - platformCenterXZ;
		glm::vec2 clamped = glm::clamp(difference, -halfExtent, halfExtent);
		glm::vec2 closest = platformCenterXZ + clamped;
		difference = closest - playerCenterXZ;
		float penetrationXZ = 1 - glm::length(difference);

		halfExtent = glm::vec2(5*scale, 0.5*scale);
		difference = playerCenterXY - platformCenterXY;
		clamped = glm::clamp(difference, -halfExtent, halfExtent);
		closest = platformCenterXY + clamped;
		difference = closest - playerCenterXY;
		float penetrationXY = 1 - glm::length(difference);

		if (penetrationXZ > 0.0f && penetrationXY > -0.1f) {
			onPlatform = true;

			if (penetrationXY > 0.0f) {
				bounces++;
			}

			if (penetrationXZ < 0) {
				destroyed = true;
			}
			else if (penetrationXY > 0.1f) {
				player->pos.y += penetrationXY;
			}

			if (bounces >= 3) {
				destroyed = true;
			}
		}
	}
}

Platforms::Platforms(Shader* shader, Player* player, glm::vec3 transform, float scale) : scale(scale), shader(shader), player(player) {
	this->platform = platform;

	this->pos = transform;
	model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::scale(model, glm::vec3(scale));
}

void Platforms::liveUpdate() {
	collisionDetection();
	shader->setVec3_v2("light.position", glm::vec3(pos.x, pos.y + 4, pos.z));
	shader->setVec3_v2("light.diffuse", glm::vec3(0.5, 0.5, 0.4));
	this->Draw();
}

void Platforms::Draw() {
	if (!destroyed) {
		shader->setMatrix4("model", model);
		platform->Draw(shader);
	}
}

void Platforms::transformPlatform(glm::vec3 transform) {
	this->pos = transform;
	model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::scale(model, glm::vec3(scale));
}