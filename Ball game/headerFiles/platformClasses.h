#include <SFML/Graphics.hpp>
#include "enums.h"

#ifndef PLATFORM_CLASSES
#define PLATFORM_CLASSES

class platform {
public:
	sf::RectangleShape rect; //the rectangle initial definition
	int width, height, posx, posy, scoreIncrement; //the internal values it hasX
	blockTypes typeOfBlock;

	int jumpedOn = 0; //how many jumps on this platform itself

	~platform() {}; //the destructor for the platform

	platform() {};
	platform(int, int, int, int, sf::Color, blockTypes, int); //the constructor

	sf::RectangleShape* shape() { //to return the actual shape on request
		return &rect;
	}

	void setColour(sf::Color);

	//one below is constructor for intersection checker
	bool checkIntersect(sf::Sprite &a, float b, float &c, float &d, float e, bool &f, bool &g, int &h, sf::Time &i, sf::Clock j, float &prevPosX, float &prevPosY);
};

class platformNormal : public platform {
public:
	bool instantDeath() { return false; };
};

class platformInstantDeath : public platform {
public:
	bool instantDeath() { return true; };
};

class chunks { //chunks, which will hold 9 platforms and each will cover the screen
public:
	sf::Vector2f chunkCoords = sf::Vector2f(); //these will be the global coordinates of the chunk
	sf::Vector2f chunkWidthHeight = sf::Vector2f(); //and its dimensions, in case I want to change them for some of em down the line

	std::vector<platform> platformsInTheChunk; //a vector to hold the platforms

	chunks(int, int, int, int, bool); //the constructor
	~chunks(); //the destructor
};

class chunksHolder { //this will hold 9 chunks
private:
	int chunkOverlap(int, int);

public:
	std::vector<chunks> chunksLoaded; //the vector to hold the chunks
	chunksHolder(int, int, int, int); //the constructor
	~chunksHolder(); //the destructor

	int chunkCoordsWidthHeight[4] = { 0 }; //this will be the virtual chunk that the ball is suppposed to be in, will be used to delete/generate chunks
	bool surroundingChunks[3][3] = { false }; //this 3x3 array will represent all the present chunks, and will be updated according to the virtual chunk above

	void updateChunks(sf::Sprite, int, int); //function to actually delete/create chunks
};

#endif