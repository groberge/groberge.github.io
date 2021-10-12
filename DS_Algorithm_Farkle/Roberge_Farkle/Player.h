#pragma once

#include <string>

using namespace std;

class Player
{
private: 
	string playerName;
	int playerScore = 0;
	bool winningPlayer;


public:
	Player();
	void setName(const string&);
	string getName();
	void setWinner(bool);
	bool getWinner();
	void setScore(int);
	int getScore();
};