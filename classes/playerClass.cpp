#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../header/classDefines.h"

Player::Player(Camera* camera) {
	this->camera = camera;
	camera->cameraPos = pos + glm::vec3(0.0f, 2.0f, 8.0f); //initial camera position
}

void Player::liveUpdate(Shader* shader) {
	this->playerMovement(lastKeyStates);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);

	shader->setMatrix4("model", model); //any required transformations are passed to the shader
	shader->setVec3("cameraPos", camera->cameraPos); //camera position is passed to the shader (basically directly behind the player)

	player->Draw(shader); //the player ball is drawn
}

void Player::playerMovement(glm::vec4 keyStates) {
	/*
	This is the format of the vector received:
	keyStates.x = W
	keyStates.y = A
	keyStates.z = S
	keyStates.w = D
	*/

	//acceleration
	if (pos.y <= 0.05) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			velocity.b += acceleration.y;
		}
		if (keyStates.x) {
			velocity.r -= acceleration.x;
		}
		if (keyStates.y) {
			velocity.g -= acceleration.x;
		}
		if (keyStates.z) {
			velocity.r += acceleration.x;
		}
		if (keyStates.w) {
			velocity.g += acceleration.x;
		}
	}

	lastKeyStates = keyStates; //this records the last state of the keys and carries this over, to ensure that acceleration works properly

	//drag calculation, basically deceleration will increase for higher velocities, so a maximum velocity is eventually reached
	dragForce.r = pow(velocity.r, 2.0f) * dragCoefficient;
	dragForce.g = pow(velocity.g, 2.0f) * dragCoefficient;
	dragForce.b = pow(velocity.b, 2.0f) * dragCoefficient;

	//deceleration
	if (pos.y > 0.5 || (velocity.b < 0 && pos.y > 0.000015)) { //y plane deceleration
		velocity.b -= deceleration.y;
		bounce = true; //if it's accelerating downwards, it will need to bounce

		//much slower acceleration provided while jumping
		if (keyStates.x) {
			velocity.r -= acceleration.x/ 5;
		}
		if (keyStates.y) {
			velocity.g -= acceleration.x / 5;
		}
		if (keyStates.z) {
			velocity.r += acceleration.x / 5;
		}
		if (keyStates.w) {
			velocity.g += acceleration.x / 5;
		}

	} else { //x and z plane deceleration
		if (velocity.r <= -0.0005 || velocity.r >= 0.0005) {
			if (velocity.r <= -0.0005) {
				velocity.r += deceleration.x + dragForce.r;
			}
			if (velocity.r >= 0.0005) {
				velocity.r -= deceleration.x + dragForce.r;
			}

			if (velocity.r >= -0.0005 && velocity.r <= 0.0005) {
				velocity.r = 0;
			}
		}

		if (velocity.g <= -0.0005 || velocity.g >= 0.0005) {
			if (velocity.g <= -0.0005) {
				velocity.g += deceleration.x + dragForce.g;
			}
			if (velocity.g >= 0.0005) {
				velocity.g -= deceleration.x + dragForce.g;
			}

			if (velocity.g >= -0.0005 && velocity.g <= 0.0005) {
				velocity.g = 0;
			}
		}
	}

	//the bounce code, the bounds are different to allow for more leeway, because when the bounds are the same, it sometimes randomly did/didn't bounce
	if (bounce && pos.y <= 0.00015 && currentBounces < maxBounce) {
		velocity.b *= -0.5; //velocity is reflected to make a bounce
		currentBounces++; //increment bounces, so that the maximum is reached
	}
	if (currentBounces >= maxBounce) { //once max bounces reached...
		bounce = false; //set bounce condition to false
		velocity.b = 0; //set the vertical velocity back to 0
		pos.y = 0; //set the position to ground level (will be changed to suit platforms later)
		currentBounces = 0; //reset the current bounces
	}
	
	//update the position based on the velocity
	pos.z += velocity.r;
	pos.x += velocity.g;
	pos.y += velocity.b;

	if (pos.y < 0) { //ensures that the minimum height is 0 (will be changed later)
		pos.y = 0;
	}

	camera->cameraPos = pos + glm::vec3(0.0f, 2.0f, 8.0f); //moving the camera with the ball
}