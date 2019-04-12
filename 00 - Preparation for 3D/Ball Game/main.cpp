#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

#include "headerFiles/enums.h"
#include "headerFiles/utilFunctions.h"
#include "headerFiles/platformClasses.h"

int main() {
	//consts and vars
	const float accelerationX = 0.2;
	const float decelerationX = 0.03;
	const float accelerationY = 6;
	const float gravityOriginal = 0.15;

	float prevPosX = 0;
	float prevPosY = 0;

	int windowWidth = 1200;
	int windowHeight = 600;
	bool removeGravity = false;
	bool hitFloor = false;
	float gravity = 0.15;
	float velocityX = 0;
	float velocityY = 0;
	bool justChanged = false;
	int score = 0;
	std::string scoreboard_text;
	std::string scoreboardFilePath = "scoreboard.txt";

	gameScreens gameState = entrance; //set initial gamestate to entrance

	//window settings
	sf::ContextSettings settings; //make settings variable
	sf::Image windowIcon; //the title bar/corner icon thing
	settings.antialiasingLevel = 5; //set anti-aliasing level to 5

	if (!windowIcon.loadFromFile("resources/char.png")) { //the character as an icon seems alright, so loads it
		std::cout << "Error: Couldn't load image." << std::endl;
	}
	const sf::Uint8 *iconPixel = windowIcon.getPixelsPtr(); //then gets a pointer to an array of the pixels making up the image
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Ball", sf::Style::Titlebar, settings); 
	//window, titlebar option disables resizing and removes titlebar
	windowWidth = window.getSize().x; //get actual width
	windowHeight = window.getSize().y; //get actual height

	window.setIcon(windowIcon.getSize().x, windowIcon.getSize().y, iconPixel);
	//(refering to above) sets the icon thing, takes the original image's width, then height, then the pixel array from earlier
	
	//sfml object definitions
	sf::Texture spriteTex;
	sf::Sprite sprite1;
	sf::RectangleShape scoreboardAndFps(sf::Vector2f(100, 60)); //make the fps box thing
	sf::RectangleShape timeLeftBox(sf::Vector2f(210, 40)); //box for the timer till death thing
	sf::View windowView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)); //make the moving view that will be centered on the ball
	sf::RectangleShape backgroundGround(sf::Vector2f(windowWidth, 100));
	sf::RectangleShape buttonStart(sf::Vector2f(168.75, 56.25)); //start button
	sf::Texture btnStartTex; //the respective texture
	sf::RectangleShape buttonHighScore(sf::Vector2f(150, 56.25)); //high score button
	sf::Texture btnHighScreTex; //the respective texture
	sf::RectangleShape buttonHelp(sf::Vector2f(61.875, 42.1875)); //high score button
	sf::Texture btnHelpTex; //the respective texture
	sf::RectangleShape buttonBack(sf::Vector2f(100, 37.5)); //back button
	sf::Texture btnBackTex; //the respective texture
	sf::RectangleShape buttonExit(sf::Vector2f(100, 37.5)); //back button
	sf::Texture btnExitTex; //the respective texture
	sf::RectangleShape logo(sf::Vector2f(250, 250)); //the logo in start screen
	sf::Texture logoTex; //the respective texture
	sf::RectangleShape scoreboard_board(sf::Vector2f(520, 540)); //scoreboard rect
	sf::RectangleShape helpBox(sf::Vector2f(520, 540)); //scoreboard rect
	sf::Font lato; //lato font
	sf::Text textScoreBoardHead("Scoreboard:", lato); //the scoreboard title
	sf::Text textScoreBoard("", lato); //the scores for the highscore bit
	sf::Text textScoreLive("", lato); //the scores for the actual in-game score
	sf::Text remainingLife("", lato);
	sf::Text youAreDead("You Died", lato);
	sf::Text helpTitle("Help", lato);
	sf::Text actualHelp("", lato);

	//loading stuff, or making rectangles/sprites with colors and stuff, or setting position of stuff
	loadTextureSprite(spriteTex, sprite1, "resources/char.png", windowWidth / 2 - 15, 100); //sprite
	loadTexture(btnStartTex, buttonStart, "resources/btnStart.png", windowWidth/2-buttonStart.getSize().x/2, 310); //start button
	loadTexture(btnHelpTex, buttonHelp, "resources/btnHelp.png", windowWidth / 2 - buttonHelp.getSize().x/2, 471.25); //help button
	loadTexture(btnBackTex, buttonBack, "resources/btnBack.png", windowWidth - buttonBack.getSize().x - 25, 25); //back button
	loadTexture(btnHighScreTex, buttonHighScore, "resources/btnHighScre.png", windowWidth / 2 - buttonHighScore.getSize().x / 2, 390); //high score button
	loadTexture(btnExitTex, buttonExit, "resources/btnExit.png", windowWidth / 2 - buttonExit.getSize().x / 2, 536.25); //exit button
	loadTexture(logoTex, logo, "resources/logo.png", windowWidth / 2 - logo.getSize().x / 2, 30);
	lato.loadFromFile("resources/Lato-Light.ttf"); //the actual font, lato

	sprite1.setPosition(300.0f,100.0f); //set sprite initial position
	sprite1.setTexture(spriteTex); //set the texture
	sprite1.setOrigin(15, 15); //set origin for transformation to center

	youAreDead.setPosition(windowWidth / 2 - 100, windowHeight / 2 - 20);
	youAreDead.setCharacterSize(50);
	youAreDead.setFillColor(sf::Color::Red);

	helpTitle.setPosition(sf::Vector2f(windowWidth / 2 - scoreboard_board.getSize().x / 2 + 40, 40)); //sets help title to halfway across screen
	helpTitle.setCharacterSize(60);

	actualHelp = sf::Text(
		"-The aim of the game is to score as high as possible\n and not die.\n\n-Your score and fps appear in the top left once in-game.\n-You die after falling for 2 seconds, and the timer\n is on the right in-game.\n-You also die if the window is out of focus.\n-Start button or spacebar to start the game.\n-Up (or spacebar), Left and Right to accelerate in those\n directions.\n-Escape button to exit the game.\n\n-All blocks disappear after being bounced off of\n thrice. \n-White blocks add 1 to your score. \n-Green blocks add 4 to your score. \n-Blue blocks add 10 to your score. \n-Red blocks kill you.\n-Cyan blocks make you jump more.", lato, 18); //set the text font and size
	actualHelp.setPosition(sf::Vector2f(windowWidth / 2 - scoreboard_board.getSize().x / 2 + 40, 124)); //set it to centre

	timeLeftBox.setFillColor(sf::Color(30, 30, 30, 200));
	timeLeftBox.setPosition(windowWidth - timeLeftBox.getSize().x, 90);

	helpBox.setFillColor(sf::Color(50, 50, 50, 255)); //set help box's color
	helpBox.setPosition(windowWidth / 2 - scoreboard_board.getSize().x / 2, 30); //set the box to halfway across screen

	scoreboardAndFps.setPosition(0, 0); //set the fps box holder thing to top left
	scoreboardAndFps.setFillColor(sf::Color(30, 30, 30, 200)); //set it's color

	//scoreboard stuff under the same subheading
	scoreboard_board.setFillColor(sf::Color(50, 50, 50, 255)); //set scoreboard's color
	scoreboard_board.setPosition(windowWidth / 2 - scoreboard_board.getSize().x / 2, 30); //set the board to halfway across screen

	textScoreBoardHead.setPosition(sf::Vector2f(windowWidth / 2 - scoreboard_board.getSize().x / 2 + 40, 40)); //sets scoreboard title to halfway across screen
	textScoreBoardHead.setCharacterSize(60);

	//making collision objects
	chunksHolder *liveChunk = new chunksHolder(windowWidth, windowHeight, (0-windowWidth), (0-windowHeight)); //this is the initial one
	//1st/2nd params are obvsly window width/height, and 3rd/4th are the beginning of the chunks
	
	int i = 0; //for the iteration through the objects to check for collisions/draw them
	int j = 0; //to loop through chunk itself
	bool tempTorF = false; //check if there is a collison variable
	float bounceMultiplier = 0.5;
	bool killFlag = false; //check if bounced on instant death block

	//timing and fps
	sf::Clock gameClock; //start the clock
	sf::Time elapsedTime = gameClock.getElapsedTime(); //get elapsed time at this instant
	sf::Time timeSinceCollision;
	sf::Time currentTime; //define the variable to compare (will be current time)

	sf::Text fpsText("0 FPS", lato); //the fps text
	fpsText.setPosition(10, 10);
	fpsText.setCharacterSize(15);
	int fps = 0; //set initial value of fps to 0
	sf::Time elapsedTimeFps = gameClock.getElapsedTime(); //elapsed time for FPS
	sf::Time tempTime; //for whatever temporary purpose to measure time in the game

	//make scoreboard file if it doesn't exist
	std::fstream scores; //the actual filestream to get the scores
	if (!std::experimental::filesystem::exists(scoreboardFilePath)) { //creates the file if it doesnt exist
		scores.open(scoreboardFilePath, std::fstream::out);
		scores << "0\n0\n0\n0\n0\n0\n0\n0\n0";
		scores.close();
	}

	bool infinitePlayFlag = false; //special secret mode

	bool clickedAwayFlag = false; //for seeing if a person has clicked away

	while (window.isOpen()) {
		sf::Event events;

		if (window.hasFocus() == true) { //if the window is not in focus pause everything
			window.clear(sf::Color(10, 10, 15, 255)); //set default background to black

			currentTime = gameClock.getElapsedTime(); //gets elapsed time since the last time it was measured

			fps++; //increment fps through ever loop, so shows it accurately

			textScoreLive = sf::Text("Score: " + std::to_string(score), lato);
			textScoreLive.setPosition(10, 30); //set the position on the screen
			textScoreLive.setCharacterSize(15); //font size of 15

			if (currentTime.asMilliseconds() >= elapsedTimeFps.asMilliseconds() + 1000) { //if a second has passed...
				fpsText = sf::Text(std::to_string(fps) + " FPS", lato); //update the fps text
				fpsText.setPosition(10, 10); //set the position on the screen
				fpsText.setCharacterSize(15); //font size of 15

				fps = 0; //reset the fps counter (it's per second, so needs to be of course)
				elapsedTimeFps = currentTime; //sets the variable to current time, to measure for passing of 1 second in later loop
			}

			switch (gameState) {
			case entrance:
				entranceScreen(gameState, buttonExit, buttonStart, buttonHighScore, buttonHelp, logo, window, justChanged, score, infinitePlayFlag);
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { //activate special mode
					infinitePlayFlag = true;
					//switch to game
					justChanged = true;
					gameState = game;
				}
				else {
					infinitePlayFlag = false;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					justChanged = true;
					gameState = game;
				}
				break;
			case game:
				if (justChanged == true) { //esentially set it to initial positions and stuff
					score = 0;
					sprite1.setPosition(windowWidth / 2, windowHeight / 2); //set sprite initial position
					velocityX = 0;
					velocityY = 0;
					timeSinceCollision = gameClock.getElapsedTime();

					liveChunk = new chunksHolder(windowWidth, windowHeight, (0 - windowWidth), (0 - windowHeight)); //reset chunks

					justChanged = false;

					killFlag = false;

					clickedAwayFlag = false;
				}

				if (clickedAwayFlag == true && infinitePlayFlag == false) {
					onDeath(gameState, scoreboardFilePath, score, tempTime, currentTime, youDied, infinitePlayFlag); //if you click away, you die
					clickedAwayFlag = false; //and reset the flag
				}

				if (currentTime.asMilliseconds() - timeSinceCollision.asMilliseconds() >= 2000 || killFlag == true) {
					if (infinitePlayFlag == false) {
						liveChunk->~chunksHolder();
						onDeath(gameState, scoreboardFilePath, score, tempTime, currentTime, youDied, infinitePlayFlag); //if it's been falling for at least 2 seconds then die
					}
				}

				if (currentTime.asMilliseconds() >= elapsedTime.asMilliseconds() + 8) { //only process interactions and have movement ever 7ms
					removeGravity = false; //initialise bool to false
					tempTorF = false; //initialise this bool to false as well

					for (i = 0; i < liveChunk->chunksLoaded.size(); i++) { //iterates through chunk vector
						for (j = 0; j < liveChunk->chunksLoaded[i].platformsInTheChunk.size(); j++) { //iterates through individual chunks
							if (liveChunk->chunksLoaded[i].platformsInTheChunk[j].checkIntersect(sprite1, gravity, velocityX, velocityY, gravityOriginal, hitFloor, removeGravity, score, timeSinceCollision, gameClock, prevPosX, prevPosY) && tempTorF == false) {
								//if the above evaluates to true even once, the variable below is set to true for this entire iteration of the loop
								//the if statement checks for collisions
								bounceMultiplier = 0.5; //default bounce multiplier

								if (liveChunk->chunksLoaded[i].platformsInTheChunk[j].typeOfBlock == instantDeath && infinitePlayFlag == false) { //if it's an instant death block
									killFlag = true; //set it to die on next loop
									bounceMultiplier = 0.1;
								}
								else if (liveChunk->chunksLoaded[i].platformsInTheChunk[j].typeOfBlock == bigBounce) {
									bounceMultiplier = 1.5;
								}

								tempTorF = true; //basically collision flag
							}
						}
					}

					if (tempTorF) //if it's true...
					{
						moveBall(windowView, window, sprite1, velocityX, velocityY, accelerationX, accelerationY, gravity, gravityOriginal, true, hitFloor, decelerationX, removeGravity, prevPosX, prevPosY, bounceMultiplier);
						//then move the ball, while accounting for the collision has taken place
					}
					else {
						//otherwise act as if there was no collision
						moveBall(windowView, window, sprite1, velocityX, velocityY, accelerationX, accelerationY, gravity, gravityOriginal, false, hitFloor, decelerationX, removeGravity, prevPosX, prevPosY, bounceMultiplier);
					}

					elapsedTime = currentTime;
				}

				window.setView(windowView); //sets the view to the moving view, and draws stuff relative to it

				for (i = 0; i < liveChunk->chunksLoaded.size(); i++) { //iterates through chunk vector
					for (j = 0; j < liveChunk->chunksLoaded[i].platformsInTheChunk.size(); j++) { //iterates through individual chunks
						window.draw(*liveChunk->chunksLoaded[i].platformsInTheChunk[j].shape()); //draws the platforms
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					liveChunk->~chunksHolder();
					onDeath(gameState, scoreboardFilePath, score, tempTime, currentTime, game, infinitePlayFlag); //basically just save and exit
					window.close();
				}

				liveChunk->updateChunks(sprite1, windowWidth, windowHeight);
				
				if (infinitePlayFlag == false) {
					remainingLife = sf::Text("Time till death: " + std::to_string(2000 - (currentTime.asMilliseconds() - timeSinceCollision.asMilliseconds())) + "ms", lato);
				}
				else {
					remainingLife = sf::Text("You are immortal", lato);
				}

				remainingLife.setCharacterSize(15);
				remainingLife.setPosition(windowWidth - 180, 100);

				window.draw(sprite1); //and draws the sprite

				window.setView(window.getDefaultView()); //sets back to the normal default view
				window.draw(timeLeftBox); //holder for time left thing
				window.draw(remainingLife); //time left thing
				window.draw(buttonBack); //draws the back button
				window.draw(scoreboardAndFps); //draws the fps text and (what will probably be score board) box
				window.draw(fpsText); //draws the fps text
				window.draw(textScoreLive); //draw the score text

				selectionProcessor(gameState, entrance, buttonBack, window, justChanged, scoreboardFilePath, score, infinitePlayFlag);
				//and adds the respective selection processor for the back button

				break;
			case scoreboard:
				if (justChanged == true) { //on first iteration when the scoreboard has just been clicked on
					scoreboard_init(scoreboardFilePath, scoreboard_text); //sort all the scores, and limit the total amount to 9

					textScoreBoard = sf::Text(scoreboard_text, lato, 37); //set the text font and size
					textScoreBoard.setPosition(sf::Vector2f(windowWidth / 2 - scoreboard_board.getSize().x / 2 + 40, 130)); //and centers it on x axis

					justChanged = false; //and set the just clicked on thing to false, for after this processing
				}

				window.draw(scoreboard_board); //draw the scoreboard
				window.draw(textScoreBoardHead); //draw the title
				window.draw(textScoreBoard); //draw the scores
				//
				window.draw(buttonBack); //draw back button
				selectionProcessor(gameState, entrance, buttonBack, window, justChanged, scoreboardFilePath, score, infinitePlayFlag); //and the back buttons's selection processor thing here too
				break;
			case help:
				window.draw(helpBox);
				window.draw(helpTitle);
				window.draw(actualHelp);
				window.draw(buttonBack); //draw back button
				selectionProcessor(gameState, entrance, buttonBack, window, justChanged, scoreboardFilePath, score, infinitePlayFlag); //and the back buttons's selection processor thing here too
				break;
			case youDied:
				window.draw(youAreDead);

				if (tempTime.asMilliseconds() < currentTime.asMilliseconds() - 1000) {
					gameState = entrance;
				}

				break;
			case end:
				window.close();
				return 0; //and end if exited
				break;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.close();
			}
		}else{
			clickedAwayFlag = true;
		}

		while (window.pollEvent(events))
		{
			//gotta poll events regardless of whether or not we use em, otherwise window will become unresponsive
			if (events.type == sf::Event::Closed)
				window.close();
		}

		window.display(); //display everything drawn
	}
}
