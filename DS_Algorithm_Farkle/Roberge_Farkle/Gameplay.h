#pragma once

#include "Player.h"
#include <vector>
#include <map>

class Gameplay
{
private:
	int numberDice = 6, diceKeep = 0, rollScore = 0;

	const int winningScore = 10000;

	vector<int> diceRoll(int);
	map<int, int> displayDice(vector<int>& dice);
	char promptForRoll(Player& currentPlayer, vector<Player> gamePlayers, int& turnScore, int& numberDice);
	pair<int, int> scoreDice(vector<int> dice, int numDice);
	int calculateTripletScore(int triplet);

public:
	void displayRules(string);
	Gameplay();
	int numberPlayers();
	int takeTurn(Player& currentPlayer, const vector<Player>& gamePlayers);
	void winningPlayer(Player& p);
	Player calculateWinner(vector<Player>& allPlayers);
};