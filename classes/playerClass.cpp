#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../header/classDefines.h"
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <glm/ext.hpp>

Player::Player(Camera* camera) {
	this->camera = camera;

	camera->cameraPos = pos + glm::vec3(0.0f, 2.0f, 8.0f); //initial camera position
}

void Player::liveUpdate(Shader* shader) {
	this->playerMovement(lastKeyStates);

	shader->setMatrix4("model", model); //any required transformations are passed to the shader

	player->Draw(shader); //the player ball is drawn
}

void Player::playerMovement(glm::vec4 keyStates) {
	if (firstFrame < 5) {
		firstFrame++;
	}
	/*
	This is the format of the vector received:
	keyStates.x = W
	keyStates.y = A
	keyStates.z = S
	keyStates.w = D
	*/
	if (onPlatform) {
		deceleration.y = 0;
	}
	else {
		deceleration.y = originalDecelerationY;
	}

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

	//acceleration, dependent on key states inputted
	if (onPlatform) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			currentBounces = 0;
			velocity.b += velocityIncrease.y;
			Platforms::onPlatform = false;
			onPlatform = false;
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
	if (!onPlatform && firstFrame > 4) { //y plane deceleration
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
		if (velocity.r <= -0.005 || velocity.r >= 0.005) {
			if (velocity.r <= -0.005) {
				velocity.r += velocityDecrease.x + dragForce.r;
			}
			if (velocity.r >= 0.005) {
				velocity.r -= velocityDecrease.x + dragForce.r;
			}

			if (velocity.r >= -0.005 && velocity.r <= 0.005) {
				velocity.r = 0;
			}
		}

		if (velocity.g <= -0.005 || velocity.g >= 0.005) {
			if (velocity.g <= -0.005) {
				velocity.g += velocityDecrease.x + dragForce.g;
			}
			if (velocity.g >= 0.005) {
				velocity.g -= velocityDecrease.x + dragForce.g;
			}

			if (velocity.g >= -0.005 && velocity.g <= 0.005) {
				velocity.g = 0;
			}
		}
	}

	//the bounce code, the bounds are different to allow for more leeway, because when the bounds are the same, it sometimes randomly did/didn't bounce
	if (bounce && onPlatform && currentBounces < maxBounce && velocity.b < 0) {
		velocity.b *= -((1.0f / 40.0f) * pow((currentBounces - 5.0f), 2.0f) + 0.15f); //velocity is reflected to make a bounce
		//basically it is -f(x) = -(1/40 * (x-5)^2 + 0.15), reflects the bounce but decreases roughly quadratically in intensity
		currentBounces++; //increment bounces, so that the maximum is reached
	}
	if (currentBounces >= maxBounce) { //once max bounces reached...
		bounce = false; //set bounce condition to false
		velocity.b = 0; //set the vertical velocity back to 0
		currentBounces = 0; //reset the current bounces
	}

	//update the position based on the velocity
	pos.z += velocity.r;
	pos.x += velocity.g;
	pos.y += velocity.b;

	model = glm::translate(glm::mat4(1.0f), pos);

	camera->cameraPos = pos + glm::vec3(0.0f, 2.0f, 8.0f); //moving the camera with the ball
}