#include "../headerFiles/platformClasses.h"
#include <SFML/Graphics.hpp>

chunks::~chunks() { //destructor definition, destructs the platforms, then empties the vector array, then destructs the chunk
	for (int i = 0; i < platformsInTheChunk.size(); i++) {
		platformsInTheChunk[i].~platform();
		platformsInTheChunk.pop_back();
	}
}

chunks::chunks(int width, int height, int posX, int posY, bool disableDeathBlock) { //the constructor definition
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
			//all of the if-statements below will result in blocks, in pretty random distributions, or none at all

			if (rand() % 100 < 50) {
				platformsInTheChunk.push_back( //this will occur 9 times, a new platform will be made, and then added to the vector
					platformNormal::platform(
						sectorWidth / 2 + (rand() % 100 - 25),
						sectorHeight / 2 + (rand() % 100 - 25),
						posX + (sectorWidth * i) + (rand() % 50) + 50,
						posY + (sectorHeight * j) + (rand() % 50) + 50,
						sf::Color::White,
						normal,
						1
					)
				);
			}
			else if(rand() % 100 >= 50 && rand() % 100 < 80 && disableDeathBlock == false){
				platformsInTheChunk.push_back( //this will occur 9 times, a new platform will be made, and then added to the vector
					platformInstantDeath::platform(
						sectorWidth / 2 + (rand() % 150 - 25),
						sectorHeight / 2 + (rand() % 50 - 25),
						posX + (sectorWidth * i) + (rand() % 50) + 70,
						posY + (sectorHeight * j) + (rand() % 50) + 70,
						sf::Color::Red,
						instantDeath,
						1
					)
				);
			}
			else if (rand() % 100 >= 80 && rand() % 100 < 87) {
				platformsInTheChunk.push_back( //this will occur 9 times, a new platform will be made, and then added to the vector
					platformInstantDeath::platform(
						sectorWidth / 2 + (rand() % 50 - 25),
						sectorHeight / 2 + (rand() % 50 - 25),
						posX + (sectorWidth * i) + (rand() % 50) + 70,
						posY + (sectorHeight * j) + (rand() % 50) + 70,
						sf::Color::Green,
						quadrupleScore,
						4
					)
				);
			}
			else if (rand() % 100 >= 87 && rand() % 100 < 95) {
				platformsInTheChunk.push_back( //this will occur 9 times, a new platform will be made, and then added to the vector
					platformInstantDeath::platform(
						sectorWidth / 2 + (rand() % 150 - 25),
						sectorHeight / 2 + (rand() % 150 - 25),
						posX + (sectorWidth * i) + (rand() % 50) + 70,
						posY + (sectorHeight * j) + (rand() % 50) + 70,
						sf::Color::Cyan,
						bigBounce,
						1
					)
				);
			}
			else if (rand() % 100 >= 95 && rand() % 100 <= 99) {
				platformsInTheChunk.push_back( //this will occur 9 times, a new platform will be made, and then added to the vector
					platformInstantDeath::platform(
						sectorWidth / 2 + (rand() % 50 - 25),
						sectorHeight / 2 + (rand() % 50 - 25),
						posX + (sectorWidth * i) + (rand() % 50) + 70,
						posY + (sectorHeight * j) + (rand() % 50) + 70,
						sf::Color::Blue,
						tenTimeScore,
						10
					)
				);
			}
		}
	}
}