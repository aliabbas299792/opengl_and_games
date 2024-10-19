#include "enums.h"

#ifndef UTIL_FUNCTIONS
#define UTIL_FUNCTIONS

//below are all the function prototypes
void entranceScreen(gameScreens &gameState, sf::RectangleShape buttonExit, sf::RectangleShape buttonStart, sf::RectangleShape buttonHighScore, sf::RectangleShape buttonHelp, sf::RectangleShape logo, sf::RenderWindow &window, bool &justChanged, int &score, bool &infinitePlayFlag);
bool timingFpsScore(sf::Time &currentTime, sf::Time &elapsedTimeFps, sf::Time &elapsedTime, sf::Clock gameClock, int &fps);
void selectionProcessor(gameScreens &gameState, gameScreens nextScreen, sf::RectangleShape &button, sf::RenderWindow &window, bool &justChanged, std::string filePath, int &score, bool &infinitePlayFlag);
void loadTexture(sf::Texture &texture, sf::RectangleShape &rect, std::string filePath, float posX, float posY);
void loadTextureSprite(sf::Texture &texture, sf::Sprite &sprite, std::string filePath, float posX, float posY);

void moveBall(sf::View &view, sf::RenderWindow &window, sf::Sprite &sprite, float &velocityX, float &velocityY, float accelerationX, float accelerationY, float &gravity, float originalGravity, bool collide, bool &hitFloor, float decelerationX, bool &removeGravity, float &prevPosX, float &prevPosY, float &bounceMultiplier);
void onDeath(gameScreens &gameState, std::string filePath, int score, sf::Time &tempTime, sf::Time currentTime, gameScreens nextScreen, bool &infinitePlayFlag);

void sortScoreBoard(std::fstream &scoreboardFile, int *fileNums, int arrayLen, std::string filePath);
void scoreboard_init(std::string filePath, std::string &finalReturnString);

#endif