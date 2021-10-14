#pragma once

#include "Player.h"
#include "HighScore.h"
#include <vector>
#include <map>

class Gameplay
{
private:
	int numberDice = 6, diceKeep = 0, rollScore = 0;

	const int winningScore = 10000;

	vector<int> diceRoll(int);
	map<int, int> displayDice(vector<int>& dice);
	char promptForRoll(Player& currentPlayer, vector<Player> gamePlayers, const int& turnScore, const int& numberDice);
	pair<int, int> scoreDice(vector<int> dice, int numDice);
	int calculateTripletScore(int triplet);

public:
	void displayRules(string);
	Gameplay();
	int numberPlayers();
	int takeTurn(Player& currentPlayer, const vector<Player>& gamePlayers);
	void winningPlayer(Player& p);
	Player calculateWinner(vector<Player>& allPlayers);
	void updateDatabase(HighScore& highScoreDb, const string& name, const int& score, const string& winners_name);
};