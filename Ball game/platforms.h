class platforms {
private:
	sf::RectangleShape rect; //the rectangle initial definition
	int width, height, posx, posy; //the internal values it has
public:
	int jumpedOn = 0; //how many jumps on this platform itself

	~platforms() {}; //the destructor for the platform
	platforms(int, int, int, int); //the constructor

	sf::RectangleShape* shape() { //to return the actual shape on request
		return &rect;
	}

	//one below is constructor for intersection checker
	bool checkIntersect(sf::Sprite &a, float b, float &c, float &d, float e, bool &f, bool &g, int &h, sf::Time &i, sf::Clock j);
};

platforms::platforms(int a, int b, int c, int d) {
	//basically sets the values of the collision object
	width = a;
	height = b;
	posx = c;
	posy = d;
	rect.setSize(sf::Vector2f(width, height));
	rect.setPosition(sf::Vector2f(posx, posy));
	rect.setFillColor(sf::Color::White);

	//platformStuff.push_back(this); //adds the collision object to a vector array
}

bool platforms::checkIntersect(sf::Sprite &sprite1, float gravity, float &velocityX, float &velocityY, float gravityOriginal, bool &hitFloor, bool &removeGravity, int &score, sf::Time &timeAtJump, sf::Clock clock) {
	//s_ is sprite, o_ is object, noGbox is rect above object, oLeft is rect to left of object, oRight is rect to right of object
	if (jumpedOn == 3) {
		return false;
	}

	float s_min_x, s_max_x, s_min_y, s_max_y,  o_min_x, o_max_x = 0, o_min_y, o_max_y, noGbox_min_x, noGbox_max_x, noGbox_min_y, noGbox_max_y, oLeft_min_x, oLeft_max_x = 0, oLeft_min_y, oLeft_max_y, oRight_min_x, oRight_max_x = 0, oRight_min_y, oRight_max_y;
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
	oLeft_min_x = posx-2;
	oLeft_max_x = posx;
	oLeft_min_y = posy+5;
	oLeft_max_y = posy + height-5;
	//ones below are the right side boundaries to prevent teleportation
	oRight_min_x = posx + width;
	oRight_max_x = posx + width + 2;
	oRight_min_y = posy+5;
	oRight_max_y = posy + height-5;
	//std::cout << velocityY << std::endl;

	if (s_max_x > noGbox_min_x && s_min_x < noGbox_max_x && s_max_y > noGbox_min_y && s_min_y < noGbox_max_y && velocityY > 0) {
		//checks for intersection in an imaginary box 3 pixels higher than the surface of a collision object
		removeGravity = true; //remove gravity if it intersects, to prevent infinite annoying bouncing
		sprite1.setPosition(sprite1.getPosition().x, o_min_y - 16); //set the position to 1px above the surface of the object
		hitFloor = true; //signal that the floor has been hit, for the movement processor to act appropriately
		truthCounter++; //increment the amount of times an intersection check turns out to be true
	}

	if (s_max_x > oLeft_min_x && s_min_x < oLeft_max_x && s_max_y > oLeft_min_y && s_min_y < oLeft_max_y) {
		//check if ball intersects with imaginary box 5px to left of the box
		velocityX = -0.3; //set velocity to go opposite it, thereby opposing motion into the box
		truthCounter++; //increment the amount of times an intersection check turns out to be true
	}

	if (s_max_x > oRight_min_x && s_min_x < oRight_max_x && s_max_y > oRight_min_y && s_min_y < oRight_max_y) {
		//check if ball intersects with imaginary box 5px to right of the box
		velocityX = 0.3; //set velocity to go opposite it, thereby opposing motion into the box
		truthCounter++; //increment the amount of times an intersection check turns out to be true
	}

	if (s_max_x > o_min_x && s_min_x < o_max_x && s_max_y > o_min_y && s_min_y < o_max_y && velocityY <= 0) {
		//check if ball intersects the box, and the velocity is negative or 0, meaning it's coming from the bottom
		sprite1.setPosition(sprite1.getPosition().x, o_max_y + 16); //set it to 1px below the bottom of the box to stop it from going into the box
		velocityY = 0.5; //set the velocity to be away from the bottom of the box
		truthCounter++; //increment the amount of times an intersection check turns out to be true
	}

	if (truthCounter > 0) { //if the intersection has occurred at least once
		if (jumpedOn < 3) { //check if it has been jumped on before
			score++; //otherwise award a point
			jumpedOn++;

			int tempAlpha = rect.getFillColor().a/2; //gets the alpha channel colour and divides by 2

			if (jumpedOn == 3) { //if 3 jumps have occurred then basically make it disappear
				rect.setFillColor(sf::Color(255, 255, 255, 0));
			}else{
				rect.setFillColor(sf::Color(255, 255, 255, tempAlpha)); //otherwise just set the alpha channel to the decreased value of tempAlpha
			}
		}

		timeAtJump = clock.getElapsedTime(); //this is for detecting how long it's been since the ball last touched a platform 

		return true; //return true, so appropriate action is taken, i.e bouncing and stuff
	}
	else {
		return false; //return false, to continue as normal
	}
}

