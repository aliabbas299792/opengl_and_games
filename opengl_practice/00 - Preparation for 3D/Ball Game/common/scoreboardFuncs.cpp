#include <SFML/Graphics.hpp>
#include <fstream>
#include <experimental/filesystem>
#include "../headerFiles/utilFunctions.h"

void sortScoreBoard(std::fstream &scoreboardFile, int *fileNums, int arrayLen, std::string filePath) {
	int i = 0;
	int j = 0;
	int carry1 = 0;

	for (; i < arrayLen; i++) {
		for (j = 0; j < arrayLen; j++) { //basically, I need to reset the value of j to 0 for each iteration of the outer loop
			if (fileNums[i] > fileNums[j]) { //if the value of index i is greater than the value of index j, swap them
				carry1 = fileNums[i]; //put value of index i into carrier
				fileNums[i] = fileNums[j]; //set value of index j to index i
				fileNums[j] = carry1; //set value of index j to what was once the value of index i
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

void scoreboard_init(std::string filePath, std::string &finalReturnString) {
	std::fstream scores; //the actual filestream to get the scores
	int fileLoopTemp; //to retrieve number of lines in file for array, populate array and return string
	std::string tempScore; //to first be used to find num of lines, then to retrieve lines and put in to array

	if (!std::experimental::filesystem::exists(filePath)) { //creates the file if it doesnt exist
		scores.open(filePath, std::fstream::out);
		scores << "0\n0\n0\n0\n0\n0\n0\n0\n0";
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
		if (std::stoi(tempScore) > 50000 || std::stoi(tempScore) < 0) {
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
		finalReturnString += (tempScore)+'\n';
	}
}
