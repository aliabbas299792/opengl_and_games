#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../header/classDefines.h"
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <glm/ext.hpp>

#include "Eigen/Dense"

Player::Player(Camera* camera) {
	this->camera = camera;

	camera->cameraPos = pos + glm::vec3(0.0f, 2.5f, 12.0f); //initial camera position
}

void Player::liveUpdate(Shader* shader) {
	this->playerMovement(lastKeyStates);

	shader->setMatrix4("model", model); //any required transformations are passed to the shader

	if (disabled == false) {
		//player->Draw(shader); //the player ball is drawn
	}
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

	timeScalar = 2500 * (glfwGetTime() - prevFrameTime); //gets delta time difference from last frame
	prevFrameTime = glfwGetTime(); //sets this as current time, for next loop

	if (gamePlaying == false && lastPlayCheck == true) {
		timePaused = glfwGetTime();
	}

	if (gamePlaying == true && lastPlayCheck == false) {
		gravTimer += glfwGetTime() - timePaused;
	}

	if (gamePlaying != false) {
		if (gravEffect == true) {
			if (glfwGetTime() >= gravTimer + 5) {
					gravEffect = false;
					gravTimer = 0;
					velocity.b = 0;
					deceleration.y = originalDecelerationY;
					dragForce.y = 0;
			}
			//std::cout << timePaused << std::endl;
		}

		if (gravEffect == true) {
			deceleration.y /= 100;
		}

		//acceleration, dependent on key states inputted
		if (onPlatform) {
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				currentBounces = 0;
				velocity.b += acceleration.y;
				Platforms::onPlatform = false;
				onPlatform = false;
			}
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

		lastKeyStates = keyStates; //this records the last state of the keys and carries this over, to ensure that acceleration works properly

		//deceleration
		if (!onPlatform && firstFrame > 4) { //y plane deceleration
			velocity.b -= deceleration.y;
			bounce = true; //if it's accelerating downwards, it will need to bounce
		}

		float timeScaleTemp = timeScalar / 2500;
		//x and z plane deceleration
		if (velocity.r <= -0.001 || velocity.r >= 0.001) {
			if (velocity.r <= -0.001) {
				velocity.r += deceleration.x + dragForce.r;
			}
			if (velocity.r >= 0.001) {
				velocity.r -= deceleration.x + dragForce.r;
			}

			if (velocity.r >= -0.001 && velocity.r <= 0.001) {
				velocity.r = 0;
			}
		}

		if (velocity.g <= -0.001 || velocity.g >= 0.001) {
			if (velocity.g <= -0.001) {
				velocity.g += deceleration.x + dragForce.g;
			}
			if (velocity.g >= 0.001) {
				velocity.g -= deceleration.x + dragForce.g;
			}

			if (velocity.g >= -0.001 && velocity.g <= 0.001) {
				velocity.g = 0;
			}
		}

		if (velocity.b <= -0.0005 || velocity.b >= 0.0005) {
			if (velocity.b <= -0.0005) {
				velocity.b += dragForce.b;
			}
			if (velocity.b >= 0.0005) {
				velocity.b -= dragForce.b;
			}
		}

		/*
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
		*/

		if (velocity.b > 1) {
			velocity.b = 1;
		}
		if (velocity.b < -1) {
			velocity.b = -1;
		}

		//update the position based on the velocity ------- ADD DELTA TIME ----------- FIX THE FUCKING GLITCHING YOU BITCH
		pos.z += velocity.r * timeScalar;
		pos.x += velocity.g * timeScalar;
		pos.y += velocity.b * timeScalar;

		camera->cameraPos = pos + glm::vec3(0.0f, 2.5f, 12.0f); //moving the camera with the ball, but lagging behind by 1 frame to give a sense of motion
	}

	//drag calculation, basically deceleration will increase for higher velocities, so a maximum velocity is eventually reached
	dragForce.r = pow(velocity.r, 2.0f) * dragCoefficient * 2;
	dragForce.g = pow(velocity.g, 2.0f) * dragCoefficient * 2;
	dragForce.b = pow(velocity.b, 2.0f) * dragCoefficient / 5;

	lastPlayCheck = gamePlaying;

	model = glm::translate(glm::mat4(1.0f), pos);
}