class chunks { //chunks, which will hold 9 platforms and each will cover the screen
public:
	sf::Vector2f chunkCoords = sf::Vector2f(); //these will be the global coordinates of the chunk
	sf::Vector2f chunkWidthHeight = sf::Vector2f(); //and its dimensions, in case I want to change them for some of em down the line

	std::vector<platforms> platformsInTheChunk; //a vector to hold the platforms

	chunks(int, int, int, int); //the constructor
	~chunks(); //the destructor
};

chunks::~chunks() { //destructor definition, destructs the platforms, then empties the vector array, then destructs the chunk
	for (int i = 0; i < platformsInTheChunk.size(); i++) {
		platformsInTheChunk[i].~platforms();
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
				platforms(
					sectorWidth / 2 + (rand() % 100 - 25),
					sectorHeight / 2 + (rand() % 100 - 25),
					posX + (sectorWidth * i) + (rand() % 50) + 50,
					posY + (sectorHeight * j) + (rand() % 50) + 50
				)
			);
		}
	}
}

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
	chunkCoordsWidthHeight[0] = (posX-((int)posX % winWidth)); //top left x coord 
	chunkCoordsWidthHeight[1] = (posY-((int)posY % winHeight)); //top left y coord
	chunkCoordsWidthHeight[2] = winWidth; //width
	chunkCoordsWidthHeight[3] = winHeight; //height

	for (int i = 0; i < chunksLoaded.size(); i++) { //loops through all chunks
		if ( //checks if chunks are outside the 3x3 space they're supposed to be in, and then destructs them if they are
			chunksLoaded[i].chunkCoords.x < chunkCoordsWidthHeight[0] - chunkCoordsWidthHeight[2] ||
			chunksLoaded[i].chunkCoords.y < chunkCoordsWidthHeight[1] - chunkCoordsWidthHeight[3] ||
			chunksLoaded[i].chunkCoords.x > chunkCoordsWidthHeight[0] + 2*(chunkCoordsWidthHeight[2]) ||
			chunksLoaded[i].chunkCoords.y > chunkCoordsWidthHeight[1] + 2*(chunkCoordsWidthHeight[3])
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

				chunksLoaded.push_back( //adds the needed chunk to the chunksLoaded vector
					chunks(
						chunkCoordsWidthHeight[2],
						chunkCoordsWidthHeight[3],
						tempCoordX,
						tempCoordY
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

chunksHolder::~chunksHolder(){ //the destructor
	for (int i = 0; i < chunksLoaded.size(); i++) { //loops through all the chunks
		chunksLoaded[i].~chunks(); //destructs all the chunks, which will actually get rid of all the platforms first, then destruct the respective chunk
	}
}

chunksHolder::chunksHolder(int width, int height, int posX, int posY) { //the initial constructor
	std::system("CLS"); //clears console
	//populates the vector with 3x3 chunks in a grid
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			chunksLoaded.push_back(
				chunks(
					width,
					height,
					posX + (width * i),
					posY + (height * j)
				)
			);
		}
	}
}