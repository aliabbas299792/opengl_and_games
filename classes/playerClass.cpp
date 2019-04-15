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

	timeScalar = glfwGetTime() - prevFrameTime; //gets delta time difference from last frame
	prevFrameTime = glfwGetTime(); //sets this as current time, for next loop
	timeScalar = pow(timeScalar, 2.0f) * 120000.0f; //squares the delta time, and multiplies by 120,000 to make movement consistent

	//apply the scalar to acceleration to get velocity
	glm::vec2 velocityIncrease = acceleration * timeScalar; 
	glm::vec2 velocityDecrease = deceleration * timeScalar;
	//the velocity is just how much more displacement in a direction per second
	//so we obtain the displacement by s=ut+1/2at^2, but u we can assume is 0, so we do s=1/2at^2, but we apply a different scale factor rather than 1/2
	//our equation is then s/a=120000*t^2
	//so increase in s=120000*t^2*a
	//and we just make replace s with velocityIncrease/velocityDecrease, and we add/subtract from velocity, and add those changes to the position as required
	//and then we have consistent movement with respect to time

	//acceleration
	if (pos.y <= 0.05) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			velocity.b += velocityIncrease.y;
		}
		if (keyStates.x) {
			velocity.r -= velocityIncrease.x;
		}
		if (keyStates.y) {
			velocity.g -= velocityIncrease.x;
		}
		if (keyStates.z) {
			velocity.r += velocityIncrease.x;
		}
		if (keyStates.w) {
			velocity.g += velocityIncrease.x;
		}
	}

	lastKeyStates = keyStates; //this records the last state of the keys and carries this over, to ensure that acceleration works properly

	//drag calculation, basically deceleration will increase for higher velocities, so a maximum velocity is eventually reached
	dragForce.r = pow(velocity.r, 2.0f) * dragCoefficient;
	dragForce.g = pow(velocity.g, 2.0f) * dragCoefficient;
	dragForce.b = pow(velocity.b, 2.0f) * dragCoefficient;

	//deceleration
	if (pos.y > 0.5 || (velocity.b < 0 && pos.y > 0.000015)) { //y plane deceleration
		velocity.b -= velocityDecrease.y;
		bounce = true; //if it's accelerating downwards, it will need to bounce

		//much slower acceleration provided while jumping
		if (keyStates.x) {
			velocity.r -= velocityIncrease.x/ 5;
		}
		if (keyStates.y) {
			velocity.g -= velocityIncrease.x / 5;
		}
		if (keyStates.z) {
			velocity.r += velocityIncrease.x / 5;
		}
		if (keyStates.w) {
			velocity.g += velocityIncrease.x / 5;
		}

	} else { //x and z plane deceleration
		if (velocity.r <= -0.0005 || velocity.r >= 0.0005) {
			if (velocity.r <= -0.0005) {
				velocity.r += velocityDecrease.x + dragForce.r;
			}
			if (velocity.r >= 0.0005) {
				velocity.r -= velocityDecrease.x + dragForce.r;
			}

			if (velocity.r >= -0.0005 && velocity.r <= 0.0005) {
				velocity.r = 0;
			}
		}

		if (velocity.g <= -0.0005 || velocity.g >= 0.0005) {
			if (velocity.g <= -0.0005) {
				velocity.g += velocityDecrease.x + dragForce.g;
			}
			if (velocity.g >= 0.0005) {
				velocity.g -= velocityDecrease.x + dragForce.g;
			}

			if (velocity.g >= -0.0005 && velocity.g <= 0.0005) {
				velocity.g = 0;
			}
		}
	}

	//the bounce code, the bounds are different to allow for more leeway, because when the bounds are the same, it sometimes randomly did/didn't bounce
	if (bounce && pos.y <= 0.00015 && currentBounces < maxBounce) {
		velocity.b *= -((1.0f / 40.0f) * pow((currentBounces - 5.0f), 2.0f) + 0.15); //velocity is reflected to make a bounce
		//basically it is -f(x) = -(1/40 * (x-5)^2 + 0.15), reflects the bounce but decreases roughly quadratically in intensity
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