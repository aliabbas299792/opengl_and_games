#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../header/classDefines.h"
#include <glm/ext.hpp>

chunks::~chunks() { //destructor definition, destructs the platforms, then empties the vector array, then destructs the chunk
	for (int i = 0; i < platformsInTheChunk.size(); i++) {
		platformsInTheChunk[i].~Platforms();
		platformsInTheChunk.pop_back();
	}
}

chunks::chunks(int posX, int posY, int posZ, Shader* shader, Player* player) { //the constructor definition
	//the 3 variables below are a sector, which should be 1/27th of the screen
	float sectorWidth, sectorHeight, sectorDepth;
	sectorWidth = dimensions.x / 5;
	sectorHeight = dimensions.y / 5;
	sectorDepth = dimensions.z / 5;

	//setting the variables
	chunkCoords.x = posX;
	chunkCoords.y = posY;
	chunkCoords.z = posZ;

	//to ensure that the generates platforms are at least pseudorandom
	srand(time(NULL));

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 5; k++) {
				platformsInTheChunk.push_back(
					Platforms::Platforms(
						shader,
						player,
						glm::vec3(
							chunkCoords.x + sectorWidth*i + (rand() % 25),
							chunkCoords.y + sectorHeight*j + (rand() % 50),
							chunkCoords.z + sectorDepth*k + (rand() % 25)
						),
						((rand() % 600) - 250) / 100
					)
				);
			}
		}
	}
}

void chunks::chunkUpdate() {
	for (int i = 0; i < platformsInTheChunk.size(); i++) {
		platformsInTheChunk[i].liveUpdate();
	}
}