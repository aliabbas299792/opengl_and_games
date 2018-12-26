#include "../headerFiles/platformClasses.h"
#include <SFML/Graphics.hpp>

void chunksHolder::updateChunks(sf::Sprite sprite1, int winWidth, int winHeight) { //updateChunks(...) definition
	//vars below hold the position of the sprite in x and y
	float posX = sprite1.getPosition().x;
	float posY = sprite1.getPosition().y;

	if (posX < 0) { //when it's less than 0, want to 'round' to the previous chunk, so will subtract windowWidth
		posX -= winWidth;
	}
	if (posY < 0) { //when it's less than 0, want to 'round' to the previous chunk, so will subtract windowHeight
		posY -= winHeight;
	}

	//below 2 use the div function, to find the quotient (rather than the remainder, like % does)
	chunkCoordsWidthHeight[0] = (posX - ((int)posX % winWidth)); //top left x coord 
	chunkCoordsWidthHeight[1] = (posY - ((int)posY % winHeight)); //top left y coord
	chunkCoordsWidthHeight[2] = winWidth; //width
	chunkCoordsWidthHeight[3] = winHeight; //height

	for (int i = 0; i < chunksLoaded.size(); i++) { //loops through all chunks
		if ( //checks if chunks are outside the 3x3 space they're supposed to be in, and then destructs them if they are
			chunksLoaded[i].chunkCoords.x < chunkCoordsWidthHeight[0] - chunkCoordsWidthHeight[2] ||
			chunksLoaded[i].chunkCoords.y < chunkCoordsWidthHeight[1] - chunkCoordsWidthHeight[3] ||
			chunksLoaded[i].chunkCoords.x > chunkCoordsWidthHeight[0] + 2 * (chunkCoordsWidthHeight[2]) ||
			chunksLoaded[i].chunkCoords.y > chunkCoordsWidthHeight[1] + 2 * (chunkCoordsWidthHeight[3])
			) {
			chunksLoaded[i].~chunks(); //destructs the object
			chunksLoaded.erase(chunksLoaded.begin() + i); //removes the item
		}
	}

	//the below are basically temporary variables to hold the virtual points in each chunk, to check if there is one there
	int x_sector_0 = chunkCoordsWidthHeight[0] - (chunkCoordsWidthHeight[2] / 2);
	int x_sector_1 = chunkCoordsWidthHeight[0] + (chunkCoordsWidthHeight[2] / 2);
	int x_sector_2 = chunkCoordsWidthHeight[0] + (chunkCoordsWidthHeight[2] * 1.5);
	int y_sector_0 = chunkCoordsWidthHeight[1] - (chunkCoordsWidthHeight[3] / 2);
	int y_sector_1 = chunkCoordsWidthHeight[1] + (chunkCoordsWidthHeight[3] / 2);
	int y_sector_2 = chunkCoordsWidthHeight[1] + (chunkCoordsWidthHeight[3] * 1.5);

	//populates the surroundingChunks[3][3] array with values corresponding to the presence (or lack thereof) of chunks
	surroundingChunks[0][0] = chunkOverlap(x_sector_0, y_sector_0);
	surroundingChunks[0][1] = chunkOverlap(x_sector_1, y_sector_0);
	surroundingChunks[0][2] = chunkOverlap(x_sector_2, y_sector_0);
	surroundingChunks[1][0] = chunkOverlap(x_sector_0, y_sector_1);
	surroundingChunks[1][2] = chunkOverlap(x_sector_2, y_sector_1);
	surroundingChunks[2][0] = chunkOverlap(x_sector_0, y_sector_2);
	surroundingChunks[2][1] = chunkOverlap(x_sector_1, y_sector_2);
	surroundingChunks[2][2] = chunkOverlap(x_sector_2, y_sector_2);

	//2 temp variables to hold the x and y coords of any chunks which are to be added
	int tempCoordX = 0;
	int tempCoordY = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1) { //continue if on the middle chunk, as there is always one there
				continue;
			}

			if (surroundingChunks[i][j] == 0) { //if a chunk is not present in where some virtual sector is, further processing
				switch (i) { //this decides the y coordinate for the chunk, based on the variable i, which belongs to the outer loop, looping down y axis chunks
				case 0:
					tempCoordY = chunkCoordsWidthHeight[1] - chunkCoordsWidthHeight[3];
					break;
				case 1:
					tempCoordY = chunkCoordsWidthHeight[1];
					break;
				case 2:
					tempCoordY = chunkCoordsWidthHeight[1] + chunkCoordsWidthHeight[3];
					break;
				}

				switch (j) { //for deciding x axis coordinates, as j is the one used to loop through the x axis, and so holds the needed value
				case 0:
					tempCoordX = chunkCoordsWidthHeight[0] - chunkCoordsWidthHeight[2];
					break;
				case 1:
					tempCoordX = chunkCoordsWidthHeight[0];
					break;
				case 2:
					tempCoordX = chunkCoordsWidthHeight[0] + chunkCoordsWidthHeight[2];
					break;
				}

				if (tempCoordX == 0 && tempCoordY == 0) {
					continue;
				}

				chunksLoaded.push_back( //adds the needed chunk to the chunksLoaded vector
					chunks(
						chunkCoordsWidthHeight[2],
						chunkCoordsWidthHeight[3],
						tempCoordX,
						tempCoordY,
						false
					)
				);

				surroundingChunks[i][j] = 1; //once processing has finished, it is obviously present, so mark it as such in the array
			}
		}
	}
}

int chunksHolder::chunkOverlap(int pointX, int pointY) { //private function for checking of the overlap between the virtual points from earlier, and a chunk
	bool overlaps = false; //initialise overlap to false

	for (int i = 0; i < chunksLoaded.size(); i++) {
		if (
			( //check if a point in the top right virtual sector is in a chunk, literally normal intersection check
				chunksLoaded[i].chunkCoords.x < pointX &&
				chunksLoaded[i].chunkCoords.x + chunksLoaded[i].chunkWidthHeight.x > pointX &&
				chunksLoaded[i].chunkCoords.y < pointY &&
				chunksLoaded[i].chunkCoords.y + chunksLoaded[i].chunkWidthHeight.y > pointY
				)
			)
		{
			overlaps = true; //obviously if an overlap is present, set it to true
		}
	}

	return overlaps; //return the value of overlaps for the surroundingChunks[3][3] array
}

chunksHolder::~chunksHolder() { //the destructor
	for (int i = 0; i < chunksLoaded.size(); i++) { //loops through all the chunks
		chunksLoaded[i].~chunks(); //destructs all the chunks, which will actually get rid of all the platforms first, then destruct the respective chunk
	}
}

chunksHolder::chunksHolder(int width, int height, int posX, int posY) { //the initial constructor
	//std::system("CLS"); //clears console
	//populates the vector with 3x3 chunks in a grid
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1) {
				continue;
			}

			if (posY + (height * j) == 600 && posX + (width * i) == 0) { //disable red blocks from forming in the chunk directly beneath it
				chunksLoaded.push_back(
					chunks(
						width,
						height,
						posX + (width * i),
						posY + (height * j),
						true
					)
				);
				continue;
			}

			chunksLoaded.push_back(
				chunks(
					width,
					height,
					posX + (width * i),
					posY + (height * j),
					false
				)
			);
		}
	}
}