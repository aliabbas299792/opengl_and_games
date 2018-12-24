#include "platformClasses.h"
#include <SFML/Graphics.hpp>

chunks::~chunks() { //destructor definition, destructs the platforms, then empties the vector array, then destructs the chunk
	for (int i = 0; i < platformsInTheChunk.size(); i++) {
		platformsInTheChunk[i].~platform();
		platformsInTheChunk.pop_back();
	}
}

chunks::chunks(int width, int height, int posX, int posY) { //the constructor definition
	//the 2 variables below are a sector, which should be 1/9th of the screen
	float sectorWidth, sectorHeight;
	sectorWidth = width / 3;
	sectorHeight = height / 3;

	//setting the variables
	chunkWidthHeight.x = width;
	chunkWidthHeight.y = height;
	chunkCoords.x = posX;
	chunkCoords.y = posY;

	//to ensure that the generates platforms are at least pseudorandom
	srand(time(NULL));

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			platformsInTheChunk.push_back( //this will occur 9 times, a new platform will be made, and then added to the vector
				platform(
					sectorWidth / 2 + (rand() % 100 - 25),
					sectorHeight / 2 + (rand() % 100 - 25),
					posX + (sectorWidth * i) + (rand() % 50) + 50,
					posY + (sectorHeight * j) + (rand() % 50) + 50
				)
			);
		}
	}
}