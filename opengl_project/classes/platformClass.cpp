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
		scale = 1;
		glm::vec2 playerCenterXZ(player->pos.x, player->pos.z);
		glm::vec2 playerCenterXY(player->pos.x, player->pos.y);

		glm::vec2 platformCenterXZ(pos.x, pos.z);
		glm::vec2 platformCenterXY(pos.x, pos.y);
		glm::vec2 platformCenterZY(pos.z, pos.y);

		glm::vec2 halfExtent(4, 4); //this seems to work best for collision detection, based on trial and error
		glm::vec2 difference = playerCenterXZ - platformCenterXZ;
		glm::vec2 clamped = glm::clamp(difference, -halfExtent, halfExtent);
		glm::vec2 closest = platformCenterXZ + clamped;
		difference = closest - playerCenterXZ;
		float penetrationXZ = 1 - glm::length(difference);

		halfExtent = glm::vec2(5, 0.25);
		difference = playerCenterXY - platformCenterXY;
		clamped = glm::clamp(difference, -halfExtent, halfExtent);
		closest = platformCenterXY + clamped;
		difference = closest - playerCenterXY;
		float penetrationXY = 1 - glm::length(difference);

		if (penetrationXZ > 0.0f && penetrationXY > 0.2f) { //collision detection bit
			onPlatform = true;

			player->velocity.b *= -0.6;
			bounces++;

			if (type == 4) {
				player->gravEffect = true;
				player->gravTimer = glfwGetTime();
			}

			if (type == 1) {
				glfwSetInputMode(player->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				player->gamePlaying = false;
				player->disabled = true;
				player->died = true;
			}

			if (penetrationXZ < 0) {
				destroyed = true;
			}
			else if (penetrationXY > 0.3f) {
				player->pos.y += penetrationXY;
			}

			if (bounces >= 2) {
				destroyed = true;
			}

			if (destroyed == true) {
				if (type == 2) {
					player->points += 10;
				}
				else if (type == 3) {
					player->points += 30;
				}
				else {
					player->points++;
				}

				//std::cout << player->points << " -- " << type << std::endl;
			}
		}
	}
}

Platforms::Platforms(Shader* shader, Player* player, glm::vec3 transform, float scale, int type) : scale(scale), shader(shader), player(player) {
	this->platform = platform;

	if (type >= 95) {
		this->type = 4;
	}
	else if (type >= 90) {
		this->type = 3;
	}
	else if (type >= 80) {
		this->type = 2;
	}
	else if (type >= 60) {
		this->type = 1;
	}
	else {
		this->type = 0;
	}

	this->pos = transform;
	model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::scale(model, glm::vec3(scale));
}

void Platforms::liveUpdate() {
	collisionDetection();
	//shader->setVec3_v2("light.position", glm::vec3(pos.x, pos.y + 4, pos.z));
	//shader->setVec3_v2("light.diffuse", glm::vec3(0.5, 0.5, 0.4));
	this->Draw();
}

void Platforms::Draw() {
	if (!destroyed) {
		shader->setMatrix4("model", model);

		if (type == 0) {
			shader->setVec3_v2("tint", glm::vec3(0.416, 0.353, 0.804));
		}
		else if (type == 1) {
			shader->setVec3_v2("tint", glm::vec3(0.698, 0.133, 0.133));
		}
		else if (type == 2) {
			shader->setVec3_v2("tint", glm::vec3(0.275, 0.510, 0.706));
		}
		else if (type == 3) {
			shader->setVec3_v2("tint", glm::vec3(0.824, 0.412, 0.118));
		}
		else if (type == 4) {
			shader->setVec3_v2("tint", glm::vec3(0.000, 0.980, 0.604));
		}

		platform->Draw(shader);
	}
}

void Platforms::transformPlatform(glm::vec3 transform) {
	this->pos = transform;
	model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::scale(model, glm::vec3(scale));
}