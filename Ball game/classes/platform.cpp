#include "../headerFiles/platformClasses.h"
#include <SFML/Graphics.hpp>

platform::platform(int a, int b, int c, int d, sf::Color e, blockTypes f, int g) {
	//basically sets the values of the collision object
	width = a;
	height = b;
	posx = c;
	posy = d;
	typeOfBlock = f;
	scoreIncrement = g;
	rect.setSize(sf::Vector2f(width, height));
	rect.setPosition(sf::Vector2f(posx, posy));
	rect.setFillColor(e);
}

bool platform::checkIntersect(sf::Sprite &sprite1, float gravity, float &velocityX, float &velocityY, float gravityOriginal, bool &hitFloor, bool &removeGravity, int &score, sf::Time &timeAtJump, sf::Clock clock, float &prevPosX, float &prevPosY) {
	//s_ is sprite, o_ is object, noGbox is rect above object, oLeft is rect to left of object, oRight is rect to right of object
	if (jumpedOn == 3) {
		return false;
	}

	float s_min_x, s_max_x, s_min_y, s_max_y, o_min_x, o_max_x = 0, o_min_y, o_max_y, noGbox_min_x, noGbox_max_x, noGbox_min_y, noGbox_max_y, oLeft_min_x, oLeft_max_x = 0, oLeft_min_y, oLeft_max_y, oRight_min_x, oRight_max_x = 0, oRight_min_y, oRight_max_y;
	int truthCounter = 0;

	//ones below are the boundaries of the sprite
	s_min_x = sprite1.getPosition().x - 15;
	s_max_x = sprite1.getPosition().x + 15;
	s_min_y = sprite1.getPosition().y - 15;
	s_max_y = sprite1.getPosition().y + 15;
	//ones below are the boundaries for the obstacle itself
	o_min_x = posx;
	o_max_x = posx + width;
	o_min_y = posy;
	o_max_y = posy + height;
	//ones below are the boundaries for the 3px above the box where I dont want gravity to act, above box
	noGbox_min_x = posx;
	noGbox_max_x = posx + width;
	noGbox_min_y = posy - 3;
	noGbox_max_y = posy;
	//ones below are the left side boundaries to prevent teleportation
	oLeft_min_x = posx - 2;
	oLeft_max_x = posx;
	oLeft_min_y = posy + 5;
	oLeft_max_y = posy + height - 5;
	//ones below are the right side boundaries to prevent teleportation
	oRight_min_x = posx + width;
	oRight_max_x = posx + width + 2;
	oRight_min_y = posy + 5;
	oRight_max_y = posy + height - 5;
	//std::cout << velocityY << std::endl;

	if (s_max_x > noGbox_min_x && s_min_x < noGbox_max_x && s_max_y > noGbox_min_y && s_min_y < noGbox_max_y && velocityY > 0) {
		//checks for intersection in an imaginary box 3 pixels higher than the surface of a collision object
		removeGravity = true; //remove gravity if it intersects, to prevent infinite annoying bouncing
		sprite1.setPosition(prevPosX, prevPosY); //set it to previous position, which is probably outside the box
		hitFloor = true; //signal that the floor has been hit, for the movement processor to act appropriately
		truthCounter++; //increment the amount of times an intersection check turns out to be true
	}

	if (s_max_x > oLeft_min_x && s_min_x < oLeft_max_x && s_max_y > oLeft_min_y && s_min_y < oLeft_max_y) {
		//check if ball intersects with imaginary box 5px to left of the box
		velocityX *= -0.3; //set velocity to go opposite it, thereby opposing motion into the box
		truthCounter++; //increment the amount of times an intersection check turns out to be true
	}

	if (s_max_x > oRight_min_x && s_min_x < oRight_max_x && s_max_y > oRight_min_y && s_min_y < oRight_max_y) {
		//check if ball intersects with imaginary box 5px to right of the box
		velocityX *= 0.3; //set velocity to go opposite it, thereby opposing motion into the box
		truthCounter++; //increment the amount of times an intersection check turns out to be true
	}

	if (s_max_x > o_min_x && s_min_x < o_max_x && s_max_y > o_min_y && s_min_y < o_max_y && velocityY <= 0) {
		//check if ball intersects the box, and the velocity is negative or 0, meaning it's coming from the bottom
		sprite1.setPosition(prevPosX, prevPosY); //set it to previous position, which is probably outside the box
		velocityY = 0.5; //set the velocity to be away from the bottom of the box
		truthCounter++; //increment the amount of times an intersection check turns out to be true
	}

	if (truthCounter > 0) { //if the intersection has occurred at least once
		if (jumpedOn < 3) { //check if it has been jumped on before
			score += scoreIncrement; //otherwise award a point
			jumpedOn++;

			int tempAlpha = rect.getFillColor().a / 2; //gets the alpha channel colour and divides by 2

			if (jumpedOn == 3) { //if 3 jumps have occurred then basically make it disappear
				rect.setFillColor(sf::Color(rect.getFillColor().r, rect.getFillColor().g, rect.getFillColor().b, 0));
			}
			else {
				rect.setFillColor(sf::Color(rect.getFillColor().r, rect.getFillColor().g, rect.getFillColor().b, tempAlpha)); //otherwise just set the alpha channel to the decreased value of tempAlpha
			}
		}

		timeAtJump = clock.getElapsedTime(); //this is for detecting how long it's been since the ball last touched a platform 

		return true; //return true, so appropriate action is taken, i.e bouncing and stuff
	}
	else {
		return false; //return false, to continue as normal
	}
}

void platform::setColour(sf::Color colour) {
	rect.setFillColor(colour);
}