#include <SFML/Graphics.hpp>
#include <fstream>
#include "utilFunctions.h"

void moveBall(sf::View &view, sf::RenderWindow &window, sf::Sprite &sprite, float &velocityX, float &velocityY, float accelerationX, float accelerationY, float &gravity, float originalGravity, bool collide, bool &hitFloor, float decelerationX, bool &removeGravity) {
	//if gravity remove flag has been raised in the intersection checks bit, disable gravity
	if (removeGravity == true) {
		gravity = 0;
	}
	else {
		gravity = originalGravity; //otherwise set it to its initial value
	}

	//apply deceleration to x velocity, and set to 0 if it's small enough
	if (velocityX <= -0.02 || velocityX >= 0.02) {
		if (velocityX <= -0.02) {
			velocityX += decelerationX;
		}
		if (velocityX >= 0.02) {
			velocityX -= decelerationX;
		}

		if (velocityX >= -0.02 && velocityX <= 0.02) {
			velocityX = 0;
		}
	}

	if (!collide) { //if it isnt colliding at this instant
		velocityY += gravity; //apply gravitational acceleration downwards

		//make sure the magnitude of velocity is capped at 10
		if (velocityY > 1000) {
			velocityY = 1000;
		}
		else if (velocityY < -1000) {
			velocityY = -1000;
		}

		//accelerate in the requested direction by the user
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			velocityX -= accelerationX;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			velocityX += accelerationX;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			if (hitFloor) { //if it just hit the floor allow for vertical acceleration, otherwise don't allow it
				velocityY -= accelerationY;
				hitFloor = false; //and set the variable to false once the acceleration is done
			}
		}
	}
	else {
		if (velocityY > -0.6 && velocityY < 0.6) { //if the velocity in y axis is small, set to 0
			velocityY = 0;
		}

		velocityY *= -0.5; //otherwise reverse velocity in y axis, and half the magnitude to simulate bouncing
	}

	sprite.move(velocityX, velocityY); //move the sprite
	view.setCenter(sprite.getPosition().x, sprite.getPosition().y); //set the center of the screen to the sprite's location
}

void onDeath(gameScreens &gameState, std::string filePath, int score) {
	if (gameState == game) {
		std::fstream scores;
		scores.open(filePath, std::ios::in);
		std::string temp1 = "";
		std::string temp = "";
		int temp3 = 1;

		while (std::getline(scores, temp1)) { //get all lines
			if (temp3 == 1) {
				temp += temp1;
				temp3++;
				continue;
			}

			temp += '\n' + temp1;
		}

		scores.close();

		scores.open(filePath, std::ios::in);
		scores.close(); //wipes the file

		scores.open(filePath, std::ios::out);
		//temp = temp + '\n' + std::to_string(score);
		//temp = "0\n0\n0\n0\n0\n0\n0\n0";
		temp += '\n' + std::to_string(score);
		scores << temp;
		scores.close();
	}

	gameState = entrance;
}