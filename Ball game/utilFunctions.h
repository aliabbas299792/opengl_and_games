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
		if (velocityY > 10) {
			velocityY = 10;
		}
		else if (velocityY < -10) {
			velocityY = -10;
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

void sortScoreBoard(std::fstream &scoreboardFile, int *fileNums, int arrayLen, std::string filePath) {
	int i = 0;
	int j = 0;
	int carry1 = 0;

	for (; i < arrayLen; i++) {
		for (j = 0; j < arrayLen; j++) { //basically, I need to reset the value of j to 0 for each iteration of the outer loop
			if (*(fileNums + i) > *(fileNums + j)) { //if the value of index i is greater than the value of index j, swap them
				carry1 = *(fileNums + i); //put value of index i into carrier
				*(fileNums + i) = *(fileNums + j); //set value of index j to index i
				*(fileNums + j) = carry1; //set value of index j to what was once the value of index i
			}
		}
	}
	//now the array is in descending order (highest to lowest)
	//if you dont understand it, think of it like this, if the 9th iteration of i is on one of the last values of the array, and j
	//is comparing one of the first values to it, they will swap. this means that the bigger values will swap with the smaller ones
	//and as such will be descending order by the end of the sorting.
	//this is the bubble sort algorithm of O(n^2)

	scoreboardFile.open(filePath);

	i = 0;

	for (; i < 9; i++) {
		scoreboardFile << *(fileNums + i) << '\n'; //just output the values directly back to the file
	}
	scoreboardFile.close();
}

void scoreboard_init(std::string filePath, std::string &finalReturnString){
	std::fstream scores; //the actual filestream to get the scores
	int fileLoopTemp; //to retrieve number of lines in file for array, populate array and return string
	std::string tempScore; //to first be used to find num of lines, then to retrieve lines and put in to array

	if (!std::experimental::filesystem::exists(filePath)) { //creates the file if it doesnt exist
		scores.open(filePath, std::fstream::in);
		scores.close();
	}

	//initialising everything to empty or 0
	fileLoopTemp = 0;
	tempScore = "";
	finalReturnString = "";

	scores.open(filePath, std::ios::in); //open on input method

	while (std::getline(scores, tempScore)) {
		fileLoopTemp++; //this will loop through the file and get the number of lines there are in the file
	}
	scores.close();

	int *fileNumPtr = new int[fileLoopTemp]; //this will make a pointer

	fileLoopTemp = 0; //reset to 0, for use in next loop

	scores.open(filePath, std::ios::in); //open in read mode
	while (std::getline(scores, tempScore)) { //loops through the entire file
		if(std::stoi(tempScore) > 50000 || std::stoi(tempScore) < 0){
			tempScore = "0";
		}

		*(fileNumPtr + fileLoopTemp) = std::stoi(tempScore); //this will store in array
		fileLoopTemp++; //this will increase to increment the index of the array
	}
	scores.close(); //close file

	scores.open(filePath, std::ios::out); //open in write mode
	scores.close(); //close it, thereby emptying the file

	sortScoreBoard(scores, fileNumPtr, fileLoopTemp, filePath); //this will sort file

	//for use in next bit, set to empty/0
	fileLoopTemp = 0;
	tempScore = "";

	scores.open(filePath, std::ios::in); //open in read mode
	while (std::getline(scores, tempScore)) { //loops through the entire file
		fileLoopTemp++; //for the rank number

		finalReturnString += "Rank ";
		finalReturnString += std::to_string(fileLoopTemp);
		finalReturnString += ")     ";
		finalReturnString += (tempScore) + '\n';
	}
}

void loadTexture(sf::Texture &texture, sf::RectangleShape &rect, std::string filePath, float posX, float posY) { //texture loading for rectangles
	if (!texture.loadFromFile(filePath)) {
		std::cout << "Error: Couldn't load image." << std::endl;
	}
	rect.setTexture(&texture);
	rect.setPosition(posX, posY);
}

void loadTextureSprite(sf::Texture &texture, sf::Sprite &sprite, std::string filePath, float posX, float posY) { //texture loading for sprites
	if (!texture.loadFromFile(filePath)) {
		std::cout << "Error: Couldn't load image." << std::endl;
	}
	sprite.setTexture(texture);
	sprite.setPosition(posX, posY);
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

void selectionProcessor(gameScreens &gameState, gameScreens nextScreen, sf::RectangleShape &button, sf::RenderWindow &window, bool &justChanged, std::string filePath, int &score) {
	//get the position of the mouse
	int mouseX = sf::Mouse::getPosition(window).x;
	int mouseY = sf::Mouse::getPosition(window).y;

	//if the mouse is within the boundaries of the button
	if (mouseX <= button.getPosition().x + button.getSize().x && mouseX >= button.getPosition().x && mouseY >= button.getPosition().y && mouseY <= button.getPosition().y + button.getSize().y) {
		button.setFillColor(sf::Color(255, 255, 255, 170)); //reduce the transparency of the button if hovering over it
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) { //and if it's clicked on
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
				
				scores.open(filePath, std::ios::out);
				temp = temp + '\n' + std::to_string(score);
				scores << temp;
				scores.close();
			}

			justChanged = true; //set the just changed variable to true, for processes that need to run once per screen
			gameState = nextScreen; //set the game state to the provided next screen enum
		}
	}
	else {
		button.setFillColor(sf::Color(255, 255, 255, 255)); //otherwise set the opacity to 100%
	}
}